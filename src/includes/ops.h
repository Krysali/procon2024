#ifndef OPS_H
#define OPS_H

#include <types.h>
#include <iostream>

// Function to apply a die to the board
void apply_die(GameState& game_state, int die_index, int x, int y, int direction);

// Function to shift pieces on the board in a given direction
void shift_pieces(Board& board, int direction);

// Function to check if the puzzle is solved
bool is_solved(const GameState& game_state);

#endif // OPS_H
