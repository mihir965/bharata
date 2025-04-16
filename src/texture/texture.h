#ifndef _TEX_
#define _TEX_

#include <glad/glad.h>
#include <iostream>
#include <stb/stb_image.h>

void generateAndBindTexture(unsigned int &texture);

void useSourceImageForTexture(const char *path);

#endif
