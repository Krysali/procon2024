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

    apply_die(game_state, 25, 1, 1, 2);

    display_game_state(game_state);

    std::string jsonData = "{\"n\":3,\"ops\":[{\"p\":4,\"x\":1,\"y\":1,\"s\":2},{\"p\":11,\"x\":3,\"y\":-12,\"s\":3},{\"p\":25,\"x\":3,\"y\":3,\"s\":0}]}";

    // POST request to submit an answer
    std::string answerResponse = PostRequest(serverUrl + "/answer", teamToken, jsonData);
    std::cout << "Answer Response:\n" << answerResponse << std::endl;

    return 0;
}