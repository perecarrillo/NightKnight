#ifndef _RAJOLA_INCLUDE
#define _RAJOLA_INCLUDE


#include "Sprite.h"


// Rajola is basically a Sprite that represents the tiles that the player needs to press. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Rajola
{

public:
	Rajola();
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2 &pos);
	glm::ivec2 getInitialPosition();
	void changePosIni(const glm::vec2 &pos);

private:
	glm::ivec2 initialPosition;
	bool bJumping;
	glm::ivec2 tileMapDispl, posRajola;
	int jumpAngle, startY, jumpLost, coyoteTime;
	Texture spritesheet;
	Sprite *sprite;
};


#endif // _RAJOLA_INCLUDE


