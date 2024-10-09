import math
import numpy as np
from random import randint
import numpy as np

class GameState:
    def __init__(self, board, dies):
        self.board = board
        self.dies = dies
        self.actions = []

class Board:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.pieces = np.full((height, width), -1)  # Initialize with -1

class Die:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.cells = []  # A 2D list of boolean values representing the die

def generate_fixed_dies():
    dies = []
    MAX_DIMENSION = 16  # Set this to your desired max dimension

    for size in [1 << i for i in range(int(MAX_DIMENSION).bit_length()) if (1 << i) <= MAX_DIMENSION]:
        # Type I Die: All cells are 1
        type1_die = Die(size, size)
        type1_die.cells = [[True for _ in range(size)] for _ in range(size)]
        dies.append(type1_die)

        if size != 1:
            # Type II Die: Even rows are 1, odd rows are 0
            type2_die = Die(size, size)
            type2_die.cells = [[(i % 2 == 0) for _ in range(size)] for i in range(size)]
            dies.append(type2_die)

            # Type III Die: Even columns are 1, odd columns are 0
            type3_die = Die(size, size)
            type3_die.cells = [[(j % 2 == 0) for j in range(size)] for _ in range(size)]
            dies.append(type3_die)

    return dies
# Define the Action class to hold the action data
class Action:
    def __init__(self, x, y, dice_num, dir):
        self.x = x
        self.y = y
        self.dice_num = dice_num
        self.dir = dir

def apply_die(game_state, action):
    punched_pieces = punch_pieces(game_state, action)
    shift_pieces(game_state, action)
    put_back_pieces(game_state, action, punched_pieces)
    game_state.actions.append(action)
    return game_state

def apply_die1(b, action):

    board = np.copy(b)

    # Set up                                        
    power = math.ceil(action.dice_num / 3)                                        
    size = int(math.pow(2, power))     
    if power == 0: dice_type = 1                            
    else: dice_type = action.dice_num - ((power - 1) * 3)                                        

    n, m = board.shape
    # print(f"x:{action.x}, y:{action.y}, dice_num:{action.dice_num}, dir:{action.dir}")
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
    with open("notepad/board.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")
    

    # Save the cut pieces to a text file
    with open("notepad/cutpieces.txt", "w") as file:
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
    with open("notepad/shift.txt", "w") as file:
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

    with open("notepad/bbbt.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")

    return board

def punch_pieces(game_state, action):
    punched_pieces = []
    die = game_state.dies[action.dice_num]
    board = game_state.board

    # Calculate the overlapping region
    start_x = max(0, action.x)
    start_y = max(0, action.y)
    end_x = min(board.shape[0], action.x + die.width)
    end_y = min(board.shape[1], action.y + die.height)

    if action.dir in [2, 3]:  # Left and Right (prioritize y)
        for dy in range(start_y, end_y):
            for dx in range(start_x, end_x):
                die_x = dx - action.x
                die_y = dy - action.y

                if die.cells[die_y][die_x]:
                    punched_pieces.append(board[dy][dx])
                    board[dy][dx] = -1
    else:  # Up and Down (prioritize x)
        for dx in range(start_x, end_x):
            for dy in range(start_y, end_y):
                die_x = dx - action.x
                die_y = dy - action.y

                if die.cells[die_y][die_x]:
                    punched_pieces.append(board[dy][dx])
                    board[dy][dx] = -1

    return punched_pieces

def shift_pieces(game_state, action):
    board = game_state.board

    if action.dir == 2:  # Left
        for y in range(board.shape[0]):
            dest_x = 0
            row_shifted = False  # Flag to track row changes

            for x in range(board.shape[1]):
                if board[y][x] != -1:
                    if x != dest_x:  # Check for change in position
                        board[y][dest_x] = board[y][x]
                        row_shifted = True  # If there are any shifts
                    dest_x += 1

            if row_shifted:
                for fill in range(dest_x, board.shape[1]):
                    board[y][fill] = -1

    elif action.dir == 3:  # Right
        for y in range(board.shape[0]):
            dest_x = board.shape[1] - 1
            row_shifted = False

            for x in range(board.shape[1] - 1, -1, -1):
                if board[y][x] != -1:
                    if x != dest_x:
                        board[y][dest_x] = board[y][x]
                        row_shifted = True
                    dest_x -= 1

            if row_shifted:
                for fill in range(dest_x, -1, -1):
                    board[y][fill] = -1

    elif action.dir == 0:  # Up
        for x in range(board.shape[1]):
            dest_y = 0
            column_shifted = False

            for y in range(board.shape[0]):
                if board[y][x] != -1:
                    if y != dest_y:
                        board[dest_y][x] = board[y][x]
                        column_shifted = True
                    dest_y += 1

            if column_shifted:
                for fill in range(dest_y, board.shape[0]):
                    board[fill][x] = -1

    elif action.dir == 1:  # Down
        for x in range(board.shape[1]):
            dest_y = board.shape[0] - 1
            column_shifted = False

            for y in range(board.shape[0] - 1, -1, -1):
                if board[y][x] != -1:
                    if y != dest_y:
                        board[dest_y][x] = board[y][x]
                        column_shifted = True
                    dest_y -= 1

            if column_shifted:
                for fill in range(dest_y, -1, -1):
                    board[fill][x] = -1

def put_back_pieces(game_state, action, punched_pieces):
    board = game_state.board
    piece_index = 0

    if action.dir in [2, 3]:  # Left and Right
        for y in range(board.shape[0]):
            for x in range(board.shape[1]):
                if board[y][x] == -1 and piece_index < len(punched_pieces):
                    board[y][x] = punched_pieces[piece_index]
                    piece_index += 1

    elif action.dir in [0, 1]:  # Up and Down
        for x in range(board.shape[1]):
            for y in range(board.shape[0]):
                if board[y][x] == -1 and piece_index < len(punched_pieces):
                    board[y][x] = punched_pieces[piece_index]
                    piece_index += 1

def display_game_state(game_state):
    board = game_state.board
    # Iterate over rows of the board
    for row in range(board.shape[0]):
        # Iterate over pieces in each row
        for col in range(board.shape[1]):
            if board[row][col] == -1:
                print(" ", end="")
            else:
                print(board[row][col], end="")
        print()  # New line after each row
    print(f"Number of actions: {len(game_state.actions)}\n")

def nearest(x):
    res = 1
    while x > res:
        res *= 2
    return res

def is_power_of_two(x):
    if x == 1:
        return True
    if x % 2 == 1:
        return False
    return is_power_of_two(x // 2)

def gen_actions(n, m):

    # Define variables
    topX, topY, botX, botY, width, height, mxSide, sz, diceNum = 0, 0, 0, 0, 0, 0, 0, 0, 0

    # Create an empty list for actions and a dictionary for check (as maps in C++ are dictionaries in Python)
    actions = []

    # Ensure that m is the longer side
    if n > m:
        n, m = m, n  # swap n and m

    # urgelj inside the board
    if True:  
        for topY in range(1, n - 1):
            for topX in range(1, m - 1):
                sz = 1
                while sz <= min(n - topY - 1, m - topX - 1):
                    # 3rd diceNum of this size
                    diceNum = math.log2(sz) * 3
                    for s in range(4):
                        if sz == 1:
                            action = Action(topX, topY, int(diceNum), s)
                            actions.append(action)
                        else:
                            for i in range(3):
                                action = Action(topX, topY, int(diceNum - i), s)
                                actions.append(action)
                    sz *= 2

    # topleft corner contained actions
    if True:  
        topX = 0
        topY = 0

        for botY in range(n - 1):
            for botX in range(m - 1):
                width = botX - topX + 1
                height = botY - topY + 1
                mxSide = max(width, height)
                sz = nearest(mxSide)

                # 3rd diceNum of this size
                diceNum = math.log2(sz) * 3

                if sz == 1:
                    for i in range(0, 3, 2):  # i goes 0, 2
                        action = Action(topX, topY, 0, i)
                        actions.append(action)
                    continue

                # TYPE I
                for i in range(0, 3, 2):  # i goes 0, 2
                    action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 2), i)
                    actions.append(action)

                # TYPE II
                if height > 2:
                    for i in range(3):
                        action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(4):
                        if i == 1:
                            continue
                        action = Action(botX - sz + 1, botY - sz + 1, int(diceNum), i)
                        actions.append(action)

    # topright corner contained actions
    if True:  
        topY = 0
        botX = m - 1

        for botY in range(n - 1):
            for topX in range(1, m):  # C++: 'for(topX = 1; topX <= m - 1; topX++)'

                height = botY - topY + 1
                width = botX - topX + 1
                mxSide = max(height, width)
                sz = nearest(mxSide)

                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                if sz == 1:
                    continue

                # TYPE II
                if height > 2:
                    for i in range(2):  # i = 0, 1
                        action = Action(topX, botY - sz + 1, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III
                if width != 1:
                    for i in range(2, 4):  # i = 2, 3
                        action = Action(topX, botY - sz + 1, int(diceNum), i)
                        actions.append(action)

    # bottomleft corner contained actions
    if True:  
        topX = 0
        botY = n - 1

        for topY in range(1, n):  
            for botX in range(m - 1):  

                height = botY - topY + 1
                width = botX - topX + 1
                mxSide = max(height, width)
                sz = nearest(mxSide)

                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                if sz == 1:
                    continue

                # TYPE II
                if height != 1:
                    for i in range(2):  # i = 0, 1
                        action = Action(botX - sz + 1, topY, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(2, 4):  # i = 2, 3
                        action = Action(botX - sz + 1, topY, int(diceNum), i)
                        actions.append(action)

    # bottom-right corner contained actions
    if True:  
        botX = m - 1
        botY = n - 1

        for topY in range(1, n):  
            for topX in range(1, m):  

                height = botY - topY + 1
                width = botX - topX + 1
                mxSide = max(height, width)
                sz = nearest(mxSide)

                if sz == 1:
                    for i in range(1, 4, 2):  # i = 1, 3
                        action = Action(topX, topY, 0, i)
                        actions.append(action)
                    continue

                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE I
                for i in range(1, 4, 2):  # i = 1, 3
                    action = Action(topX, topY, int(diceNum - 2), i)
                    actions.append(action)

                # TYPE II
                if height != 1:
                    for i in range(4):  # i = 0, 1, 2, 3
                        if i == 2:
                            continue
                        action = Action(topX, topY, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III
                if width != 1:
                    for i in range(1, 4):  # i = 1, 2, 3
                        action = Action(topX, topY, int(diceNum), i)
                        actions.append(action)

    # Pairs of corners
    if True:

        # from above
        if True:  # 'if(1)' in C++ is 'if True' in Python
            topX = 0
            topY = 0
            botX = m - 1

            for botY in range(n - 1):  # C++: 'for(botY = 0; botY < n - 1; botY++)'
                width = m
                height = botY - topY + 1
                mxSide = max(width, height)
                sz = nearest(mxSide)
                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE I
                action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 2), 0)
                actions.append(action)

                # TYPE II
                if height > 2:
                    for i in range(2):  # C++: 'for(int i = 0; i < 2; i++)'
                        action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III
                if m % 2 == 0:
                    action1 = Action(botX - sz + 1, botY - sz + 1, int(diceNum), 0)
                    actions.append(action1)
                else:
                    action1 = Action(botX + 1 - sz + 1, botY - sz + 1, int(diceNum), 0)
                    actions.append(action1)

        # from left
        if True:  # 'if(1)' in C++ is 'if True' in Python
            topX = 0
            topY = 0
            botY = n - 1

            for botX in range(m - 1):  # C++: 'for(botX = 0; botX < m - 1; botX++)'
                width = botX - topX + 1
                height = n
                mxSide = max(width, height)
                sz = nearest(mxSide)
                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE I
                action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 2), 2)
                actions.append(action)

                # TYPE II
                if n % 2 == 0:
                    action = Action(botX - sz + 1, botY - sz + 1, int(diceNum - 1), 2)
                    actions.append(action)
                else:
                    action = Action(botX - sz + 1, botY + 1 - sz + 1, int(diceNum - 1), 2)
                    actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(2, 4):  # C++: 'for(int i = 2; i < 4; i++)'
                        action = Action(botX - sz + 1, botY - sz + 1, int(diceNum), i)
                        actions.append(action)

        # from right
        if True:  # 'if(1)' in C++ is 'if True' in Python
            topX = m - 1
            topY = 0
            botY = n - 1

            for botX in range(m - 1, 0, -1):  # C++: 'for(botX = m - 1; botX > 0; botX--)'
                width = topX - botX + 1
                height = n
                mxSide = max(width, height)
                sz = nearest(mxSide)
                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE III
                if width != 1:
                    for i in range(2, 4):
                        action = Action(botX, botY - sz + 1, int(diceNum), i)
                        actions.append(action)

        # from bottom
        if True:  # 'if(1)' in C++ is 'if True' in Python
            topX = 0
            botX = m - 1
            botY = n - 1

            for topY in range(n - 1, 0, -1):  # C++: 'for(topY = n - 1; topY > 0; topY--)'
                width = m
                height = topY - botY + 1
                mxSide = max(width, height)
                sz = nearest(mxSide)
                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE II
                if height != 1:
                    for i in range(2):  # C++: 'for(int i = 0; i < 2; i++)'
                        action = Action(topX, topY, int(diceNum - 1), i)
                        actions.append(action)

                # TYPE III??? (Add logic here as needed)

    # edges
    # Top sejig
    if True:  # 'if(1)' in C++ is 'if True' in Python
        topY = 0
        for topX in range(1, m - 1):  # C++: 'for(topX = 1; topX < m - 1; topX++)'
            width = 1
            while width <= (m - 1 - topX):  # C++: 'for(width = 1; width <= (m - 1 - topX); width *= 2)'
                for height in range(1, min(width + 1, n)): # C++: 'for(height = 1; height <= min(width, n - 1); height++)'
                    sz = width

                    if sz == 1:
                        if topX == 1 or topX == m - 2:
                            continue

                        for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                            if i == 1:
                                continue
                            action = Action(topX, topY, 0, i)
                            actions.append(action)
                        continue

                    # 3rd type diceNum of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                        if i == 1:
                            continue
                        action = Action(topX, height - width, diceNum - 2, i)
                        actions.append(action)

                    # TYPE II
                    if height > 2:
                        for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                            action = Action(topX, height - width, diceNum - 1, i)
                            actions.append(action)

                    # TYPE III
                    if topX == 1 or topX == m - 2:
                        continue
                    if sz == 2 and height == 1:
                        continue
                    for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                        if i == 1:
                            continue
                        action = Action(topX, height - width, diceNum, i)
                        actions.append(action)

                width *= 2  # Move to the next width for the outer loop

    # Bottom edge actions
    if True:
        topY = n - 1
        for topX in range(1, m - 1):
            width = 1
            while width <= (m - 1 - topX):
                for height in range(1, min(width, n - 1) + 1):

                    oh = n - height
                    ow = width

                    sz = width

                    if sz == 1:
                        if topX == 1 or topX == m - 2:
                            continue
                        if oh > ow:
                            action = Action(topX, topY, 0, 1)
                            actions.append(action)
                        for i in range(2, 4):
                            action = Action(topX, topY, 0, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    if oh > ow:
                        action = Action(topX, topY - height + 1, diceNum - 2, 1)
                        actions.append(action)
                    for i in range(2, 4):
                        action = Action(topX, topY - height + 1, diceNum - 2, i)
                        actions.append(action)

                    # TYPE II
                    if height != 1:
                        for i in range(4):
                            action = Action(topX, topY - height + 1, diceNum - 1, i)
                            actions.append(action)

                    # TYPE III
                    if topX == 1 or topX == m - 2:
                        continue
                    if sz == 2 and height == 1:
                        continue
                    if oh > ow:
                        action = Action(topX, topY - height + 1, diceNum, 1)
                        actions.append(action)

                    for i in range(2, 4):
                        action = Action(topX, topY - height + 1, diceNum, i)
                        actions.append(action)

                width *= 2  # Increment width for the next iteration

    # Left edge actions
    if True:
        topX = 0
        for topY in range(1, n - 1):
            height = 1
            while height <= (n - 1 - topY):
                for width in range(1, height + 1):
                    sz = height

                    if sz == 1:
                        if topY == 1 or topY == n - 2:
                            continue
                        for i in range(3):
                            action = Action(width - height, topY, 0, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    for i in range(3):
                        action = Action(width - height, topY, diceNum - 2, i)
                        actions.append(action)

                    # TYPE III
                    if width > 2:
                        for i in range(4):
                            action = Action(width - height, topY, diceNum, i)
                            actions.append(action)

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue
                    for i in range(3):
                        action = Action(width - height, topY, diceNum - 1, i)
                        actions.append(action)

                height *= 2  # Increment height for the next iteration

    # Right edge actions
    if True:
        topX = m - 1
        for topY in range(1, n - 1):
            height = 1
            while height <= (n - 1 - topY):
                for width in range(1, height + 1):
                    sz = height

                    oh = height
                    ow = m - width

                    if sz == 1:
                        if topY == 1 or topY == n - 2:
                            continue
                        if ow > oh:
                            action = Action(topX, topY, 0, 3)
                            actions.append(action)
                        for i in range(2):
                            action = Action(topX, topY, 0, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    if ow > oh:
                        action = Action(topX - width + 1, topY, diceNum - 2, 3)
                        actions.append(action)
                    for i in range(2):
                        action = Action(topX - width + 1, topY, diceNum - 2, i)
                        actions.append(action)

                    # TYPE III
                    if width != 1:
                        for i in range(4):
                            action = Action(topX - width + 1, topY, diceNum, i)
                            actions.append(action)

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue

                    if ow > oh:
                        action = Action(topX - width + 1, topY, diceNum - 1, 3)
                        actions.append(action)

                    for i in range(2):
                        action = Action(topX - width + 1, topY, diceNum - 1, i)
                        actions.append(action)

                height *= 2  # Increment height for the next iteration

    # Pairs of edges
    if True:
        topY = 0
        sz = math.log2(n - 1) + 1

        szz = 2 ** int(sz)
        while szz < m - 1:
            for topX in range(1, m - szz):
                # 3rd type dice of this size
                if n + 1 < m and szz <= m - 2 and topX + szz < m:
                    diceNum = int(math.log2(szz)) * 3

                    # TYPE I
                    for i in range(2, 4):
                        action = (topX, topY, diceNum - 2, i)
                        actions.append(action)

                    # TYPE II
                    for i in range(2, 4):
                        action = (topX, topY, diceNum - 1, i)
                        actions.append(action)

                    if topX == 1 or topX == m - 2:
                        continue

                    # TYPE III
                    for i in range(2, 4):
                        action = (topX, topY, diceNum, i)
                        actions.append(action)

            szz *= 2

    return actions

def is_inside(x, y, dtype, n , m ):
    if dtype == 2:
        return (0 <= y < n)
    else:
        return (0 <= x < m)

def reverse(board, a):
    # Set up     
    n, m = board.shape                                   
    power = math.ceil(a.dice_num / 3)                                        
    size = int(math.pow(2, power))     
    if power == 0: dice_type = 1                            
    else: dice_type = a.dice_num - ((power - 1) * 3)  

    cboard = board.copy()

    if dice_type == 1: return reverse_type_I(a.x, a.y, size, a.dir, n, m, cboard)
    elif dice_type == 2: return reverse_type_II(a.x, a.y, size, a.dir, n, m, cboard)
    else: return reverse_type_III(a.x, a.y, size, a.dir, n, m, cboard)

def reverse_type_I(X, Y, size, s, n, m, board):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    rem = [[] for _ in range(256)]
    chosen = [[] for _ in range(128)]

    if s == 1:
        for index_y in range(h, oby + 1):
            for index_x in range(otx, obx + 1):
                rem[index_y - h].append(board[index_y][index_x])

        for index_y in range(h):
            for index_x in range(otx, obx + 1):
                chosen[index_y].append(board[index_y][index_x])

        for index_y in range(oby + 1):
            cnt = 0
            if index_y < oby - h + 1:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = rem[index_y][cnt]
                    cnt += 1
            else:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = chosen[index_y - (oby - h + 1)][cnt]
                    cnt += 1

    elif s == 0:
        for index_y in range(oty, n - h):
            for index_x in range(otx, obx + 1):
                rem[index_y - oty].append(board[index_y][index_x])

        for index_y in range(n - h, n):
            for index_x in range(otx, obx + 1):
                chosen[index_y - (n - h)].append(board[index_y][index_x])

        for index_y in range(oty, n):
            cnt = 0
            if index_y < oty + h:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = chosen[index_y - oty][cnt]
                    cnt += 1
            else:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = rem[index_y - (oty + h)][cnt]
                    cnt += 1

    elif s == 2:
        count = 0
        for index_y in range(oty, oby + 1):
            for index_x in range(otx, m - w):
                rem[count].append(board[index_y][index_x])
            count += 1

        count = 0
        for index_y in range(oty, oby + 1):
            for index_x in range(m - w, m):
                chosen[count].append(board[index_y][index_x])
            count += 1

        for index_x in range(otx, m):
            cnt = 0
            if index_x <= obx:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1
            else:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = rem[cnt][index_x - (obx + 1)]
                    cnt += 1

    elif s == 3:
        count = 0
        for index_y in range(oty, oby + 1):
            for index_x in range(w, obx + 1):
                rem[count].append(board[index_y][index_x])
            count += 1

        count = 0
        for index_y in range(oty, oby + 1):
            for index_x in range(0, w):
                chosen[count].append(board[index_y][index_x])
            count += 1

        for index_x in range(obx + 1):
            cnt = 0
            if index_x < otx:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = rem[cnt][index_x]
                    cnt += 1
            else:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1

    return board

def reverse_type_II(X, Y, size, s, n, m, board):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_row_num = h // 2
    first = (h + 1) % 2

    if is_inside(X, Y, 2, n, m):
        if not first:
            chosen_row_num += 1
        first = 1
    elif abs(Y) % 2 != 0:
        first = 0
    elif abs(Y) % 2 == 0:
        if h % 2 == 1:
            first = 1
            chosen_row_num += 1

    rem = [[] for _ in range(256)]
    chosen = [[] for _ in range(128)]

    print(f"chosen_row_num:{chosen_row_num}, first:{first}")

    if s == 0:
        rem_cnt, chosen_cnt = 0, 0
        for index_y in range(oty, n - chosen_row_num):
            rem[index_y - oty] = board[index_y][otx:obx+1].tolist()
            rem_cnt += 1
        
        for index_y in range(n - chosen_row_num, n):
            chosen[index_y - (n - chosen_row_num)] = board[index_y][otx:obx+1].tolist()
            chosen_cnt += 1

        cnt = 0
        for index_y in range(oty, min(oty + chosen_row_num * 2, n)):
            if first:
                if chosen_cnt > (cnt // 2):
                    board[index_y][otx:obx+1] = chosen[cnt // 2]
                first = 0
            else:
                if rem_cnt > (cnt // 2):
                    board[index_y][otx:obx+1] = rem[cnt // 2]
                first = 1
            cnt += 1

        index = 0
        for index_y in range(oty + chosen_row_num * 2, n):
            board[index_y][otx:obx+1] = rem[index + chosen_row_num]
            index += 1

    if s == 1:
        chosen_cnt = 0
        for index_y in range(chosen_row_num):
            chosen[index_y] = board[index_y][otx:obx+1].tolist()
            chosen_cnt += 1
        
        rem_cnt = 0
        for index_y in range(chosen_row_num, oby + 1):
            rem[index_y - chosen_row_num] = board[index_y][otx:obx+1].tolist()
            rem_cnt += 1
        
        for index_y in range(oty):
            board[index_y][otx:obx+1] = rem[index_y]
        
        cnt = 0
        for index_y in range(oty, oty + chosen_row_num * 2):
            if first:
                if chosen_cnt > (cnt // 2):
                    board[index_y][otx:obx+1] = chosen[cnt // 2]
                first = 0
            else:
                if rem_cnt > (cnt // 2 + oty):
                    board[index_y][otx:obx+1] = rem[cnt // 2 + oty]
                first = 1
            cnt += 1

        index = 0
        for index_y in range(oty + chosen_row_num * 2, oby + 1):
            board[index_y][otx:obx+1] = rem[index + chosen_row_num + oty]
            index += 1
    if s == 2:
        count = 0
        for index_y in range(oty if first else oty+1, oby + 1, 2):
            for index_x in range(otx, m - w):
                rem[count].append(board[index_y][index_x])
            count += 1
        
        count = 0
        for index_y in range(oty if first else oty+1, oby + 1, 2):
            for index_x in range(m - w, m):
                chosen[count].append(board[index_y][index_x])
            count += 1
        
        for index_x in range(otx, m):
            if index_x <= obx:
                cnt = 0
                for index_y in range(oty if first else oty+1, oby + 1, 2):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1
            else:
                cnt = 0
                for index_y in range(oty if first else oty+1, oby + 1, 2):
                    board[index_y][index_x] = rem[cnt][index_x - (obx + 1)]
                    cnt += 1

    if s == 3:
        count = 0
        for index_y in range(oty if first else oty+1, oby + 1, 2):
            for index_x in range(w, obx + 1):
                rem[count].append(board[index_y][index_x])
            count += 1
        
        count = 0
        for index_y in range(oty if first else oty+1, oby + 1, 2):
            for index_x in range(0, w):
                chosen[count].append(board[index_y][index_x])
            count += 1
        
        for index_x in range(0, obx + 1):
            if index_x < otx:
                cnt = 0
                for index_y in range(oty if first else oty+1, oby + 1, 2):
                    board[index_y][index_x] = rem[cnt][index_x]
                    cnt += 1
            else:
                cnt = 0
                for index_y in range(oty if first else oty+1, oby + 1, 2):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1
    return board

def reverse_type_III(X, Y, size, s, n, m, board):

    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_col_num = w // 2
    first = (w + 1) % 2

    if is_inside(X, Y, 3, n, m):
        if not first:
            chosen_col_num += 1
        first = 1
    elif abs(X) % 2 != 0:
        first = 0
    elif abs(X) % 2 == 0 and w % 2 == 1:
        first = 1
        chosen_col_num += 1

    print(f"chosen_col_num:{chosen_col_num}, first:{first}, size:{size}")

    rem = [[] for _ in range(256)]
    chosen = [[] for _ in range(128)]

    if s == 0:
        for index_y in range(oty, n - h):
            for index_x in range(otx if first else otx + 1, obx + 1, 2):
                rem[index_y - oty].append(board[index_y][index_x])
        for index_y in range(n - h, n):
            for index_x in range(otx if first else otx + 1, obx + 1, 2):
                chosen[index_y - (n - h)].append(board[index_y][index_x])

        for index_y in range(oty, n):
            cnt = 0
            if index_y < oty + h:
                for index_x in range(otx if first else otx + 1, obx + 1, 2):
                    board[index_y][index_x] = chosen[index_y - oty][cnt]
                    cnt += 1
            else:
                for index_x in range(otx if first else otx + 1, obx + 1, 2):
                    board[index_y][index_x] = rem[index_y - (oty + h)][cnt]
                    cnt += 1

    if s == 1:
        for index_y in range(h, oby + 1):
            for index_x in range(otx if first else otx + 1, obx + 1, 2):
                rem[index_y - h].append(board[index_y][index_x])
        for index_y in range(h):
            for index_x in range(otx if first else otx + 1, obx + 1, 2):
                chosen[index_y].append(board[index_y][index_x])

        for index_y in range(oby + 1):
            cnt = 0
            if index_y < oby - h + 1:
                for index_x in range(otx if first else otx + 1, obx + 1, 2):
                    board[index_y][index_x] = rem[index_y][cnt]
                    cnt += 1
            else:
                for index_x in range(otx if first else otx + 1, obx + 1, 2):
                    board[index_y][index_x] = chosen[index_y - (oby - h + 1)][cnt]
                    cnt += 1

    if s == 2:
        rem_cnt, chosen_cnt = 0, 0
        for index_x in range(otx, m - chosen_col_num):
            rem[index_x - otx] = [board[index_y][index_x] for index_y in range(oty, oby + 1)]
            rem_cnt += 1

        for index_x in range(m - chosen_col_num, m):
            chosen[index_x - (m - chosen_col_num)] = [board[index_y][index_x] for index_y in range(oty, oby + 1)]
            chosen_cnt += 1

        cnt = 0
        for index_x in range(otx, otx + chosen_col_num * 2):
            if first:
                for index_y in range(oty, oby + 1):
                    if chosen_cnt > (cnt // 2):
                        board[index_y][index_x] = chosen[(cnt // 2)][index_y - oty]
                first = 0
            else:
                for index_y in range(oty, oby + 1):
                    if rem_cnt > (cnt // 2):
                        board[index_y][index_x] = rem[(cnt // 2)][index_y - oty]
                first = 1
            cnt += 1

        index = 0
        for index_x in range(otx + chosen_col_num * 2, m):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index + chosen_col_num][index_y - oty]
            index += 1

    if s == 3:

        rem_cnt, chosen_cnt = 0, 0

        for index_x in range(chosen_col_num):
            for index_y in range(oty, oby + 1):
                chosen[chosen_cnt].append(board[index_y][index_x])
                print(chosen[chosen_cnt][index_y - oty], end=" ")
            print()
            chosen_cnt += 1

        print(f"chosen_cnt:{chosen_cnt}")

        for index_x in range(chosen_col_num, obx + 1):
            for index_y in range(oty, oby + 1):
                rem[rem_cnt].append(board[index_y][index_x])
                print(rem[rem_cnt][index_y - oty], end=" ")
            print() 
            rem_cnt += 1

        for index_x in range(otx):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index_x][index_y - oty]

        cnt = 0
        for index_x in range(otx, min(otx + chosen_col_num * 2, m)):
            if first:
                for index_y in range(oty, oby + 1):
                    if chosen_cnt > (cnt // 2):
                        board[index_y][index_x] = chosen[cnt // 2][index_y - oty]
                first = 0
            else:
                for index_y in range(oty, oby + 1):
                    if rem_cnt > ((cnt // 2) + otx):
                        board[index_y][index_x] = rem[(cnt // 2) + otx][index_y - oty]
                first = 1
            cnt += 1

        index = 0
        for index_x in range(otx + chosen_col_num * 2, obx + 1):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index + chosen_col_num + otx][index_y - oty]
            index += 1

    return board


    board = np.copy(b)

    # Set up                                        
    power = math.ceil(action.dice_num / 3)                                        
    size = int(math.pow(2, power))     
    if power == 0: dice_type = 1                            
    else: dice_type = action.dice_num - ((power - 1) * 3)                                        

    n, m = board.shape
    # print(f"x:{action.x}, y:{action.y}, dice_num:{action.dice_num}, dir:{action.dir}")
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
    with open("notepad/board.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")
    

    # Save the cut pieces to a text file
    with open("notepad/cutpieces.txt", "w") as file:
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
    with open("notepad/shift.txt", "w") as file:
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

    with open("notepad/bbbt.txt", "w") as file:
        for row in board:
            file.write(" ".join(map(str, row)) + "\n")

    return board

def checker(n, m):

    board = np.arange(1, (n * m) + 1).reshape((n, m))
    forAD = GameState(board , generate_fixed_dies())

    tooluur = 0
    number = 0
    check = {}  # This will be used to store unique board states

    actions = gen_actions(n, m)

    for a in actions:
        # Debug output can be enabled if needed
        # print(f"x: {a.x} y: {a.y} diceType: {a.diceType} dir: {a.dir}")
        next_board = apply_die(forAD, a)

        pre_board = reverse(next_board.board, a)

        err = 0
        for i in range(n):
            for j in range(m):
                if board[i][j] != pre_board[i][j]:
                    print("ERROR")
                    print(f"x: {a.x} y: {a.y} dice_num: {a.dice_num} dir: {a.dir}")

                    print("ORIGINAL BOARD")
                    print(board)

                    print("actioned BOARD")
                    print(next_board)

                    print("reversed BOARD")
                    print(pre_board)

                    err = 1
                    break
            if err: break

        llr = []
        for i in range(n):
            for j in range(m):
                too = next_board.board[i][j]
                llr.append(too)

        # Convert list to tuple to use it as a key in the dictionary
        llr_tuple = tuple(llr)
        if llr_tuple not in check:
            check[llr_tuple] = a
            tooluur += 1
            # print(f"x: {a.x} y: {a.y} dice: {a.dice_num} dir: {a.dir}")  # Uncomment if needed for debugging
        else:
            b = check[llr_tuple]
            print("DAVHARDAJ BAINA")
            print(f"x: {b.x} y: {b.y} dice: {b.dice_num} dir: {b.dir}")  # Duplicate state
            # display_game_state(nextState)  # Uncomment if needed for debugging

        number += 1

    print("BOLOOOOOOOO")
        
def __main__():

    # n = height, m = width
    n = int(input("Enter value for n (height): "))
    m = int(input("Enter value for m (width): "))

    # Ensure that m is the longer side
    if n > m:
        n, m = m, n  # swap n and m

    checker(n, m)

# game.reset(), game.random_action(), game.apply_action(state, action)
# game.is_goal(state), game.get_children(state), game.evaluate_state(state)

class Game:

    def __init__(self, model):
        self.model = model

    def reset(self):
        self.current_state = self.goal_state
        return

    def is_goal(self):
        if self.current_state == self.goal_state: return 1
        else: return 0

    def random_action(self):
        random_index = randint(1, len(self.actions) - 1)
        return (self.actions[random_index])
    
    def apply_die(state, action):
        board = np.copy(state)

        # Set up                                        
        power = math.ceil(action.dice_num / 3)                                        
        size = int(math.pow(2, power))     
        if power == 0: dice_type = 1                            
        else: dice_type = action.dice_num - ((power - 1) * 3)                                        

        n, m = board.shape
        # print(f"x:{action.x}, y:{action.y}, dice_num:{action.dice_num}, dir:{action.dir}")
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
        with open("notepad/board.txt", "w") as file:
            for row in board:
                file.write(" ".join(map(str, row)) + "\n")


        # Save the cut pieces to a text file
        with open("notepad/cutpieces.txt", "w") as file:
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
        with open("notepad/shift.txt", "w") as file:
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

        with open("notepad/bbbt.txt", "w") as file:
            for row in board:
                file.write(" ".join(map(str, row)) + "\n")

        return board

    def encode_state(self, state):
        n, m = state.shape
        one = np.zeros((n, m))
        two = np.zeros((n, m))
        three = np.zeros((n, m))
        four = np.zeros((n, m))

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

        return res

    def evaluate_state(self, state):
        encoded_state = self.encode_state(state)
        output = self.model(encoded_state)
        return output["heuristic_value"]   

    def get_children(self, state):
        children = []
        for action in self.actions:
            child = self.apply_die(state, action)
            children.append((child, action))

        return children


__main__()

