#include "Rata.h"

#define JUMP_ANGLE_STEP 5
#define JUMP_HEIGHT 8

using namespace std;


Rata::Rata(int x, int y)
{
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.8;
	jumpingSpeed = 1.3;
	movement = { { MOVE_LEFT,112 }, {JUMP_LEFT, 100}, {STAND_LEFT, 250}, {JUMP_RIGHT, 10}, { MOVE_RIGHT, 112}, { STAND_RIGHT, 100} };
	animationLength = 10;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, JUMP_LEFT, JUMP_RIGHT};
	WIDTH = 12;
	WIDTH_OFFSET = 18;
	WIDTH_OFFSET_LEFT = 14;
	HEIGHT = 8;
	HEIGHT_OFFSET = 24;
	spriteSize = glm::ivec2(32, 32);
	jumpingLeft = false;
	jumpingRight = false;
	jumpAngle = 0;
}

void Rata::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == MOVE_LEFT)
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		position.x -= speed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x += speed;
			if (sprite->animation() != STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT);
		}

	}
	else if (movement[actualMovement.first].first == MOVE_RIGHT)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		position.x += speed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x -= speed;
			if (sprite->animation() != STAND_RIGHT)
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
	if (jumpingLeft)
	{
		if (sprite->animation() != JUMP_LEFT)
			sprite->changeAnimation(JUMP_LEFT);
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180)
		{
			sprite->changeAnimation(STAND_LEFT);
			jumpingLeft = false;
			++actualMovement.first;
			nextAnimation();
			//position.y = startY + jumpLost;
		}
		else
		{

			position.y = int((startY) - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				jumpingLeft = !map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
				if (!jumpingLeft) {
					++actualMovement.first;
					nextAnimation();
					sprite->changeAnimation(STAND_LEFT);
				}
			}
		}
		position.x -= jumpingSpeed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
			position.x += jumpingSpeed;
	}

	else if (jumpingRight)
	{
		if (sprite->animation() != JUMP_RIGHT)
			sprite->changeAnimation(JUMP_RIGHT);
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180)
		{
			sprite->changeAnimation(STAND_RIGHT);
			jumpingRight= false;
			++actualMovement.first;
			nextAnimation();
			//position.y = startY + jumpLost;
		}
		else
		{

			position.y = int((startY)-JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				jumpingRight = !map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
				if (!jumpingRight) {
					++actualMovement.first;
					nextAnimation();
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
		position.x += jumpingSpeed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
			position.x -= jumpingSpeed;
	}

	else
	{
		position.y += FALL_STEP;
		bool collisionDown;// = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT || sprite->animation() == JUMP_LEFT)
			collisionDown = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
		else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT)
			collisionDown = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);

		if (collisionDown)
		{
			if (movement[actualMovement.first].first == JUMP_LEFT)
			{
				jumpingLeft = true;
				jumpAngle = 0;
				startY = position.y;
			}
			else if (movement[actualMovement.first].first == JUMP_RIGHT)
			{
				jumpingRight = true;
				jumpAngle = 0;
				startY = position.y;
			}
		}

		--actualMovement.second;
		if (actualMovement.second == -1) {
			++actualMovement.first;
			nextAnimation();
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}

//We reduce by 2 px the boundingBox because we can
glm::ivec2 Rata::getBoundingBoxMin()
{
	if (jumpingLeft)
		return glm::ivec2(position.x + 17, position.y + 24);
	else if (sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) // If movement is to the right, we take out the tail
		return glm::ivec2(position.x + WIDTH_OFFSET + 1, position.y + HEIGHT_OFFSET);
	else
		return glm::ivec2(position.x + WIDTH_OFFSET_LEFT + 1, position.y + HEIGHT_OFFSET);
}

glm::ivec2 Rata::getBoundingBoxMax()
{
	if (jumpingLeft)
		return glm::ivec2(position.x + 17 + 10, position.y + 24 + 7);
	else if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) // If movement is to the left, we take out the tail
		return glm::ivec2(position.x + WIDTH_OFFSET_LEFT + WIDTH - 1, position.y + HEIGHT + HEIGHT_OFFSET);
	else
		return glm::ivec2(position.x + WIDTH_OFFSET + WIDTH - 1, position.y + HEIGHT + HEIGHT_OFFSET);
}

void Rata::nextAnimation()
{
	if (actualMovement.first == movement.size()) actualMovement.first = 0;
	actualMovement.second = movement[actualMovement.first].second;
}
