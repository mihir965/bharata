#include "game.h"
#include "grid/grid.h"
#include <memory>

Game::Game() : isRunning(false) {}

Game::~Game() {}

bool Game::init() {
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

void Game::runLoop() {
	while (isRunning) {
		handleEvents();
	}
}
