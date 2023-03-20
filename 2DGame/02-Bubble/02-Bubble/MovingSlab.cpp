#include "MovingSlab.h"



MovingSlab::MovingSlab(int firstX, int firstY, int lastX, int lastY, int tileSize)
{
	initialPosition = glm::ivec2(firstX, firstY);
	position = glm::ivec2(initialPosition.x * tileSize, initialPosition.y * tileSize);
	speed = 0.5; 
	animationLength = 1;
	animationsUsed = { MOVE_LEFT };
	spriteSize = glm::ivec2(16, 8);
	WIDTH = 16;
	HEIGHT = 8;
	movingLeft = true;
	start = glm::ivec2(firstX * tileSize, firstY * tileSize);
	end = glm::ivec2(lastX * tileSize, lastY * tileSize);
}

void MovingSlab::update(int deltaTime) {
	sprite->update(deltaTime);

	if (movingLeft)
	{
		position.x -= speed;
		if (position.x < start.x)
		{
			position.x += speed;
			movingLeft = false;
		}

	}
	else
	{
		position.x += speed;
		if (position.x > end.x)
		{
			position.x -= speed;
			movingLeft = true;
		}

	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}