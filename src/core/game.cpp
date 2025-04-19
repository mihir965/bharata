#include "game.h"
#include "grid/grid.h"
#include "texture/texture.h"
#include <fstream>
#include <memory>
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
	occupancyGrid =
		std::vector<vector<unsigned int>>(mapH, vector<unsigned int>(mapW, 0));

	for (int i = 0; i < mapH; i++) {
		for (int j = 0; j < mapW; j++) {
			occupancyGrid[i][j] = grid->getValue(i, j);
		}
	}
	return true;
}

void Game::handleEvents() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	grid->draw();
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
		cout << "The shader has not been loaded" << std::endl;
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
