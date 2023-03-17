#include "Rata.h"


Rata::Rata(int x, int y)
{
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.8;
	movement = { { LEFT,120 }, {RIGHT, 120} };
	animationLength = 10;
	numberOfAnimations = 5;
	WIDTH = 12;
	WIDTH_OFFSET = 20;
	WIDTH_OFFSET_LEFT = 14;
	HEIGHT = 8;
	HEIGHT_OFFSET = 24;
	spriteSize = glm::ivec2(32, 32);
}

void Rata::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == LEFT)
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		position.x -= speed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x += speed;
			sprite->changeAnimation(STAND_LEFT);
		}

	}
	else if (movement[actualMovement.first].first == RIGHT)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		position.x += speed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x -= speed;
			sprite->changeAnimation(STAND_RIGHT);
		}

	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}

	position.y += FALL_STEP;
	if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT)
		map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
	else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
		map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);


	--actualMovement.second;
	if (actualMovement.second == -1) {
		++actualMovement.first;
		if (actualMovement.first == movement.size()) actualMovement.first = 0;
		actualMovement.second = movement[actualMovement.first].second;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}
