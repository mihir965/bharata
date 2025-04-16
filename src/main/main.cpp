#include "../ai/ai.h"
#include "../grid/grid.h"
#include "../map/map.h"
#include "../texture/texture.h"
#include "SDL2/SDL_image.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <new>
#include <ostream>
#include <queue>
#include <sstream>
#include <string.h>
#include <string_view>
#include <utility>
#include <vector>

// GLOBALS
int gScreenHeight = 480;
int gScreenWidth = 640;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

std::queue<std::pair<int, int>> movementQueue;

bool gQuit = false;
unsigned int gridVAO;
unsigned int gridVBO;
// unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;
unsigned int texture01;

void GetOpenGLVersionInfo() {
	std::cout << "Vendor" << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer" << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version" << glGetString(GL_VERSION) << std::endl;
}

void InitializeProgram() {

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL2 could not initialize video subsystem" << std::endl;
		exit(1);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	gGraphicsApplicationWindow = SDL_CreateWindow(
		"Bharata", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

	if (gGraphicsApplicationWindow == nullptr) {
		std::cout << "SDL_Window was not able to be created" << std::endl;
		exit(1);
	}

	gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

	if (gOpenGLContext == nullptr) {
		std::cout << "OpenGL context not available" << std::endl;
		exit(1);
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
		std::cout << "glad was not initialized" << std::endl;
		exit(1);
	}

	GetOpenGLVersionInfo();
}

std::string readVertexShader() {
	std::ifstream inputVertex;
	inputVertex.open("./src/shaders/vertex.vert");
	if (!inputVertex.is_open()) {
		std::cerr << "Therre was an error" << std::endl;
	}
	std::stringstream shaderStream;
	shaderStream << inputVertex.rdbuf();
	inputVertex.close();
	return shaderStream.str();
}

std::string readFragmentShader() {
	std::ifstream inputFragment;
	inputFragment.open("./src/shaders/fragment.frag");
	if (!inputFragment.is_open()) {
		std::cerr << "Therre was an error" << std::endl;
	}
	std::stringstream shaderStream;
	shaderStream << inputFragment.rdbuf();
	inputFragment.close();
	return shaderStream.str();
}

void VertexSpecification(const std::vector<std::vector<int>> &grid) {

	// Loading the spritesheet
	generateAndBindTexture(texture01);

	useSourceImageForTexture("./src/assets/grassland_tiles.png");

	std::vector<float> vertices = getVertexData(grid);

	// Graphics Code
	// setting up the grid buffers
	gridGraphicsSetup(gridVAO, gridVBO, vertices);

	std::string vertexShaderCode = readVertexShader();

	const char *vertexShaderSource = vertexShaderCode.c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	std::string fragmentShaderCode = readFragmentShader();
	const char *fragShaderSource = fragmentShaderCode.c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(shaderProgram);

	glm::mat4 projection = glm::ortho(0.0f, float(gScreenWidth),
									  float(gScreenHeight), 0.0f, -1.0f, 1.0f);

	GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void CreateGraphicsPipeline() {}

void Input(const std::vector<std::vector<int>> &grid,
		   std::pair<int, int> &player_pos) {
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0) {
		switch (e.type) {
		case SDL_QUIT:
			std::cout << "Goodbye" << std::endl;
			gQuit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			std::cout << "The player_pos is " << player_pos.first << " "
					  << player_pos.second << std::endl;
			int x, y;
			SDL_GetMouseState(&x, &y);
			float gridWidth = (MAP_WIDTH + MAP_HEIGHT) * (TILE_WIDTH / 2.0f);
			float gridHeight = (MAP_WIDTH + MAP_HEIGHT) * (TILE_HEIGHT / 2.0f);
			float offsetX = (float(gScreenWidth) - gridWidth) / 2.0f +
							(MAP_WIDTH * TILE_WIDTH / 2.0f);
			float offsetY = (float(gScreenHeight) - gridHeight) / 2.0f +
							(MAP_HEIGHT * TILE_HEIGHT / 8.0f);
			float xRel = x - offsetX;
			float yRel = y - offsetY;

			float colF = (xRel / TILE_WIDTH) + (yRel / TILE_HEIGHT);
			float rowF = (yRel / TILE_HEIGHT) - (xRel / TILE_WIDTH);

			int col = static_cast<int>(colF);
			int row = static_cast<int>(rowF);
			std::cout << "The coordinate selected is " << row << " " << col
					  << std::endl;

			std::cout << "The grid cell is\n" << grid[row][col] << std::endl;

			std::vector<std::pair<int, int>> pathToFollow =
				plan_path(grid, player_pos, std::make_pair(row, col));

			for (auto &road : pathToFollow) {
				std::cout << "Pushing " << road.first << " " << road.second
						  << std::endl;
				movementQueue.push(road);
			}
			break;
		}
	}
}

void PreDraw(std::vector<std::vector<int>> &grid,
			 std::pair<int, int> &player_pos) {
	if (!movementQueue.empty()) {
		grid = move_player(grid, player_pos, movementQueue.front());
		player_pos = movementQueue.front();
		movementQueue.pop();
	}
	VertexSpecification(grid);
}

void Draw() {
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture01);
	glBindVertexArray(gridVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * MAP_HEIGHT * MAP_WIDTH);
	glBindVertexArray(0);
}

void MainLoop(std::vector<std::vector<int>> &grid,
			  std::pair<int, int> &player_pos) {
	while (!gQuit) {
		Input(grid, player_pos);

		PreDraw(grid, player_pos);

		Draw();

		SDL_GL_SwapWindow(gGraphicsApplicationWindow);
		SDL_Delay(100);
	}
}

void CleanUp() {
	SDL_DestroyWindow(gGraphicsApplicationWindow);
	SDL_Quit();
}

int main() {
	std::srand(static_cast<unsigned>(std::time(
		nullptr))); // This is for making sure that the maps are different
	std::cout << "current working directory: "
			  << std::filesystem::current_path().string() << std::endl;

	std::vector<std::vector<int>> grid = grid_init(MAP_HEIGHT, MAP_WIDTH);

	int player_init_x = std::rand() % MAP_HEIGHT - 1;
	int player_init_y = std::rand() % MAP_WIDTH - 1;
	std::pair<int, int> player_pos =
		std::make_pair(player_init_x, player_init_y);

	// Place player on grid
	grid = place_element(grid, player_init_x, player_init_y, 2);

	InitializeProgram();

	CreateGraphicsPipeline();

	MainLoop(grid, player_pos);

	CleanUp();

	return 0;
}
