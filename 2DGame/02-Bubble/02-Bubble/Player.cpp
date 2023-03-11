#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 28
#define FALL_STEP 2
#define PLAYER_SIZE 16
#define PLAYER_SPEED 1
#define INIT_PLAYER_X_TILES 27
#define INIT_PLAYER_Y_TILES 18


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/minero_vertical.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
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
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= PLAYER_SPEED;
		if(map->collisionMoveLeft(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE)))
		{
			posPlayer.x += PLAYER_SPEED;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += PLAYER_SPEED;
		if(map->collisionMoveRight(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE)))
		{
			posPlayer.x -= PLAYER_SPEED;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if(sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if(sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	
	if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			
			if (map->collisionMoveUp(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE), &posPlayer.y)) {
				if (jumpAngle < 90)
					jumpAngle = 90;
			}
			posPlayer.y = int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
			if (jumpAngle > 90) {
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE), &posPlayer.y);
			}
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}
	if (map->collisionSpikes(posPlayer, glm::ivec2(PLAYER_SIZE, PLAYER_SIZE), &posPlayer.y)) {
		sprite->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
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




