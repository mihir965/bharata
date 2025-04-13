#ifndef MAP_H_
#define MAP_H_

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "stdbool.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

#define TILE_WIDTH 64.0f
#define TILE_HEIGHT 32.0f

std::vector<float> getVertexData(const std::vector<std::vector<int>> &mapVal);
#endif // MAP_H_
