#include <ops.h>

void apply_die(GameState& game_state, int die_index, int x, int y, int direction) {
    const Die& die = game_state.dies[die_index];
    Board& board = game_state.board;
    
    // Calculate overlap boundaries
    int overlap_x_start = std::max(0, x);
    int overlap_y_start = std::max(0, y); 
    int overlap_x_end = std::min(board.m, x + die.width);
    int overlap_y_end = std::min(board.n, y + die.height);

    // Store punched-out pieces
    struct Piece {
        int x;
        int y;
        int value;
    };
    std::vector<Piece> punched_pieces;

    for (int i = overlap_y_start; i < overlap_y_end; ++i) {
        for (int j = overlap_x_start; j < overlap_x_end; ++j) {
            int die_i = i - y;
            int die_j = j - x;
            if (die.cells[die_i][die_j]) {
                punched_pieces.push_back({i, j, board.pieces[i][j]});
                board.pieces[i][j] = -1; // Mark as empty
            } 
        }
    } 

    // Shift pieces
    shift_pieces(board, direction);
    
    // Reinsert punched pieces
    switch (direction) {
        case 0:
            for (int x_index = 0; x_index < board.m; ++x_index) {
                int current_y = board.n - 1;
                for (int piece_index = punched_pieces.size() - 1; piece_index >= 0; --piece_index){
                    if (punched_pieces[piece_index].y == x_index) {
                        board.pieces[current_y][x_index] = punched_pieces[piece_index].value;
                        current_y--;
                    }
                }
            }
        break;
        case 1:
            for (int i = 0; i < board.m; ++i) {
                int current_y = 0;
                for (int j = 0; j < punched_pieces.size(); ++j) {
                    if (punched_pieces[j].y == i) {
                        board.pieces[current_y][i] = punched_pieces[j].value;
                        current_y++;
                    }
                }
            }
        break;
        case 2:
            for (int i = 0; i < board.n; ++i) {
                int current_x = board.m - 1;
                for (int j = punched_pieces.size() - 1; j >= 0; --j) {
                    if (punched_pieces[j].x == i) {
                        board.pieces[i][current_x] = punched_pieces[j].value;
                        current_x--;
                    }
                }
            }
        break;
        case 3:
            for (int i = 0; i < board.n; ++i) {
                int current_x = 0;
                for (int j = 0; j < punched_pieces.size(); ++j) {
                    if (punched_pieces[j].x == i) {
                        board.pieces[i][current_x] = punched_pieces[j].value;
                        current_x++;
                    }
                }
            }
        break;
    }
    // add move to game state
    game_state.moves.push_back({die_index, x, y, direction});
}

void shift_pieces(Board& board, int direction) {
    int start, end, step, write_index;

    if (direction % 2 == 0) { 
        start = 0;
        end = (direction == 0) ? board.n : board.m;
        step = 1;
    } else {
        start = (direction == 1) ? board.n - 1 : board.m - 1;
        end = -1;
        step = -1;
    }

    if (direction < 2) { // Vertical shift
        for (int j = 0; j < board.m; ++j) {
            write_index = (direction == 0) ? 0 : board.n - 1;
            for (int i = start; i != end; i += step) {
                if (board.pieces[i][j] != -1) {
                    board.pieces[write_index][j] = board.pieces[i][j];
                    write_index += step;
                }
            }
            // Fill remaining spaces with -1
            while (write_index != end) {
                board.pieces[write_index][j] = -1;
                write_index += step;
            }
        }
    } else { // Horizontal shift
        for (int i = 0; i < board.n; ++i) {
            write_index = (direction == 2) ? 0 : board.m - 1;
            for (int j = start; j != end; j += step) {
                if (board.pieces[i][j] != -1) {
                    board.pieces[i][write_index] = board.pieces[i][j];
                    write_index += step;
                }
            }
            // Fill remaining spaces with -1
            while (write_index != end) {
                board.pieces[i][write_index] = -1;
                write_index += step;
            }
        }
    }
}

void display_game_state(const GameState& game_state) {
    const Board& board = game_state.board;
    std::cout << "┏";
    for (int i = 0; i < board.m * 2 + 1; ++i) {
        std::cout << "━";
    }
    std::cout << "┓" << std::endl;
    // Iterate over rows of the board
    for (int row = 0; row < board.n; ++row) {
        std::cout << "┃ ";
        // Iterate over pieces in each row
        for (int col = 0; col < board.m; ++col) {
            if (board.pieces[row][col] == -1) {
                std::cout << " ";
            } else {
                std::cout << board.pieces[row][col];
            }
            std::cout << " ";
        }
        std::cout << "┃" << std::endl;
    }
    std::cout << "┗";
    for (int i = 0; i < board.m * 2 + 1; ++i) {
        std::cout << "━";
    }
    std::cout << "┛" << std::endl;

    std::cout << "Number of moves: " << game_state.num_moves << std::endl;
}

bool is_solved(const GameState& game_state) {
    return (game_state.board.pieces == game_state.goal_state.pieces);
}

