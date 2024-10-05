#ifndef TEST_H
#define TEST_H

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

    Action(int x, int y, int die_index, int direction) {
        this -> x = x;
        this -> y = y;
        this -> die_index = die_index;
        this -> direction = direction;
    }
};

// Structure to represent the entire game state
class GameState {
public:
    Board board;
    Board goal_state;
    std::vector<Action> actions;

    GameState() {
    }

    GameState(Board b, Board g){
        board = b;
        goal_state = g;
    }
    
    void apply_die(Action action);
    void raction(Action action);
    void display_game_state();
    bool is_solved();
};

void reverseTypeI(int X, int Y, int size, int s, int n, int m, Board& _board);
void reverseTypeII(int X, int Y, int size, int s, int n, int m, Board& _board);
void reverseTypeIII(int X, int Y, int size, int s, int n, int m, Board& _board);

#endif // TEST_H