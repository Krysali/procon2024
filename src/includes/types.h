#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>

const int MAX_DIMENSION = 256;

// Structure to represent a die
class Die {
public:
    int width, height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values
};

// Structure to represent the game board
class Board {
public:
    int width, height;
    std::vector<std::vector<int>> pieces;
};

// Structure to represent a move
class Move {
public:
    int die_index;
    int x, y;
    int direction;
};

// Class to represent the entire game state
class GameState {
public:
    Board board;
    Board goal_state;
    std::vector<Die> dies;
    std::vector<Move> moves;
	int num_moves;
};

// DEPRECATED: Use only as a fallback when the API fails
std::string ReadJsonFile(const std::string& file_path);

// Function to parse json data into a game state
GameState ParseJson(const std::string& problem_json);

// Function to output json using the game state
std::string OutputJson(const GameState& game_state);

// Function to generate dies
std::vector<Die> GenerateFixedDies();

#endif // TYPES_H
