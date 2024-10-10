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

class Game:
    def __init__(self, model, device):
        self.model = model
        self.device = device
        self.initial_state_state, self.goal_state = self.gen_board()
        self.actions = gen_actions(self.initial_state_state.shape[0], self.initial_state_state.shape[1])

    def random_action(self):
        random_index = randint(1, len(self.actions) - 1)
        return (self.actions[random_index])
    
    def apply_die(self, state, action):
        board = np.copy(state)

        # Set up                                        
        power = math.ceil(action.die_index / 3)                                        
        size = int(math.pow(2, power))     
        if power == 0: dice_type = 1                            
        else: dice_type = action.die_index - ((power - 1) * 3)                                        

        n, m = board.shape
        # print(f"x:{action.x}, y:{action.y}, die_index:{action.die_index}, direction:{action.direction}")
        # print(f"power:{power}, size:{size}, dice_type:{dice_type}")

        cut_pieces = []

        x_start = max(action.x, 0)
        y_start = max(action.y, 0)
        x_end = min(m, action.x + size) - 1
        y_end = min(n, action.y + size) - 1

        width = x_end - x_start + 1
        height = y_end - y_start + 1

        chosen_row_num = math.floor(height / 2)
        chosen_col_num = math.floor(width / 2)
        first = 1

        if dice_type == 2:
            first = (height + 1) % 2
            if is_inside(action.x, action.y, dice_type, n, m):
                if not first:
                    chosen_row_num += 1
                first = 1
            elif abs(action.y) % 2 != 0:
                first = 0
            elif abs(action.y) % 2 == 0:
                if height % 2 == 1: #n bsn
                    first = 1
                    chosen_row_num += 1

        if dice_type == 3:
            first = (width + 1) % 2
            if is_inside(action.x, action.y, dice_type, n, m):
                if not first: 
                    chosen_col_num += 1 
                first = 1
            elif abs(action.x) % 2 != 0:
                first = 0
            elif abs(action.x) % 2 == 0:
                if width % 2 == 1: # m bsn
                    first = 1
                    chosen_col_num += 1

        #print(f"x_start:{x_start}, x_end:{x_end}, ystart:{y_start}, y_end:{y_end}, width:{width}, height:{height}, chosen_row_num:{chosen_row_num}, chosen_col_num:{chosen_col_num}")


        # CUT PHASE
        if dice_type == 1:
            for r in range(y_start, y_end + 1):
                for c in range(x_start, x_end + 1):
                    cut_pieces.append(board[r][c])
                    board[r][c] = 0

        if dice_type == 2:
            x = (first + 1) % 2
            if is_inside(action.x, action.y, dice_type, n, m):
                for r in range(y_start, y_end + 1, 2):
                    for c in range(x_start, x_end + 1):
                        cut_pieces.append(board[r][c])
                        board[r][c] = 0
            else:
                for r in range(y_start + x, y_end + 1, 2):
                    for c in range(x_start, x_end + 1):
                        cut_pieces.append(board[r][c])
                        board[r][c] = 0

        if dice_type == 3:
            x = (first + 1) % 2
            if is_inside(action.x, action.y, dice_type, n, m):
                for r in range(y_start, y_end + 1):
                    for c in range(x_start, x_end + 1, 2):
                        cut_pieces.append(board[r][c])
                        board[r][c] = 0
            else:
                for r in range(y_start, y_end + 1):
                    for c in range(x_start + x, x_end + 1, 2):
                        cut_pieces.append(board[r][c])
                        board[r][c] = 0

        #print(cut_pieces)
        #print(board)

        # Save the board after cut to a text file
        with open("Train/notepad/board.txt", "w") as file:
            for row in board:
                file.write(" ".join(map(str, row)) + "\n")


        # Save the cut pieces to a text file
        with open("Train/notepad/cutpieces.txt", "w") as file:
                file.write(" ".join(map(str, cut_pieces)) + "\n")


        # SHIFT PHASE

        if action.direction == 0:
            for c in range(x_start, x_end + 1):
                write_index = y_start
                for r in range(y_start, n):
                    if board[r][c] != 0:
                        board[write_index][c] = board[r][c]
                        write_index += 1
                for r in range(write_index, n):
                    board[r][c] = 0

        if action.direction == 1:
            for c in range(x_start, x_end + 1):
                write_index = y_end
                for r in range(y_end, -1, -1):
                    if board[r][c] != 0:
                        board[write_index][c] = board[r][c]
                        write_index -= 1
                for r in range(write_index, -1, -1):
                    board[r][c] = 0

        if action.direction == 2:
            for r in range(y_start, y_end + 1):
                write_index = x_start
                for c in range(x_start, m):
                    if board[r][c] != 0:
                        board[r][write_index] = board[r][c]
                        write_index += 1
                for c in range(write_index, m):
                    board[r][c] = 0

        if action.direction == 3:
            for r in range(y_start, y_end + 1):
                write_index = x_end
                for c in range(x_end, -1, -1):
                    if board[r][c] != 0:
                        board[r][write_index] = board[r][c]
                        write_index -= 1
                for c in range(write_index, -1, -1):
                    board[r][c] = 0

        # Save the shifted board to a text file
        with open("Train/notepad/shift.txt", "w") as file:
            for row in board:
                file.write(" ".join(map(str, row)) + "\n")

        # BBBT PHASE
        bxs = 0
        bxe = 0
        bys = 0
        bye = 0

        if dice_type == 1:
            if action.direction < 2:
                bxs = x_start
                bxe = x_end
                if action.direction == 0:
                    bys = n - height
                    bye = n - 1
                else:
                    bys = 0
                    bye = height - 1
            else:
                bys = y_start
                bye = y_end
                if action.direction == 2:
                    bxs = m - width
                    bxe = m - 1
                else:
                    bxs = 0
                    bxe = width - 1
        if dice_type == 2:
            if action.direction < 2:
                bxs = x_start
                bxe = x_end
                if action.direction == 0:
                    bys = n - chosen_row_num
                    bye = n - 1
                else:
                    bys = 0
                    bye = chosen_row_num - 1
            else:
                bys = y_start
                bye = y_end
                if action.direction == 2:
                    bxs = m - width
                    bxe = m - 1
                else:
                    bxs = 0
                    bxe = width - 1
        if dice_type == 3:
            if action.direction < 2:
                bxs = x_start
                bxe = x_end
                if action.direction == 0:
                    bys = n - height
                    bye = n - 1
                else:
                    bys = 0
                    bye = height - 1
            else:
                bys = y_start
                bye = y_end
                if action.direction == 2:
                    bxs = m - chosen_col_num
                    bxe = m - 1
                else:
                    bxs = 0
                    bxe = chosen_col_num - 1

        #print(f"bxs:{bxs}, bxe:{bxe}, bys:{bys}, bye:{bye}")

        if dice_type == 1:
            cnt = 0
            for r in range(bys, bye + 1):
                for c in range(bxs, bxe + 1):
                    board[r][c] = cut_pieces[cnt]
                    cnt += 1
        elif dice_type == 2:
            cnt = 0
            for r in range(bys, bye + 1):
                for c in range(bxs, bxe + 1):
                    if board[r][c] == 0:
                        board[r][c] = cut_pieces[cnt]
                        cnt += 1
        else:
            cnt = 0
            for r in range(bys, bye + 1):
                for c in range(bxs, bxe + 1):
                    if board[r][c] == 0:
                        board[r][c] = cut_pieces[cnt]
                        cnt += 1

        for r in range(n):
            for c in range(m):
                if board[r][c] == 0:
                    print("AAAAAAIIIIIIIIIIIINNNNNNNNN")
                    # Save the shuffled array to a text file

        with open("Train/notepad/bbbt.txt", "w") as file:
            for row in board:
                file.write(" ".join(map(str, row)) + "\n")

        return board

    def encode_state(self, state):
        n, m = state.shape
        one = np.zeros((256, 256))
        two = np.zeros((256, 256))
        three = np.zeros((256, 256))
        four = np.zeros((256, 256))

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

        res = np.stack((one, two, three, four), axis=-1)
        res = np.expand_dims(res, axis=0)
        res = torch.tensor(res, dtype=torch.float16).to(self.device)
        res = res.permute(0, 3, 1, 2)

        print(res.shape)

        return res
    
    def encode_state_opt(self, state):
        res = np.zeros((256, 256))
        n, m = state.shape
        for r in range(n):
            for c in range(m):
                res[r][c] = state[r][c]
        
        res = torch.tensor(res, dtype=torch.float16).to(self.device)
        res = res.unsqueeze(dim=0)
        print(res.shape)

        return res

    def evaluate_state(self, state):
        encoded_state = self.encode_state(state)#float16
        with torch.inference_mode():
            output = self.model(encoded_state)
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
        print(width)
        print(height)

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
        image_path = "Train/images/" + str(image_index) + ".jpg"

        is_noise = random.randint(0, 1)
        power = random.randint(1, 1)
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

        # Save the initial state to a text file
        with open("Train/notepad/initial.txt", "w") as file:
            for row in shuffled_array:
                file.write(" ".join(map(str, row)) + "\n")

        # returns (initial state and goal state)
        if is_noise:
            # shuffle again
            random.shuffle(flat_array)
            shuffled_array_prime = flat_array.reshape(grayscale_array.shape)

            # Save the goal state to a text file
            with open("Train/notepad/goal.txt", "w") as file:
                for row in shuffled_array_prime:
                    file.write(" ".join(map(str, row)) + "\n")

            return(shuffled_array, shuffled_array_prime)
        else:

            # Save the goal state to a text file
            with open("Train/notepad/goal.txt", "w") as file:
                for row in grayscale_array:
                    file.write(" ".join(map(str, row)) + "\n")

            return (shuffled_array, grayscale_array)

class PROCONNet(nn.Module):
    def __init__(self):
        super(PROCONNet, self).__init__()

        # Convolutional layers for feature extraction
        self.conv1 = nn.Conv2d(in_channels=1, out_channels=16, kernel_size=3, padding=1,dtype=float)  # (256, 256, 1) -> (256, 256, 32)
        self.conv2 = nn.Conv2d(in_channels=16, out_channels=32, kernel_size=3, padding=1,dtype=float)  # -> (256, 256, 64)
        self.conv3 = nn.Conv2d(in_channels=32, out_channels=64, kernel_size=3, padding=1,dtype=float)  # -> (256, 256, 128)

        # Batch normalization
        self.batch_norm = nn.BatchNorm2d(32)

        # Max pooling layer
        self.max_pool = nn.MaxPool2d(kernel_size=2, stride=2)  # Reduces size to (128, 128)

        # Fully connected layers
        self.fc1 = nn.Linear(128 * 128 * 64, 1024,dtype=float)  # Input size: 128 channels * 128 * 128
        self.fc2 = nn.Linear(1024, 512,dtype=float)

        # Policy head
        self.policy_die = nn.Linear(512, 25,dtype=float)  # Die selection: 25 options
        self.policy_x = nn.Linear(512, 511,dtype=float)    # X-coordinate: 511 options
        self.policy_y = nn.Linear(512, 511,dtype=float)    # Y-coordinate: 511 options
        self.policy_direction = nn.Linear(512, 4,dtype=float)  # direction: 4 options

        # Value head
        self.value_head = nn.Linear(512, 1,dtype=float)  # Heuristic value

    def forward(self, x):
        # Convolutional layers with ReLU activation and max pooling
        x = F.relu(self.conv1(x))

        print("dim DEBUG")
        print(x.shape)  # Debugging the shape
        # Ensure the input is 4D
        if x.dim() == 3:  # If input is 3D
            x = x.unsqueeze(1)  # Add a channel dimension

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

    # Data generation using ADI
    def generate_training_data(self):
        training_data = []

        scramble_steps = random.randint(1, self.args["num_god"])  # Random number of scramble steps
        state = np.copy(self.game.goal_state)
        
        # Scramble the board by applying random actions
        for _ in range(scramble_steps):
            action = self.game.random_action()
            state = self.game.apply_die(state, action)

            # Generate child states and evaluate their values using current model
            best_value = -float('inf')
            best_action = None
                
            for action in self.game.actions:
                child = self.game.apply_die(state, action)
                encoded_state = self.game.encode_state_opt(child)
                value = self.game.evaluate_state(encoded_state)
                if value > best_value:
                    best_value = value
                    best_action = action

            print(state)
            print(f"x:{best_action.x}, y:{best_action.y}, dice_index:{best_action.die_index}, direction:{best_action.direction}")
            print(best_action)

            # Store the state, best action, and value as a training sample

            best_action = np.array([best_action.die_index, (best_action.x + 255), (best_action.y + 255), best_action.direction])
            best_action = torch.tensor(best_action, dtype=torch.float16).to(self.device)
            print(best_action)

            encoded_state = self.game.encode_state_opt(state)
            encoded_state = encoded_state.squeeze()

            best_value = torch.tensor(best_value, dtype=torch.float16).to(self.device)

            training_data.append((encoded_state, best_action, best_value))

            print(f"type of states:{type(encoded_state)}, type of best actions:{type(best_action)}, type of best_values{type(best_value)}")
            # del best_action
            # del best_value
            # del encoded_state

        print("TRAINING DATA GENERATED")
        return training_data

    def train(self, memory):

        random.shuffle(memory)
        for batchIdx in range(0, len(memory), self.args["batch_size"]):
            sample = memory[batchIdx:min(len(memory) - 1, batchIdx + self.args["batch_size"])] 
            states, best_actions, best_values = zip(*sample)

            print(states)
            print(best_actions)
            print(best_values)

            states = torch.stack(states)

            best_actions = torch.stack(best_actions)

            best_values = list(best_values)
            best_values = torch.stack(best_values)

            print(f"type of states:{type(states)}, type of best actions:{type(best_actions)}, type of best_values{type(best_values)}")
            
            #states = torch.tensor(states, dtype=torch.float32).to(self.device)
            # best_actions = np.array(best_actions)
            # best_actions = torch.tensor(best_actions, dtype=torch.float32).to(self.device)
            # #best_values = np.array(best_values).reshape(-1, 1)
            # best_values = torch.tensor(best_values, dtype=torch.float32).to(self.device)

            # Forward pass through the model
            outputs = self.model(states)

            # Compute policy loss
            die_loss = self.policy_loss_fn(outputs['die_probs'], best_actions[:, 0].long())
            x_loss = self.policy_loss_fn(outputs['x_probs'], best_actions[:, 1].long())
            y_loss = self.policy_loss_fn(outputs['y_probs'], best_actions[:, 2].long())
            direction_loss = self.policy_loss_fn(outputs['direction_probs'], best_actions[:, 3].long())
            # Compute value loss
            value_loss = self.value_loss_fn(outputs['heuristic_value'].squeeze(), best_values)
            # Total loss (combination of policy and value loss)
            total_loss = die_loss + x_loss + y_loss + direction_loss + value_loss

            # Backpropagation and optimization
            self.optimizer.zero_grad()
            total_loss.backward()
            self.optimizer.step()
            # del die_loss
            # del x_loss
            # del y_loss
            # del direction_loss
            # del value_loss
            print(f"Iteration {batchIdx}, Loss: {total_loss.item()}")
        #     del total_loss
        # del outputs
        # del states
        # del best_actions
        # del best_values

    def learn(self):
        for iteration in range(self.args["num_iterations"]):
            memory = []

            self.model.eval()
            for adi_iteration in trange(self.args["num_gen_data"]):
                memory += self.generate_training_data()

            print("DATA SIZE:", len(memory))

            self.model.train()
            for epoch in trange(self.args["num_epochs"]):
                self.train(memory)

            torch.save(self.model.state_dict(), f"model_{iteration}.pt")
            torch.save(self.optimizer.state_dict(), f"optimizer_{iteration}.pt")
        # del memory
        
# Example usage
def test():
    # Detect if CUDA (GPU) is available, otherwise use CPU
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    model = PROCONNet().to(device)
    model.to(torch.float16)
    input_tensor = torch.randn(3, 1, 256, 256).to(device)  # Batch size of 1, 4 channels, 256x256 input
    
    model.eval()
    with torch.inference_mode():
        output = model(input_tensor)

    print("Die Probabilities:", output['die_probs'])
    print("X Probabilities:", output['x_probs'])
    print("Y Probabilities:", output['y_probs'])
    print("direction Probabilities:", output['direction_probs'])
    print("Heuristic Value:", output['heuristic_value'])

def main():

    # Detect if CUDA (GPU) is available, otherwise use CPU
    device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
    
    
    print(device)
    # Training hyperparameters
    learning_rate = 0.001
    args = {
        "num_epochs": 100,
        "num_iterations": 1,
        "batch_size": 32,
        "num_gen_data": 1,
        "num_god" : 3
    }

    # Instantiate the model, optimizer, and loss functions
    model = PROCONNet().to(device)
    model.to(dtype=torch.float16)
    game = Game(model, device)
    optimizer = optim.Adam(model.parameters(), lr=learning_rate)
    policy_loss_fn = nn.CrossEntropyLoss()  # For policy head
    value_loss_fn = nn.MSELoss()  # For value head

    sigmax = SigmaX(game, device, model, optimizer, policy_loss_fn, value_loss_fn, args)
    sigmax.learn()

main()