#include "ai.h"
#include "../cell/cell.h"
#include "../map/map.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

struct CompareNode {
	bool operator()(const Node &a, const Node &b) const {
		return a.f > b.f;
	}
};

bool isBlocked(int row, int col, const std::vector<std::vector<int>> &grid) {
	return grid[row][col] == 1;
}

bool is_valid(int row, int col, int mapH, int mapW) {
	return ((row >= 0) && (row < mapH) && (col >= 0) && (col < mapW));
}

int calculateHeuristic(int row, int col, std::pair<int, int> dest) {
	return (glm::abs(row - dest.first) + glm::abs(col - dest.second));
}

bool is_destination(int row, int col, std::pair<int, int> dest) {
	return (row == dest.first && col == dest.second);
}

std::vector<std::pair<int, int>>
track_path(std::vector<std::vector<Cell>> cell_details, std::pair<int, int> src,
		   std::pair<int, int> dest) {
	std::vector<std::pair<int, int>> path;
	int i = dest.first;
	int j = dest.second;
	while (!(i == src.first && j == src.second)) {
		path.push_back(std::make_pair(i, j));
		int temp_i = cell_details[i][j].parent_i;
		int temp_j = cell_details[i][j].parent_j;
		if (!is_valid(temp_i, temp_j, MAP_HEIGHT, MAP_WIDTH)) {
			std::cout << "The parent cell is invalid and cannot be used"
					  << std::endl;
			break;
		}
		i = temp_i;
		j = temp_j;
	}
	path.push_back(std::make_pair(src.first, src.second));
	std::reverse(path.begin(), path.end());
	return path;
}

std::vector<std::pair<int, int>>
plan_path(const std::vector<std::vector<int>> &grid,
		  std::pair<int, int> player_pos, std::pair<int, int> dest) {
	std::vector<std::vector<bool>> closed_list(
		MAP_HEIGHT, std::vector<bool>(MAP_WIDTH, false));

	std::vector<std::vector<Cell>> cell_details(
		MAP_HEIGHT, std::vector<Cell>(MAP_WIDTH, Cell()));

	std::priority_queue<Node, std::vector<Node>, CompareNode> openList;
	int i = player_pos.first;
	int j = player_pos.second;

	cell_details[i][j].f = 0.0f;
	cell_details[i][j].g = 0.0f;
	cell_details[i][j].h = 0.0f;
	cell_details[i][j].parent_i = i;
	cell_details[i][j].parent_j = j;

	openList.push(Node{i, j, 0.0f});
	bool destFound = false;
	std::vector<std::pair<int, int>> tracked_path;
	if (isBlocked(dest.first, dest.second, grid)) {
		std::cout << "The selected destination is water" << std::endl;
		return tracked_path;
	}
	while (openList.size() > 0) {
		Node p = openList.top();
		openList.pop();
		int i = p.row;
		int j = p.col;
		closed_list[i][j] = 0;
		std::vector<std::pair<int, int>> movement = {
			{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

		for (std::pair<int, int> move : movement) {
			int new_i = i + move.first;
			int new_j = j + move.second;
			if (is_valid(new_i, new_j, MAP_HEIGHT, MAP_WIDTH)) {
				if (is_destination(new_i, new_j, dest)) {
					cell_details[new_i][new_j].parent_i = i;
					cell_details[new_i][new_j].parent_j = j;
					destFound = true;
					tracked_path = track_path(
						cell_details, std::make_pair(new_i, new_j), dest);
					break;
				}
				if (closed_list[new_i][new_j] == false &&
					!isBlocked(new_i, new_j, grid)) {
					float g_new = cell_details[new_i][new_j].g + 1.0;
					float h_new = calculateHeuristic(new_i, new_j, dest);
					float f_new = g_new + h_new;
					if (cell_details[new_i][new_j].f ==
							std::numeric_limits<float>::infinity() ||
						cell_details[new_i][new_j].f > f_new) {
						cell_details[new_i][new_j].f = f_new;
						cell_details[new_i][new_j].g = g_new;
						cell_details[new_i][new_j].h = h_new;
						cell_details[new_i][new_j].parent_i = i;
						cell_details[new_i][new_j].parent_j = j;
					}
				}
			}
		}
		if (destFound)
			break;
	}
	if (!destFound)
		std::cout << "Destination not found" << std::endl;
	else {
		std::cout << "Destination found!!" << std::endl;
	}
	return tracked_path;
}
