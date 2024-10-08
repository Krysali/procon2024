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

	game_state.display_current_board();

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
		game_state.apply_die({ i % 27, i % 200, i % 200, i % 4 });
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	
	game_state.display_current_board();

	std::cout << "Optimized apply_die time: " << duration.count() << " microseconds\nAverage time:" << duration.count() / 100000 << " microseconds" << std::endl;

	PostRequest(serverUrl + "/answer", teamToken, OutputJson(game_state));

	return 0;
}