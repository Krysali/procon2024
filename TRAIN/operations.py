import math
import numpy as np
from copy import deepcopy
from random import randint
from Actions import *

# action(top_x, top_y, dice_num, action.dir)

def is_inside(x, y, n, m):
    if((x >= 0 and x < m) and (y >= 0 and y < n)): return 1
    else: return 0

def apply_die(b, action):

    board = np.copy(b)

    # Set up                                        
    power = math.ceil(action.dice_num / 3)                                        
    size = int(math.pow(2, power))     
    if power == 0: dice_type = 1                            
    else: dice_type = action.dice_num - ((power - 1) * 3)                                        

    n, m = board.shape

    print(f"dice_num:{action.dice_num}, n:{n}, m:{m}, power:{power}, size:{size}, dice_type:{dice_type}, dir:{action.dir}")

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
        if is_inside(action.x, action.y, n, m):
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
        if is_inside(action.x, action.y, n, m):
            if not first: 
                chosen_col_num += 1 
            first = 1
        elif abs(action.x) % 2 != 0:
            first = 0
        elif abs(action.x) % 2 == 0:
            if width % 2 == 1: # m bsn
                first = 1
                chosen_col_num += 1
    
    print(f"x_start:{x_start}, x_end:{x_end}, ystart:{y_start}, y_end:{y_end}, width:{width}, height:{height}, chosen_row_num:{chosen_row_num}, chosen_col_num:{chosen_col_num}")
    

    # CUT PHASE
    if dice_type == 1:
        for r in range(y_start, y_end + 1):
            for c in range(x_start, x_end + 1):
                cut_pieces.append(board[r][c])
                board[r][c] = 0

    if dice_type == 2:
        x = (first + 1) % 2
        if is_inside(action.x, action.y, n, m):
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
        if is_inside(action.x, action.y, n, m):
            for r in range(y_start, y_end + 1):
                for c in range(x_start, x_end + 1, 2):
                    cut_pieces.append(board[r][c])
                    board[r][c] = 0
        else:
            for r in range(y_start, y_end + 1):
                for c in range(x_start + x, x_end + 1, 2):
                    cut_pieces.append(board[r][c])
                    board[r][c] = 0

    print(cut_pieces)
    print(board)

    # Save the board after cut to a text file
    with open("board.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")
    

    # Save the cut pieces to a text file
    with open("cutpieces.txt", "w") as file:
            file.write(" ".join(map(str, cut_pieces)) + "\n")


    # SHIFT PHASE

    if action.dir == 0:
        for c in range(x_start, x_end + 1):
            write_index = y_start
            for r in range(y_start, n):
                if board[r][c] != 0:
                    board[write_index][c] = board[r][c]
                    write_index += 1
            for r in range(write_index, n):
                board[r][c] = 0

    if action.dir == 1:
        for c in range(x_start, x_end + 1):
            write_index = y_end
            for r in range(y_end, -1, -1):
                if board[r][c] != 0:
                    board[write_index][c] = board[r][c]
                    write_index -= 1
            for r in range(write_index, -1, -1):
                board[r][c] = 0

    if action.dir == 2:
        for r in range(y_start, y_end + 1):
            write_index = x_start
            for c in range(x_start, m):
                if board[r][c] != 0:
                    board[r][write_index] = board[r][c]
                    write_index += 1
            for c in range(write_index, m):
                board[r][c] = 0

    if action.dir == 3:
        for r in range(y_start, y_end + 1):
            write_index = x_end
            for c in range(x_end, -1, -1):
                if board[r][c] != 0:
                    board[r][write_index] = board[r][c]
                    write_index -= 1
            for c in range(write_index, -1, -1):
                board[r][c] = 0

    # Save the shifted board to a text file
    with open("shift.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")

    # BBBT PHASE
    bxs = 0
    bxe = 0
    bys = 0
    bye = 0

    if dice_type == 1:
        if action.dir < 2:
            bxs = x_start
            bxe = x_end
            if action.dir == 0:
                bys = n - height
                bye = n - 1
            else:
                bys = 0
                bye = height - 1
        else:
            bys = y_start
            bye = y_end
            if action.dir == 2:
                bxs = m - width
                bxe = m - 1
            else:
                bxs = 0
                bxe = width - 1
    if dice_type == 2:
        if action.dir < 2:
            bxs = x_start
            bxe = x_end
            if action.dir == 0:
                bys = n - chosen_row_num
                bye = n - 1
            else:
                bys = 0
                bye = chosen_row_num - 1
        else:
            bys = y_start
            bye = y_end
            if action.dir == 2:
                bxs = m - width
                bxe = m - 1
            else:
                bxs = 0
                bxe = width - 1
    if dice_type == 3:
        if action.dir < 2:
            bxs = x_start
            bxe = x_end
            if action.dir == 0:
                bys = n - height
                bye = n - 1
            else:
                bys = 0
                bye = height - 1
        else:
            bys = y_start
            bye = y_end
            if action.dir == 2:
                bxs = m - chosen_col_num
                bxe = m - 1
            else:
                bxs = 0
                bxe = chosen_col_num - 1
    
    print(f"bxs:{bxs}, bxe:{bxe}, bys:{bys}, bye:{bye}")

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

    with open("bbbt.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")
            
    return board


"""
# Define the Action class
class Action:
    def __init__(self, x, y, dice_num, dir):
        self.x = x
        self.y = y
        self.dice_num = dice_num
        self.dir = dir


A = np.arange(1, 37)
A = A.reshape((6, 6))

action = Action(1, 1, 12, 1)

B = apply_die(A, action)
print(B)
"""
