#ifndef _MOVINGPLATFORM_INCLUDE
#define _MOVINGPLATFORM_INCLUDE

#include "Entity.h"
#include "Player.h"


class MovingSlab : public Entity
{
public:
	MovingSlab(int firstX, int firstY, int lastX, int lastY, int tileSize);
	void update(int deltaTime, Player *player);
	bool isPainted();

private:
	glm::ivec2 start;
	glm::ivec2 end;
	bool movingLeft;
	bool painted;
};

#endif