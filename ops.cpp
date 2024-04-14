#include "ops.h"
#include <iostream>
#include <queue>

void apply_stencil(GameState& game_state, int stencil_index, int x, int y, int direction) {
    const Stencil& stencil = game_state.stencils[stencil_index];
    Board& board = game_state.board;

    // Calculate overlap boundaries
    int overlap_x_start = std::max(0, x);
    int overlap_y_start = std::max(0, y); 
    int overlap_x_end = std::min(board.width, x + stencil.width);
    int overlap_y_end = std::min(board.height, y + stencil.height);

    // Store punched-out pieces
    std::queue<int> punched_pieces; 
    for (int i = overlap_y_start; i < overlap_y_end; ++i) {
        for (int j = overlap_x_start; j < overlap_x_end; ++j) {
            int stencil_i = i - y;
            int stencil_j = j - x;
            if (stencil.cells[stencil_i][stencil_j]) {
                punched_pieces.push(board.pieces[i][j]);
                board.pieces[i][j] = -1; // Mark as empty
            } 
        }
    } 
    display_game_state(game_state);
    // Shift pieces
    shift_pieces(board, direction);
    display_game_state(game_state);
    // Reinsert punched pieces
    for (int i = 0; i < board.height; ++i) {
        for (int j = 0; j < board.width; ++j) {
            if (board.pieces[i][j] == -1) {
                board.pieces[i][j] = punched_pieces.front();
                punched_pieces.pop(); 
            }
        }
    }
    game_state.num_moves++;
}

void shift_pieces(Board& board, int direction) {
    switch (direction) {
        case 0: // Up
            for (int j = 0; j < board.width; ++j) {
                int write_index = 0;
                for (int i = 0; i < board.height; ++i) {
                    if (board.pieces[i][j] != -1) {
                        board.pieces[write_index][j] = board.pieces[i][j];
                        write_index++;
                    }
                }
                // Fill remaining spaces with -1
                for (int i = write_index; i < board.height; ++i) {
                    board.pieces[i][j] = -1;
                }
            }
            break;
        case 1: // Down
            for (int j = 0; j < board.width; ++j) {
                int write_index = board.height - 1;
                for (int i = board.height - 1; i >= 0; --i) {
                    if (board.pieces[i][j] != -1) {
                        board.pieces[write_index][j] = board.pieces[i][j];
                        write_index--;
                    }
                }
                // Fill remaining spaces with -1
                for (int i = write_index; i >= 0; --i) {
                    board.pieces[i][j] = -1;
                }
            }
            break;

        case 2: // Left
            for (int i = 0; i < board.height; ++i) {
                int write_index = 0;
                for (int j = 0; j < board.width; ++j) {
                    if (board.pieces[i][j] != -1) {
                        board.pieces[i][write_index] = board.pieces[i][j];
                        write_index++;
                    }
                }
                // Fill remaining spaces with -1
                for (int j = write_index; j < board.width; ++j) {
                    board.pieces[i][j] = -1;
                } 
            }
            break;

        case 3: // Right
            for (int i = 0; i < board.height; ++i) {
                int write_index = board.width - 1;
                for (int j = board.width - 1; j >= 0; --j) {
                    if (board.pieces[i][j] != -1) {
                        board.pieces[i][write_index] = board.pieces[i][j];
                        write_index--;
                    }
                }
                // Fill remaining spaces with -1
                for (int j = write_index; j >= 0; --j) { 
                    board.pieces[i][j] = -1;
                }
            }
            break;
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
            std::cout << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Number of moves: " << game_state.num_moves << std::endl;
}

bool is_solved(const GameState& game_state) {
    return (game_state.board.pieces == game_state.goal_state.pieces);
}