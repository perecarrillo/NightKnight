#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <vector>

class Entity
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
	Entity();
	virtual void init(string textureFile, int numberOfAnimations, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	virtual void update(int deltaTime);
	virtual void render();

	virtual void setTileMap(TileMap *tileMap);
	virtual void setPosition(const glm::vec2 &pos);
	virtual glm::ivec2 getInitialPosition();

protected:
	float speed = 0.5; //Should be changed at constructor
	glm::ivec2 initialPosition = glm::ivec2(1, 1); //Should be changed at constructor
	vector<pair<int, int>> movement;// Must be changed at constructor for ex: { { LEFT,150 },{ STOP, 50 },{ RIGHT, 150 },{ STOP, 50 } };
	float FALL_STEP = 2;
	int WIDTH = 16;
	int HEIGHT = 16;
	int WIDTH_OFFSET = 0;
	int HEIGHT_OFFSET = 0;
	
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	pair<int, int> actualMovement;
};

#endif