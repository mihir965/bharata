#include "unit.h"
#include "texture/texture.h"
#include <utility>
#include <vector>

unsigned int Unit::nextID = 0;
unsigned int Unit::VAO = 0;
unsigned int Unit::VBO = 0;
unsigned int Unit::vertexShader = 0;
unsigned int Unit::fragmentShader = 0;
unsigned int Unit::shaderProgram = 0;
std::vector<float> Unit::vertices = {
	// pos      // tex (u, v)
	0.0f, 1.0f, 0.0f, 1.0f, // top-left
	1.0f, 1.0f, 1.0f, 1.0f, // top-right
	1.0f, 0.0f, 1.0f, 0.0f, // bottom-right

	0.0f, 1.0f, 0.0f, 1.0f, // top-left
	1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
	0.0f, 0.0f, 0.0f, 0.0f	// bottom-left
};

Unit::Unit(int row, int col, Texture *sprite) {
	this->row = row;
	this->col = col;
	this->texture = sprite;
	this->unitId = nextID++;
}

void Unit::initGraphics() {

	glGenVertexArrays(1, &Unit::VAO);
	glGenBuffers(1, &Unit::VBO);
	glBindVertexArray(Unit::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Unit::vertices.size() * sizeof(float),
				 Unit::vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
						  (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
						  (void *)(2 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Unit::compileShaders(std::string *vSrc, std::string *fSrc) {
	const char *vertexShaderSource = vSrc->c_str();

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	const char *fragShaderSource = fSrc->c_str();
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex Shader compilation failed:\n"
				  << infoLog << std::endl;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDetachShader(shaderProgram, vertexShader);
	glDetachShader(shaderProgram, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Unit::drawSprite() {
	glUseProgram(Unit::shaderProgram);
	int isoX = (col - row) * (64.0f / 2);
	int isoY = (col + row) * (32.0f / 2);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(
		model, glm::vec3(isoX + (1024 / 2.0f) - 64.0f,
						 isoY + (786 / 4.0f) - (12.25 * 32.0f), 0.0f));
	model = glm::scale(model, glm::vec3(128.0f, 64.0f * 2.0f, 1.0f));

	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 786.0f, 0.0f, -1.0f, 1.0f);

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint texLoc = glGetUniformLocation(shaderProgram, "image");

	glUniform1i(texLoc, 0);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	int C = atlasCols, R = atlasRows;
	// int idx = selected ? selectedFrameIndex : normalFrameIndex;
	int idx = state;
	float fx = idx % C;
	float fy = float(idx) / C;

	glm::vec2 uvScale = glm::vec2(1.0f / float(C), 1.0f / float(R));
	glm::vec2 uvOffset = glm::vec2(fx * uvScale.x, (R - 1) * uvScale.y);

	glUniform2fv(glGetUniformLocation(shaderProgram, "uvScale"), 1,
				 &uvScale[0]);
	glUniform2fv(glGetUniformLocation(shaderProgram, "uvOffset"), 1,
				 &uvOffset[0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getID());
	glBindVertexArray(Unit::VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	if (this->selected && this->state < this->selectedFrameIndex)
		state++;
}

int Unit::getRow() {
	return this->row;
}

int Unit::getCol() {
	return this->col;
}

unsigned int Unit::getID() {
	return this->unitId;
}

void Unit::setTexture(Texture *sprite) {
	this->texture = sprite;
}

void Unit::setSelected(bool s) {
	this->selected = s;
}

void Unit::moveTo(int targetRow, int targetCol) {
	this->row = targetRow;
	this->col = targetCol;
}

void Unit::addMovement(const std::vector<std::vector<int>> &grid,
					   std::pair<int, int> target) {
	std::vector<std::pair<int, int>> pathToFollow =
		plan_path(grid, std::make_pair(this->row, this->col), target);
	for (auto &road : pathToFollow) {
		movementQueue.push(road);
	}
}

bool Unit::isSelected() {
	return this->selected;
}
