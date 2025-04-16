#include "map.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdbool.h>
#include <utility>
#include <vector>

// Grass tile is 0.0f - 0.0625f - x
// 0.0f - 0.0238 - y
//
// Water - 0.453f - 0.477f - y
// Same on x

std::vector<float> getVertexData(const std::vector<std::vector<int>> &mapVal) {
	std::vector<float> vertices;

	float gridWidth = (MAP_WIDTH + MAP_HEIGHT) * (TILE_WIDTH / 2.0f);
	float gridHeight = (MAP_WIDTH + MAP_HEIGHT) * (TILE_HEIGHT / 2.0f);
	float offsetX = (640 - gridWidth) / 2.0f + (MAP_WIDTH * TILE_WIDTH / 2.0f);
	float offsetY =
		(480 - gridHeight) / 2.0f + (MAP_HEIGHT * TILE_HEIGHT / 8.0f);

	const float tex_tile_width = 64.0f / 1024.0f;
	const float tex_tile_height = 32.0f / 1344.0f;

	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			float xCenter = ((col - row) * TILE_WIDTH / 2.0f) + offsetX;
			float yCenter = ((col + row) * TILE_HEIGHT / 2.0f) + offsetY;

			float xTop = xCenter;
			float yTop = yCenter - TILE_HEIGHT / 2.0f;

			float xRight = xCenter + TILE_WIDTH / 2.0f;
			float yRight = yCenter;

			float xBottom = xCenter;
			float yBottom = yCenter + TILE_HEIGHT / 2.0f;

			float xLeft = xCenter - TILE_WIDTH / 2.0f;
			float yLeft = yCenter;

			float u_min, u_max;
			float v_min, v_max;

			switch (mapVal[row][col]) {
			case 0:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 0.0f;
				v_max = tex_tile_height;
				break;
			case 1:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 19 * tex_tile_height;
				v_max = 20 * tex_tile_height;
				break;
			case 2:
				u_min = 0.0f;
				u_max = tex_tile_width;
				v_min = 0.0f;
				v_max = tex_tile_height;
				break;
			default:
				break;
			}
			float u_mid = (u_min + u_max) / 2.0f;
			float v_mid = (v_min + v_max) / 2.0f;

			float u_top = u_mid;
			float v_top = v_min;

			float u_right = u_max;
			float v_right = v_mid;

			float u_bottom = u_mid;
			float v_bottom = v_max;

			float u_left = u_min;
			float v_left = v_mid;

			vertices.push_back(xTop);
			vertices.push_back(yTop);
			vertices.push_back(0.0f);
			vertices.push_back(u_top);
			vertices.push_back(v_top);

			vertices.push_back(xRight);
			vertices.push_back(yRight);
			vertices.push_back(0.0f);
			vertices.push_back(u_right);
			vertices.push_back(v_right);

			vertices.push_back(xBottom);
			vertices.push_back(yBottom);
			vertices.push_back(0.0f);
			vertices.push_back(u_bottom);
			vertices.push_back(v_bottom);

			vertices.push_back(xTop);
			vertices.push_back(yTop);
			vertices.push_back(0.0f);
			vertices.push_back(u_top);
			vertices.push_back(v_top);

			vertices.push_back(xBottom);
			vertices.push_back(yBottom);
			vertices.push_back(0.0f);
			vertices.push_back(u_bottom);
			vertices.push_back(v_bottom);

			vertices.push_back(xLeft);
			vertices.push_back(yLeft);
			vertices.push_back(0.0f);
			vertices.push_back(u_left);
			vertices.push_back(v_left);
		}
	}
	return vertices;
}

void gridGraphicsSetup(unsigned int &VAO, unsigned int &VBO,
					   std::vector<float> vertices) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
				 vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
						  (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}
