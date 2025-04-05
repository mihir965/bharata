#include "map.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdbool.h>
#include <vector>

map_terrain_types map[MAP_HEIGHT][MAP_WIDTH];

std::vector<std::vector<int>> mapVal = {
    {0, 1, 0, 0, 1, 0, 1, 0, 0, 1}, {1, 0, 1, 0, 1, 0, 0, 1, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 0, 1, 0}, {1, 0, 0, 1, 1, 0, 1, 0, 0, 1},
    {0, 1, 0, 1, 0, 1, 1, 2, 0, 0}, {1, 0, 1, 0, 1, 0, 0, 1, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0, 0, 1}, {1, 0, 1, 0, 0, 1, 0, 1, 1, 0},
    {0, 0, 1, 1, 0, 0, 1, 0, 1, 0}, {1, 1, 0, 0, 1, 1, 0, 1, 0, 1}};

void map_reset() {
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      map[y][x] = MAP_TERRAIN_GRASS;
    }
  }
}

std::vector<float> getVertexData() {

  float tileWidth = 64.0f;
  float tileHeight = 32.0f;
  float gridWidth = (MAP_WIDTH + MAP_HEIGHT) * (tileWidth / 2.0f);
  float gridHeight = (MAP_WIDTH + MAP_HEIGHT) * (tileHeight / 2.0f);

  float offsetX = (640 - gridWidth) / 2.0f + (MAP_WIDTH * tileWidth / 2.0f);
  float offsetY = (480 - gridHeight) / 2.0f + (MAP_HEIGHT * tileHeight / 8.0f);

  std::vector<float> vertices;

  for (int row = 0; row < MAP_HEIGHT; row++) {
    for (int col = 0; col < MAP_WIDTH; col++) {
      // std::cout << mapVal[row][col] << std::endl;
      //      float x1 = offsetX + col * tileSize + tileSize / 2.0f;
      //      float y1 = offsetY + row * tileSize;
      float xCenter = ((col - row) * tileWidth / 2.0f) + offsetX;
      float yCenter = ((col + row) * tileHeight / 2.0f) + offsetY;

      float x1 = xCenter;
      float y1 = yCenter - tileHeight / 2.0f;

      float x2 = xCenter + tileWidth / 2.0f;
      float y2 = yCenter;

      float x3 = xCenter;
      float y3 = yCenter + tileHeight / 2.0f;

      float x4 = xCenter - tileWidth / 2.0f;
      float y4 = yCenter;

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
