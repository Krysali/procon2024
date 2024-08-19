#include <bits/stdc++.h>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std ; 
const int DOWN = 0;
const int UP = 1;
const int LEFT = 2;
const int RIGHT = 3;



struct Node {
    std::vector<std::vector<int>> board;
    std::vector<Node*> neighbors;
};


struct PathNode {
    Node* node;
    std::vector<std::tuple<int, int, int, int>> path;
};

void shift_pieces(std::vector<std::vector<int>>& board, int direction) {
    int board_width = board[0].size();
    int board_height = board.size();
    int start, end, step, write_index;

    if (direction % 2 == 0) { 
        start = 0;
        end = (direction == DOWN) ? board_height : board_width;
        step = 1;
    } else {
        start = (direction == UP) ? board_height - 1 : board_width - 1;
        end = -1;
        step = -1;
    }

    if (direction < 2) { 
        for (int j = 0; j < board_width; ++j) {
            write_index = (direction == DOWN) ? 0 : board_height - 1;
            for (int i = start; i != end; i += step) {
                if (board[i][j] != -1) {
                    board[write_index][j] = board[i][j];
                    write_index += step;
                }
            }
           
            while (write_index != end) {
                board[write_index][j] = -1;
                write_index += step;
            }
        }
    } else {
        for (int i = 0; i < board_height; ++i) {
            write_index = (direction == LEFT) ? 0 : board_width - 1;
            for (int j = start; j != end; j += step) {
                if (board[i][j] != -1) {
                    board[i][write_index] = board[i][j];
                    write_index += step;
                }
            }
            
            while (write_index != end) {
                board[i][write_index] = -1;
                write_index += step;
            }
        }
    }
}



void apply_die(std::vector<std::vector<int>>& board, const std::vector<std::vector<int>>& die, int x, int y, int direction) {
    int board_width = board[0].size();
    int board_height = board.size();
    int die_width = die[0].size();
    int die_height = die.size();
    

    int overlap_x_start = std::max(0, x);
    int overlap_y_start = std::max(0, y); 
    int overlap_x_end = std::min(board_width, x + die_width);
    int overlap_y_end = std::min(board_height, y + die_height);


    struct Piece {
        int x;
        int y;
        int value;
    };
    std::vector<Piece> punched_pieces;

    for (int i = overlap_y_start; i < overlap_y_end; ++i) {
        for (int j = overlap_x_start; j < overlap_x_end; ++j) {
            int die_i = i - y;
            int die_j = j - x;
            if (die[die_i][die_j]) {
                punched_pieces.push_back({i, j, board[i][j]});
                board[i][j] = -1; // Mark as empty
            } 
        }
    } 


    shift_pieces(board, direction);
    

    switch (direction) {
        case DOWN:
            for (int x_index = 0; x_index < board_width; ++x_index) {
                int current_y = board_height - 1;
                for (int piece_index = punched_pieces.size() - 1; piece_index >= 0; --piece_index){
                    if (punched_pieces[piece_index].y == x_index) {
                        board[current_y][x_index] = punched_pieces[piece_index].value;
                        current_y--;
                    }
                }
            }
            break;
        case UP:
            for (int i = 0; i < board_width; ++i) {
                int current_y = 0;
                for (int j = 0; j < punched_pieces.size(); ++j) {
                    if (punched_pieces[j].y == i) {
                        board[current_y][i] = punched_pieces[j].value;
                        current_y++;
                    }
                }
            }
            break;
        case LEFT:
            for (int i = 0; i < board_height; ++i) {
                int current_x = board_width - 1;
                for (int j = punched_pieces.size() - 1; j >= 0; --j) {
                    if (punched_pieces[j].x == i) {
                        board[i][current_x] = punched_pieces[j].value;
                        current_x--;
                    }
                }
            }
            break;
        case RIGHT:
            for (int i = 0; i < board_height; ++i) {
                int current_x = 0;
                for (int j = 0; j < punched_pieces.size(); ++j) {
                    if (punched_pieces[j].x == i) {
                        board[i][current_x] = punched_pieces[j].value;
                        current_x++;
                    }
                }
            }
            break;
    }
}



std::string board_to_string(const std::vector<std::vector<int>>& board) {
    std::string result;
    for (const auto& row : board) {
        for (int cell : row) {
            result += std::to_string(cell) + ",";
        }
        result += ";";
    }
    return result;
}


int poooo= 0 ; 

std::vector<std::tuple<int, int, int, int>> bfs_find_path(const std::vector<std::vector<int>>& startBoard,
                                                          const std::vector<std::vector<int>>& finalBoard,
                                                          const std::vector<std::vector<std::vector<int>>>& dies) {
    std::unordered_map<std::string, Node*> graph;
    std::queue<PathNode> to_visit;
    std::unordered_map<std::string, bool> visited;

    // Initialize the start node
    Node* start_node = new Node{startBoard, {}};
    graph[board_to_string(startBoard)] = start_node;
    to_visit.push({start_node, {}});

    while (!to_visit.empty()) {
        PathNode current_path_node = to_visit.front();
        Node* current_node = current_path_node.node;
        to_visit.pop();

        std::string current_board_str = board_to_string(current_node->board);
        if (visited[current_board_str]) continue;
        visited[current_board_str] = true;

        // Check if the current state is the final state
        if (current_node->board == finalBoard) {
            // Clean up
            for (auto& entry : graph) {
                delete entry.second;
            }
            return current_path_node.path;  // Return the path to the final state
        }

     
        for (int die_index = 0; die_index < dies.size(); ++die_index) {
            const auto& die = dies[die_index];
            int board_width = current_node->board[0].size();
            int board_height = current_node->board.size();

            for (int x = 0; x < board_width; ++x) {
                for (int y = 0; y < board_height; ++y) {
                    for (int direction = DOWN; direction <= RIGHT; ++direction) {
                        std::vector<std::vector<int>> new_board = current_node->board;
                        apply_die(new_board, die, x, y, direction);

                        std::string new_board_str = board_to_string(new_board);

                     
                        if (graph.find(new_board_str) == graph.end()) {
                            Node* new_node = new Node{new_board, {}};

                            poooo++; 
                            cout << "Suudriin huvilbar : "<< poooo;
                            cout << endl ;  
                            for (const auto& row : new_board) {
                                 for (int cell : row) {
                	                cout << cell<<"  " ;
                                }
                                cout << endl ; 
                            }
                            graph[new_board_str] = new_node;
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                            std::vector<std::tuple<int, int, int, int>> new_path = current_path_node.path;
                            new_path.emplace_back(x, y, die_index, direction);

                            to_visit.push({new_node, new_path});
                        }
                    }
                }
            }
        }
    }

    // Clean up
    for (auto& entry : graph) {
        delete entry.second;
    }
    
    return {};  
}

int main() {

    std::vector<std::vector<int>> startBoard = {  
        {3,3,2,3},
        {3,3,2,0},
        {3,3,2,3},
        {3,3,2,3}
        // {3 ,2},
        // {1 ,0}   
        };
       
    std::vector<std::vector<int>> finalBoard = {   
        {3,3,3,3},
        {2,2,2,2},
        {3,3,3,3},
        {3,3,3,0} 
        // {0, 2},
        // {1, 3}
        };


    std::vector<std::vector<std::vector<int>>> dies = { 
        {{1}},
        {{1,1}, {1,1}},
        {{1,1}, {0,0}},
        {{1,0}, {1,0}},
        {{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}},
        {{1,1,1,1}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        {{1,0,1,0}, {1,0,1,0}, {1,0,1,0}, {1,0,1,0}}
       
    };

  
    std::vector<std::tuple<int, int, int, int>> path = bfs_find_path(startBoard, finalBoard, dies);

    if (path.empty()) {
        std::cout << "bandi " << std::endl;
    } else {
        std::cout << " Oldtsooon bndi " << std::endl;
        for (const auto& step : path) {
            int x, y, die_index, direction;
            std::tie(x, y, die_index, direction) = step;
            std::cout << "Die  " << die_index << " bairlal (" << x << ", " << y << ")  direction " << direction << std::endl;
        }
    }

    return 0;
}
