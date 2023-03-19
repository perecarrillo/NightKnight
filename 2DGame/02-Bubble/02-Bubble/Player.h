#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Entity.h"




// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player : public Entity
{

public:
	Player(int x, int y);
	void update(int deltaTime);
	void render();
	
	int getNumHearts();
	
	void loseHeart();
	bool isInmune();

private:
	bool bJumping;
	int jumpAngle, startY, jumpLost, coyoteTime;
	int hearts;
	bool inmune;
	bool show;
	int time;
	int timeInmune;
};


#endif // _PLAYER_INCLUDE


