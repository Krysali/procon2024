#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <string>
#include "json.hpp"

const int MAX_DIMENSION = 256;

// Structure to represent a stencil
struct Stencil {
    int width;
    int height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values
};

// Structure to represent the game board
struct Board {
    int width;
    int height;
    std::vector<std::vector<int>> pieces;
};

// Structure to represent the entire game state
struct GameState {
    Board board;
    Board goal_state;
    std::vector<Stencil> stencils;  // Includes both fixed and general stencils
    int num_moves;
};

// Function to read json file
std::string ReadJsonFile(const std::string& file_path);

// Function to parse json
GameState ParseJson(const std::string& problem_json);

// Function to generate stencils
std::vector<Stencil> GenerateStencils();

#endif // TYPES_H