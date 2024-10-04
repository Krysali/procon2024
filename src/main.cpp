#include <types.h>
#include <ops.h>
#include <procon_api.h>
using namespace std;

int main() {

    std::string serverUrl = "localhost:8080";
    std::string teamToken = "token1";
    std::string problemData;
    problemData = ReadJsonFile("../problem.json");
    std::cout << "Problem Data:\n" << problemData << std::endl;
    GameState game_state = ParseJson(problemData);

    // Display the game state
    std::cout << "Initial Game State:" << endl;
    display_game_state(game_state);

    // example die application
    apply_die(game_state, 26, 1, 1, 2);

    display_game_state(game_state);

    // Post the answer and get the revision
    std::cout << OutputJson(game_state) << endl;

    return 0;
}