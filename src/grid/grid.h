#ifndef _GRID_
#define _GRID_

#include "texture/texture.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>
#include <sstream>
#include <utility>
#include <vector>

class Grid {
  public:
	Grid(int h, int w);
	~Grid();

	std::vector<std::vector<int>> generate();
	std::vector<std::pair<int, int>>
	blocked_cells(const std::vector<std::vector<int>> &grid);
	std::vector<std::pair<int, int>>
	dead_end_cells(const std::vector<std::vector<int>> &grid);
	int count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
							 int j);
	std::vector<std::vector<int>> grid_init();

	void draw();
	int getValue(int row, int col);
	void setValue(int row, int col, int value);
	std::pair<float, float> gridToScreen(int row, int col);

	void assignTexture(Texture *srcTex);

	void getVertexData(const std::vector<std::vector<int>> &map);

	void gridGraphicsSetup(unsigned int &VAO, unsigned int &VBO,
						   std::vector<float> vertexData);

	std::string readVertexShader();

	std::string readFragmentShader();

  private:
	std::vector<std::vector<int>> map;
	int tileWidth = 64;
	int tileHeight = 32;
	int mapWidth;
	int mapHeight;
	Texture *sprite;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
	std::vector<float> vertexData;
};

#endif
