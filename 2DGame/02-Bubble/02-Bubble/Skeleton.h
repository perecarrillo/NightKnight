#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <vector>
#include "Entity.h"

class Skeleton : public Entity
{
	enum Movements
	{
		LEFT, RIGHT, STOP
	};

	enum Animations
	{
		MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
	};
	

public:
	Skeleton();
};

#endif