#include "game.h"
#include "grid/grid.h"
#include "texture/texture.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <memory>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

Game::Game() : isRunning(false) {}

Game::~Game() {}

bool Game::init(int mapH, int mapW, int num) {
	cout << "Welcome to Bharata" << endl;

	// Set status of the game to true
	isRunning = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "SDL2 could not initialize video subsystem" << endl;
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	window = SDL_CreateWindow("Bharata", 0, 0, 1024, 768, SDL_WINDOW_OPENGL);

	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(window);

	if (!context) {
		cout << "OpenGL context not created" << endl;
		exit(1);
	}

	gladLoadGLLoader(SDL_GL_GetProcAddress);

	// The init function will also create the grid
	//
	// This is also assigning the sprite for the grid
	//
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	grid = std::make_unique<Grid>(mapH, mapW);
	grid->generate();
	cout << "Grid generation over" << endl;

	// Loading textures
	loadTexture("gridSprite", "./src/assets/grassland_tiles.png");
	grid->assignTexture(this->getTexture("gridSprite"));
	cout << "Returning game init" << endl;

	// Initialize the vertices of the grid and compile shaders for the grid
	grid->getVertexData();
	grid->gridGraphicsSetup();

	loadShader("gridVertex", "./src/shaders/vShader.vert");
	loadShader("gridFragment", "./src/shaders/fShader.frag");
	grid->compileShaders(getShader("gridVertex"), getShader("gridFragment"));

	// Create units on the grid
	// We need to update the occupancy grid with the way the actual grid is
	// initialized
	occupancyGrid =
		std::vector<vector<unsigned int>>(mapH, vector<unsigned int>(mapW, 0));

	for (int i = 0; i < mapH; i++) {
		for (int j = 0; j < mapW; j++) {
			occupancyGrid[i][j] = grid->getValue(i, j);
		}
	}

	// Loading unit textures
	loadTexture("knightSprite", "./src/assets/knight.png");
	loadShader("unitVertex", "./src/shaders/unitVShader.vert");
	loadShader("unitFragment", "./src/shaders/unitFShader.frag");
	Unit::compileShaders(getShader("unitVertex"), getShader("unitFragment"));
	Unit::initGraphics();

	vector<pair<int, int>> openCells;
	for (int i = 0; i < mapH; i++) {
		for (int j = 0; j < mapW; j++) {
			if (occupancyGrid[i][j] == 0) {
				openCells.push_back(make_pair(i, j));
			}
		}
	}
	vector<pair<int, int>> random_cells;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::sample(openCells.begin(), openCells.end(),
				std::back_inserter(random_cells), num, gen);

	for (const auto &cell : random_cells) {
		units.emplace_back(make_unique<Unit>(cell.first, cell.second,
											 getTexture("knightSprite")));
	}

	return true;
}

void Game::getUnits(int i_x, int i_y, int f_x, int f_y) {
	for (int i = min(i_x, f_x); i <= max(i_x, f_x); i++) {
		for (int j = min(i_y, f_y); j <= max(i_y, f_y); j++) {
			for (auto &unit : units) {
				if (unit->getCol() == i && unit->getRow() == j) {
					unit->setSelected(true);
				}
			}
		}
	}
}

void Game::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			isRunning = false;
			break;
		} else if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				isSelecting = true;
				init_x = event.button.x;
				init_y = event.button.y;
			}
		} else if (event.type == SDL_MOUSEBUTTONUP) {
			if (event.button.button == SDL_BUTTON_LEFT && isSelecting) {
				final_x = event.button.x;
				final_y = event.button.y;
				isSelecting = false;
				// cout << init_x << " " << init_y << endl;
				// cout << final_x << " " << final_y << endl;
				float init_dx = float(init_x) - 1024 / 2.0f;
				float init_dy = float(init_y) + 786 / 7.0f;

				float final_dx = float(final_x) - 1024 / 2.0f;
				float final_dy = float(init_y) + 786 / 7.0f;

				float init_colF = (init_dx / 32.0f + init_dy / 16.0f) * 0.5f;
				float init_rowF = (init_dy / 16.0f - init_dx / 32.0f) * 0.5f;

				float final_colF = (final_dx / 32.0f + final_dy / 16.0f) * 0.5f;
				float final_rowF = (final_dy / 16.0f - final_dx / 32.0f) * 0.5f;

				int grid_init_col = int(floor(init_colF));
				int grid_init_row = int(floor(init_rowF));

				int grid_final_col = int(floor(final_colF));
				int grid_final_row = int(floor(final_rowF));

				cout << "This is the two coords" << endl;
				cout << grid_init_row << " " << grid_init_col << endl;
				cout << grid_final_row << " " << grid_final_col << endl;

				cout << "Lets look at the cells that are getting selected"
					 << endl;

				for (int i = min(grid_init_col, grid_final_col);
					 i <= max(grid_init_col, grid_final_col); i++) {
					for (int j = min(grid_init_row, grid_final_row);
						 j <= max(grid_init_row, grid_final_row); j++) {
						cout << "The cell is : " << j << " " << i << endl;
					}
				}
				getUnits(grid_init_col, grid_init_row, grid_final_col,
						 grid_final_row);
			}
		}
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	grid->draw();
	for (auto &unit : this->units) {
		unit->drawSprite();
	}
	SDL_GL_SwapWindow(window);
}

void Game::runLoop() {
	while (isRunning) {
		handleEvents();
		render();
	}
}

void Game::loadTexture(const std::string &name, const char *path) {
	if (textureMap.find(name) == textureMap.end()) {
		cout << "The texture was not found" << endl;
		textureMap[name] = std::make_unique<Texture>(path);
		cout << "Texture loaded" << endl;
	}
}

Texture *Game::getTexture(const std::string &name) {
	if (textureMap.find(name) == textureMap.end()) {
		return nullptr;
	}
	return textureMap.find(name)->second.get();
}

void Game::loadShader(const std::string &name, const char *path) {
	if (ShaderMap.find(name) == ShaderMap.end()) {
		std::ifstream inputVertex;
		inputVertex.open(path);
		if (!inputVertex.is_open()) {
			std::cerr << "There was an error" << std::endl;
		}
		std::stringstream shaderStream;
		shaderStream << inputVertex.rdbuf();
		inputVertex.close();
		ShaderMap[name] = std::make_unique<std::string>(shaderStream.str());
	}
}

std::string *Game::getShader(const std::string &name) {
	if (ShaderMap.find(name) == ShaderMap.end()) {
		return nullptr;
	}
	return ShaderMap.find(name)->second.get();
}
