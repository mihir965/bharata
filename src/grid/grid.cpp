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

void Grid::generate() {
	this->map = grid_init();
}

void Grid::assignTexture(Texture *srcTex) {
	this->sprite = srcTex;
}

void Grid::getVertexData() {
	const float tex_tile_width = this->tileWidth / this->sprite->width;
	const float tex_tile_height = this->tileHeight / this->sprite->height;

	for (int row = 0; row < this->mapHeight; row++) {
		for (int col = 0; col < this->mapWidth; col++) {
			float x = ((col - row) * tileWidth / 2.0f) +
					  1024 / 2.0f; // Treating half of the screen as the offset
								   // for width and 1/4th for height
			float y = ((col + row) * tileHeight / 2.0f) - 768 / 7.0f;

			float xTop = x;
			float yTop = y;

			float xRight = xTop + this->tileWidth / 2.0f;
			float yRight = yTop + this->tileHeight / 2.0f;

			float xBottom = xTop;
			float yBottom = yTop + this->tileHeight;

			float xLeft = xTop - this->tileWidth / 2.0f;
			float yLeft = yTop + this->tileHeight / 2.0f;

			float u_min, u_max;
			float v_min, v_max;

			switch (this->map[row][col]) {
			case 0:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 0.0f;
				v_max = tex_tile_height;
				break;
			case 1:
				u_min = tex_tile_width;
				u_max = 2 * tex_tile_width;
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

			this->vertexData.push_back(xTop);
			this->vertexData.push_back(yTop);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_top);
			this->vertexData.push_back(v_top);

			this->vertexData.push_back(xRight);
			this->vertexData.push_back(yRight);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_right);
			this->vertexData.push_back(v_right);

			this->vertexData.push_back(xBottom);
			this->vertexData.push_back(yBottom);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_bottom);
			this->vertexData.push_back(v_bottom);

			this->vertexData.push_back(xTop);
			this->vertexData.push_back(yTop);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_top);
			this->vertexData.push_back(v_top);

			this->vertexData.push_back(xBottom);
			this->vertexData.push_back(yBottom);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_bottom);
			this->vertexData.push_back(v_bottom);

			this->vertexData.push_back(xLeft);
			this->vertexData.push_back(yLeft);
			this->vertexData.push_back(0.0f);
			this->vertexData.push_back(u_left);
			this->vertexData.push_back(v_left);
		}
	}
}

void Grid::gridGraphicsSetup() {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertexData.size() * sizeof(float),
				 this->vertexData.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Grid::compileShaders(std::string *vSrc, std::string *fSrc) {
	const char *vertexShaderSource = vSrc->c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char *fragShaderSource = fSrc->c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(this->vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader compilation failed:\n"
				  << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Grid::draw() {
	// Bind the texture
	// Bind the VAO
	// Use the shaders
	// Draw the elements

	glUseProgram(shaderProgram);
	glm::mat4 projection =
		glm::ortho(0.0f, float(1024), float(786), 0.0f, -1.0f, 1.0f);

	GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindTexture(GL_TEXTURE_2D, this->sprite->getID());
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * this->mapHeight * this->mapWidth);
	glBindVertexArray(0);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		//	std::cerr << "OpenGL Error: " << err << std::endl;
	}
}

int Grid::getValue(int row, int col) {
	return this->map[row][col];
}
