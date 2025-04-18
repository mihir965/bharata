#include "core/game.h"
#include "grid/grid.h"
#include <glad/glad.h>
#include <iostream>
#include <ostream>
#include <vector>

int main() {
	Game game;

	bool gameInitialized = game.init();

	if (gameInitialized) {
		std::cout << "The game has been initialized" << std::endl;

		Grid grid;

		vector<vector<int>> myGrid = grid.generate(40, 40);

		std::cout << myGrid.size() << std::endl;

		game.runLoop();
	}

	game.clean();

	return 0;
}
