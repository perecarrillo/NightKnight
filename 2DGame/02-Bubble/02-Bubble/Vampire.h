#ifndef _VAMPIRE_INCLUDE
#define _VAMPIRE_INCLUDE

#include "Entity.h"

class Vampire : public Entity
{
public:
	Vampire(int x, int y);

	void update(int deltaTime);

private:
	bool isVampire;
	bool movingLeft;

	int BAT_HEIGHT, BAT_WIDTH, BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET;
};

#endif