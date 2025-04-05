#include "map.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdbool.h>
#include <vector>

map_terrain_types map[MAP_HEIGHT][MAP_WIDTH];

std::vector<std::vector<int>> mapVal = {{1, 0, 2, 1, 0, 0, 0, 1, 1, 1},
                                        {1, 0, 0, 0, 1, 0, 0, 0, 1, 1}};

void map_reset() {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      map[y][x] = MAP_TERRAIN_GRASS;
    }
  }
}

std::vector<float> getVertexData() {

  int tileSize = 32;
  int gridWidth = MAP_WIDTH * tileSize;
  int gridHeight = MAP_HEIGHT * tileSize;

  float offsetX = (640 - gridWidth) / 2.0f;
  float offsetY = (480 - gridHeight) / 2.0f;

  std::vector<float> vertices;

  for (int row = 0; row < MAP_HEIGHT; row++) {
    for (int col = 0; col < MAP_WIDTH; col++) {
      // std::cout << mapVal[row][col] << std::endl;
      float x1 = offsetX + col * tileSize;
      float y1 = offsetY + row * tileSize;
      float x2 = x1 + tileSize;
      float y2 = y1;
      float x3 = x1 + tileSize;
      float y3 = y1 + tileSize;
      float x4 = x1;
      float y4 = y1 + tileSize;

      float r, g, b;
      switch (mapVal[row][col]) {
      case 1:
        r = 0.0f;
        g = 1.0f;
        b = 0.0f;
        break;
      case 2:
        r = 1.0f;
        g = 0.0f;
        b = 0.0f;
        break;
      case 0:
        r = 0.0f;
        g = 0.0f;
        b = 1.0f;
        break;
      }
      vertices.push_back(x1);
      vertices.push_back(y1);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
      vertices.push_back(x2);
      vertices.push_back(y2);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
      vertices.push_back(x3);
      vertices.push_back(y3);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
      vertices.push_back(x1);
      vertices.push_back(y1);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
      vertices.push_back(x3);
      vertices.push_back(y3);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
      vertices.push_back(x4);
      vertices.push_back(y4);
      vertices.push_back(0.0f);
      vertices.push_back(r);
      vertices.push_back(g);
      vertices.push_back(b);
    }
  }
  return vertices;
}

std::vector<int> getIndices() {
  std::vector<int> indices = {0, 1, 2, 0, 2, 3};
  return indices;
}
