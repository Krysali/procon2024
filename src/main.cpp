#include <types.h>
#include <ops.h>
using namespace std;

int main() {
    cout << "compiled using CMake on 2024-08-12" << endl;
    // Read the JSON file
    GameState game_state = ParseJson(ReadJsonFile("problem.json"));

    // Display the game state
    cout << "Initial Game State:" << endl;
    display_game_state(game_state);

    apply_die(game_state, 25, 1, 1, 2);

    display_game_state(game_state);

    return 0;
}