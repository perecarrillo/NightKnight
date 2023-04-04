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
	void init(string textureFile, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, TileMap *map);
	void update(int deltaTime, bool finalLevel);
	void openChest();
	void unlockChest();
	bool isOpened();
	bool playerHasEntered();
	glm::vec2 getPosition();
private:
	int time;
	bool open, unlock, opened;
	bool playerIn;
};

#endif