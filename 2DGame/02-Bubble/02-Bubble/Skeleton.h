#ifndef _SKELETON_INCLUDE
#define _SKELETON_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include <vector>

class Skeleton
{
	enum movements
	{
		LEFT, RIGHT, STOP
	};
public:
	Skeleton();
	void init(string textureFile, int numberOfAnimations, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getInitialPosition();

private:
	glm::ivec2 initialPosition;
	glm::ivec2 tileMapDispl;
	glm::vec2 positionFloat;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	vector<pair<int, int>> movement = { {LEFT,150}, {STOP, 50}, {RIGHT, 150}, {STOP, 50} };
	pair<int, int> actualMovement;
};

#endif