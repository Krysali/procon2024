#include "types.h"
#include <iostream>
#include <fstream>

std::string ReadJsonFile(const std::string& file_path) {
    std::ifstream file(file_path);
    std::string json_data;
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            json_data += line + "\n";
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << file_path << std::endl;
    }
    return json_data;
}

GameState ParseJson(const std::string& problem_json) {
    GameState game_state;
    game_state.stencils = GenerateStencils();
    auto json_data = nlohmann::json::parse(problem_json);
    auto board_data = json_data["board"];

    // Parse board pieces
    std::vector<std::vector<int>> board_pieces;
    for (auto& row : board_data["start"]) {
        std::string row_string = row.get<std::string>();
        std::vector<int> row_pieces;
        for (char& c : row_string) {
            row_pieces.push_back(static_cast<int>(c - '0'));
        }
        board_pieces.push_back(row_pieces);
    }
    game_state.board = {board_data["width"], board_data["height"], board_pieces};

    // Parse goal state
    auto goal_data = board_data["goal"];
    std::vector<std::vector<int>> goal_pieces;
    for (auto& row : goal_data) {
        std::vector<int> row_pieces;
        for (char& c : row.get<std::string>()) {
            row_pieces.push_back(static_cast<int>(c - '0'));
        }
        goal_pieces.push_back(row_pieces);
    }
    game_state.goal_state = {board_data["width"], board_data["height"], goal_pieces};

    // Parse stencil data
    auto general_stencils_data = json_data["general"];
    for (auto& stencil : general_stencils_data["patterns"]) {
        std::vector<std::vector<bool>> general_stencil_cells;
        for (auto& row : stencil["cells"]) {
            std::vector<bool> stencil_row;
            for (char& c : row.get<std::string>()) {
                stencil_row.push_back(c == '1');
            }
            general_stencil_cells.push_back(stencil_row);
        }
        game_state.stencils.push_back({stencil["width"], stencil["height"], general_stencil_cells});
    }

    game_state.num_moves = 0;
    
    return game_state;
}

std::vector<Stencil> GenerateStencils() {
    std::vector<Stencil> stencils;

    for (int size = 1; size <= MAX_DIMENSION; size *= 2) {
        // Type I Stencil: All cells are 1
        Stencil type1_stencil;
        type1_stencil.width = size;
        type1_stencil.height = size;
        type1_stencil.cells.resize(size, std::vector<bool>(size, true));
        stencils.push_back(type1_stencil);

        // Type II Stencil: Even rows are 1, odd rows are 0
        Stencil type2_stencil;
        type2_stencil.width = size;
        type2_stencil.height = size;
        type2_stencil.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type2_stencil.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type2_stencil.cells[i][j] = (i % 2 == 0);
                }
            }
            stencils.push_back(type2_stencil);
        }
        // Type III Stencil: Even columns are 1, odd columns are 0
        Stencil type3_stencil;
        type3_stencil.width = size;
        type3_stencil.height = size;
        type3_stencil.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type3_stencil.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type3_stencil.cells[i][j] = (j % 2 == 0);
                }
            }
        stencils.push_back(type3_stencil);
        }
    }

    return stencils;
}