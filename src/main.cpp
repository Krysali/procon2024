#include <types.h>
#include <ops.h>
#include <procon_api.h>
#include <chrono>

int main() {
    std::string serverUrl = "localhost:8080";
    std::string teamToken = "token1";
    std::string problemData;
    problemData = GetRequest(serverUrl + "/problem", teamToken);
    // std::cout << "Problem Data:\n" << problemData << std::endl;
    GameState game_state = ParseJson(problemData);
    game_state.num_actions = 0;

    Action action = { 22, 1, 3, 2 };
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 5000; ++i) {
        apply_die(game_state, action);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    display_game_state(game_state);
    
    std::cout << "Optimized apply_die time: " << duration.count() << " microseconds\nAverage time:" << duration.count() / 5000 << " microseconds" << std::endl;
	std::string outputData = OutputJson(game_state);
    PostRequest(serverUrl + "/answer", teamToken, outputData);

    return 0;
}