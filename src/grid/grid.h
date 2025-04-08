#ifndef GRID_H_
#define GRID_H_
#include <utility>
#include <vector>

std::vector<std::pair<int, int>>
blocked_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);

std::vector<std::pair<int, int>>
dead_end_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);

std::vector<std::vector<int>> grid_init(int mapH, int mapW);

int count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
						 int j);

std::vector<std::vector<int>> place_element(std::vector<std::vector<int>> &grid,
											int pos_x, int pos_y, int value);

std::vector<std::vector<int>> move_player(std::vector<std::vector<int>> &grid,
										  std::pair<int, int> init_pos,
										  std::pair<int, int> new_pos);

#endif
