#include <ops.h>

void apply_die(GameState& game_state, int die_index, int x, int y, int direction) {
    const Die& die = game_state.dies[die_index];
    Board& board = game_state.board;
    
    // Calculate overlap boundaries
    int overlap_x_start = std::max(0, x);
    int overlap_y_start = std::max(0, y); 
    int overlap_x_end = std::min(board.width, x + die.width);
    int overlap_y_end = std::min(board.height, y + die.height);

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
    shift_pieces(board, direction, overlap_x_start, overlap_y_start, overlap_x_end, overlap_y_end);
    // Reinsert punched pieces
    switch (direction) {
        case 0:
            for (int x_index = 0; x_index < board.width; ++x_index) {
                int current_y = board.height - 1;
                for (int piece_index = punched_pieces.size() - 1; piece_index >= 0; --piece_index){
                    if (punched_pieces[piece_index].y == x_index) {
                        board.pieces[current_y][x_index] = punched_pieces[piece_index].value;
                        current_y--;
                    }
                }
            }
        break;
        case 1:
            for (int i = 0; i < board.width; ++i) {
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
            for (int i = 0; i < board.height; ++i) {
                int current_x = board.width - 1;
                for (int j = punched_pieces.size() - 1; j >= 0; --j) {
                    if (punched_pieces[j].x == i) {
                        board.pieces[i][current_x] = punched_pieces[j].value;
                        current_x--;
                    }
                }
            }
        break;
        case 3:
            for (int i = 0; i < board.height; ++i) {
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
    game_state.num_moves++;
}

void shift_pieces(Board& board, int direction, int xstart, int ystart, int xend, int yend) 
{
    switch (direction)
    {
        case 0:
            for(int index_x = xstart; index_x < xend; index_x++){
                int last_empty = ystart;
                for(int index_y = ystart; index_y < board.height; index_y++){
                    if(board.pieces[index_y][index_x] != -1){
                        board.pieces[last_empty][index_x] = board.pieces[index_y][index_x];
                        last_empty++;
                    }
                }
                for(int index_y = last_empty; index_y < board.height; index_y++){
                    board.pieces[index_y][index_x] = -1;
                }
            }
            break;
    
        case 1:
            for(int index_x = xstart; index_x < xend; index_x++){
                int last_empty = yend - 1;
                for(int index_y = yend - 1; index_y >=0; index_y--){
                    if(board.pieces[index_y][index_x] != -1){
                        board.pieces[last_empty][index_x] = board.pieces[index_y][index_x];
                        last_empty--;
                    }
                }
                for(int index_y = last_empty; index_y >= 0; index_y--){
                    board.pieces[index_y][index_x] = -1;
                }
            }

            break;
        case 2:
            for(int index_y = ystart; index_y < yend; index_y++){
                int last_empty = xstart;
                for(int index_x = xstart; index_x < board.width; index_x++){
                    if(board.pieces[index_y][index_x] != -1){
                        board.pieces[index_y][last_empty] = board.pieces[index_y][index_x];
                        last_empty++;
                    }
                }
                for(int index_x = last_empty; index_x < board.width; index_x++){
                    board.pieces[index_y][index_x] = -1;
                }
            }
            break;
        case 3:
            for(int index_y = ystart; index_y < yend; index_y++){
                int last_empty = xend - 1;
                for(int index_x = xend - 1; index_x >=0; index_x--){
                    if(board.pieces[index_y][index_x] != -1){
                        board.pieces[index_y][last_empty] = board.pieces[index_y][index_x];
                        last_empty--;
                    }
                }
                for(int index_x = last_empty; index_x >=0; index_x--){
                    board.pieces[index_y][index_x] = -1;
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
    std::cout << "Number of moves: " << game_state.num_moves << "\n\n";
}

bool is_solved(const GameState& game_state) {
    return (game_state.board.pieces == game_state.goal_state.pieces);
}