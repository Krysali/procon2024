#ifndef JSON_OPS_H
#define JSON_OPS_H

#include <string>
#include <types.h>

std::string ReadJsonFile(const std::string& file_path);

GameState ParseJson(const std::string& problem_json);

std::string OutputJson(const GameState& game_state);

#endif // JSON_OPS_H