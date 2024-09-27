from random import randint
from operations import *
from Actions import *
from converter import *


def gen(K):

    # get input
    initial, goal = MapGenerate();  

    # get possible actions
    actions = a(initial.shape.split())

    data = []
    samples = []

    # determine k
    k = randint(1, K)

    current_node = goal

    #creating k samples doing random k moves from final state
    for i in range(k):

        rand_index = randint(0, actions.size() - 1)

        samples.append(current_node)

        current_node = apply_die(current_node, actions[rand_index])

    # looping through k samples we created earlier
    for sample in samples:

        # from each sample state do depth-1 breadth first search and generate target
        for action in actions:
            target = apply_die(sample, action)
            data.append({sample, target})




 


