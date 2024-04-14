#ifndef OPS_H
#define OPS_H

#include "types.h"
#include <iostream>

// Function to apply a stencil to the board
void apply_stencil(GameState& game_state, int stencil_index, int x, int y, int direction);

// Function to shift pieces on the board in a given direction
void shift_pieces(Board& board, int direction);

// Function to check if the puzzle is solved
bool is_solved(const GameState& game_state);

// Function to display the current game state to the console
void display_game_state(const GameState& game_state);

#endif // OPS_H