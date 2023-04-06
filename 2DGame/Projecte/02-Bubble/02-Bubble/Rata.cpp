#include "Rata.h"
#include <iostream>

using namespace std;

#define WAIT_TIME 100


Rata::Rata(int x, int y)
{
	initialPosition = glm::ivec2(x, y);
	useAI = true;
	movingLeft = true;
	jumping = false;
	setUp();
}

Rata::Rata(int x, int y, vector<pair<int, int>> movement)
{
	initialPosition = glm::ivec2(x, y);
	useAI = false;
	setUp();
}

void Rata::setUp() 
{
	speed = 0.8;
	jumpingSpeed = 1.3;
	//movement = { { MOVE_LEFT,112 },{ JUMP_LEFT, 5 },{ STAND_LEFT, 250 },{ JUMP_RIGHT, 5 },{ MOVE_RIGHT, 112 },{ JUMP_RIGHT, 5 },{ MOVE_RIGHT, 250 },{ MOVE_LEFT, 20 },{ JUMP_LEFT, 3 } };
	animationLength = 10;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, JUMP_LEFT, JUMP_RIGHT };
	WIDTH = 12;
	WIDTH_OFFSET = 18;
	WIDTH_OFFSET_LEFT = 14;
	HEIGHT = 8;
	HEIGHT_OFFSET = 24;
	spriteSize = glm::ivec2(32, 32);
	jumpingLeft = false;
	jumpingRight = false;
	jumpAngle = 0;
	JUMP_HEIGHT = 8;
	waiting = 0;
}

void Rata::update(int deltaTime, bool frozen)
{
	time += deltaTime;
	if (frozen) return;
	if (useAI)
		updateAI(deltaTime);
	else
		updatePredefinedMovements(deltaTime);
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

void Rata::updateAI(int deltaTime)
{
	float trash = 0;
	sprite->update(deltaTime);
	if (!jumping)
	{
		if (movingLeft)
		{
			if (waiting > 0)
			{
				if (sprite->animation() != STAND_RIGHT)
					sprite->changeAnimation(STAND_RIGHT);
				--waiting;
				if (waiting == 0)
				{
					movingLeft = false;
					sprite->changeAnimation(MOVE_RIGHT);
				}
			}
			else
			{
				if (sprite->animation() != MOVE_LEFT)
					sprite->changeAnimation(MOVE_LEFT);
				position.y += FALL_STEP;
				bool collisionDown;
				collisionDown = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);

				position.x -= speed;
				bool collisionLeft = map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false);
				bool collisionDownLeft = map->collisionMoveDown(position + glm::vec2(-map->getTileSize() * 2, map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET);
				pair<int, int> jumpTo = map->closestJumpPosition(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), true);
				if (collisionLeft || (collisionDown && !collisionDownLeft && jumpTo.first == -1))
				{
					position.x += speed;
					waiting = WAIT_TIME;
					sprite->changeAnimation(STAND_RIGHT);
				}
				else if (collisionDown && !collisionDownLeft)
				{
					jumping = true;
					jumpAngle = 0;
					startY = position.y;
					calculateJumpParameters(jumpTo);
					sprite->changeAnimation(JUMP_LEFT);
				}
			}
		}
		else
		{
			if (waiting > 0)
			{
				if (sprite->animation() != STAND_LEFT)
					sprite->changeAnimation(STAND_LEFT);
				--waiting;
				if (waiting == 0)
				{
					movingLeft = true;
					sprite->changeAnimation(MOVE_LEFT);
				}
			}
			else
			{
				if (sprite->animation() != MOVE_RIGHT)
					sprite->changeAnimation(MOVE_RIGHT);
				position.y += FALL_STEP;
				bool collisionDown;
				collisionDown = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);

				position.x += speed;
				bool collisionRight = map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false);
				bool collisionDownRight = map->collisionMoveDown(position + glm::vec2(map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET);
				pair<int, int> jumpTo = map->closestJumpPosition(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false);
				if (collisionRight || (collisionDown && !collisionDownRight && jumpTo.first == -1))
				{
					position.x -= speed;
					waiting = WAIT_TIME;
					sprite->changeAnimation(MOVE_LEFT);
				}
				else if (collisionDown && !collisionDownRight)
				{
					jumping = true;
					jumpAngle = 0;
					startY = position.y;
					calculateJumpParameters(jumpTo);
					sprite->changeAnimation(JUMP_RIGHT);
				}
			}
		}
	}
	else
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180)
		{
			if (movingLeft) sprite->changeAnimation(MOVE_LEFT);
			else sprite->changeAnimation(MOVE_RIGHT);
			jumping = false;
		}
		else
		{
			position.y = int((startY)-JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				jumping = !map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
				if (!jumping) {
					if (movingLeft) sprite->changeAnimation(MOVE_LEFT);
					else sprite->changeAnimation(MOVE_RIGHT);
				}
			}
		}
		if (movingLeft)
		{
			position.x -= jumpingSpeed;
			if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
				position.x += jumpingSpeed;
		}
		else
		{
			position.x += jumpingSpeed;
			if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
				position.x -= jumpingSpeed;
		}

	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}

void Rata::updatePredefinedMovements(int deltaTime)
{

	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == MOVE_LEFT)
	{
		position.x -= speed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
		{
			position.x += speed;
			if (sprite->animation() != STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT);
		}
		else if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);

	}
	else if (movement[actualMovement.first].first == MOVE_RIGHT)
	{
		position.x += speed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
		{
			position.x -= speed;
			if (sprite->animation() != STAND_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);

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

			position.y = int((startY)-JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
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
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET_LEFT, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
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
			jumpingRight = false;
			++actualMovement.first;
			nextAnimation();
			//position.y = startY + jumpLost;
		}
		else
		{

			position.y = int((startY)-JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				jumpingRight = !map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);
				if (!jumpingRight) {
					++actualMovement.first;
					nextAnimation();
					sprite->changeAnimation(STAND_RIGHT);
				}
			}
		}
		position.x += jumpingSpeed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
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
				JUMP_ANGLE_STEP = movement[actualMovement.first].second;
			}
			else if (movement[actualMovement.first].first == JUMP_RIGHT)
			{
				jumpingRight = true;
				jumpAngle = 0;
				startY = position.y;
				JUMP_ANGLE_STEP = movement[actualMovement.first].second;
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

void Rata::calculateJumpParameters(pair<int, int> jumpTo)
{
	int x, y, nx, ny;
	x = (position.x + WIDTH_OFFSET / 2.) / map->getTileSize() + 5;
	nx = jumpTo.first;
	y = (position.y + HEIGHT_OFFSET + 1) / map->getTileSize();
	ny = jumpTo.second - 1;
	//cout << "y: " << y << " ny: " << ny << endl;

	if (y == ny)
	{
		//cout << "Same height" << endl;
		JUMP_ANGLE_STEP = 5;
		JUMP_HEIGHT = 8;
		return;
	}
	else if (y == ny + 1)
	{
		//cout << "from below" << endl;
		JUMP_ANGLE_STEP = 3;
		JUMP_HEIGHT = 12;
		return;
	}
	else if (y == ny - 1)
	{
		//cout << "from above" << endl;
		JUMP_ANGLE_STEP = 5;
		JUMP_HEIGHT = 4;
		return;
	}
	else if (y == ny + 2)
	{
		//cout << "from very below" << endl;
		JUMP_ANGLE_STEP = 3;
		JUMP_HEIGHT = 16;
		return;
	}
	else if (y == ny - 2)
	{
		//cout << "from very above" << endl;
		JUMP_ANGLE_STEP = 5;
		JUMP_HEIGHT = 4;
		return;
	}
}
