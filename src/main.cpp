#include <types.h>
#include <json_ops.h>
#include <procon_api.h>
#include <iostream>
using namespace std;

int main() {
    std::string serverUrl = "http://localhost:8080";
    std::string teamToken = "token1";

    std::string problemData = GetRequest(serverUrl + "/problem", teamToken);
    std::cout << "Problem Data:\n" << problemData << std::endl;
    GameState game_state = ParseJson(problemData);

    // Display the game state
    std::cout << "Initial Game State:" << endl;
    game_state.display_game_state();

    // example die application
    Action action(6, 1, 25, 0);
    game_state.apply_die(action);

    game_state.display_game_state();

    // Post the answer and get the revision
    cout << OutputJson(game_state) << endl;
    auto revision = PostRequest(serverUrl + "/answer", teamToken, OutputJson(game_state));
    cout << revision;

    return 0;
}