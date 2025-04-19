#include "grid.h"
#include "texture/texture.h"
#include <iostream>
#include <string>

Grid::Grid(int h, int w) {
	std::cout << "A grid got initialized" << std::endl;
	this->mapHeight = h;
	this->mapWidth = w;
}

Grid::~Grid() {}

int Grid::count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
							   int j) {
	std::vector<std::pair<int, int>> movement = {
		{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	int open_neighbors = 0;
	for (std::pair<int, int> move : movement) {
		int test_i = i + move.first;
		int test_j = j + move.second;
		if (test_i >= 0 && test_i < this->mapHeight && test_j >= 0 &&
			test_j < this->mapWidth && grid[test_i][test_j] == 0) {
			open_neighbors += 1;
		}
	}
	return open_neighbors;
}

std::vector<std::pair<int, int>>
Grid::blocked_cells(const std::vector<std::vector<int>> &grid) {
	std::vector<std::pair<int, int>> blocked_cells;
	for (int i = 1; i < this->mapHeight; i++) {
		for (int j = 1; j < this->mapWidth; j++) {
			if (grid[i][j] == 1) {
				int open_neighbors = count_open_neighbors(grid, i, j);
				if (open_neighbors == 1) {
					blocked_cells.push_back(std::make_pair(i, j));
				}
			}
		}
	}
	return blocked_cells;
}

std::vector<std::pair<int, int>>
Grid::dead_end_cells(const std::vector<std::vector<int>> &grid) {
	std::vector<std::pair<int, int>> dead_ends;
	for (int i = 1; i < this->mapHeight; i++) {
		for (int j = 1; j < this->mapWidth; j++) {
			if (grid[i][j] == 0 && count_open_neighbors(grid, i, j) == 1) {
				dead_ends.push_back(std::make_pair(i, j));
			}
		}
	}
	return dead_ends;
}

std::vector<std::vector<int>> Grid::grid_init() {
	// std::cout << "This is happening" << mapH << " " << mapW << std::endl;
	std::vector<std::vector<int>> grid(this->mapHeight,
									   std::vector<int>(this->mapWidth, 1));
	int init_x = std::rand() % this->mapHeight;
	int init_y = std::rand() % this->mapWidth;
	// std::cout << init_x << " " << init_y << std::endl;
	grid[init_x][init_y] = 0;
	while (true) {
		std::vector<std::pair<int, int>> b_cells = blocked_cells(grid);
		//   std::cout << b_cells.size() << std::endl;
		if (b_cells.empty())
			break;
		int rand = std::rand() % b_cells.size();
		int a = b_cells[rand].first;
		int b = b_cells[rand].second;
		grid[a][b] = 0;
	}
	std::vector<std::pair<int, int>> d_cells = dead_end_cells(grid);
	int toOpen = d_cells.size() / 2;
	//  std::cout << toOpen << std::endl;
	while (toOpen != 0) {
		if (d_cells.empty())
			break;
		int rand2 = std::rand() % d_cells.size();
		int a = d_cells[rand2].first;
		int b = d_cells[rand2].second;
		d_cells.erase(d_cells.begin() + rand2);
		std::vector<std::pair<int, int>> movement = {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
		std::vector<std::pair<int, int>> closed_neighbors;
		for (const auto &[mi, mj] : movement) {
			int ni = a + mi;
			int nj = b + mj;
			if (ni >= 0 && ni < this->mapHeight && nj >= 0 &&
				nj < this->mapWidth) {
				if (grid[ni][nj] == 1)
					closed_neighbors.push_back(std::make_pair(ni, nj));
			}
		}
		if (!closed_neighbors.empty()) {
			int rand3 = std::rand() % closed_neighbors.size();
			int toOpenx = closed_neighbors[rand3].first;
			int toOpeny = closed_neighbors[rand3].second;
			grid[toOpenx][toOpeny] = 0;
		}
		toOpen--;
	}
	return grid;
}

std::vector<std::vector<int>> Grid::generate() {
	map = grid_init();
	return map;
}

void Grid::assignTexture(Texture *srcTex) {
	this->sprite = srcTex;
}

void Grid::getVertexData(const std::vector<std::vector<int>> &map) {
	float gridWidth =
		(this->mapWidth + this->mapHeight) * (this->tileWidth / 2.0f);
	float gridHeight =
		(this->mapWidth + this->mapHeight) * (this->tileHeight / 2.0f);

	float offsetX =
		(1024 - gridWidth) / 2.0f + (this->mapWidth * this->tileWidth / 2.0f);
	float offsetY =
		(786 - gridHeight) / 2.0f + (this->mapHeight * this->tileHeight / 2.0f);

	const float tex_tile_width = this->tileWidth / this->sprite->width;
	const float tex_tile_height = this->tileHeight / this->sprite->height;

	for (int row = 0; row < this->mapHeight; row++) {
		for (int col = 0; col < this->mapWidth; col++) {
			float xCenter = ((col - row) * this->tileWidth / 2.0f) + offsetX;
			float yCenter = ((col + row) * this->tileHeight / 2.0f) + offsetY;

			float xTop = xCenter;
			float yTop = yCenter - this->tileHeight / 2.0f;

			float xRight = xCenter + this->tileWidth / 2.0f;
			float yRight = yCenter;

			float xBottom = xCenter;
			float yBottom = yCenter + this->tileHeight / 2.0f;

			float xLeft = xCenter - this->tileWidth / 2.0f;
			float yLeft = yCenter;

			float u_min, u_max;
			float v_min, v_max;

			switch (map[row][col]) {
			case 0:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 0.0f;
				v_max = tex_tile_height;
				break;
			case 1:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 19 * tex_tile_height;
				v_max = 20 * tex_tile_height;
				break;
			case 2:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 0.0f;
				v_max = tex_tile_height;
				break;
			default:
				break;
			}
			float u_mid = (u_min + u_max) / 2.0f;
			float v_mid = (v_min + v_max) / 2.0f;

			float u_top = u_mid;
			float v_top = v_min;

			float u_right = u_max;
			float v_right = v_mid;

			float u_bottom = u_mid;
			float v_bottom = v_max;

			float u_left = u_min;
			float v_left = v_mid;

			vertexData.push_back(xTop);
			vertexData.push_back(yTop);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_top);
			vertexData.push_back(v_top);

			vertexData.push_back(xRight);
			vertexData.push_back(yRight);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_right);
			vertexData.push_back(v_right);

			vertexData.push_back(xBottom);
			vertexData.push_back(yBottom);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_bottom);
			vertexData.push_back(v_bottom);

			vertexData.push_back(xTop);
			vertexData.push_back(yTop);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_top);
			vertexData.push_back(v_top);

			vertexData.push_back(xBottom);
			vertexData.push_back(yBottom);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_bottom);
			vertexData.push_back(v_bottom);

			vertexData.push_back(xLeft);
			vertexData.push_back(yLeft);
			vertexData.push_back(0.0f);
			vertexData.push_back(u_left);
			vertexData.push_back(v_left);
		}
	}
}

void Grid::gridGraphicsSetup(unsigned int &VAO, unsigned int &VBO,
							 std::vector<float> vertexData) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float),
				 vertexData.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

std::string Grid::readVertexShader() {
	std::ifstream inputVertex;
	inputVertex.open("./src/shaders/vShader.vert");
	if (!inputVertex.is_open()) {
		std::cerr << "Therre was an error" << std::endl;
	}
	std::stringstream shaderStream;
	shaderStream << inputVertex.rdbuf();
	inputVertex.close();
	return shaderStream.str();
}

std::string Grid::readFragmentShader() {
	std::ifstream inputFragment;
	inputFragment.open("./src/shaders/fShader.frag");
	if (!inputFragment.is_open()) {
		std::cerr << "Therre was an error" << std::endl;
	}
	std::stringstream shaderStream;
	shaderStream << inputFragment.rdbuf();
	inputFragment.close();
	return shaderStream.str();
}

void Grid::draw() {
	// Bind the texture
	// Bind the VAO
	// Use the shaders
	// Draw the elements
	//
	//
	// THIS FUNCITON NEEDS TO BE MODIFIED WE CANT CALL vertexData all the time
	// it keeps adding to vertexData variable think about draw function more
	glBindTexture(GL_TEXTURE_2D, this->sprite->getID());
	getVertexData(this->map);
	std::cout << this->vertexData.size() << std::endl;
	gridGraphicsSetup(this->VAO, this->VBO, this->vertexData);

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

	glm::mat4 projection =
		glm::ortho(0.0f, float(1024), float(768), 0.0f, -1.0f, 1.0f);

	GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES, 0, 6 * this->mapHeight * this->mapWidth);
	glBindVertexArray(0);
}
