import math

def a():
    # Declare variables
    topX = topY = botX = botY = width = height = mxSide = sz = diceNum = 0

    # Define the Action class
    class Action:
        def __init__(self, x, y, dice_num, direction):
            self.x = x
            self.y = y
            self.dice_num = dice_num
            self.direction = direction

    # List to store actions
    actions = []

    # Dictionary to store checks (like unordered_map in C++)
    check = {}

    # Function to find the nearest power of 2 greater than or equal to x
    def nearest(x):
        res = 1
        while x > res:
            res *= 2
        return res

    # Function to check if a number is a power of two
    def is_power_of_two(x):
        if x == 1:
            return True
        if x % 2 == 1:
            return False
        return is_power_of_two(x // 2)

    # Input values
    n, m = map(int, input().split())

    # Ensure m is always the larger dimension
    if n > m:
        n, m = m, n

    # Inside the board
    for topY in range(1, n - 1):
        for topX in range(1, m - 1):
            sz = 1
            while sz <= min(n - topY - 1, m - topX - 1):
                diceNum = math.log2(sz) * 3
                for s in range(4):
                    if sz == 1:
                        actions.append(Action(topX, topY, int(diceNum), s))
                    else:
                        for i in range(3):
                            actions.append(Action(topX, topY, int(diceNum - i), s))
                sz *= 2

    # Corner contained actions

    # Topleft corner contained actions
    topX, topY = 0, 0
    for botY in range(n - 1):
        for botX in range(m - 1):
            width = botX - topX + 1
            height = botY - topY + 1
            mxSide = max(width, height)
            sz = nearest(mxSide)
            diceNum = math.log2(sz) * 3

            if sz == 1:
                for i in range(0, 3, 2):
                    actions.append(Action(topX, topY, 0, i))
                continue

            # TYPE I
            for i in range(0, 3, 2):
                actions.append(Action(botX - sz + 1, botY - sz + 1, int(diceNum - 2), i))

            # TYPE II
            if height > 2:
                for i in range(3):
                    actions.append(Action(botX - sz + 1, botY - sz + 1, int(diceNum - 1), i))

            # TYPE III
            if width > 2:
                for i in range(4):
                    if i == 1:
                        continue
                    actions.append(Action(botX - sz + 1, botY - sz + 1, int(diceNum), i))

    # Topright corner contained actions
    topY, botX = 0, m - 1
    for botY in range(n - 1):
        for topX in range(1, m):
            height = botY - topY + 1
            width = botX - topX + 1
            mxSide = max(height, width)
            sz = nearest(mxSide)
            diceNum = math.log2(sz) * 3

            if sz == 1:
                continue

            # TYPE II
            if height > 2:
                for i in range(2):
                    actions.append(Action(topX, botY - sz + 1, int(diceNum - 1), i))

            # TYPE III
            if width != 1:
                for i in range(2, 4):
                    actions.append(Action(topX, botY - sz + 1, int(diceNum), i))

    # Bottomleft corner contained actions
    topX, botY = 0, n - 1
    for topY in range(1, n):
        for botX in range(m - 1):
            height = botY - topY + 1
            width = botX - topX + 1
            mxSide = max(height, width)
            sz = nearest(mxSide)
            diceNum = math.log2(sz) * 3

            if sz == 1:
                continue

            # TYPE II
            if height != 1:
                for i in range(2):
                    actions.append(Action(botX - sz + 1, topY, int(diceNum - 1), i))

            # TYPE III
            if width > 2:
                for i in range(2, 4):
                    actions.append(Action(botX - sz + 1, topY, int(diceNum), i))

    # Bottomright corner contained actions
    botX, botY = m - 1, n - 1
    for topY in range(1, n):
        for topX in range(1, m):
            height = botY - topY + 1
            width = botX - topX + 1
            mxSide = max(height, width)
            sz = nearest(mxSide)

            if sz == 1:
                for i in range(1, 4, 2):
                    actions.append(Action(topX, topY, 0, i))
                continue

            diceNum = math.log2(sz) * 3

            # TYPE I
            for i in range(1, 4, 2):
                actions.append(Action(topX, topY, int(diceNum - 2), i))

            # TYPE II
            if height != 1:
                for i in range(4):
                    if i == 2:
                        continue
                    actions.append(Action(topX, topY, int(diceNum - 1), i))

            # TYPE III
            if width != 1:
                for i in range(1, 4):
                    actions.append(Action(topX, topY, int(diceNum), i))

    # Pairs of corners

    # From above
    topX, topY = 0, 0
    botX = m - 1

    for botY in range(n - 1):
        width = m
        height = botY - topY + 1
        mxSide = max(width, height)
        sz = nearest(mxSide)
        diceNum = int(math.log2(sz)) * 3

        # TYPE I
        actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum - 2, 0))

        # TYPE II
        if height > 2:
            for i in range(2):
                actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum - 1, i))

        # TYPE III
        if m % 2 == 0:
            actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum, 0))
        else:
            actions.append(Action(botX + 1 - sz + 1, botY - sz + 1, diceNum, 0))

    # From left
    topX, topY = 0, 0
    botY = n - 1

    for botX in range(m - 1):
        width = botX - topX + 1
        height = n
        mxSide = max(width, height)
        sz = nearest(mxSide)
        diceNum = int(math.log2(sz)) * 3

        # TYPE I
        actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum - 2, 2))

        # TYPE II
        if n % 2 == 0:
            actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum - 1, 2))
        else:
            actions.append(Action(botX - sz + 1, botY + 1 - sz + 1, diceNum - 1, 2))

        # TYPE III
        if width > 2:
            for i in range(2, 4):
                actions.append(Action(botX - sz + 1, botY - sz + 1, diceNum, i))

    # From right
    topX, topY = m - 1, 0
    botY = n - 1

    for botX in range(m - 1, 0, -1):
        width = topX - botX + 1
        height = n
        mxSide = max(width, height)
        sz = nearest(mxSide)
        diceNum = int(math.log2(sz)) * 3

        # TYPE III
        if width != 1:
            for i in range(2, 4):
                actions.append(Action(botX, botY - sz + 1, diceNum, i))

    # From bottom
    topX, botX, botY = 0, m - 1, n - 1

    for topY in range(n - 1, 0, -1):
        width = m
        height = topY - botY + 1
        mxSide = max(width, height)
        sz = nearest(mxSide)
        diceNum = int(math.log2(sz)) * 3

        # TYPE II
        if height != 1:
            for i in range(2):
                actions.append(Action(topX, topY, diceNum - 1, i))

    # Edge cases

    # Top
    topY = 0

    for topX in range(1, m - 1):
        width = 1
        while width < m - 1 - topX:
            for height in range(1, min(width, n - 1) + 1):
                sz = width

                if sz == 1:
                    if topX == 1 or topX == m - 2:
                        continue
                    for i in range(4):
                        if i == 1:
                            continue
                        actions.append(Action(topX, topY, 0, i))
                    continue

                diceNum = int(math.log2(sz)) * 3

                # TYPE I
                for i in range(4):
                    if i == 1:
                        continue
                    actions.append(Action(topX, height - width, diceNum - 2, i))

                # TYPE II
                if height > 2:
                    for i in range(4):
                        actions.append(Action(topX, height - width, diceNum - 1, i))

                # TYPE III
                if topX == 1 or topX == m - 2:
                    continue
                if sz == 2 and height == 1:
                    continue
                for i in range(4):
                    if i == 1:
                        continue
                    actions.append(Action(topX, height - width, diceNum, i))

            width *= 2

    # Bottom
    topY = n - 1

    for topX in range(1, m - 1):
        width = 1
        while width <= m - 1 - topX:
            for height in range(1, min(width, n - 1) + 1):
                oh = n - height
                ow = width
                sz = width

                if sz == 1:
                    if topX == 1 or topX == m - 2:
                        continue
                    if oh > ow:
                        actions.append(Action(topX, topY, 0, 1))
                    for i in range(2, 4):
                        actions.append(Action(topX, topY, 0, i))
                    continue

                diceNum = int(math.log2(sz)) * 3

                # TYPE I
                if oh > ow:
                    actions.append(Action(topX, topY - height + 1, diceNum - 2, 1))
                for i in range(2, 4):
                    actions.append(Action(topX, topY - height + 1, diceNum - 2, i))

                # TYPE II
                if height != 1:
                    for i in range(4):
                        actions.append(Action(topX, topY - height + 1, diceNum - 1, i))

                # TYPE III
                if topX == 1 or topX == m - 2:
                    continue
                if sz == 2 and height == 1:
                    continue
                if oh > ow:
                    actions.append(Action(topX, topY - height + 1, diceNum, 1))
                for i in range(2, 4):
                    actions.append(Action(topX, topY - height + 1, diceNum, i))

            width *= 2

    # left
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
                            actions.append(Action(width - height, topY, 0, i))
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    for i in range(3):
                        actions.append(Action(width - height, topY, diceNum - 2, i))

                    # TYPE III
                    if width > 2:
                        for i in range(4):
                            actions.append(Action(width - height, topY, diceNum, i))

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue
                    for i in range(3):
                        actions.append(Action(width - height, topY, diceNum - 1, i))

                height *= 2

    # right
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
                            actions.append(Action(topX, topY, 0, 3))
                        for i in range(2):
                            actions.append(Action(topX, topY, 0, i))
                        continue

                    # 3rd type dice of this size
                    diceNum = int(math.log2(sz) * 3)

                    # TYPE I
                    if ow > oh:
                        actions.append(Action(topX - width + 1, topY, diceNum - 2, 3))
                    for i in range(2):
                        actions.append(Action(topX - width + 1, topY, diceNum - 2, i))

                    # TYPE III
                    if width != 1:
                        for i in range(4):
                            actions.append(Action(topX - width + 1, topY, diceNum, i))

                    # TYPE II
                    if topY == 1 or topY == n - 2:
                        continue
                    if sz == 2 and width == 1:
                        continue

                    if ow > oh:
                        actions.append(Action(topX - width + 1, topY, diceNum - 1, 3))
                    for i in range(2):
                        actions.append(Action(topX - width + 1, topY, diceNum - 1, i))

                height *= 2

    # pairs of edges
    if is_power_of_two(n):
        if n + 1 < m:
            topY = 0
            for topX in range(1, m - n):
                sz = n
                # 3rd type dice of this size
                diceNum = int(math.log2(sz) * 3)

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

    return actions
