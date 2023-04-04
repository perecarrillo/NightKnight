#ifndef _VAMPIRE_INCLUDE
#define _VAMPIRE_INCLUDE

#include "Entity.h"

class Vampire : public Entity
{
public:
	Vampire(int x, int y);

	void update(int deltaTime);

	glm::ivec2 getBoundingBoxMin();
	glm::ivec2 getBoundingBoxMax();

private:
	bool isVampire;
	bool movingLeft;
	bool movingUp;

	int BAT_HEIGHT, BAT_WIDTH, BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET;
};

#endif