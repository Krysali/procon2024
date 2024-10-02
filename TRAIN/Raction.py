import numpy as np

def is_inside(x, y, dtype, n , m ):
    if dtype == 2:
        return 0 <= y < n
    else:
        return 0 <= x < m
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
def reverse_type_II(X, Y, size, s, n , m, board):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_row_num = h // 2
    first = (h + 1) % 2

    if is_inside(X, Y, 2):
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
def reverse_type_III(X, Y, size, s, n, m, board):
    otx = max(X, 0)
    oty = max(Y, 0)
    obx = min(X + size - 1, m - 1)
    oby = min(Y + size - 1, n - 1)
    w = obx - otx + 1
    h = oby - oty + 1

    chosen_col_num = w // 2
    first = (w + 1) % 2

    if is_inside(X, Y, 3):
        if not first:
            chosen_col_num += 1
        first = 1
    elif abs(X) % 2 != 0:
        first = 0
    elif abs(X) % 2 == 0 and w % 2 == 1:
        first = 1
        chosen_col_num += 1

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
        rem_cnt = chosen_cnt = 0
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
        chosen = [[board[index_y][index_x] for index_y in range(oty, oby + 1)] for index_x in range(chosen_col_num)]
        rem = [[board[index_y][index_x] for index_y in range(oty, oby + 1)] for index_x in range(chosen_col_num, obx + 1)]

        cnt = 0
        for index_x in range(otx, min(otx + chosen_col_num * 2, m)):
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
        for index_x in range(otx + chosen_col_num * 2, obx + 1):
            for index_y in range(oty, oby + 1):
                board[index_y][index_x] = rem[index + chosen_col_num][index_y - oty]
            index += 1
