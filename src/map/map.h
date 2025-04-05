#ifndef MAP_H_
#define MAP_H_

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "stdbool.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define MAP_WIDTH 10
#define MAP_HEIGHT 2

enum map_terrain_types {
  MAP_TERRAIN_GRASS,
  MAP_TERRAIN_WATER,
  MAP_TERRAIN_GROUND
};

std::vector<float> getVertexData();
std::vector<int> getIndices();
void map_reset();
#endif // MAP_H_
