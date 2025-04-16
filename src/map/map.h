#ifndef MAP_H_
#define MAP_H_

#include "SDL2/SDL.h"
#include "glm/glm.hpp"
#include "stdbool.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <vector>

#define MAP_WIDTH 10
#define MAP_HEIGHT 10

#define TILE_WIDTH 64.0f
#define TILE_HEIGHT 32.0f

std::vector<float> getVertexData(const std::vector<std::vector<int>> &mapVal);

void gridGraphicsSetup(unsigned int &VAO, unsigned int &VBO,
					   std::vector<float> vertices);
#endif // MAP_H_
