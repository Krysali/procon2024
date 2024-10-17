#include <stdafx.h>
#include <types.h>
#include <ops.h>
#include <procon_api.h>
#include <gui.h>

void Main() {
	std::string serverUrl = "localhost:8080";

	//new_mongold97d641d60f622182ccfa9044d258e1d1d931afde9ca0299e21360
	std::string teamToken = "token1";
	std::string problemData;

	try {
		// Get the problem data from the server
		problemData = GetRequest(serverUrl + "/problem", teamToken);
		Console << U"Successfully read problem data from " << Unicode::Widen(serverUrl);
	}
	catch (std::runtime_error) {
		// If the request fails, fallback to reading the problem data from a file
		Console << U"curl request failed, fallback to file json parsing\n";
		problemData = ReadJsonFile("../problem.json");
	}

	// Parse the problem data into a GameState object
	GameState game_state = ParseJson(problemData);
	
	GUI game(game_state);
	game.serverUrl = serverUrl;
	game.token = teamToken;
	
	while (System::Update()) {
		game.Render();
	}
}
