#include <vector>
#include <fstream>

/*** Abstract Environment Class ***/

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

class Environment {
  public:
		virtual ~Environment()=0;
    virtual Environment *getNextState(int x , int y, int die_index, int dir) const = 0;

    virtual std::vector< Environment*> getNextStates() const = 0;

    virtual std::vector<std::vector<int>>  getState() const = 0;

    virtual bool isSolved() const = 0;

    virtual int getNumActions() const = 0;
};

/*** Gamestate ***/
class GameState: public Environment {
	private:
		std::vector<std::vector<int>> state; 
        std::vector<std::vector<int>> goalstate;
		int height_state;
		int width_state;
        int numActions=0; 
        std::vector<Action>actions; 

    // virtual void construct(std::vector<std::vector<int>> state, std::vector<std::vector<int>> goalstate, int height, int width);
	public:
		GameState(std::vector<std::vector<int>> state, std::vector<std::vector<int>> goalstate, int height, int width );
		~GameState();

		
        
    virtual GameState *getNextState(int x , int y, int die_index, int dir) const ;

    virtual std::vector<Environment*> getNextStates() const;

    virtual std::vector<std::vector<int>>  getState() const;

    virtual bool isSolved() const;

    virtual int getNumActions() const;
};

