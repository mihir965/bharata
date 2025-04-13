#include "map.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>
#include <stdbool.h>
#include <vector>

// Grass tile is 0.0f - 0.0625f - x
// 0.0f - 0.0238 - y
//
// Water - 0.453f - 0.477f - y
// Same on x

std::vector<float> getVertexData(const std::vector<std::vector<int>> &mapVal) {
	std::vector<float> vertices;

	// Compute grid dimensions and offsets (screen-space)
	float gridWidth = (MAP_WIDTH + MAP_HEIGHT) * (TILE_WIDTH / 2.0f);
	float gridHeight = (MAP_WIDTH + MAP_HEIGHT) * (TILE_HEIGHT / 2.0f);
	float offsetX = (640 - gridWidth) / 2.0f + (MAP_WIDTH * TILE_WIDTH / 2.0f);
	float offsetY =
		(480 - gridHeight) / 2.0f + (MAP_HEIGHT * TILE_HEIGHT / 8.0f);

	// Normalized texture dimensions for a 64x32 tile in a 192x32 sheet
	const float tex_tile_width = 64.0f / 192.0f; // 0.3333
	const float tex_tile_height = 32.0f / 32.0f; // 1.0

	// For each cell, calculate isometric center and diamond corners
	// (screen-space)
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int col = 0; col < MAP_WIDTH; col++) {
			float xCenter = ((col - row) * TILE_WIDTH / 2.0f) + offsetX;
			float yCenter = ((col + row) * TILE_HEIGHT / 2.0f) + offsetY;

			// Diamond corners (screen-space)
			float xTop = xCenter;
			float yTop = yCenter - TILE_HEIGHT / 2.0f;

			float xRight = xCenter + TILE_WIDTH / 2.0f;
			float yRight = yCenter;

			float xBottom = xCenter;
			float yBottom = yCenter + TILE_HEIGHT / 2.0f;

			float xLeft = xCenter - TILE_WIDTH / 2.0f;
			float yLeft = yCenter;

			// Determine which sub-region of the spritesheet to use based on
			// mapVal[row][col]
			float u_min, u_max;
			float v_min = 0.0f; // Top of the sub-image
			float v_max = 1.0f; // Bottom of the sub-image

			// Our convention: 0=grass, 1=water, 2=red.
			// Adjust these as needed if your mapping is different.
			switch (mapVal[row][col]) {
			case 0: // Grass tile: leftmost tile: pixels [0,64]
				u_min = 0.0f;
				u_max = tex_tile_width; // 0.3333
				break;
			case 1: // Water tile: middle tile: pixels [64,128]
				u_min = tex_tile_width;		// 0.3333
				u_max = 2 * tex_tile_width; // 0.6667
				break;
			case 2: // Red tile: rightmost tile: pixels [128,192]
			default:
				u_min = 2 * tex_tile_width; // 0.6667
				u_max = 1.0f;
				break;
			}
			// Compute midpoints
			float u_mid = (u_min + u_max) / 2.0f;
			float v_mid = (v_min + v_max) / 2.0f;

			// Assign texture coordinates for diamond vertices:
			// Top vertex: use horizontal midpoint, top edge.
			float u_top = u_mid;
			float v_top = v_min;
			// Right vertex: right edge, vertical midpoint.
			float u_right = u_max;
			float v_right = v_mid;
			// Bottom vertex: horizontal midpoint, bottom edge.
			float u_bottom = u_mid;
			float v_bottom = v_max;
			// Left vertex: left edge, vertical midpoint.
			float u_left = u_min;
			float v_left = v_mid;

			// Assemble two triangles per diamond.
			// Triangle 1: top, right, bottom
			vertices.push_back(xTop);  // position x
			vertices.push_back(yTop);  // position y
			vertices.push_back(0.0f);  // position z
			vertices.push_back(u_top); // texture coord u
			vertices.push_back(v_top); // texture coord v

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

			// Triangle 2: top, bottom, left
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
