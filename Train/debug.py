import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

import random

from Actions import *
from tqdm import trange

import numpy as np
import os

import pickle

class PROCONNet(nn.Module):
    def __init__(self):
        super(PROCONNet, self).__init__()

        # Convolutional layers for feature extraction
        self.conv1 = nn.Conv2d(in_channels=8, out_channels=32, kernel_size=3, padding=1)  # (256, 256, 8) -> (256, 256, 32)
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
        self.policy_direction = nn.Linear(512, 4)  # direction: 4 options

        # Value head
        self.value_head = nn.Linear(512, 1)  # Heuristic value

    def forward(self, x):
        # Convolutional layers with ReLU activation and max pooling
        x = F.relu(self.conv1(x))
        x = F.relu(self.batch_norm(self.conv2(x)))
        x = F.relu(self.conv3(x))
        x = self.max_pool(x)

        # Flatten the output for the fully connected layers
        x = x.reshape(x.size(0), -1)  # Flatten

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




model = PROCONNet()


def topK(input_tensor, K):

        model.eval()
        with torch.inference_mode():
            output = model(input_tensor)

        print(type(output['die_probs']))

        die_pro, index_die = torch.sort(output['die_probs'][0], descending=True)
        x_pro, index_x = torch.sort(output['x_probs'][0], descending=True)
        y_pro, index_y = torch.sort(output['y_probs'][0], descending=True)
        dir_pro, index_dir = torch.sort(output['direction_probs'][0], descending=True)

        die_pro = torch.cat((die_pro, torch.tensor([-1])))
        x_pro = torch.cat((x_pro, torch.tensor([-1])))
        y_pro = torch.cat((y_pro, torch.tensor([-1])))
        dir_pro = torch.cat((dir_pro, torch.tensor([-1])))

        print(die_pro)
        print(die_pro, index_die)

        i = 0
        j = 0
        I = 0
        J = 0

        while (i + 1) * (j + 1) * (I + 1) * (J + 1) <= K:

            highest = [(die_pro[i], 'i'), (x_pro[j], 'j'), (y_pro[I], 'I'), (dir_pro[J], 'J')]

            p = 0
            ind = -1
            for high in highest:
                if high[0] > p:
                    p = high[0]
                    ind = high[1]

            if ind == 'i':
                if i == 24:
                    die_pro[i] = -float('inf')
                else: i += 1
            elif ind == 'j':
                if j == 510:
                    x_pro[j] = -float('inf')
                else: j += 1
            elif ind == 'I':
                if I == 510:
                    y_pro[I] = -float('inf')
                else: I += 1
            else:
                if J == 3:
                    dir_pro[J] = -float('inf')
                else: J += 1

        values = []

        for die in range(0, i + 1):
            for x in range(0, j + 1):
                for y in range(0, I + 1):
                    for dir in range(0, J + 1):
                        f = die_pro[die] * x_pro[x] * y_pro[y] * dir_pro[dir]
                        s = (index_die[die], index_x[x], index_y[y], index_dir[dir])

                        values.append((f, s))

        values.sort(key=lambda x : x[0], reverse=True)

        res = []
        for i in range(0, K + 1):
            res.append(Action(values[i][1][0], values[i][1][1], values[i][1][2], values[i][1][3]))
            print(values[i][0], end=" ")
            print(values[i][1])

        return res



input_tensor = torch.randn(1, 8, 256, 256)
res = topK(input_tensor, 1000)

print("TEST")
for action in res:

    print(f"die_index:{action.die_index}, x:{action.x}, y:{action.y}, dir:{action.direction}")