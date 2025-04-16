#include "texture.h"

void generateAndBindTexture(unsigned int &texture) {
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
					GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void useSourceImageForTexture(const char *path) {
	SDL_Surface *spritesheet = IMG_Load(path);
	GLenum fmt = (spritesheet->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, spritesheet->w, spritesheet->h, 0, fmt,
				 GL_UNSIGNED_BYTE, spritesheet->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
}
