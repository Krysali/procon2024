from typing import List, Tuple, Dict, Callable, Optional, Any
from environments.environment_abstract import Environment, State
import numpy as np
from heapq import heappush, heappop
from subprocess import Popen, PIPE
from environments.Game import GameState
from argparse import ArgumentParser
import torch
from utils import  nnet_utils, search_utils, misc_utils, data_utils
import pickle
import time
import sys
import os
import socket
from torch.multiprocessing import Process


def main():
    # parse arguments
    parser: ArgumentParser = ArgumentParser()
    parser.add_argument('--states', type=str, required=True, help="File containing states to solve")
    parser.add_argument('--goal_states', type=str, required=True, help="File containing states to solve")
    parser.add_argument('--height', type=int, required=True, help="File containing states to solve")
    parser.add_argument('--width', type=int, required=True, help="File containing states to solve")
    parser.add_argument('--model_dir', type=str, required=True, help="Directory of nnet model")
    parser.add_argument('--env', type=str, required=True, help="Environment: cube3, 15-puzzle, 24-puzzle")
    parser.add_argument('--batch_size', type=int, default=1, help="Batch size for BWAS")
    parser.add_argument('--weight', type=float, default=1.0, help="Weight of path cost")
    parser.add_argument('--language', type=str, default="python", help="python or cpp")

    parser.add_argument('--results_dir', type=str, required=True, help="Directory to save results")
    parser.add_argument('--start_idx', type=int, default=0, help="")
    parser.add_argument('--nnet_batch_size', type=int, default=None, help="Set to control how many states per GPU are "
                                                                          "evaluated by the neural network at a time. "
                                                                          "Does not affect final results, "
                                                                          "but will help if nnet is running out of "
                                                                          "memory.")

    parser.add_argument('--verbose', action='store_true', default=False, help="Set for verbose")
    parser.add_argument('--debug', action='store_true', default=False, help="Set when debugging")

    args = parser.parse_args()
    # --results_dir results/puzzle15/
    if not os.path.exists(args.results_dir):
        os.makedirs(args.results_dir)
  
    results_file: str = "%s/results.pkl" % args.results_dir
    output_file: str = "%s/output.txt" % args.results_dir 
    if not args.debug:
        sys.stdout = data_utils.Logger(output_file, "w")

    # --states data/puzzle15/test/data_0.pkl
    # --start_idx 0
    # get  
    input_data = pickle.load(open(args.states, "rb"))
    states: List[State] = input_data['states'][args.start_idx:]



    # --env puzzle15
    # environment
    env: Environment = GameState(args.goal_states, args.height, args.width)

    # initialize results
    results: Dict[str, Any] = dict()
    results["states"] = states

    if args.language == "cpp":
        solns, paths, times, num_nodes_gen = bwas_cpp(args, env, states, results_file)
    else:
        raise ValueError("Unknown language %s" % args.language)

    results["solutions"] = solns
    results["paths"] = paths
    results["times"] = times
    results["num_nodes_generated"] = num_nodes_gen

    pickle.dump(results, open(results_file, "wb"), protocol=-1)


def bwas_cpp(args, env: Environment, states: List[State], results_file: str):
    assert (args.env.upper() in ['GameState'])

    # Make c++ socket
    socket_name: str = "%s_cpp_socket" % results_file.split(".")[0]

    try:
        os.unlink(socket_name)
    except OSError:
        if os.path.exists(socket_name):
            raise

    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    sock.bind(socket_name)

    # Get state dimension
    if args.env.upper() == 'GameState':
        state_dim: int = 1024
    else:
        raise ValueError("Unknown c++ environment: %s" % args.env)

    # start heuristic proc
    num_parallel: int = len(os.environ['CUDA_VISIBLE_DEVICES'].split(","))
    device, devices, on_gpu = nnet_utils.get_device()



    heur_fn_i_q, heur_fn_o_qs, heur_procs = nnet_utils.start_heur_fn_runners(num_parallel, args.model_dir, device,
                                                                             on_gpu, env, all_zeros=False,
                                                                             clip_zero=True,
                                                                             batch_size=args.nnet_batch_size)
    
    
    nnet_utils.heuristic_fn_par(states, env, heur_fn_i_q, heur_fn_o_qs)  # initialize

    heur_proc = Process(target=cpp_listener, args=(sock, args, env, state_dim, heur_fn_i_q, heur_fn_o_qs))
    heur_proc.daemon = True
    heur_proc.start()

    time.sleep(2)  # give socket time to intialize

    solns: List[List[int]] = []
    paths: List[List[State]] = []
    times: List = []
    num_nodes_gen: List[int] = []

    for state_idx, state in enumerate(states):
        # Get string rep of state
        if args.env.upper() == "GameState":
            state_str: str = " ".join([str(x) for x in state.colors])
        else:
            raise ValueError("Unknown c++ environment: %s" % args.env)

        popen = Popen(['./cpp/parallel_weighted_astar', state_str, str(args.weight), str(args.batch_size),
                       socket_name, args.env, "0"], stdout=PIPE, stderr=PIPE, bufsize=1, universal_newlines=True)
        lines = []
        for stdout_line in iter(popen.stdout.readline, ""):
            stdout_line = stdout_line.strip('\n')
            lines.append(stdout_line)
            if args.verbose:
                sys.stdout.write("%s\n" % stdout_line)
                sys.stdout.flush()

        moves = [int(x) for x in lines[-5].split(" ")[:-1]]
        soln = [x for x in moves][::-1]
        num_nodes_gen_idx = int(lines[-3])
        solve_time = float(lines[-1])

        # record solution information
        path: List[State] = [state]
        next_state: State = state
        transition_costs: List[float] = []

        for move in soln:
            next_states, tcs = env.next_state([next_state], move)

            next_state = next_states[0]
            tc = tcs[0]

            path.append(next_state)
            transition_costs.append(tc)

        solns.append(soln)
        paths.append(path)
        times.append(solve_time)
        num_nodes_gen.append(num_nodes_gen_idx)

        path_cost: float = sum(transition_costs)

        # check soln
        assert search_utils.is_valid_soln(state, soln, env)

        # print to screen
        print("State: %i, SolnCost: %.2f, # Moves: %i, "
              "# Nodes Gen: %s, Time: %.2f" % (state_idx, path_cost, len(soln),
                                               format(num_nodes_gen_idx, ","),
                                               solve_time))

    os.unlink(socket_name)

    nnet_utils.stop_heuristic_fn_runners(heur_procs, heur_fn_i_q)

    return solns, paths, times, num_nodes_gen


def cpp_listener(sock, args, env: Environment, state_dim: int, heur_fn_i_q, heur_fn_o_qs):
    sock.listen(1)
    connection, client_address = sock.accept()

    # device, devices, on_gpu = nnet_utils.get_device()
    # heuristic_fn = nnet_utils.load_heuristic_fn(args.model_dir, device, on_gpu, env.get_nnet_model(),
    #                                             env, clip_zero=True, batch_size=args.nnet_batch_size)

    max_bytes: int = 4096
    while True:
        data_rec = connection.recv(8)
        while not data_rec:
            connection, client_address = sock.accept()
            data_rec = connection.recv(8)

        num_bytes_recv = np.frombuffer(data_rec, dtype=np.int64)[0]

        num_bytes_seen = 0
        data_rec = b""
        while num_bytes_seen < num_bytes_recv:
            con_rec = connection.recv(max_bytes)
            data_rec = data_rec + con_rec
            num_bytes_seen = num_bytes_seen + len(con_rec)

        states_np = np.frombuffer(data_rec, dtype=env.dtype)
        states_np = states_np.reshape(int(len(states_np)/state_dim), state_dim)

        # Get nnet representation of state
        if args.env.upper() == "GameState":
            states_np = states_np/9
            states_np = states_np.astype(env.dtype)
            states_nnet: List[np.ndarray] = [states_np]
        else:
            raise ValueError("Unknown c++ environment %s" % args.env)

        # get heuristic
        results = heuristic_fn_par(states_nnet, heur_fn_i_q, heur_fn_o_qs)

        # send results
        connection.sendall(results.astype(np.float32))


def heuristic_fn_par(states_nnet: List[np.ndarray], heur_fn_i_q, heur_fn_o_qs):
    num_parallel: int = len(heur_fn_o_qs)

    num_states: int = states_nnet[0].shape[0]

    parallel_nums = range(min(num_parallel, num_states))
    split_idxs = np.array_split(np.arange(num_states), len(parallel_nums))
    for idx in parallel_nums:
        states_nnet_idx = [x[split_idxs[idx]] for x in states_nnet]
        heur_fn_i_q.put((idx, states_nnet_idx))

    # Check until all data is obtaied
    results = [None]*len(parallel_nums)
    for idx in parallel_nums:
        results[idx] = heur_fn_o_qs[idx].get()

    results = np.concatenate(results, axis=0)

    return results


if __name__ == "__main__":
    main()
