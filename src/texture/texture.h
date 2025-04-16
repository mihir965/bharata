#ifndef _TEX_
#define _TEX_

#include <SDL2/SDL_image.h>
#include <glad/glad.h>
#include <iostream>

void generateAndBindTexture(unsigned int &texture);

void useSourceImageForTexture(const char *path);

#endif
