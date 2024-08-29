#include <types.h>
#include <ops.h>
#include <procon_api.h>
using namespace std;

int main() {
    std::string serverUrl = "http://localhost:8080";
    std::string teamToken = "token1";

    std::string problemData = GetRequest(serverUrl + "/problem", teamToken);
    std::cout << "Problem Data:\n" << problemData << std::endl;
    GameState game_state = ParseJson(problemData);

    // Display the game state
    cout << "Initial Game State:" << endl;
    display_game_state(game_state);
    for (int i = 0; i < 9000; i++)
        apply_die(game_state, 25, 1, 1, i % 4);

    display_game_state(game_state);

    auto revision = PostRequest(serverUrl + "/answer", teamToken, OutputJson(game_state));
    cout << revision;

    return 0;
}