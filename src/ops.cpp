#include <ops.h>

void apply_die(GameState& game_state, Action action) {

    auto board = game_state.board.pieces;
    int n = game_state.board.height;
    int m = game_state.board.width;
    // Set up
    int power = ceil(action.die_index / 3.0);
    int size = int(pow(2, power));
    int dice_type = (power == 0) ? 1 : action.die_index - ((power - 1) * 3);

    // cout << "dice_index:" << action.dice_index << ", n:" << n << ", m:" << m << ", power:" << power << ", size:" << size 
    //      << ", dice_type:" << dice_type << ", dir:" << action.direction << endl;

    std::vector<int> cut_pieces;

    int x_start = std::max(action.x, 0);
    int y_start = std::max(action.y, 0);
    int x_end = std::min(m, action.x + size) - 1;
    int y_end = std::min(n, action.y + size) - 1;

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
    if (action.direction == 0) {
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
    }else if(action.direction == 1) {
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
    }else if (action.direction == 2) {
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
    }else if (action.direction == 3) {
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
        if (action.direction < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.direction == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.direction == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 2) {
        if (action.direction < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.direction == 0) {
                bys = n - chosen_row_num;
                bye = n - 1;
            } else {
                bys = 0;
                bye = chosen_row_num - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.direction == 2) {
                bxs = m - width;
                bxe = m - 1;
            } else {
                bxs = 0;
                bxe = width - 1;
            }
        }
    } else if (dice_type == 3) {
        if (action.direction < 2) {
            bxs = x_start;
            bxe = x_end;
            if (action.direction == 0) {
                bys = n - height;
                bye = n - 1;
            } else {
                bys = 0;
                bye = height - 1;
            }
        } else {
            bys = y_start;
            bye = y_end;
            if (action.direction == 2) {
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

bool is_inside(int x, int y, int n, int m, int dtype) {
    if (dtype == 2) {
        return (x >= 0 && x < m);
    } else {
        return (y >= 0 && y < n);
    }
}

void display_game_state(const GameState& game_state) {
    const Board& board = game_state.board;
    // Iterate over rows of the board
    for (int row = 0; row < board.height; ++row) {
        // Iterate over pieces in each row
        for (int col = 0; col < board.width; ++col) {
            if (board.pieces[row][col] == -1) {
                std::cout << " ";
            } else {
                std::cout << board.pieces[row][col];
            }
        }
        std::cout << std::endl;
    }
    std::cout << "Number of moves: " << game_state.num_moves << "\n\n";
}
