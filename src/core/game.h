#ifndef _COREGAME_
#define _COREGAME_

#include "../grid/grid.h"
#include "texture/texture.h"
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <queue>
#include <string>
#include <unit/unit.h>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

class Game {
  public:
	Game();
	~Game();

	bool init(int mapH, int mapW,
			  int num); // Third value is the number of units to create
	void createGrid();
	void runLoop();
	void handleEvents();
	void update();
	void render();
	void clean();

	void loadTexture(const std::string &name, const char *path);
	Texture *getTexture(const std::string &name);

	void loadShader(const std::string &name, const char *path);
	std::string *getShader(const std::string &name);

	void getUnits(int i_x, int i_y, int f_x, int f_y);

  private:
	SDL_Window *window;
	SDL_GLContext context;
	bool isRunning;
	std::vector<std::unique_ptr<Unit>> selectedUnits;
	std::unordered_map<std::string, std::unique_ptr<Texture>> textureMap;
	std::unordered_map<std::string, std::unique_ptr<std::string>> ShaderMap;
	std::unique_ptr<Grid> grid;
	std::vector<std::unique_ptr<Unit>> units;
	std::vector<vector<unsigned int>> occupancyGrid;
	bool isSelecting;
	int init_x, init_y;

	int final_x, final_y;

	int moveToX, moveToY;
};

#endif
