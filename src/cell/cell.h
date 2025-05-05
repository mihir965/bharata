#ifndef CELL_H_
#define CELL_H_

#include <limits.h>
#include <limits>
#include <utility>
#include <vector>

class Cell {
  public:
	int parent_i = 0;
	int parent_j = 0;
	float f = std::numeric_limits<float>::infinity();
	float g = std::numeric_limits<float>::infinity();
	float h = 0.0f;
	Cell() = default;
};

#endif
