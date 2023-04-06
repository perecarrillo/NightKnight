#ifndef _ENTITY_INCLUDE
#define _ENTITY_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include <vector>

enum Animations 
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, JUMP_LEFT, JUMP_RIGHT, DYING, NOT_PAINTED, PAINTED, MOVE_UP, LOCKED, UNLOCKING, UNLOCKED, OPENING_EMPTY, OPENED_EMPTY, OPENING_FULL, OPENED_FULL, ENTERING, MOVE_LEFT_TERMINATOR, MOVE_RIGHT_TERMINATOR, STAND_LEFT_TERMINATOR, STAND_RIGHT_TERMINATOR, TRANSFORM_TO_BAT_RIGHT, TRANSFORM_TO_BAT_LEFT, TRANSFORM_TO_VAMPIRE_RIGHT, TRANSFORM_TO_VAMPIRE_LEFT, FLY_LEFT, FLY_RIGHT
};

class Entity
{

public:
	Entity();
	Entity(int x, int y);
	virtual void init(string textureFile, const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram, TileMap *map);
	virtual void update(int deltaTime, bool frozen = false);
	virtual void render(bool unfreezing = false);

	virtual void setTileMap(TileMap *tileMap);
	virtual void setPosition(const glm::vec2 &pos);
	virtual glm::ivec2 getInitialPosition();
	virtual glm::ivec2 getSpriteSize();
	virtual glm::ivec2 getBoundingBoxMin();
	virtual glm::ivec2 getBoundingBoxMax();
	virtual void nextAnimation();

protected:
	float speed = 0.5; //Should be changed at constructor
	glm::ivec2 initialPosition = glm::ivec2(1, 1); //Should be changed at constructor
	vector<pair<int, int>> movement;// Must be changed at constructor for ex: { { LEFT,150 },{ STOP, 50 },{ RIGHT, 150 },{ STOP, 50 } };
	float FALL_STEP = 2;
	int WIDTH = 16;
	int HEIGHT = 16;
	int WIDTH_OFFSET = 0;
	int HEIGHT_OFFSET = 0;
	glm::ivec2 spriteSize = glm::ivec2(16, 16);
	int animationLength = 8;
	vector<Animations> animationsUsed;
	int animationSpeed = -1;
	
	int time;
	glm::ivec2 tileMapDispl;
	glm::vec2 position;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	pair<int, int> actualMovement;
};

#endif