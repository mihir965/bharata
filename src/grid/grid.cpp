#include "grid.h"
#include <utility>
#include <vector>

Grid::Grid() {
	std::cout << "A grid got initialized" << std::endl;
}

Grid::~Grid() {}

int Grid::count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
							   int j, int mapH, int mapW) {
	std::vector<std::pair<int, int>> movement = {
		{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
	int open_neighbors = 0;
	for (std::pair<int, int> move : movement) {
		int test_i = i + move.first;
		int test_j = j + move.second;
		if (test_i >= 0 && test_i < mapH && test_j >= 0 && test_j < mapW &&
			grid[test_i][test_j] == 0) {
			open_neighbors += 1;
		}
	}
	return open_neighbors;
}

std::vector<std::pair<int, int>>
Grid::blocked_cells(const std::vector<std::vector<int>> &grid, int mapH,
					int mapW) {
	std::vector<std::pair<int, int>> blocked_cells;
	for (int i = 1; i < mapH; i++) {
		for (int j = 1; j < mapW; j++) {
			if (grid[i][j] == 1) {
				int open_neighbors =
					count_open_neighbors(grid, i, j, mapH, mapW);
				if (open_neighbors == 1) {
					blocked_cells.push_back(std::make_pair(i, j));
				}
			}
		}
	}
	return blocked_cells;
}

std::vector<std::pair<int, int>>
Grid::dead_end_cells(const std::vector<std::vector<int>> &grid, int mapH,
					 int mapW) {
	std::vector<std::pair<int, int>> dead_ends;
	for (int i = 1; i < mapH; i++) {
		for (int j = 1; j < mapW; j++) {
			if (grid[i][j] == 0 &&
				count_open_neighbors(grid, i, j, mapH, mapW) == 1) {
				dead_ends.push_back(std::make_pair(i, j));
			}
		}
	}
	return dead_ends;
}

std::vector<std::vector<int>> Grid::grid_init(int h, int w) {
	// std::cout << "This is happening" << mapH << " " << mapW << std::endl;
	std::vector<std::vector<int>> grid(h, std::vector<int>(w, 1));
	int init_x = std::rand() % h;
	int init_y = std::rand() % w;
	// std::cout << init_x << " " << init_y << std::endl;
	grid[init_x][init_y] = 0;
	while (true) {
		std::vector<std::pair<int, int>> b_cells = blocked_cells(grid, h, w);
		//   std::cout << b_cells.size() << std::endl;
		if (b_cells.empty())
			break;
		int rand = std::rand() % b_cells.size();
		int a = b_cells[rand].first;
		int b = b_cells[rand].second;
		grid[a][b] = 0;
	}
	std::vector<std::pair<int, int>> d_cells = dead_end_cells(grid, h, w);
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
			if (ni >= 0 && ni < h && nj >= 0 && nj < w) {
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

std::vector<std::vector<int>> Grid::generate(int mapH, int mapW) {
	map = grid_init(mapH, mapW);
	return map;
}
