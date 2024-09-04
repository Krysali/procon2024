#include <types.h>
#include <ops.h>
#include <procon_api.h>
using namespace std;

int main() {
    std::string serverUrl = "localhost:8080";
    std::string teamToken = "token1";
    std::string problemData;
    try {
        problemData = GetRequest(serverUrl + "/problem", teamToken);
    }
    catch (runtime_error) {
        std::cerr << "curl request failed, fallback to file json parsing\n";
        problemData = ReadJsonFile("../problem.json");
    }
    std::cout << "Problem Data:\n" << problemData << std::endl;
    GameState game_state = ParseJson(problemData);

    // Display the game state
    cout << "Initial Game State:" << endl;
    display_game_state(game_state);

    // example die application
    apply_die(game_state, 26, 1, 1, 2);

    display_game_state(game_state);

    // Post the answer and get the revision
    cout << OutputJson(game_state) << endl;
    std::string revision;
    try {
        revision = PostRequest(serverUrl + "/answer", teamToken, OutputJson(game_state));
    }
    catch (runtime_error) {
        std::cerr << "curl request failed\n";
        revision = "None";
    }
    cout << revision;

    return 0;
}