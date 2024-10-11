import math
import numpy as np
from random import randint

# Define the Action class to hold the action data
class Action:
    def __init__(self, die_index, x, y, direction):
        self.die_index = die_index
        self.x = x
        self.y = y
        self.direction = direction

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
                            action = Action(int(diceNum), topX, topY, s)
                            actions.append(action)
                        else:
                            for i in range(3):
                                action = Action(int(diceNum - i), topX, topY, s)
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
                        action = Action(0, topX, topY, i)
                        actions.append(action)
                    continue

                # TYPE I
                for i in range(0, 3, 2):  # i goes 0, 2
                    action = Action(int(diceNum - 2), botX - sz + 1, botY - sz + 1, i)
                    actions.append(action)

                # TYPE II
                if height > 2:
                    for i in range(3):
                        action = Action(int(diceNum - 1), botX - sz + 1, botY - sz + 1, i)
                        actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(4):
                        if i == 1:
                            continue
                        action = Action(int(diceNum), botX - sz + 1, botY - sz + 1, i)
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
                        action = Action(int(diceNum - 1), topX, botY - sz + 1, i)
                        actions.append(action)

                # TYPE III
                if width != 1:
                    for i in range(2, 4):  # i = 2, 3
                        action = Action(int(diceNum), topX, botY - sz + 1, i)
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
                        action = Action(int(diceNum - 1), botX - sz + 1, topY, i)
                        actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(2, 4):  # i = 2, 3
                        action = Action(int(diceNum), botX - sz + 1, topY, i)
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
                        action = Action(0, topX, topY, i)
                        actions.append(action)
                    continue

                # 3rd type diceNum of this size
                diceNum = math.log2(sz) * 3

                # TYPE I
                for i in range(1, 4, 2):  # i = 1, 3
                    action = Action(int(diceNum - 2), topX, topY, i)
                    actions.append(action)

                # TYPE II
                if height != 1:
                    for i in range(4):  # i = 0, 1, 2, 3
                        if i == 2:
                            continue
                        action = Action(int(diceNum - 1), topX, topY, i)
                        actions.append(action)

                # TYPE III
                if width != 1:
                    for i in range(1, 4):  # i = 1, 2, 3
                        action = Action(int(diceNum), topX, topY, i)
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
                action = Action(int(diceNum - 2), botX - sz + 1, botY - sz + 1, 0)
                actions.append(action)

                # TYPE II
                if height > 2:
                    for i in range(2):  # C++: 'for(int i = 0; i < 2; i++)'
                        action = Action(int(diceNum - 1), botX - sz + 1, botY - sz + 1, i)
                        actions.append(action)

                # TYPE III
                if m % 2 == 0:
                    action1 = Action(int(diceNum), botX - sz + 1, botY - sz + 1, 0)
                    actions.append(action1)
                else:
                    action1 = Action(int(diceNum), botX + 1 - sz + 1, botY - sz + 1, 0)
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
                action = Action(int(diceNum - 2), botX - sz + 1, botY - sz + 1, 2)
                actions.append(action)

                # TYPE II
                if n % 2 == 0:
                    action = Action(int(diceNum - 1), botX - sz + 1, botY - sz + 1, 2)
                    actions.append(action)
                else:
                    action = Action(int(diceNum - 1), botX - sz + 1, botY + 1 - sz + 1, 2)
                    actions.append(action)

                # TYPE III
                if width > 2:
                    for i in range(2, 4):  # C++: 'for(int i = 2; i < 4; i++)'
                        action = Action(int(diceNum), botX - sz + 1, botY - sz + 1, i)
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
                        action = Action(int(diceNum), botX, botY - sz + 1, i)
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
                        action = Action(int(diceNum - 1), topX, topY, i)
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
                            action = Action(0, topX, topY, i)
                            actions.append(action)
                        continue

                    # 3rd type diceNum of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                        if i == 1:
                            continue
                        action = Action(diceNum - 2, topX, height - width, i)
                        actions.append(action)

                    # TYPE II
                    if height > 2:
                        for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                            action = Action(diceNum - 1, topX, height - width, i)
                            actions.append(action)

                    # TYPE III
                    if topX == 1 or topX == m - 2:
                        continue
                    if sz == 2 and height == 1:
                        continue
                    for i in range(4):  # C++: 'for(int i = 0; i < 4; i++)'
                        if i == 1:
                            continue
                        action = Action(diceNum, topX, height - width, i)
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
                            action = Action(0, topX, topY, 1)
                            actions.append(action)
                        for i in range(2, 4):
                            action = Action(0, topX, topY, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    if oh > ow:
                        action = Action(diceNum - 2, topX, topY - height + 1, 1)
                        actions.append(action)
                    for i in range(2, 4):
                        action = Action(diceNum - 2, topX, topY - height + 1, i)
                        actions.append(action)

                    # TYPE II
                    if height != 1:
                        for i in range(4):
                            action = Action(diceNum - 1, topX, topY - height + 1, i)
                            actions.append(action)

                    # TYPE III
                    if topX == 1 or topX == m - 2:
                        continue
                    if sz == 2 and height == 1:
                        continue
                    if oh > ow:
                        action = Action(diceNum, topX, topY - height + 1, 1)
                        actions.append(action)

                    for i in range(2, 4):
                        action = Action(diceNum, topX, topY - height + 1, i)
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
                            action = Action(0, width - height, topY, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    for i in range(3):
                        action = Action(diceNum - 2, width - height, topY, i)
                        actions.append(action)

                    # TYPE III
                    if width > 2:
                        for i in range(4):
                            action = Action(diceNum, width - height, topY, i)
                            actions.append(action)

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue
                    for i in range(3):
                        action = Action(diceNum - 1, width - height, topY, i)
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
                            action = Action(0, topX, topY, 3)
                            actions.append(action)
                        for i in range(2):
                            action = Action(0, topX, topY, i)
                            actions.append(action)
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    if ow > oh:
                        action = Action(diceNum - 2, topX - width + 1, topY, 3)
                        actions.append(action)
                    for i in range(2):
                        action = Action(diceNum - 2, topX - width + 1, topY, i)
                        actions.append(action)

                    # TYPE III
                    if width != 1:
                        for i in range(4):
                            action = Action(diceNum, topX - width + 1, topY, i)
                            actions.append(action)

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue

                    if ow > oh:
                        action = Action(diceNum - 1, topX - width + 1, topY, 3)
                        actions.append(action)

                    for i in range(2):
                        action = Action(diceNum - 1, topX - width + 1, topY, i)
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
                        action = Action(diceNum - 2, topX, topY, i)
                        actions.append(action)

                    # TYPE II
                    for i in range(2, 4):
                        action = Action(diceNum - 1, topX, topY, i)
                        actions.append(action)

                    if topX == 1 or topX == m - 2:
                        continue

                    # TYPE III
                    for i in range(2, 4):
                        action = Action(diceNum, topX, topY, i)
                        actions.append(action)

            szz *= 2

    return actions

def is_inside(x, y, dtype, n , m ):
    if dtype == 2:
        return (0 <= y < n)
    else:
        return (0 <= x < m)

def raction(board, a):
    # Set up     
    n, m = board.shape                                   
    power = math.ceil(a.die_index / 3)                                        
    size = int(math.pow(2, power))     
    if power == 0: dice_type = 1                            
    else: dice_type = a.die_index - ((power - 1) * 3)  

    cboard = board.copy()

    if dice_type == 1: return reverse_type_I(a.x, a.y, size, a.direction, n, m, cboard)
    elif dice_type == 2: return reverse_type_II(a.x, a.y, size, a.direction, n, m, cboard)
    else: return reverse_type_III(a.x, a.y, size, a.direction, n, m, cboard)

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

    #print(f"chosen_row_num:{chosen_row_num}, first:{first}")

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

    #print(f"chosen_col_num:{chosen_col_num}, first:{first}, size:{size}")

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
            chosen_cnt += 1

        #print(f"chosen_cnt:{chosen_cnt}")

        for index_x in range(chosen_col_num, obx + 1):
            for index_y in range(oty, oby + 1):
                rem[rem_cnt].append(board[index_y][index_x])
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

def apply_die(state, action):
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
    

    return board

def checker(n, m):
    board = np.arange(1, (n * m) + 1).reshape((n, m))

    tooluur = 0
    number = 0
    check = {}  # This will be used to store unique board states

    actions = gen_actions(n, m)

    for a in actions:
        # Debug output can be enabled if needed
        # print(f"x: {a.x} y: {a.y} diceType: {a.diceType} dir: {a.dir}")

        next_board = apply_die(board, a)

        pre_board = raction(next_board, a)

        if not np.array_equal(pre_board, board):
            print("ERROR")
            print(f"x: {a.x} y: {a.y} die_index: {a.die_index} dir: {a.direction}")

            print("ORIGINAL BOARD")
            print(board)

            print("actioned BOARD")
            print(next_board)

            print("reversed BOARD")
            print(pre_board)

    print("BOLOOOOOOOO")
        
def __main__():

    # n = height, m = width
    n = int(input("Enter value for n (height): "))
    m = int(input("Enter value for m (width): "))

    # Ensure that m is the longer side
    if n > m:
        n, m = m, n  # swap n and m

    checker(n, m)


