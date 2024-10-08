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

	display_game_state(game_state);

	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 100000; i++)
		apply_die(game_state, { i % 27, i % 200, i% 200, i % 4 });
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
	

	display_game_state(game_state);

	std::cout << "Optimized apply_die time: " << duration.count() << " microseconds\nAverage time:" << duration.count() / 100000 << " microseconds" << std::endl;

	PostRequest(serverUrl + "/answer", teamToken, OutputJson(game_state));

	return 0;
}