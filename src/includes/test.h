#ifndef TEST_H
#define TEST_H

#include <vector>

// Structure to represent a custom die
struct CustomDie {
    int width, height;
    std::vector<std::vector<bool>> cells;  // Use bool for 0/1 values

    CustomDie(int w, int h){
        width = w;
        height = h;
    }
};

// Structure to represent the game board
struct Board {
    int n, m;
    std::vector<std::vector<int>> pieces;

    Board() : n(0), m(0) {}

    Board(int N, int M, std::vector<std::vector<int>> p){
        n = N;
        m = M;
        pieces = p;
    }
};

// Structure to represent a move
struct Action {
    int x, y;
    int die_index;
    int direction;

    Action(int X, int Y, int di, int dir){
        x = X;
        y = Y;
        die_index = di;
        direction = dir;
    }
};

// Structure to represent the entire game state
struct GameState {
    Board board;
    Board goal_state;
    std::vector<Action> moves;

    GameState(Board b, Board g){
        board = b;
        goal_state = g;
    }
    
    GameState apply_die(Action action);
    GameState raction(Action action);
    bool is_solved();
};


#endif // TEST_H