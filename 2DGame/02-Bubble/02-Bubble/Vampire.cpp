#include "Vampire.h"



Vampire::Vampire(int x, int y)
{
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.5;
	WIDTH = 16;
	HEIGHT = 16;
	WIDTH_OFFSET = 0;
	HEIGHT_OFFSET = 0;
	BAT_HEIGHT = 16;
	BAT_WIDTH = 16;
	BAT_WIDTH_OFFSET = 0;
	BAT_HEIGHT_OFFSET = 0;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, TRANSFORM_TO_BAT, TRANSFORM_TO_VAMPIRE, FLY_LEFT, FLY_RIGHT};
	animationLength = 8;
	movingLeft = false;
	isVampire = true;
}

void Vampire::update(int deltaTime)
{
	sprite->update(deltaTime);
	float trash = 0;
	
	if (sprite->animation() == TRANSFORM_TO_BAT || sprite->animation() == TRANSFORM_TO_VAMPIRE)
	{
		//it hasn't finished transforming
		if (sprite->getKeyFrame() < animationLength - 1) return;
	}

	if (isVampire)
	{
		if (movingLeft)
		{
			position.x -= speed;
			if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)) || 
				!map->collisionMoveDown(position + glm::vec2(-map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET, false))
			{
				position.x += speed;
				movingLeft = false;
			}
			else if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		else
		{
			position.x += speed;
			if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)) || 
				!map->collisionMoveDown(position + glm::vec2(map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET, false))
			{
				position.x -= speed;
				movingLeft = true;
			}
			else if (sprite->animation() != MOVE_RIGHT)
				sprite->changeAnimation(MOVE_RIGHT);
		}

		position.y += FALL_STEP;
		map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET, false);

		if (rand() % 100 == 0)
		{
			isVampire = !isVampire;
			sprite->changeAnimation(TRANSFORM_TO_BAT);
		}
	}

	else
	{
		if (movingLeft)
		{
			position.x -= speed;
			if (map->collisionMoveLeft(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT)))
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
			if (map->collisionMoveRight(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT)))
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
			if (map->collisionMoveUp(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), &position.y))
			{
				movingUp = false;
			}
		}
		else
		{
			position.y += speed;
			if (map->collisionMoveDown(position + glm::vec2(BAT_WIDTH_OFFSET, BAT_HEIGHT_OFFSET), glm::ivec2(BAT_WIDTH, BAT_HEIGHT), &position.y, BAT_HEIGHT_OFFSET, false))
			{
				movingUp = true;
				if (rand() % 5 == 0) {
					isVampire = !isVampire;
					sprite->changeAnimation(TRANSFORM_TO_VAMPIRE);
				}
			}
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}
