#ifndef TYPES_H
#define TYPES_H

#include <vector>

// Structure to represent a custom die
class CustomDie {
public:
    int width, height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values

    CustomDie(int width, int height){
        this -> width = width;
        this -> height = height;
    }
};

// Structure to represent the game board
class Board {
public:
    int height, width;
    std::vector<std::vector<int>> pieces;

    Board() : height(0), width(0) {}

    Board(int height, int width, std::vector<std::vector<int>> pieces){
        this -> height = height;
        this -> width = width;
        this -> pieces = pieces;
    }
};

// Structure to represent a move
class Action {
public:
    int x, y;
    int die_index;
    int direction;
    Action(int x = 0, int y = 0, int die_index = 0, int direction = 0) {
        this -> x = x;
        this -> y = y;
        this -> die_index = die_index;
        this -> direction = direction;
    }
};

// Structure to represent the entire game state
class GameState {
public:
    int h = 0, g, f;
    Board board;
    Board goal_state;
    Action action;
    GameState* parent;

    GameState(Board board, Board goal_state, GameState* parent = nullptr, Action action = Action(), int g = 0){
        this -> board = board;
        this -> goal_state = goal_state;
        this -> parent = parent;
        this -> action = action;
        this -> g = g;
        compute_heuristics();
        f = g + h;
    }
    
    void apply_die(Action action);
    void raction(Action action);
    void display_game_state();
    void compute_heuristics();
    void print_path();
    void setter(GameState* parent = nullptr, Action action = Action(), int g = 0);

    bool is_solved();
    bool operator<(const GameState & other) const;
};

void reverseTypeI(int X, int Y, int size, int s, int n, int m, Board& _board);
void reverseTypeII(int X, int Y, int size, int s, int n, int m, Board& _board);
void reverseTypeIII(int X, int Y, int size, int s, int n, int m, Board& _board);

int nearest(int x);
std::vector<Action> gen_actions(int n, int m);
bool is_inside(int x, int y, int n, int m, int dtype);

int rowcheck(Board odoogiinstate , Board goalstate , int n , int m);
int columncheck(Board odoogiinstate , Board goalstate , int n , int m);

~
#endif //TYPES_H