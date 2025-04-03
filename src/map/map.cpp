#include "map.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdbool.h>
#include <vector>

map_terrain_types map[MAP_HEIGHT][MAP_TERRAIN_GRASS];

void map_reset() {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      map[y][x] = MAP_TERRAIN_GRASS;
    }
  }
}

std::vector<float> getVertexData() {
  std::vector<float> vertices = {
      // x, y, z
      -0.5f, 1.0f,  0.0f, 0.5f,  1.0f,  0.0f,
      0.5f,  -1.0f, 0.0f, -0.5f, -1.0f, 0.0f,
  };
  return vertices;
}

std::vector<int> getIndices() {
  std::vector<int> indices = {0, 1, 3, 0, 2, 3};
  return indices;
}
