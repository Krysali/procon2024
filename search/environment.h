#include <vector>
#include <fstream>
#include <cstdint>

/*** Abstract Environment Class ***/

class Board {
public:
    int height, width;
    std::vector<std::vector<int>> pieces;

    Board() : height(0), width(0) {}

    Board(int height, int width, std::vector<std::vector<int>> pieces) {
        this->height = height;
        this->width = width;
        this->pieces = pieces;
    }
};

class Action {
public:
    int x, y;
    int die_index;
    int direction;

    Action(int x, int y, int die_index, int direction) {
        this->x = x;
        this->y = y;
        this->die_index = die_index;
        this->direction = direction;
    }
};
class Environment {
  public:
    virtual ~Environment() {}
    virtual Environment *getNextState(Action action) const = 0;
    virtual std::vector<Environment*> getNextStates() const = 0;
    virtual std::vector<int> getState() const = 0;
    virtual bool is_solved() const = 0;
    virtual int getNumActions() const = 0;
};


class Gamestate : public Environment {
private:
    Board board;
    Board goal_state;
    int numActions;
    std::vector<Action> actions;

public:
    // Constructor that initializes the game state from a 1D vector representing the initial state.
    Gamestate(const std::vector<uint8_t>& initialState, int height, int width) {
    }
    Gamestate(const Board& newBoard);

    ~Gamestate() {}

    // Override Environment's pure virtual functions
    virtual Environment* getNextState(Action action) const override {
        // Logic to get the next state based on the action
        return nullptr;
    }

    virtual std::vector<Environment*> getNextStates() const override {
        // Logic to generate all possible next states
        std::vector<Environment*> nextStates;
        return nextStates;
    }

    virtual std::vector<int> getState() const override {
        // Flatten the board state into a 1D vector and return it
        std::vector<int> flattenedState;
        for (const auto& row : board.pieces) {
            flattenedState.insert(flattenedState.end(), row.begin(), row.end());
        }
        return flattenedState;
    }

    virtual bool is_solved() const override {

        bool solved= true ; 
        if (board.pieces != goal_state.pieces){
            solved=false ; 
        }
	return (solved);
    }

    virtual int getNumActions() const override {
        return numActions;
    }
};
