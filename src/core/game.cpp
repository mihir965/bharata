#include "game.h"
#include "grid/grid.h"
#include "texture/texture.h"
#include <memory>

Game::Game() : isRunning(false) {}

Game::~Game() {}

bool Game::init(int mapH, int mapW) {
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
	loadTexture("gridSprite", "./src/assets/grassland_tiles.png");
	grid->assignTexture(this->getTexture("gridSprite"));
	cout << "Returning game init" << endl;
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
