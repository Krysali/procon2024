#ifndef OPS_H
#define OPS_H

#include <types.h>
#include <iostream>

// Function to apply a die to the board
void apply_die(GameState& game_state, Action action);

// Function to check if a given cell is inside the board
bool is_inside(int x, int y, int n, int m, int dtype);

// Function to shift pieces on the board in a given direction
void shift_pieces(Board& board, int direction, const std::vector<bool>& cellsToRemove);

// Function to check if the puzzle is solved
bool is_solved(const GameState& game_state);

// Function to display the current game state to the console
void display_game_state(const GameState& game_state);

#endif // OPS_H