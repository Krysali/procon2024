#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <test.h>
#include <nlohmann/json.hpp>

// DEPRECATED: Use only as a fallback when the API fails
std::string ReadJsonFile(const std::string& file_path);

// Function to parse json data into a game state
GameState ParseJson(const std::string& problem_json);

// Function to output json using the game state
std::string OutputJson(const GameState& game_state);

#endif // TYPES_H