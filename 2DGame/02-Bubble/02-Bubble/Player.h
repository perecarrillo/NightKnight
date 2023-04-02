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
	void moveToChest(int deltaTime, glm::vec2 chestPosition);
	void render();
	
	int getNumHearts();

	void setNumHearts(int num);

	int getNumCoins();

	void setNumCoins(int num);
	
	void loseHeart();
	bool isInmune();

	void isOnPlatform(int id);
	void isNotOnPlatform(int id);
	int getWidth();
	int getHeight();
	void addXMovement(float value);
	void addYMovement(float value);
	void toggleInmunity();

	glm::vec2 getPosition();
	void setPosition(glm::vec2 pos);

private:
	bool bJumping;
	int jumpAngle, startY, jumpLost, coyoteTime;
	int hearts;
	bool inmune;
	bool show;
	int time;
	int timeInmune;
	int coins;
	bool invincible;
	set<int> onPlatforms;
};


#endif // _PLAYER_INCLUDE


