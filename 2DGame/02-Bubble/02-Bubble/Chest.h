#ifndef _CHEST_INCLUDE
#define _CHEST_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <vector>
#include "Entity.h"

class Chest : public Entity
{

public:
	Chest(int x, int y);
	void update(int deltaTime);
	void openChest();
	void unlockChest();
	bool isOpened();
	int time;
	bool open, unlock, opened;
};

#endif