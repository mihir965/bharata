#include "unit.h"

Unit::Unit(int row, int col, Texture *sprite) {
	this->row = row;
	this->col = col;
	this->texture = sprite;
	this->unitId = nextID++;
}
