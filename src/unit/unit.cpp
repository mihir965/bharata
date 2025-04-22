#include "unit.h"
#include "texture/texture.h"
#include <vector>

unsigned int Unit::nextID = 0;
unsigned int Unit::VAO = 0;
unsigned int Unit::VBO = 0;
unsigned int Unit::vertexShader = 0;
unsigned int Unit::fragmentShader = 0;
unsigned int Unit::shaderProgram = 0;
std::vector<float> Unit::vertices = {
	// pos      // tex (u, v)
	0.0f, 1.0f, 0.0f,	1.0f, // top-left
	1.0f, 1.0f, 0.047f, 1.0f, // top-right
	1.0f, 0.0f, 0.047f, 0.0f, // bottom-right

	0.0f, 1.0f, 0.0f,	1.0f, // top-left
	1.0f, 0.0f, 0.047f, 0.0f, // bottom-right
	0.0f, 0.0f, 0.0f,	0.0f  // bottom-left
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

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Unit::vertices.size() * sizeof(float),
				 Unit::vertices.data(), GL_STATIC_DRAW);
	glBindVertexArray(Unit::VAO);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
						  (void *)0);
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
	float gridWidth = (16 + 16) * (64.0f / 2.0f);
	float gridHeight = (16 + 16) * (32.0f / 2.0f);

	float offsetX = (1024 - gridWidth) / 2.0f;
	float offsetY = (786 - gridHeight) / 2.0f;
	int isoX = (col - row) * (64.0f / 2) + offsetX;
	int isoY = (col + row) * (32.0f / 2) + offsetY;

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(isoX, isoY, 0.0f));
	model = glm::scale(model, glm::vec3(128.0f * 2.0f, 64.0f * 2.0f, 1.0f));

	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glm::mat4 projection = glm::ortho(0.0f, 1024.0f, 786.0f, 0.0f, -1.0f, 1.0f);

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint texLoc = glGetUniformLocation(shaderProgram, "image");

	glUniform1i(texLoc, 0);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture->getID());
	glBindVertexArray(Unit::VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
