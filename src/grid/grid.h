#ifndef GRID_H_
#define GRID_H_
#include <vector>

std::vector<std::pair<int, int>>
blocked_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);
std::vector<std::pair<int, int>>
dead_end_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);

std::vector<std::vector<int>> grid_init(int mapH, int mapW);

int count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
                         int j);

#endif
