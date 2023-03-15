#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <vector>
#include "Entity.h"

class Skeleton : public Entity
{
	enum movements
	{
		LEFT, RIGHT, STOP
	};

	enum SkeletonAnims
	{
		MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
	};
	

public:
	Skeleton();
};

#endif