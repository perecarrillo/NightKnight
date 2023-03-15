#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#include <algorithm>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 28
#define FALL_STEP 2
#define PLAYER_WIDTH 12
#define PLAYER_WIDTH_OFFSET 2
#define PLAYER_HEIGHT 16
#define PLAYER_SIZE 16
#define PLAYER_SPEED 1
#define COYOTE_TIME 4
#define INIT_PLAYER_X_TILES 11
#define INIT_PLAYER_Y_TILES 11


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DYING
};

Player::Player() {
	initialPosition = glm::ivec2(INIT_PLAYER_X_TILES, INIT_PLAYER_Y_TILES);
	hearts = 5;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/provabomba.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(DYING, 8);
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 1 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 2 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 3 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 4 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 5 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 6 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 7 / 8.f));

	sprite->setAnimationSpeed(STAND_LEFT, 1);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 1);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 1 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 2 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 3 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 4 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 5 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 6 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 7 / 8.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 1 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 2 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 3 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 4 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 5 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 6 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 7 / 8.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= PLAYER_SPEED;
		if (map->collisionMoveLeft(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT)))
		{
			posPlayer.x += PLAYER_SPEED;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += PLAYER_SPEED;
		if (map->collisionMoveRight(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT)))
		{
			posPlayer.x -= PLAYER_SPEED;
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

	if (bJumping)
	{
		++coyoteTime;
		jumpAngle += JUMP_ANGLE_STEP;
		if (jumpAngle >= 180)
		{
			bJumping = false;
			//posPlayer.y = startY + jumpLost;
		}
		else
		{

			posPlayer.y = int((startY + jumpLost) - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (map->collisionMoveUp(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y)) {
				jumpAngle = 90;
				jumpLost += posPlayer.y - int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			}
			if (jumpAngle > 90) {
				bJumping = !map->collisionMoveDown(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y);
				if (!bJumping) coyoteTime = 0;
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		bool collisionDown = map->collisionMoveDown(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y);
		if (!collisionDown) ++coyoteTime;
		if (collisionDown || coyoteTime < COYOTE_TIME)
		{
			if (Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				coyoteTime = COYOTE_TIME;
				bJumping = true;
				jumpAngle = 0;
				jumpLost = 0;
				startY = posPlayer.y;
			}
		}
	}
	map->collisionRajola(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y);

	if (map->collisionSpikes(posPlayer + glm::ivec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y)) {
		if (sprite->animation() != DYING) {
			sprite->changeAnimation(DYING);
			if (hearts > 0) --hearts;
		}
		else if (sprite->getKeyFrame() > 6) {
			sprite->changeAnimation(STAND_LEFT);
			setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
		}
	}
	else sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getInitialPosition()
{
	return initialPosition;
}

int Player::getNumHearts() {
	return hearts;
}
