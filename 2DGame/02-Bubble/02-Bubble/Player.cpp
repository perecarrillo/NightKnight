#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#include <algorithm>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 28
#define COYOTE_TIME 4



Player::Player(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	hearts = 5;
	inmune = false;
	show = true;
	time = 0;
	speed = 0.5;
	WIDTH = 12;
	WIDTH_OFFSET = 2;
	HEIGHT = 16;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, DYING };
	animationLength = 8;
	bJumping = false;
	speed = 1;
	coins = 0;
	isOnPlatform = false;
	invincible = false;

}


void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (time%500 < 250) show = false;
	else show = true;
	if (!inmune || time - timeInmune > 3000) inmune = false;
	if (sprite->animation() != DYING) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			position.x -= speed;
			if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT)))
			{
				position.x += speed;
				if (sprite->animation() != STAND_LEFT)
					sprite->changeAnimation(STAND_LEFT);
			}
			else if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			position.x += speed;
			if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT)))
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

		if (bJumping)
		{
			++coyoteTime;
			jumpAngle += JUMP_ANGLE_STEP;
			if (jumpAngle >= 180)
			{
				bJumping = false;
				//position.y = startY + jumpLost;
			}
			else
			{

				position.y = int((startY + jumpLost) - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				if (map->collisionMoveUp(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET)) {
					jumpAngle = 90;
					jumpLost += position.y - int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				}
				if (jumpAngle > 90) {
					bJumping = !isOnPlatform && !map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT), &position.y, 0);
					if (!bJumping) coyoteTime = 0;
				}
			}
		}
		else
		{
			if (!isOnPlatform)
				position.y += FALL_STEP;

			bool collisionDown = isOnPlatform || map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT), &position.y, 0);
			//bool collisionDown = map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT), &position.y, 0, isOnPlatform);
			if (!collisionDown) ++coyoteTime;
			if (collisionDown || coyoteTime < COYOTE_TIME)
			{
				if (Game::instance().getSpecialKey(GLUT_KEY_UP))
				{
					coyoteTime = COYOTE_TIME;
					bJumping = true;
					jumpAngle = 0;
					jumpLost = 0;
					startY = position.y;
				}
			}
		}
		map->collisionRajola(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT));
		
		if (!invincible && !inmune && map->collisionSpikes(position + glm::vec2(WIDTH_OFFSET, 0), glm::ivec2(WIDTH, HEIGHT))) {
			sprite->changeAnimation(DYING);
			if (hearts > 0) --hearts;
			position.y += 4;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + position.x), float(tileMapDispl.y + position.y)));
	}
	// if the player has died and we are rendering the die animation after rendering the last frame we will restart the player position
	else if (sprite->getKeyFrame() > 6) {
		sprite->changeAnimation(STAND_LEFT);
		setPosition(glm::vec2(initialPosition.x * map->getTileSize(), initialPosition.y * map->getTileSize()));
		inmune = true;
		timeInmune = time;
	}

}

void Player::render()
{	
	if (!inmune || show) sprite->render();
}

void Player::loseHeart()
{
	if (hearts > 0)
		--hearts;
	inmune = true;
	timeInmune = time;
}

bool Player::isInmune()
{
	return invincible || inmune;
}

void Player::setIsOnPlatform(bool b)
{
	isOnPlatform = b;
}

int Player::getWidth()
{
	return WIDTH;
}

int Player::getHeight()
{
	return HEIGHT;
}

void Player::addXMovement(float value)
{
	position.x += value;
}

void Player::addYMovement(float value)
{
	position.y += value;
}

void Player::toggleInmunity()
{
	invincible = !invincible;
}

int Player::getNumHearts() {
	return hearts;
}

void Player::setNumHearts(int num) {
	hearts = num;
}

int Player::getNumCoins() {
	return coins;
}

void Player::setNumCoins(int num) {
	coins = num;
}

glm::vec2 Player::getPosition()
{
	return glm::vec2(position.x + WIDTH_OFFSET, position.y);
}

void Player::setPosition(glm::vec2 pos)
{
	position.x = pos.x - WIDTH_OFFSET;
	position.y = pos.y;
}

