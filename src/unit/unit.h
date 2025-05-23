#ifndef _UNIT_
#define _UNIT_

#include "../ai/ai.h"
#include "../texture/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <queue>
#include <vector>

class Unit {
  public:
	Unit(int row, int col, Texture *sprite);
	void draw();
	void moveTo(int targetRow, int targetCol);
	static void initGraphics();
	void drawSprite();
	static void compileShaders(std::string *vSrc, std::string *fSrc);
	void cleanUpGraphics();
	int getRow();
	int getCol();
	unsigned int getID();
	void setTexture(Texture *sprite);
	void setSelected(bool s);
	int state = 1;
	void addMovement(const std::vector<std::vector<int>> &grid,
					 std::pair<int, int> target);
	void clearMovementQueue();
	bool isSelected();

	void setScreenPosFromGrid();

	void update(float dt);

	std::queue<std::pair<int, int>> movementQueue;

  private:
	int row, col;
	Texture *texture;
	unsigned int unitId;
	static std::vector<float> vertices;
	static unsigned int nextID;
	static unsigned int VAO;
	static unsigned int VBO;
	static unsigned int vertexShader;
	static unsigned int fragmentShader;
	static unsigned int shaderProgram;
	int frameIndex;
	bool selected = false;
	int atlasRows = 1;
	int atlasCols = 68;
	int selectedFrameIndex = 37;
	int normalFrameIndex = 1;
};
#endif
