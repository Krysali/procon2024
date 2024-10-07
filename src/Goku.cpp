#include <types.h>
#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <set>
#include <string>
bool Ans = true;
// A* search algorithm
void aStarSearch(GameState CurrentState) {
    std::priority_queue<GameState> open;
    std::set<GameState> closed;
    
    std::map<std::vector<std::vector<int>>, GameState*> google;

    std::vector<Action> actions;
    actions = gen_actions(CurrentState.board.height , CurrentState.board.width);
    
    open.push(CurrentState);

    while (!open.empty()) {
        GameState current = open.top();
        open.pop();
        std::map<std::vector<std::vector<int>> , int> check;

        // If the goal state is reached, print the solution and exit
        if(current.is_solved() && Ans == false){
            std::cout << "Solution found";
            current.print_path();
            Ans = true;
            return;
        }
        
        // inserting current
        closed.insert(current);

        //Find neighbors
        for(int a = 0; a < actions.size(); a++){
            GameState next = current;

            next.apply_die(actions[a]);

            if(check[next.board.pieces] == 0){
                check[next.board.pieces] = 1;
                GameState* ptr = google[next.board.pieces];

                int tentative_g = current.g - 1;

                if (ptr == nullptr || tentative_g < next.g) {
                  // Update neighbor's g and f values
                  next.g = tentative_g;
                  next.compute_heuristics();
                  next.f = next.g + next.h;

                  // Add neighbor to open list if not already evaluated
                  if (ptr == nullptr) {
                     next.setter(&current, actions[a], current.g - 1);
                     open.push(next);
                  }
                }
            }
        }
    }
    std::cout << "No solution found!" << std::endl;
}

int main() {
    std::cout << "Astar" << std::endl;
    //inputs
    int n, m;
    std::cout << "Width: ";
    std::cin >> m;
    std::cout << std::endl;
    std::cout << "Height: ";
    std::cin >> n;

    // urgelj m ni urt tal ni baina 
    if(n > m) std::swap(n, m);

    // Start state 
    Board StartBoard , GoalBoard;

    //Startboard
    std::cout << "Startboard ;" << std::endl;
    StartBoard.height = n;
    StartBoard.width = m;
    for(int i = 0; i < n; i++){
        std::vector<int> row;
        for(int j = 0; j < m; j++){
            int piece;
            std::cin >> piece;
            row.push_back(piece);
        }
        StartBoard.pieces.push_back(row);
    }
    
    //GoalBoard
    std::cout << "Goalboard ;" << std::endl;
    GoalBoard.height = n;
    GoalBoard.width = m;
    for(int i = 0; i < n; i++){
        std::vector<int> row;
        for(int j = 0; j < m; j++){
            int piece;
            std::cin >> piece;
            row.push_back(piece);
        }
        GoalBoard.pieces.push_back(row);
    }

    GameState CurrentState(StartBoard , GoalBoard);

    // Perform A* search to solve the Game
    aStarSearch(CurrentState);

    return 0;
}