import math
import numpy as np

# action(top_x, top_y, dice_num, action.dir)

def is_inside(x, y, n, m):
    if((x >= 0 and x < m) and (y >= 0 and y < n)): return 1
    else: return 0

def apply_die(board, action):

    # Set up
    power = math.ceil(action.dice_num / 3)
    size = int(math.pow(2, power))
    dice_type = action.dice_num - ((power - 1) * 3)

    n, m = board.shape

    print(f"n:{n}, m:{m}, power:{power}, size:{size}, dice_type:{dice_type}")

    cut_pieces = []

    x_start = max(action.x, 0)
    y_start = max(action.y, 0)
    x_end = min(m, action.x + size) - 1
    y_end = min(n, action.y + size) - 1

    width = x_end - x_start + 1
    height = y_end - y_start + 1

    chosenRowNum = height // 2
    chosenColNum = width // 2


    if(is_inside(action.x, action.y, n, m)):
        if (height % 2 == 1): chosenRowNum += 1
        if (width % 2 == 1): chosenColNum += 1
    else:
        if(abs(action.y) % 2 == 0):
            if(n % 2 == 1): chosenRowNum += 1
        if(abs(action.x) % 2 == 0):
            if(m % 2 == 1): chosenColNum += 1

    
    print(f"x_start:{x_start}, x_end:{x_end}, ystart:{y_start}, y_end:{y_end}, width:{width}, height:{height}, chosenRowNum:{chosenRowNum}, chosenColNum:{chosenColNum}")

    # CUT PHASE
    if dice_type == 1:
        for r in range(y_start, y_end + 1):
            for c in range(x_start, x_end + 1):
                cut_pieces.append(board[r][c])
                board[r][c] = 0

    if dice_type == 2:
        for r in range(y_end - 1, y_start - 1, -2):
            for c in range(x_end, x_start - 1, -1):
                cut_pieces.append(board[r][c])
                board[r][c] = 0
        cut_pieces = np.flip(cut_pieces, 0)

    if dice_type == 3:
        for r in range(y_end, y_start - 1, -1):
            for c in range(x_end - 1, x_start - 1, -2):
                cut_pieces.append(board[r][c])
                board[r][c] = 0
        cut_pieces = np.flip(cut_pieces, 0)

    print(cut_pieces)
    print(board)

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
                bye = width - 1
    if dice_type == 2:
        if action.dir < 2:
            bxs = x_start
            bxe = x_end
            if action.dir == 0:
                bys = n - chosenRowNum
                bye = n - 1
            else:
                bys = 0
                bye = chosenRowNum - 1
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
                bxs = m - chosenColNum
                bxe = m - 1
            else:
                bxs = 0
                bxe = chosenColNum - 1

    if dice_type == 1:
        cnt = 0
        for r in range(bys, bye + 1):
            for c in range(bxs, bxe + 1):
                print(f"r:{r}, c:{c}")
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
        for c in range(bxs, bxe + 1):
            for r in range(bys, bye + 1):
                if board[r][c] == 0:
                    board[r][c] = cut_pieces[cnt]
                    cnt += 1

    return board
