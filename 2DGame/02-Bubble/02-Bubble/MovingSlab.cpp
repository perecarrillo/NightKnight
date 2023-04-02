#include "MovingSlab.h"
#include <iostream>

using namespace std;



MovingSlab::MovingSlab(int firstX, int firstY, int lastX, int lastY, int tileSize)
{
	initialPosition = glm::ivec2(firstX, firstY);
	position = glm::ivec2(initialPosition.x * tileSize, initialPosition.y * tileSize);
	speed = 0.5;
	verticalSpeed = 0.35;
	animationLength = 1;
	animationsUsed = { NOT_PAINTED, PAINTED };
	spriteSize = glm::ivec2(16, 8);
	WIDTH = 16;
	HEIGHT = 8;
	movingLeft = true;
	movingUp = true;
	start = glm::ivec2(firstX * tileSize, firstY * tileSize);
	end = glm::ivec2(lastX * tileSize, lastY * tileSize);
	painted = false;
}

void MovingSlab::update(int deltaTime, Player *player) {
	sprite->update(deltaTime);

	bool hasPlayerAbove = false;
	glm::vec2 playerPosition = player->getPosition();
	int playerHeight = player->getHeight();
	int playerWidth = player->getWidth();
	if (playerPosition.y + playerHeight >= position.y - 3 && playerPosition.y + playerHeight <= position.y + 3 && playerPosition.x < position.x + WIDTH && position.x < playerPosition.x + playerWidth)
	{
		//cout << "isOnPlatform" << endl;
		hasPlayerAbove = true;
		player->setIsOnPlatform(true);
		player->setPosition(glm::vec2(playerPosition.x, position.y - playerHeight));
		if (sprite->animation() != PAINTED)
			sprite->changeAnimation(PAINTED);
		painted = true;
	}
	else player->setIsOnPlatform(false);

	if (start.x != end.x) {
		if (movingLeft) {
			if (position.x - speed < start.x)
				movingLeft = false;
			else {
				position.x -= speed;
				if (hasPlayerAbove)
					player->addXMovement(-speed);
			}
		}
		else {
			if (position.x + speed > end.x)
				movingLeft = true;
			else {
				position.x += speed;
				if (hasPlayerAbove)
					player->addXMovement(speed);
			}
		}
	}
	if (start.y != end.y) {
		if (movingUp) {
			if (position.y - verticalSpeed < end.y)
				movingUp = false;
			else {
				position.y -= verticalSpeed;
				if (hasPlayerAbove)
					player->addYMovement(-verticalSpeed - 1);
			}
		}
		else {
			if (position.y + verticalSpeed > start.y)
				movingUp = true;
			else {
				position.y += verticalSpeed;
				if (hasPlayerAbove)
					player->addYMovement(verticalSpeed - 1);
			}
		}
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}

bool MovingSlab::isPainted()
{
	return painted;
}
