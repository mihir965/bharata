#ifndef AI_H_
#define AI_H_

#include <queue>
#include <utility>
#include <vector>

struct Node {
	int row;
	int col;
	float f;
};

bool isBlocked(int row, int col, const std::vector<std::vector<int>> &grid);

bool is_valid(int row, int col, int mapH, int mapW);

int calculateHeuristic(int row, int col, std::pair<int, int> dest);

std::vector<std::pair<int, int>>
plan_path(const std::vector<std::vector<int>> &grid,
		  std::pair<int, int> player_pos, std::pair<int, int> dest);

bool is_destination(int row, int col, std::pair<int, int> dest);

#endif
