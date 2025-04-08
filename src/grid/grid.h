#ifndef GRID_H_
#define GRID_H_
#include <utility>
#include <vector>

std::vector<std::pair<int, int>>
blocked_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);
std::vector<std::pair<int, int>>
dead_end_cells(const std::vector<std::vector<int>> &grid, int mapH, int mapW);
std::pair<std::vector<std::vector<int>>, std::pair<int, int>>
grid_init(int mapH, int mapW);

int count_open_neighbors(const std::vector<std::vector<int>> &grid, int i,
                         int j);

std::pair<int, int> place_player(std::vector<std::vector<int>> &grid, int mapH,
                                 int mapW);

#endif
