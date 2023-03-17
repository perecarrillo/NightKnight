#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"



// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	Player();
	Player(int x, int y);
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	
	int getNumHearts();
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getInitialPosition();
	glm::ivec2 getBoundingBoxMin();
	glm::ivec2 getBoundingBoxMax();
	void loseHeart();
	bool isInmune();

private:
	glm::ivec2 initialPosition;
	bool bJumping;
	glm::ivec2 tileMapDispl;
	glm::vec2 posPlayer;
	int jumpAngle, startY, jumpLost, coyoteTime;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;
	int hearts;
	bool inmune;
	bool show;
	int time;
	int timeInmune;
};


#endif // _PLAYER_INCLUDE


