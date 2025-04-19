#ifndef _TEXTURE_
#define _TEXTURE_

#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <glad/glad.h>

class Texture {
  public:
	Texture(const char *path);
	~Texture();
	void bind();
	void unbind();
	unsigned int getID();
	float width, height;

  private:
	unsigned int id;
};

#endif
