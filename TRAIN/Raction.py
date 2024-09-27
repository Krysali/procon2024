import numpy as np

n, m = 6, 6
board = np.zeros((6, 6), dtype=int)

def is_inside(x, y):
    return 0 <= x < m and 0 <= y < n

def reverse_type_i(X, Y, size, s):
    global board
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1
    # x m y n 
    if s == 0:
        rem = board[h:oby+1, otx:obx+1].copy()
        chosen = board[:h, otx:obx+1].copy()
        board[:oby+1, otx:obx+1] = np.vstack((rem, chosen))
    
    elif s == 1:
        rem = board[oty:n-h, otx:obx+1].copy()
        chosen = board[n-h:, otx:obx+1].copy()
        board[oty:, otx:obx+1] = np.vstack((chosen, rem))
    
    elif s == 2:
        rem = board[oty:oby+1, otx:m-w].copy()
        chosen = board[oty:oby+1, m-w:].copy()
        board[oty:oby+1, otx:] = np.hstack((chosen, rem))
    
    elif s == 3:
        rem = board[oty:oby+1, w:obx+1].copy()
        chosen = board[oty:oby+1, :w].copy()
        board[oty:oby+1, :obx+1] = np.hstack((rem, chosen))
def reverse_type_ii(X, Y, size, s):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_row_num = h // 2
    first = (h + 1) % 2

    if is_inside(X, Y):
        if not first:
            chosen_row_num += 1
        first = 1
    elif abs(Y) % 2 != 0:
        first = 0
    elif abs(X) % 2 == 0:
        if n % 2 == 1:
            first = 1
            chosen_row_num += 1

    print(f"otx:{otx} oty:{oty} obx:{obx} oby:{oby} w:{w} h:{h} chosenRowNum:{chosen_row_num} first:{first}")

    rem = [[] for _ in range(256)]
    chosen = [[] for _ in range(128)]

    if s == 0:
        print("REM")
        for index_y in range(oty, n - chosen_row_num):
            rem[index_y - oty].clear()
            print(f"index:{index_y - oty}")
            for index_x in range(otx, obx + 1):
                rem[index_y - oty].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()

        print("CHOSEN")
        for index_y in range(n - chosen_row_num, n):
            chosen[index_y - (n - chosen_row_num)].clear()
            print(f"index:{index_y - (n - chosen_row_num)}")
            for index_x in range(otx, obx + 1):
                chosen[index_y - (n - chosen_row_num)].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()

        cnt = 0
        for index_y in range(oty, min((oty + chosen_row_num * 2), n)):
            if first:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = chosen[cnt // 2][index_x - otx]
                first = 0
            else:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = rem[cnt // 2][index_x - otx]
                first = 1
            cnt += 1

        index = 0
        for index_y in range((oty + chosen_row_num * 2), n):
            for index_x in range(otx, obx + 1):
                board[index_y][index_x] = rem[index + chosen_row_num][index_x - otx]
            index += 1

    if s == 1:
        print("CHOSEN")
        for index_y in range(chosen_row_num):
            chosen[index_y].clear()
            print(f"index:{index_y}")
            for index_x in range(otx, obx + 1):
                chosen[index_y].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()

        print("REM")
        for index_y in range(chosen_row_num, oby + 1):
            rem[index_y - chosen_row_num].clear()
            print(f"index:{index_y - chosen_row_num}")
            for index_x in range(otx, obx + 1):
                rem[index_y - chosen_row_num].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()

        for index_y in range(oty):
            for index_x in range(otx, obx + 1):
                board[index_y][index_x] = rem[index_y][index_x - otx]

        cnt = 0
        for index_y in range(oty, (oty + chosen_row_num * 2)):
            if first:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = chosen[cnt // 2][index_x - otx]
                first = 0
            else:
                for index_x in range(otx, obx + 1):
                    board[index_y][index_x] = rem[((cnt // 2) + oty)][index_x - otx]
                first = 1
            cnt += 1

        index = 0
        for index_y in range((oty + chosen_row_num * 2), oby + 1):
            for index_x in range(otx, obx + 1):
                board[index_y][index_x] = rem[index + chosen_row_num + oty][index_x - otx]
            index += 1

    if s == 2:
        count = 0
        for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
            for index_x in range(otx, m - w):
                rem[count].append(board[index_y][index_x])
            count += 1
        count = 0
        for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
            for index_x in range(m - w, m):
                chosen[count].append(board[index_y][index_x])
            count += 1
        cnt = 0
        for index_x in range(otx, m):
            if index_x <= obx:
                cnt = 0
                for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1
            else:
                cnt = 0
                for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
                    board[index_y][index_x] = rem[cnt][index_x - (obx + 1)]
                    cnt += 1

    if s == 3:
        count = 0
        for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
            for index_x in range(w, obx + 1):
                rem[count].append(board[index_y][index_x])
            count += 1
            print()
        count = 0
        for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
            for index_x in range(0, w):
                chosen[count].append(board[index_y][index_x])
            count += 1
        cnt = 0
        for index_x in range(0, obx + 1):
            if index_x < otx:
                cnt = 0
                for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
                    board[index_y][index_x] = rem[cnt][index_x]
                    cnt += 1
            else:
                cnt = 0
                for index_y in range((oby - oty + 1) % 2 == 0 and oty or oty + 1, oby + 1, 2):
                    board[index_y][index_x] = chosen[cnt][index_x - otx]
                    cnt += 1
    return
def reverse_type_iii(X, Y, size, s):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_col_num = w // 2
    first = (w + 1) % 2

    if is_inside(X, Y):
        if not first: 
            chosen_col_num += 1 
        first = 1
    elif abs(X) % 2 != 0:
        first = 0
    elif abs(X) % 2 == 0:
        if m % 2 == 1:
            first = 1
            chosen_col_num += 1

    print(f"otx:{otx} oty:{oty} obx:{obx} oby:{oby} w:{w} h:{h} chosenColNum:{chosen_col_num} first:{first}")

    rem = [[] for _ in range(256)]
    chosen = [[] for _ in range(128)]
    
    if s == 0:
        for index_y in range(oty, n - h):
            for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                rem[index_y - oty].append(board[index_y][index_x])
        for index_y in range(n - h, n):
            for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                chosen[index_y - (n - h)].append(board[index_y][index_x])
        cnt = 0
        for index_y in range(oty, n):
            if index_y < oty + h:
                cnt = 0
                for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                    board[index_y][index_x] = chosen[index_y - oty][cnt]
                    cnt += 1
            else:
                cnt = 0
                for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                    board[index_y][index_x] = rem[index_y - (oty + h)][cnt]
                    cnt += 1

    if s == 1:
        for index_y in range(h, oby + 1):
            for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                rem[index_y - h].append(board[index_y][index_x])
        for index_y in range(0, h):
            for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                chosen[index_y].append(board[index_y][index_x])
        cnt = 0
        for index_y in range(0, oby + 1):
            if index_y < oby - h + 1:
                cnt = 0
                for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                    board[index_y][index_x] = rem[index_y][cnt]
                    cnt += 1
            else:
                cnt = 0
                for index_x in range(otx + (obx - otx + 1) % 2, obx + 1, 2):
                    board[index_y][index_x] = chosen[index_y - (oby - h + 1)][cnt]
                    cnt += 1

    if s == 2:
        print("REM")
        for index_x in range(otx, m - chosen_col_num):
            rem[index_x - otx].clear()
            print(f"index:{index_x - otx}")
            for index_y in range(oty, oby + 1):
                rem[index_x - otx].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()
        
        print("CHOSEN")
        for index_x in range(m - chosen_col_num, m):
            chosen[index_x - (m - chosen_col_num)].clear()
            print(f"index:{index_x - m + chosen_col_num}")
            for index_y in range(oty, oby + 1):
                chosen[index_x - (m - chosen_col_num)].append(board[index_y][index_x])
                print(board[index_y][index_x], end=' ')
            print()

        cnt = 0
        for index_x in range(otx, otx + chosen_col_num * 2):
            if first:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = chosen[cnt // 2][index_y - oty]
                first = 0
            else:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = rem[cnt // 2][index_y - oty]
                first = 1
            cnt += 1

        index = 0
        for index_x in range(otx + chosen_col_num * 2, m):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index + chosen_col_num][index_y - oty]
            index += 1

    if s == 3:
        print("CHOSEN")
        for index_x in range(chosen_col_num):
            chosen[index_x].clear()
            print(f"index:{index_x}")
            for index_y in range(oty, oby + 1):
                chosen[index_x].append(board[index_y][index_x])
                print(chosen[index_x][index_y - oty], end=' ')
            print()
        
        print("REM")
        for index_x in range(chosen_col_num, obx + 1):
            rem[index_x - chosen_col_num].clear()
            print(f"index:{index_x - chosen_col_num}")
            for index_y in range(oty, oby + 1):
                rem[index_x - chosen_col_num].append(board[index_y][index_x])
                print(rem[index_x - chosen_col_num][index_y - oty], end=' ')
            print()

        for index_x in range(0, otx):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index_x][index_y - oty]

        cnt = 0
        for index_x in range(otx, min(otx + chosen_col_num * 2, m)):
            if first:
                for index_y in range(oty, oby + 1):
                    board[index_y][index_x] = chosen[cnt // 2][index_y - oty]
                first = 0
            else:
                for index_y in range(oty, oby + 1):
                    ind = (cnt // 2) + otx
                    board[index_y][index_x] = rem[ind][index_y - oty]
                first = 1
            cnt += 1
        index = 0
        for index_x in range(otx + chosen_col_num * 2, obx + 1):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index + chosen_col_num + otx][index_y - oty]
            index += 1
    return

def display():
    for i in range(n):
        for j in range(m):
            print(f"{board[i][j]:2d}", end=" ")
        print()
    print("\n\n\n")

def main():
    global board
    n, m = 6, 6
    board = np.arange(n*m).reshape(n, m)
    
    t = 10
    while t > 0:
        X, Y, size, dir = map(int, input().split())
        display()
        reverse_type_i(X, Y, size, dir)
        display()
        t -= 1

if __name__ == "__main__":
    main()
