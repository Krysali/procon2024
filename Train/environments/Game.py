from typing import List, Dict, Tuple, Union
import numpy as np
from torch import nn
from random import randrange
import math
from utils.pytorch_models import ResnetModel
from .environment_abstract import Environment, State

class Action:
    def __init__(self, x: int = 0, y: int = 0, die_index: int = 0, direction: int = 0):
        self.die_index = die_index
        self.x = x
        self.y = y
        self.direction = direction


class GmState(State):
    __slots__ = ['board', 'hash']

    def __init__(self, board: np.ndarray):
        self.board: np.ndarray = board
        self.hash = None

    def __hash__(self):
        if self.hash is None:
            self.hash = hash(self.board.tostring())

        return self.hash

    def __eq__(self, other):
        return np.array_equal(self.board, other.board)


def is_inside(x, y, dtype, n , m ):
        if dtype == 2:
            return (0 <= y < n)
        else:
            return (0 <= x < m)
 

class GameState(Environment):
    def __init__(self,  goalstate: List[List[int]], height: int, width: int):
        self.goalstate = goalstate  # Goal state of the board
        self.height_state = height  # Board height
        self.width_state = width  # Board width
        self.num_actions = 0  # Initialize number of actions
        self.actions: List[Action] = []  # List to hold actions
        
    def next_state(self, states: List[GmState], x: int, y: int, die_index: int, direction: int) -> Tuple[List[GmState], List[float]]:
        # Stack the board states of all input GmState instances into a single numpy array
        states_np = np.stack([np.copy(state.board) for state in states], axis=0)
    
        # Apply the _move_np function to get the next state of the boards
        states_next_np, transition_costs = self._move_np(states_np, x, y, die_index, direction)
    
        # Convert the resulting numpy array of boards back to GmState objects
        states_next: List[GmState] = [GmState(board) for board in states_next_np]
    
        # Return the next states and their corresponding transition costs
        return states_next, transition_costs
    
    def is_solved(self) -> bool:
        return self.state == self.goalstate

    def state_to_nnet_input(self, states: List[GmState]) -> List[np.ndarray]:
        states_np = np.stack([state.board for state in states], axis=0)

        representation_np: np.ndarray = self.height_state*self.width_state
        representation_np: np.ndarray = representation_np.astype(self.dtype)

        representation: List[np.ndarray] = [representation_np]

        return representation

    def get_num_moves(self) -> int:
        return len(self.moves)

    def get_nnet_model(self) -> nn.Module:
        state_dim: int = self.height_state*self.width_state
        nnet = ResnetModel(state_dim, 6, 5000, 1000, 4, 1, True)

        return nnet

    def expand(self, states: List[State]) -> Tuple[List[List[State]], List[np.ndarray]]:
        assert self.fixed_actions, "Environments without fixed actions must implement their own method"

        num_states: int = len(states)
        num_env_moves: int = self.get_num_moves()

        # Initialize expanded states and transition costs
        states_exp: List[List[State]] = [[] for _ in range(num_states)]
        tc: np.ndarray = np.empty((num_states, num_env_moves))

        # Create a numpy array from the state boards for efficient processing
        states_np: np.ndarray = np.stack([state.board for state in states])

        # Iterate over each possible move
        for move_idx in range(num_env_moves):
            # Get the next states and transition costs for the current move
            states_next_np, tc_move = self._move_np(states_np, move_idx)

            # Store transition costs
            tc[:, move_idx] = np.array(tc_move)

            # Expand the states for the current move
            for idx in range(num_states):
                states_exp[idx].append(GmState(states_next_np[idx]))

        # Convert transition costs to a list of arrays for each state
        tc_l: List[np.ndarray] = [tc[i] for i in range(num_states)]

        return states_exp, tc_l
        
    def _move_np(self, states_np: np.ndarray, x: int, y: int, die_index: int, direction: int):
        states_next_np: np.ndarray = states_np.copy()

        # Set up
        power = math.ceil(die_index / 3)
        size = int(math.pow(2, power))
        dice_type = 1 if power == 0 else die_index - ((power - 1) * 3)

        n, m = states_next_np.shape[1], states_next_np.shape[2]  # Assuming states_np has shape (num_states, rows, cols)

        cut_pieces = []
        x_start = max(x, 0)
        y_start = max(y, 0)
        x_end = min(m, x + size) - 1
        y_end = min(n, y + size) - 1
        width = x_end - x_start + 1
        height = y_end - y_start + 1

        chosen_row_num = math.floor(height / 2)
        chosen_col_num = math.floor(width / 2)
        first = 1

        # Determine cutting logic based on dice_type
        if dice_type == 2:
            first = (height + 1) % 2
            if is_inside(x, y, dice_type, n, m):
                if not first:
                    chosen_row_num += 1
                first = 1
            elif abs(y) % 2 != 0:
                first = 0
            elif abs(y) % 2 == 0 and height % 2 == 1:
                first = 1
                chosen_row_num += 1

        elif dice_type == 3:
            first = (width + 1) % 2
            if is_inside(x, y, dice_type, n, m):
                if not first:
                    chosen_col_num += 1
                first = 1
            elif abs(x) % 2 != 0:
                first = 0
            elif abs(x) % 2 == 0 and width % 2 == 1:
                first = 1
                chosen_col_num += 1

        # CUT PHASE
        if dice_type == 1:
            for r in range(y_start, y_end + 1):
                for c in range(x_start, x_end + 1):
                    cut_pieces.append(states_next_np[r][c])
                    states_next_np[r][c] = 0
        elif dice_type == 2:
            x = (first + 1) % 2
            for r in range(y_start + (0 if is_inside(x, y, dice_type, n, m) else x), y_end + 1, 2):
                for c in range(x_start, x_end + 1):
                    cut_pieces.append(states_next_np[r][c])
                    states_next_np[r][c] = 0
        elif dice_type == 3:
            x = (first + 1) % 2
            for r in range(y_start, y_end + 1):
                for c in range(x_start + (0 if is_inside(x, y, dice_type, n, m) else x), x_end + 1, 2):
                    cut_pieces.append(states_next_np[r][c])
                    states_next_np[r][c] = 0

        # SHIFT PHASE
        if direction == 0:  # Up
            for c in range(x_start, x_end + 1):
                write_index = y_start
                for r in range(y_start, n):
                    if states_next_np[r][c] != 0:
                        states_next_np[write_index][c] = states_next_np[r][c]
                        write_index += 1
                for r in range(write_index, n):
                    states_next_np[r][c] = 0
        elif direction == 1:  # Down
            for c in range(x_start, x_end + 1):
                write_index = y_end
                for r in range(y_end, -1, -1):
                    if states_next_np[r][c] != 0:
                        states_next_np[write_index][c] = states_next_np[r][c]
                        write_index -= 1
                for r in range(write_index, -1, -1):
                    states_next_np[r][c] = 0
        elif direction == 2:  # Left
            for r in range(y_start, y_end + 1):
                write_index = x_start
                for c in range(x_start, m):
                    if states_next_np[r][c] != 0:
                        states_next_np[r][write_index] = states_next_np[r][c]
                        write_index += 1
                for c in range(write_index, m):
                    states_next_np[r][c] = 0
        elif direction == 3:  # Right
            for r in range(y_start, y_end + 1):
                write_index = x_end
                for c in range(x_end, -1, -1):
                    if states_next_np[r][c] != 0:
                        states_next_np[r][write_index] = states_next_np[r][c]
                        write_index -= 1
                for c in range(write_index, -1, -1):
                    states_next_np[r][c] = 0

        # BBBT PHASE
        bxs, bxe, bys, bye = 0, 0, 0, 0
        if dice_type == 1:
            if direction < 2:
                bxs, bxe = x_start, x_end
                bys, bye = (n - height, n - 1) if direction == 0 else (0, height - 1)
            else:
                bys, bye = y_start, y_end
                bxs, bxe = (m - width, m - 1) if direction == 2 else (0, width - 1)
        elif dice_type == 2:
            if direction < 2:
                bxs, bxe = x_start, x_end
                bys, bye = (n - chosen_row_num, n - 1) if direction == 0 else (0, chosen_row_num - 1)
            else:
                bys, bye = y_start, y_end
                bxs, bxe = (m - width, m - 1) if direction == 2 else (0, width - 1)
        elif dice_type == 3:
            if direction < 2:
                bxs, bxe = x_start, x_end
                bys, bye = (n - height, n - 1) if direction == 0 else (0, height - 1)
            else:
                bys, bye = y_start, y_end
                bxs, bxe = (m - chosen_col_num, m - 1) if direction == 2 else (0, chosen_col_num - 1)

        # Fill back the cut pieces
        cnt = 0
        for r in range(bys, bye + 1):
            for c in range(bxs, bxe + 1):
                if states_next_np[r][c] == 0 and cnt < len(cut_pieces):
                    states_next_np[r][c] = cut_pieces[cnt]
                    cnt += 1

        transition_costs = [1.0 for _ in range(states_np.shape[0])]

        return states_next_np, transition_costs

