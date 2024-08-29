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

// Function to read json file
std::string ReadJsonFile(const std::string& file_path);

// Function to parse json
GameState ParseJson(const std::string& problem_json);

// Function to generate dies
std::vector<Die> GenerateFixedDies();

#endif // TYPES_H