### GameState


###### Solve with A* search, use --verbose for more information

python3 search_method/Astar.py --states encoded_data/32x32/0.pkl --goal_states encoded_data/32x32/0.pkl --height 4 --width 4 --model_dir model_0.pt --env GameState --weight 0.8 --batch_size 20000 --results_dir results/procon/ --language cpp --nnet_batch_size 10000

# ###### Compare solutions to shortest path

# python scripts/compare_solutions.py --soln1 data/cube3/test/data_0.pkl --soln2 results/cube3/results.pkl

