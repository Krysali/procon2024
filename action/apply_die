// Function to apply the die action
void apply_die(GameState& game_state, Move action) {

    auto board = game_state.board.pieces;
    int n = game_state.board.height;
    int m = game_state.board.width;
    // Set up
    int power = ceil(action.dice_index / 3.0);
    int size = int(pow(2, power));
    int dice_type = (power == 0) ? 1 : action.dice_index - ((power - 1) * 3);

    // cout << "dice_index:" << action.dice_index << ", n:" << n << ", m:" << m << ", power:" << power << ", size:" << size 
    //      << ", dice_type:" << dice_type << ", dir:" << action.dir << endl;

    vector<int> cut_pieces;

    int x_start = max(action.x, 0);
    int y_start = max(action.y, 0);
    int x_end = min(m, action.x + size) - 1;
    int y_end = min(n, action.y + size) - 1;

    int width = x_end - x_start + 1;
    int height = y_end - y_start + 1;

    int chosen_row_num = floor(height / 2);
    int chosen_col_num = floor(width / 2);
    int first = 1;

    // Determining first and chosen_x_num
    if (dice_type == 2) {
        first = (height + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            if (!first) {
                chosen_row_num += 1;
            }
            first = 1;
        } else if (abs(action.y) % 2 != 0) {
            first = 0;
        } else if (abs(action.y) % 2 == 0) {
            if (height % 2 == 1) { // n bsn
                first = 1;
                chosen_row_num += 1;
            }
        }
    }

    if (dice_type == 3) {
        first = (width + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            if (!first) {
                chosen_col_num += 1;
            }
            first = 1;
        } else if (abs(action.x) % 2 != 0) {
            first = 0;
        } else if (abs(action.x) % 2 == 0) {
            if (width % 2 == 1) { // m bsn
                first = 1;
                chosen_col_num += 1;
            }
        }
    }

    // cout << "x_start:" << x_start << ", x_end:" << x_end << ", y_start:" << y_start 
    //      << ", y_end:" << y_end << ", width:" << width << ", height:" << height 
    //      << ", chosen_row_num:" << chosen_row_num << ", chosen_col_num:" << chosen_col_num << endl;

    // CUT PHASE
    if (dice_type == 1) {
        for (int r = y_start; r <= y_end; ++r) {
            for (int c = x_start; c <= x_end; ++c) {
                cut_pieces.push_back(board[r][c]);
                board[r][c] = 0;
            }
        }
    }

    if (dice_type == 2) {
        int x = (first + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            for (int r = y_start; r <= y_end; r += 2) {
                for (int c = x_start; c <= x_end; ++c) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        } else {
            for (int r = y_start + x; r <= y_end; r += 2) {
                for (int c = x_start; c <= x_end; ++c) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        }
    }

    if (dice_type == 3) {
        int x = (first + 1) % 2;
        if (is_inside(action.x, action.y, n, m, dice_type)) {
            for (int r = y_start; r <= y_end; ++r) {
                for (int c = x_start; c <= x_end; c += 2) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        } else {
            for (int r = y_start; r <= y_end; ++r) {
                for (int c = x_start + x; c <= x_end; c += 2) {
                    cut_pieces.push_back(board[r][c]);
                    board[r][c] = 0;
                }
            }
        }
    }

    // SHIFT PHASE
    if (action.dir == 0) {
        for (int c = x_start; c <= x_end; ++c) {
            int write_index = y_start;
            for (int r = y_start; r < n; ++r) {
                if (board[r][c] != 0) {
                    board[write_index][c] = board[r][c];
                    ++write_index;
                }
            }
            for (int r = write_index; r < n; ++r) {
                board[r][c] = 0;
            }
        }
    }else if(action.dir == 1) {
        for (int c = x_start; c <= x_end; ++c) {
            int write_index = y_end;
            for (int r = y_end; r >= 0; --r) {
                if (board[r][c] != 0) {
                    board[write_index][c] = board[r][c];
                    --write_index;
                }
            }
            for (int r = write_index; r >= 0; --r) {
                board[r][c] = 0;
            }
        }
    }else if (action.dir == 2) {
        for (int r = y_start; r <= y_end; ++r) {
            int write_index = x_start;
            for (int c = x_start; c < m; ++c) {
                if (board[r][c] != 0) {
                    board[r][write_index] = board[r][c];
                    ++write_index;
                }
            }
            for (int c = write_index; c < m; ++c) {
                board[r][c] = 0;
            }
        }
    }else if (action.dir == 3) {
        for (int r = y_start; r <= y_end; ++r) {
            int write_index = x_end;
            for (int c = x_end; c >= 0; --c) {
                if (board[r][c] != 0) {
                    board[r][write_index] = board[r][c];
                    --write_index;
                }
            }
            for (int c = write_index; c >= 0; --c) {
                board[r][c] = 0;
            }
        }
    }

    // BBBT PHASE
    int bxs = 0, bxe = 0, bys = 0, bye = 0;

    if (dice_type == 1) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 2) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - chosen_row_num;
                bye = n - 1;
            } else {
                bys = 0;
                bye = chosen_row_num - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 3) {
        if (action.dir < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.dir == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.dir == 2) {
                bxs = m - chosen_col_num;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = chosen_col_num - 1;
            }
        }
    }

    //cout << "bxs:" << bxs << ", bxe:" << bxe << ", bys:" << bys << ", bye:" << bye << endl;

    int cnt = 0;
    if (dice_type == 1) {
        for (int r = bys; r <= bye; ++r) {
            for (int c = bxs; c <= bxe; ++c) {
                board[r][c] = cut_pieces[cnt];
                cnt++;
            }
        }
    } else if (dice_type == 2 || dice_type == 3) {
        for (int r = bys; r <= bye; ++r) {
            for (int c = bxs; c <= bxe; ++c) {
                if (board[r][c] == 0) {
                    board[r][c] = cut_pieces[cnt];
                    cnt++;
                }
            }
        }
    }

    // Checking for empty cells
    // for (int r = 0; r < n; ++r) {
    //     for (int c = 0; c < m; ++c) {
    //         if (board[r][c] == 0) {
    //             cout << "AAAAAAIIIIIIIIIIIINNNNNNNNN" << endl;
    //         }
    //     }
    // }

    return;
}

