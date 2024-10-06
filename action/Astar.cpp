#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>

using namespace std;

const int N = 3;  // Size of the puzzle (3x3 grid)

// Utility to represent the goal state
vector<vector<int>> goal = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

// Structure to represent a state of the puzzle
struct PuzzleState {
    vector<vector<int>> board;  // Current state of the puzzle
    int x, y;  // Position of the empty tile (0)
    int g;     // Cost to reach this state (number of moves)
    int h;     // Heuristic cost to reach the goal state
    int f;     // Total cost f = g + h

    // Compare function for priority queue (min-heap)
    bool operator<(const PuzzleState& other) const {
        return f > other.f;  // Min-heap (lower f gets higher priority)
    }
}; 

// Directions for movement (up, down, left, right)
int dx[4] = { -1, 1, 0, 0 };
int dy[4] = { 0, 0, -1, 1 };

// Function to calculate the Manhattan Distance heuristic
int manhattanDistance(vector<vector<int>>& board) {
    int dist = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (board[i][j] != 0) {
                int value = board[i][j];
                int target_x = (value - 1) / N;
                int target_y = (value - 1) % N;
                dist += abs(i - target_x) + abs(j - target_y);
            }
        }
    }
    return dist;
}

// Utility to check if the current state is the goal state
bool isGoalState(vector<vector<int>>& board) {
    return board == goal;
}

// Function to print the current state of the puzzle
void printBoard(const vector<vector<int>>& board) {
    for (const auto& row : board) {
        for (int tile : row) {
            if (tile == 0) cout << "  ";
            else cout << tile << " ";
        }
        cout << endl;
    }
}

// A* search algorithm
void aStarSearch(vector<vector<int>>& start, int x, int y) {
    priority_queue<PuzzleState> pq;
    map<vector<vector<int>>, bool> visited;

    // Initial state
    PuzzleState initial = { start, x, y, 0, manhattanDistance(start), manhattanDistance(start) };
    pq.push(initial);

    while (!pq.empty()) {
        PuzzleState current = pq.top();
        pq.pop();

        // If the goal state is reached, print the solution and exit
        if (isGoalState(current.board)) {
            cout << "Solution found in " << current.g << " moves!" << endl;
            printBoard(current.board);
            return;
        }

        // Mark the current state as visited
        visited[current.board] = true;

        // Explore neighbors (possible moves)
        for (int i = 0; i < 4; ++i) {
            int new_x = current.x + dx[i];
            int new_y = current.y + dy[i];

            // Check if the new position is within bounds
            if (new_x >= 0 && new_x < N && new_y >= 0 && new_y < N) {
                vector<vector<int>> new_board = current.board;
                swap(new_board[current.x][current.y], new_board[new_x][new_y]);

                // If the new state has not been visited yet
                if (!visited[new_board]) {
                    int g_new = current.g + 1;
                    int h_new = manhattanDistance(new_board);
                    int f_new = g_new + h_new;

                    PuzzleState neighbor = { new_board, new_x, new_y, g_new, h_new, f_new };
                    pq.push(neighbor);
                }
            }
        }
    }

    cout << "No solution found!" << endl;
}

int main() {
    // Initial state of the 8-puzzle
    vector<vector<int>> start = {
        { 5, 0, 3 },
        { 4, 1, 6 },
        { 2, 7, 8 }
    };

    // Position of the empty tile (0)
    int x = 2, y = 0;

    // Perform A* search to solve the puzzle
    aStarSearch(start, x, y);

    return 0;
}
