#include <bits/stdc++.h>
#include<iostream>
#include<map>
#include<set>
#include<cmath>
#include<queue>
#include<deque>
#include<stack>
#include<string>
#include<math.h>
#include<vector>
#include<stdio.h>
#include<utility>
#include<iomanip>
#include<string.h>
#include<limits.h>
#include<algorithm>
#include<functional>
#include<unordered_map> 
using namespace std;

#pragma GCC target("popcnt")
#define MOD 1000000007
// #define int long long
#define ss second
#define ff first
#define endl '\n'

const int DOWN = 0;
const int UP = 1;
const int LEFT = 2;
const int RIGHT = 3;

struct Node {
    vector<vector<int>> board;
    vector<Node*> neighbors;
};

struct PathNode {
    Node* node;
    vector<tuple<int, int, int, int>> path;
};

void shift_pieces(vector<vector<int>>& board, int direction) {
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

void apply_die(vector<vector<int>>& board, const vector<vector<int>>& die, int x, int y, int direction) {
    int board_width = board[0].size();
    int board_height = board.size();
    int die_width = die[0].size();
    int die_height = die.size();
    
    int overlap_x_start = max(0, x);
    int overlap_y_start = max(0, y); 
    int overlap_x_end = min(board_width, x + die_width);
    int overlap_y_end = min(board_height, y + die_height);

    struct Piece {
        int x;
        int y;
        int value;
    };
    vector<Piece> punched_pieces;

    for (int i = overlap_y_start; i < overlap_y_end; ++i) {
        for (int j = overlap_x_start; j < overlap_x_end; ++j) {
            int die_i = i - y;
            int die_j = j - x;
            if (die[die_i][die_j]) {
                punched_pieces.push_back({i, j, board[i][j]});
                board[i][j] = -1; 
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

string board_to_string(const vector<vector<int>>& board) {
    string result;
    for (const auto& row : board) {
        for (int cell : row) {
            result += to_string(cell) + ",";
        }
        result += ";";
    }
    return result;
}

int poooo= 0 ; 

vector<tuple<int, int, int, int>> bfs_find_path(const vector<vector<int>>& startBoard,
                                                const vector<vector<int>>& finalBoard,
                                                const vector<vector<vector<int>>>& dies) {
    unordered_map<string, Node*> graph;
    queue<PathNode> to_visit;
    unordered_map<string, bool> visited;

    Node* start_node = new Node{startBoard, {}};
    graph[board_to_string(startBoard)] = start_node;
    to_visit.push({start_node, {}});

    while (!to_visit.empty()) {
        PathNode current_path_node = to_visit.front();
        Node* current_node = current_path_node.node;
        to_visit.pop();

        string current_board_str = board_to_string(current_node->board);
        if (visited[current_board_str]) continue;
        visited[current_board_str] = true;

        if (current_node->board == finalBoard) {
            for (auto& entry : graph) {
                delete entry.second;
            }
            return current_path_node.path; 
        }

        for (int die_index = 0; die_index < dies.size(); ++die_index) {
            const auto& die = dies[die_index];
            int board_width = current_node->board[0].size();
            int board_height = current_node->board.size();

            for (int x = 0; x < board_width; ++x) {
                for (int y = 0; y < board_height; ++y) {
                    for (int direction = DOWN; direction <= RIGHT; ++direction) {
                        vector<vector<int>> new_board = current_node->board;
                        apply_die(new_board, die, x, y, direction);

                        string new_board_str = board_to_string(new_board);

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
                            vector<tuple<int, int, int, int>> new_path = current_path_node.path;
                            new_path.emplace_back(x, y, die_index, direction);

                            to_visit.push({new_node, new_path});
                        }
                    }
                }
            }
        }
    }

    for (auto& entry : graph) {
        delete entry.second;
    }
    
    return {};  
}

int main() {

    vector<vector<int>> startBoard = {  
        {3,3,2,3},
        {3,3,2,0},
        {3,3,2,3},
        {3,3,2,3}
        // {3 ,2},
        // {1 ,0}   
        };
       
    vector<vector<int>> finalBoard = {   
        {3,3,3,3},
        {2,2,2,2},
        {3,3,3,3},
        {3,3,3,0} 
        // {0, 2},
        // {1, 3}
        };

    vector<vector<vector<int>>> dies = {
        {{1}},
        {{1,1}, {1,1}},
        {{1,1}, {0,0}},
        {{1,0}, {1,0}},
        {{1,1,1,1}, {1,1,1,1}, {1,1,1,1}, {1,1,1,1}},
        {{1,1,1,1}, {0,0,0,0}, {1,1,1,1}, {0,0,0,0}},
        {{1,0,1,0}, {1,0,1,0}, {1,0,1,0}, {1,0,1,0}}
    };

    vector<tuple<int, int, int, int>> path = bfs_find_path(startBoard, finalBoard, dies);

    for (const auto& step : path) {
        int x, y, die_index, direction;
        tie(x, y, die_index, direction) = step;
        cout << "X: " << x << " Y: " << y << " Die Index: " << die_index << " Direction: " << direction << endl;
    }

    return 0;
}
