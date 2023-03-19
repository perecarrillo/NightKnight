#ifndef _KEY_INCLUDE
#define _KEY_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <vector>
#include "Entity.h"

class Key : public Entity
{
	enum Movements
	{
		STOP
	};

	enum Animations
	{
		STAND_LEFT
	};


public:
	Key();
	Key(int x, int y);
};

#endif