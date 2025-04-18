#ifndef _GRID_
#define _GRID_

#include <iostream>
#include <utility>
#include <vector>

class Grid {
  public:
	Grid();
	~Grid();

	std::vector<std::vector<int>> generate(int mapH, int mapW);
	std::vector<std::pair<int, int>>
	blocked_cells(const std::vector<std::vector<int>> &grid, int mapH,
				  int mapW);
	std::vector<std::pair<int, int>>
	dead_end_cells(const std::vector<std::vector<int>> &grid, int h, int w);
	int count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
							 int j, int mapH, int mapW);
	std::vector<std::vector<int>> grid_init(int h, int w);

	void draw();
	int getValue(int row, int col);
	void setValue(int row, int col, int value);
	std::pair<float, float> gridToScreen(int row, int col);

  private:
	std::vector<std::vector<int>> map;
	int tileWidth = 64;
	int tileHeight = 32;
};

#endif
