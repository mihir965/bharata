#ifndef _UNIT_
#define _UNIT_

#include "../texture/texture.h"

class Unit {
  public:
	Unit(int row, int col, Texture *sprite);
	void draw();
	void moveTo(int targetRow, int targetCol);

  private:
	int row, col;
	Texture *texture;
};

#endif
