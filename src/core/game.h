#ifndef _COREGAME_
#define _COREGAME_

#include "../grid/grid.h"
#include <SDL2/SDL.h>
#include <iostream>
#include <memory>
#include <unit/unit.h>

using namespace std;

class Game {
  public:
	Game();
	~Game();

	bool init();
	void createGrid();
	void runLoop();
	void handleEvents();
	void update();
	void render();
	void clean();

  private:
	SDL_Window *window;
	SDL_GLContext context;
	bool isRunning;

	std::unique_ptr<Grid> grid;
	std::vector<Unit> unit;
};

#endif
