#include "core/game.h"
#include "grid/grid.h"
#include <glad/glad.h>
#include <iostream>
#include <ostream>
#include <vector>

int main() {
	Game game;

	bool gameInitialized =
		game.init(40, 40, 1); // The arguements are the dimensions for the grid

	if (gameInitialized) {
		std::cout << "The game has been initialized" << std::endl;
		game.runLoop();
	}

	game.clean();

	return 0;
}
