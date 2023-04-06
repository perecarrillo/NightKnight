#include "Vampire.h"



Vampire::Vampire(int x, int y)
{
	initialPosition = glm::ivec2(x, y);
	speed = 0.3;
	WIDTH = 11;
	HEIGHT = 16;
	WIDTH_OFFSET = 9;
	HEIGHT_OFFSET = 16;
	BAT_HEIGHT = 7;
	BAT_WIDTH = 11;
	BAT_WIDTH_OFFSET = 3;
	BAT_HEIGHT_OFFSET = 19;
	animationsUsed = {FLY_RIGHT, FLY_LEFT, MOVE_RIGHT, MOVE_LEFT, TRANSFORM_TO_BAT_RIGHT, TRANSFORM_TO_BAT_LEFT, TRANSFORM_TO_VAMPIRE_RIGHT, TRANSFORM_TO_VAMPIRE_LEFT};
	animationLength = 16;
	movingLeft = false;
	isVampire = true;
	spriteSize = glm::vec2(32, 32);
	animationSpeed = 8;
}

void Vampire::update(int deltaTime, bool frozen)
{
	time += deltaTime;
	if (frozen) return;
	sprite->update(deltaTime);
	float trash = 0;

	if (sprite->animation() == TRANSFORM_TO_BAT_RIGHT || sprite->animation() == TRANSFORM_TO_BAT_LEFT || sprite->animation() == TRANSFORM_TO_VAMPIRE_RIGHT || sprite->animation() == TRANSFORM_TO_VAMPIRE_LEFT)
	{
		//it hasn't finished transforming
		if (sprite->getKeyFrame() < animationLength - 1) return;
		if (sprite->animation() == TRANSFORM_TO_BAT_RIGHT || sprite->animation() == TRANSFORM_TO_BAT_LEFT) position.x += 7;

	}

	if (isVampire)
	{
		if (movingLeft)
		{
			position.x -= speed;
			if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false) || 
				!map->collisionMoveDown(position + glm::vec2(-map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET))
			{
				position.x += speed;
				movingLeft = false;
			}
			else if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			if (rand() % 500 == 0)
			{
				isVampire = !isVampire;
				sprite->changeAnimation(TRANSFORM_TO_BAT_LEFT);
			}
		}
		else
		{
			position.x += speed;
			if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false) || 
				!map->collisionMoveDown(position + glm::vec2(map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET))
			{
				position.x -= speed;
				movingLeft = true;
			}
			else if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
			if (rand() % 500 == 0)
			{
				isVampire = !isVampire;
				sprite->changeAnimation(TRANSFORM_TO_BAT_RIGHT);
			}
		}

		position.y += FALL_STEP;
		map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
	}

	else
	{
		if (movingLeft)
		{
			position.x -= speed;
			if (map->collisionMoveLeft(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), true))
			{
				position.x += speed;
				movingLeft = false;
			}
			else if (sprite->animation() != FLY_LEFT)
				sprite->changeAnimation(FLY_LEFT);

		}
		else
		{
			position.x += speed;
			if (map->collisionMoveRight(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), true))
			{
				position.x -= speed;
				movingLeft = true;
			}
			else if (sprite->animation() != FLY_RIGHT)
				sprite->changeAnimation(FLY_RIGHT);

		}

		if (movingUp)
		{
			position.y -= speed;
			if (map->collisionMoveUp(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), &position.y, BAT_HEIGHT_OFFSET, true))
			{
				movingUp = false;
			}
		}
		else
		{
			position.y += speed;
			if (map->collisionMoveDown(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), &position.y, BAT_HEIGHT_OFFSET))
			{
				movingUp = true;
				if (rand() % 3 == 0) {
					isVampire = !isVampire;
					sprite->changeAnimation(TRANSFORM_TO_VAMPIRE_LEFT);
					position.x -= 7;
					position.y -= 6;
				}
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}

glm::ivec2 Vampire::getBoundingBoxMin()
{
	if (isVampire)
		return glm::ivec2(position.x + WIDTH_OFFSET + 1, position.y + HEIGHT_OFFSET);
	else
		return glm::ivec2(position.x + BAT_WIDTH_OFFSET + 1, position.y + BAT_HEIGHT_OFFSET);
}

glm::ivec2 Vampire::getBoundingBoxMax()
{
	if (isVampire)
		return glm::ivec2(position.x + BAT_WIDTH_OFFSET + BAT_WIDTH + BAT_WIDTH_OFFSET - 1, position.y + BAT_HEIGHT + BAT_HEIGHT_OFFSET);
	else
		return glm::ivec2(position.x + BAT_WIDTH_OFFSET + BAT_WIDTH + BAT_WIDTH_OFFSET - 1, position.y + BAT_HEIGHT + BAT_HEIGHT_OFFSET);
}