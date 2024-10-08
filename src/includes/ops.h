#ifndef OPS_H
#define OPS_H

#include <types.h>
#include <iostream>

void apply_die(GameState& game_state, const Action& action);
std::vector<int> punch_pieces(GameState& game_state, const Action& action);
void shift_pieces(GameState& game_state, const Action& action);
void put_back_pieces(GameState& game_state, const Action& action, std::vector<int>& punched_pieces);

// Function to display the current game state to the console
void display_game_state(const GameState& game_state);

#endif // OPS_H
