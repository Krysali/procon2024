#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

using namespace std;

struct Action {
    int dice_num;
    int x;
    int y;
    int dir;
};

// Function to check if the point (x, y) is inside the boundaries
bool is_inside(int x, int y, int n, int m, int dtype) {
    if (dtype == 2) {
        return (x >= 0 && x < m);
    } else {
        return (y >= 0 && y < n);
    }
}

// Function to apply the die action
void apply_die(int n, int m, int board[256][256], Action action) {

    // Set up
    int power = ceil(action.dice_num / 3.0);
    int size = int(pow(2, power));
    int dice_type = (power == 0) ? 1 : action.dice_num - ((power - 1) * 3);

    cout << "dice_num:" << action.dice_num << ", n:" << n << ", m:" << m << ", power:" << power << ", size:" << size 
         << ", dice_type:" << dice_type << ", dir:" << action.dir << endl;

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

    cout << "x_start:" << x_start << ", x_end:" << x_end << ", y_start:" << y_start 
         << ", y_end:" << y_end << ", width:" << width << ", height:" << height 
         << ", chosen_row_num:" << chosen_row_num << ", chosen_col_num:" << chosen_col_num << endl;

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

    // Printing cut pieces
    for (int piece : cut_pieces) {
        cout << piece << " ";
    }
    cout << endl;

    // Printing modified board
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << board[i][j] << ' ';
        }
        cout << '\n';
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

    cout << "bxs:" << bxs << ", bxe:" << bxe << ", bys:" << bys << ", bye:" << bye << endl;

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
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < m; ++c) {
            if (board[r][c] == 0) {
                cout << "AAAAAAIIIIIIIIIIIINNNNNNNNN" << endl;
            }
        }
    }

    return;
}




