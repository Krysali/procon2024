#include <types.h>
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

// Parse the JSON data and return the game state
GameState ParseJson(const std::string& problem_json) {
    GameState game_state;
    game_state.dies = GenerateFixedDies();
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

    // Parse die data
    auto general_dies_data = json_data["general"];
    for (auto& die : general_dies_data["patterns"]) {
        std::vector<std::vector<bool>> general_die_cells;
        for (auto& row : die["cells"]) {
            std::vector<bool> die_row;
            for (char& c : row.get<std::string>()) {
                die_row.push_back(c == '1');
            }
            general_die_cells.push_back(die_row);
        }
        game_state.dies.push_back({die["width"], die["height"], general_die_cells});
    }

    game_state.num_moves = 0;
    
    return game_state;
}

std::string OutputJson(const GameState& game_state) {
    nlohmann::json output_json;
    output_json["n"] = game_state.num_moves;
    output_json["ops"] = nlohmann::json::array();
    for (const auto& move : game_state.moves) {
        nlohmann::json ops;
        ops["p"] = move.die_index;
        ops["x"] = move.x;
        ops["y"] = move.y;
        ops["s"] = move.direction;
        output_json["ops"].push_back(ops);
    }
    return output_json.dump();
}

std::vector<Die> GenerateFixedDies() {
    std::vector<Die> dies;

    for (int size = 1; size <= MAX_DIMENSION; size *= 2) {
        // Type I Die: All cells are 1
        Die type1_die;
        type1_die.width = size;
        type1_die.height = size;
        type1_die.cells.resize(size, std::vector<bool>(size, true));
        dies.push_back(type1_die);

        // Type II Die: Even rows are 1, odd rows are 0
        Die type2_die;
        type2_die.width = size;
        type2_die.height = size;
        type2_die.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type2_die.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type2_die.cells[i][j] = (i % 2 == 0);
                }
            }
            dies.push_back(type2_die);
        }
        // Type III Die: Even columns are 1, odd columns are 0
        Die type3_die;
        type3_die.width = size;
        type3_die.height = size;
        type3_die.cells.resize(size);
        if (size != 1) {
            for (int i = 0; i < size; ++i) {
                type3_die.cells[i].resize(size);
                for (int j = 0; j < size; ++j) {
                    type3_die.cells[i][j] = (j % 2 == 0);
                }
            }
        dies.push_back(type3_die);
        }
    }

    return dies;
}