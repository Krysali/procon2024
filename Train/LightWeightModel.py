import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

import random
from collections import deque

from Actions import *

class PROCONNet(nn.Module):
    def __init__(self):
        super(PROCONNet, self).__init__()

        # Convolutional layers for feature extraction
        self.conv1 = nn.Conv2d(in_channels=4, out_channels=32, kernel_size=3, padding=1)  # (256, 256, 4) -> (256, 256, 32)
        self.conv2 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3, padding=1)  # -> (256, 256, 64)
        self.conv3 = nn.Conv2d(in_channels=64, out_channels=128, kernel_size=3, padding=1)  # -> (256, 256, 128)

        # Batch normalization
        self.batch_norm = nn.BatchNorm2d(64)

        # Max pooling layer
        self.max_pool = nn.MaxPool2d(kernel_size=2, stride=2)  # Reduces size to (128, 128)

        # Fully connected layers
        self.fc1 = nn.Linear(128 * 128 * 128, 1024)  # Input size: 128 channels * 128 * 128
        self.fc2 = nn.Linear(1024, 512)

        # Policy head
        self.policy_die = nn.Linear(512, 25)  # Die selection: 25 options
        self.policy_x = nn.Linear(512, 511)    # X-coordinate: 511 options
        self.policy_y = nn.Linear(512, 511)    # Y-coordinate: 511 options
        self.policy_direction = nn.Linear(512, 4)  # Direction: 4 options

        # Value head
        self.value_head = nn.Linear(512, 1)  # Heuristic value

    def forward(self, x):
        # Convolutional layers with ReLU activation and max pooling
        x = F.relu(self.conv1(x))
        x = F.relu(self.batch_norm(self.conv2(x)))
        x = F.relu(self.conv3(x))
        x = self.max_pool(x)

        # Flatten the output for the fully connected layers
        x = x.view(x.size(0), -1)  # Flatten

        # Fully connected layers
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))

        # Policy outputs
        die_probs = F.softmax(self.policy_die(x), dim=-1)
        x_probs = F.softmax(self.policy_x(x), dim=-1)
        y_probs = F.softmax(self.policy_y(x), dim=-1)
        direction_probs = F.softmax(self.policy_direction(x), dim=-1)

        # Value output
        heuristic_value = self.value_head(x)

        return {
            'die_probs': die_probs,
            'x_probs': x_probs,
            'y_probs': y_probs,
            'direction_probs': direction_probs,
            'heuristic_value': heuristic_value
        }

# BFS to evaluate the value of states and generate targets
def bfs_evaluate(game, state):
    queue = deque([(state, 0)])  # (state, distance from goal)
    visited = set()
    while queue:
        current_state, depth = queue.popleft()
        if game.is_goal(current_state):
            return depth  # Return the distance (value) when goal is reached
        for child in game.get_children(current_state):
            if child not in visited:
                visited.add(child)
                queue.append((child, depth + 1))
    return float('inf')  # If no goal found, return large value

# Data generation using ADI
def generate_training_data(game, num_samples):
    training_data = []
    for _ in range(num_samples):
        game.reset()  # Start from the solved state
        scramble_steps = random.randint(1, 30)  # Random number of scramble steps
        state = game.get_current_state()
        
        # Scramble the board by applying random actions
        for _ in range(scramble_steps):
            action = game.random_action()
            state = game.apply_action(state, action)

        # Generate child states and evaluate their values using BFS
        children = game.get_children(state)
        best_value = -float('inf')
        best_action = None
        for action, child_state in children:
            value = game.evaluate(child_state)
            if value > best_value:
                best_value = value
                best_action = action
        
        # Store the state, best action, and value as a training sample
        training_data.append((state, best_action, best_value))

    return training_data

# Example usage
def test():
    model = PROCONNet()
    input_tensor = torch.randn(3, 4, 256, 256)  # Batch size of 1, 4 channels, 256x256 input
    output = model(input_tensor)

    print("Die Probabilities:", output['die_probs'])
    print("X Probabilities:", output['x_probs'])
    print("Y Probabilities:", output['y_probs'])
    print("Direction Probabilities:", output['direction_probs'])
    print("Heuristic Value:", output['heuristic_value'])

test()