#ifndef _TEXTURE_
#define _TEXTURE_

class Texture {
  public:
	bool loadFromFile(const char *);
	void bind();
	void unbind();

  private:
	unsigned int id;
	int widthm, height;
};

#endif
