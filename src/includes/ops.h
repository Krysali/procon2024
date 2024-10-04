#ifndef OPS_H
#define OPS_H

#include <types.h>
#include <iostream>

// Function to apply a die to the board
void apply_die(GameState& game_state, Action action);

// Function to display the current game state to the console
void display_game_state(const GameState& game_state);

#endif // OPS_H