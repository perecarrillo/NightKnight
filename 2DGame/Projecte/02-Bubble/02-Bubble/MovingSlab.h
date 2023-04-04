#ifndef _MOVINGPLATFORM_INCLUDE
#define _MOVINGPLATFORM_INCLUDE

#include "Entity.h"
#include "Player.h"


class MovingSlab : public Entity
{
public:
	MovingSlab(int id, int firstX, int firstY, int lastX, int lastY, int tileSize);
	void update(int deltaTime, Player *player);
	bool isPainted();

private:
	int id;
	glm::ivec2 start;
	glm::ivec2 end;
	bool movingLeft;
	bool movingUp;
	bool painted;
	float verticalSpeed;
};

#endif