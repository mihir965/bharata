// The cell class will be used for the AI in the game. The movement over the
// shortest distance using the A* search algorithm

#ifndef CELL_H_
#define CELL_H_

#include "../map/map.h"
#include <limits.h>
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
