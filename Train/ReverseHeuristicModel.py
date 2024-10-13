import torch
import torch.nn as nn
import torch.optim as optim
import torch.nn.functional as F

import random
from collections import deque

from Actions import *
from tqdm import trange

import numpy as np
import cv2
import random
import os

os.environ["PYTORCH_CUDA_ALLOC_CONF"] = "expandable_segments:True"

class Game:
    def __init__(self, model, device):
        self.model = model
        self.device = device
        self.initial_state_state, self.goal_state = self.gen_board()
        self.encoded_goal_state = self.encode_state(self.goal_state, True)
        self.actions = gen_actions(self.initial_state_state.shape[0], self.initial_state_state.shape[1])

    def random_action(self):
        random_index = randint(1, len(self.actions) - 1)
        return (self.actions[random_index])

    def encode_state(self, state, first):
        n, m = state.shape
        one = np.zeros((256, 256))
        two = np.zeros((256, 256))
        three = np.zeros((256, 256))
        four = np.zeros((256, 256)) 
        # daraa harii
        mp = {
            1 : one,
            2 : two,
            3 : three,
            4 : four
        }   
        for r in range(n):
            for c in range(m):
                x = int(state[r][c])
                mp[x][r][c] = x 

        # daraa harii bur chuhal
        res = np.stack((one, two, three, four))
        res = np.expand_dims(res, axis=0)
        res = torch.tensor(res, dtype=torch.float16).to(self.device)  
          
        # N C W H --> N W H C
        # res = res.permute(0, 2, 3, 1) 
        if np.array_equal(self.goal_state, state) and first:
            return res

        # concat with goal state
        res = torch.cat((res, self.encoded_goal_state), dim=1)

        return res

    def evaluate_state(self, state):
        encoded_state = self.encode_state(state, False)

        with torch.inference_mode():
            output = self.model(encoded_state)

        # REWARD +1; -1
        if np.array_equal(state, self.goal_state):
            output['heuristic_value'] = output['heuristic_value'] + 1
        else:
            output['heuristic_value'] = output['heuristic_value'] - 1

        return output["heuristic_value"]

    def convert_to_4_color_grayscale(self, image_path, width, height):
        """
        Converts an image to a 4-color grayscale array.

        Args:
            image_path: Path to the input image.
            width: Desired width of the output image.
            height: Desired height of the output image.

        Returns:
            A NumPy array representing the 4-color grayscale image.
        """

      # Check if size is within the valid range
        if not 2 <= width <= 256 or not 2 <= height <= 256:
            raise ValueError("Width and height must be between 64 and 256.")

        # Load the image
        img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)

        # Resize the image
        img = cv2.resize(img, (width, height))

        # Quantize the grayscale values to 4 levels
        bins = np.array([0, 64, 128, 192, 256])
        quantized_img = np.digitize(img, bins)

        return quantized_img

    def gen_board(self):
        # Get user input picture path
        image_index = random.randint(0, 25)
        image_path =  os.path.join(os.path.dirname(__file__), "images/" + str(image_index) + ".jpg")

        power = random.randint(5, 5)
        size = pow(2, power)

        width = size
        height = size
        
        # Convert the image
        grayscale_array = self.convert_to_4_color_grayscale(image_path, width, height)

        # Flatten the array and shuffle it
        flat_array = grayscale_array.flatten()
        random.shuffle(flat_array)

        # Reshape the shuffled array to the original dimensions
        shuffled_array = flat_array.reshape(grayscale_array.shape)

        # if random
        random.shuffle(flat_array)
        shuffled_array_prime = flat_array.reshape(grayscale_array.shape)
    
        
        # returns (initial state and goal state)
        return(shuffled_array, shuffled_array_prime)

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

class SigmaX:
    
    def __init__(self, game, device, model, optimizer, policy_loss_fn, value_loss_fn, args):
        self.game = game
        self.device = device
        self.model = model
        self.optimizer = optimizer
        self.policy_loss_fn = policy_loss_fn
        self.value_loss_fn = value_loss_fn
        self.args = args   

    def new_game(self):
        self.game = Game(self.model, self.device)

    # Data generation using ADI
    def generate_training_data(self):
        training_data = []

        scramble_steps = random.randint(2, self.args["num_god"])  # Random number of scramble steps
        state = np.copy(self.game.goal_state)
        
        # Scramble the board by applying random actions
        for _ in range(scramble_steps):
            action = self.game.random_action()
            state = apply_die(state, action)

            # Generate child states and evaluate their values using current model
            best_value = -float('inf')
            best_action = None
                
            for action in self.game.actions:
                child = raction(state, action)
                value = self.game.evaluate_state(child)
                if value > best_value:
                    best_value = value
                    best_action = action

            # Store the state, best action, and value as a training sample
            
            encoded_initial_state = self.game.encode_state(state, False)
            encoded_initial_state = encoded_initial_state.squeeze()

            best_action = np.array([best_action.die_index, (best_action.x + 255), (best_action.y + 255), best_action.direction])
            best_action = torch.tensor(best_action, dtype=torch.int64).to(self.device)

            best_value = torch.tensor(best_value, dtype=torch.float16).to(self.device)

            training_data.append((encoded_initial_state, best_action, best_value))

            #print(f"type of states:{type(encoded_initial_state)}, type of best actions:{type(best_action)}, type of best_values{type(best_value)}")

        print("TRAINING DATA GENERATED")
        return training_data

    def train(self, memory):

        random.shuffle(memory)
        for batchIdx in range(0, len(memory), self.args["batch_size"]):
            sample = memory[batchIdx:min(len(memory) - 1, batchIdx + self.args["batch_size"])] 
            states, best_actions, best_values = zip(*sample)

            # print(states)
            # print(best_actions)
            # print(best_values)

            states = torch.stack(states)

            best_actions = torch.stack(best_actions)

            best_values = torch.stack(best_values)

            print("states shape:", states.shape)
            print("actions shape:", best_actions.shape)
            print("best values shape:", best_values.shape)

            #print(f"type of states:{type(states)}, type of best actions:{type(best_actions)}, type of best_values{type(best_values)}")

            # Forward pass through the model
            outputs = self.model(states)

            # Compute policy loss
            die_loss = self.policy_loss_fn(outputs['die_probs'], best_actions[:, 0])
            x_loss = self.policy_loss_fn(outputs['x_probs'], best_actions[:, 1])
            y_loss = self.policy_loss_fn(outputs['y_probs'], best_actions[:, 2])
            direction_loss = self.policy_loss_fn(outputs['direction_probs'], best_actions[:, 3])

            print("die_prob output shape", outputs['die_probs'].shape)
            print("x_prob output shape", outputs['x_probs'].shape)
            print("y_prob output shape", outputs['y_probs'].shape)
            print("direction_prob output shape", outputs['direction_probs'].shape)

            # Compute value loss
            value_loss = self.value_loss_fn(outputs['heuristic_value'].squeeze(), best_values.squeeze())

            # Total loss (combination of policy and value loss)
            total_loss = die_loss + x_loss + y_loss + direction_loss + value_loss

            # Backpropagation and optimization
            self.optimizer.zero_grad()
            total_loss.backward()
            self.optimizer.step()
            
            print(f"Iteration {batchIdx}, Loss: {total_loss.item()}")

    def learn(self):
        for iteration in range(self.args["num_iterations"]):
            memory = []

            self.new_game()
            self.model.eval()
            for adi_iteration in trange(self.args["num_gen_data"]):
                memory += self.generate_training_data()

            print("DATA SIZE:", len(memory))

            self.model.train()
            for epoch in trange(self.args["num_epochs"]):
                self.train(memory)

        torch.save(self.model.state_dict(), f"model_x.pt")
        torch.save(self.optimizer.state_dict(), f"optimizer_x.pt")
        
def test():
    # Detect if CUDA (GPU) is available, otherwise use CPU
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    input_tensor = torch.randn(1, 4, 256, 256).to(device)  # Batch size of 1, 4 channels, 256x256 input
    input_tensor.to(torch.float16)
    model = PROCONNet().to(device)
    model.eval()
    with torch.inference_mode():
        output = model(input_tensor)

    print("Die Probabilities:", output['die_probs'])
    print("X Probabilities:", output['x_probs'])
    print("Y Probabilities:", output['y_probs'])
    print("direction Probabilities:", output['direction_probs'])
    print("Heuristic Value:", output['heuristic_value'])

    sz1 = len(output['die_probs'])
    sz2 = len(output['x_probs'])  
    sz3 = len(output['y_probs'])
    sz4 = len(output['direction_probs'])
    
    print(type(output['die_probs']))

    die_pro, index_die = torch.sort(output['die_probs'][0], descending=True)
    x_pro, index_x = torch.sort(output['x_probs'][0], descending=True)
    y_pro, index_y = torch.sort(output['y_probs'][0], descending=True)
    dir_pro, index_dir = torch.sort(output['direction_probs'][0], descending=True)

    sz1 = len(die_pro)
    sz2 = len(x_pro)
    sz3 = len(y_pro)
    sz4 = len(dir_pro)

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

    while (i + 1) * (j + 1) * (I + 1) * (J + 1) <= 1000:
        
        highest = [(die_pro[i + 1], 'i'), (x_pro[j + 1], 'j'), (y_pro[I + 1], 'I'), (dir_pro[J + 1], 'J')]

        p = 0
        ind = -1
        for high in highest:
            if high[0] > p:
                p = high[0]
                ind = high[1]

        if ind == 'i':
            i += 1
        elif ind == 'j':
            j += 1
        elif ind == 'I':
            I += 1
        else:
            J += 1
    
    values = []

    for die in range(0, i + 1):
        for x in range(0, j + 1):
            for y in range(0, I + 1):
                for dir in range(0, J + 1):
                    f = die_pro[die] * x_pro[x] * y_pro[y] * dir_pro[dir]
                    s = (index_die[die], index_x[x], index_y[y], index_die[dir])

                    values.append((f, s))

    values.sort(key=lambda x : x[0], reverse=True)

    for i in range(1, 1001):
        print(values[i][0], end=" ")
        print(values[i][1])

def main():

    # Detect if CUDA (GPU) is available, otherwise use CPU
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

    print(device)
    
    # Training hyperparameters
    learning_rate = 0.001
    args = {
        "num_epochs": 10000,
        "num_iterations": 5,    
        "batch_size": 64,
        "num_gen_data": 5,
        "num_god" : 150
    }

    # Instantiate the model, optimizer, and loss functions
    model = PROCONNet().to(device)
    model.to(torch.float16)
    game = Game(model, device)
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    policy_loss_fn = nn.CrossEntropyLoss()  # For policy head
    value_loss_fn = nn.MSELoss()  # For value head

    sigmax = SigmaX(game, device, model, optimizer, policy_loss_fn, value_loss_fn, args)

    sigmax.learn()

main()