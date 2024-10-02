import math

# Define the Action class to hold the action data
class Action:
    def __init__(self, x, y, dice_num, direction):
        self.x = x
        self.y = y
        self.dice_num = dice_num
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


# Define variables
topX, topY, botX, botY, width, height, mxSide, sz, diceNum = 0, 0, 0, 0, 0, 0, 0, 0, 0

# Print test message
print("test")

# n = height, m = width
n = int(input("Enter value for n (height): "))
m = int(input("Enter value for m (width): "))

# Create an empty list for actions and a dictionary for check (as maps in C++ are dictionaries in Python)
actions = []
check = {}

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

    # edge

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
if is_power_of_two(n):

    if n + 1 < m:
        topY = 0
        for topX in range(1, m - n):
            sz = n
            # 3rd type dice of this size
            diceNum = int(math.log2(sz) * 3)

            # TYPE I
            for i in range(2, 4):
                action = Action(topX, topY, diceNum - 2, i)
                actions.append(action)

            # TYPE II
            for i in range(2, 4):
                if i == 1 and sz == 2:
                    continue
                action = Action(topX, topY, diceNum - 1, i)
                actions.append(action)

            if topX == 1 or topX == m - 2:
                continue

            # TYPE III
            for i in range(2, 4):
                action = Action(topX, topY, diceNum, i)
                actions.append(action)

    if n + 1 < m:
        topY = 0
        for topX in range(1, m - n):
            sz = n
            # 3rd type dice of this size
            diceNum = int(math.log2(sz)) * 3

            # TYPE I
            for i in range(2, 4):
                actions.append(Action(topX, topY, diceNum - 2, i))

            # TYPE II
            for i in range(2, 4):
                if i == 1 and sz == 2:
                    continue
                actions.append(Action(topX, topY, diceNum - 1, i))

            if topX == 1 or topX == m - 2:
                continue

            # TYPE III
            for i in range(2, 4):
                actions.append(Action(topX, topY, diceNum, i))

print(len(actions))


# CHECK
if False:
    board = Board()
    board.height = n
    board.width = m

    for i in range(n):
        row = []
        for j in range(m):
            row.append(i * n + j + 1)
        board.pieces.append(row)

    tooluur = 0
    number = 0
    check = {}  # This will be used to store unique board states

    for a in actions:
        # Debug output can be enabled if needed
        # print(f"x: {a.x} y: {a.y} diceType: {a.diceType} dir: {a.dir}")

        nextState = GameState()
        nextState.board = board
        nextState.dies = GenerateFixedDies()
        apply_die(nextState, a.diceNum, a.x, a.y, a.dir)
        # display_game_state(nextState)  # Uncomment if needed for debugging

        llr = []
        for i in range(n):
            for j in range(m):
                too = nextState.board.pieces[i][j]
                llr.append(too)

        # Convert list to tuple to use it as a key in the dictionary
        llr_tuple = tuple(llr)
        if llr_tuple not in check:
            check[llr_tuple] = 1
            tooluur += 1
            # print(f"x: {a.x} y: {a.y} dice: {a.diceNum} dir: {a.dir}")  # Uncomment if needed for debugging
        else:
            print(f"x: {a.x} y: {a.y} dice: {a.diceNum} dir: {a.dir}")  # Duplicate state
            # display_game_state(nextState)  # Uncomment if needed for debugging

        number += 1

    print(f"{number} {tooluur}")
