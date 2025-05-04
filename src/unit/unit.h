#ifndef _UNIT_
#define _UNIT_

#include "../texture/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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
	// void setVertices(int unitType);
	void setSelected(bool s);
	void setHighlightTexture(Texture *sprite);

  private:
	int row, col;
	Texture *texture;
	unsigned int unitId;
	bool selected = false;
	Texture *highlightedTex = nullptr;
	static std::vector<float> vertices;
	static unsigned int nextID;
	static unsigned int VAO;
	static unsigned int VBO;
	static unsigned int vertexShader;
	static unsigned int fragmentShader;
	static unsigned int shaderProgram;
};
#endif
