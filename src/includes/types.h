#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

const int MAX_DIMENSION = 256;

// Structure to represent a die
struct Die {
    int width, height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values
};

// Structure to represent the game board
struct Board {
    int width, height;
    std::vector<std::vector<int>> pieces;
};

// Structure to represent a move
struct Move {
    int die_index;
    int x, y;
    int direction;
};

// Structure to represent the entire game state
struct GameState {
    Board board;
    Board goal_state;
    std::vector<Die> dies;  // Includes both fixed and general dies
    int num_moves;
    std::vector<Move> moves;
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