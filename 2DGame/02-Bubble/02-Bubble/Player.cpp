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
//#define INIT_PLAYER_X_TILES 11
//#define INIT_PLAYER_Y_TILES 11


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT, DYING 
};


Player::Player() {
	initialPosition = glm::ivec2(0, 0);
	hearts = 5;
	inmune = false;
	show = true;
	time = 0;
}

Player::Player(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	hearts = 5;
	inmune = false;
	show = true;
	time = 0;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/Player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(DYING, 10);
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 1 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 2 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 3 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 4 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 5 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 6 / 8.f));
	sprite->addKeyframe(DYING, glm::vec2(0.5f, 7 / 8.f));

	sprite->setAnimationSpeed(STAND_LEFT, 1);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT, 1);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 16);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 1 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 2 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 3 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 4 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 5 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 6 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 7 / 8.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 16);
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
	time += deltaTime;
	if (time%500 < 250) show = false;
	else show = true;
	if (!inmune || time - timeInmune > 3000) inmune = false;
	if (sprite->animation() != DYING) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			if (sprite->animation() != MOVE_LEFT)
				sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= PLAYER_SPEED;
			if (map->collisionMoveLeft(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT)))
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
			if (map->collisionMoveRight(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT)))
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
				if (map->collisionMoveUp(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y)) {
					jumpAngle = 90;
					jumpLost += posPlayer.y - int(startY - JUMP_HEIGHT * sin(3.14159f * jumpAngle / 180.f));
				}
				if (jumpAngle > 90) {
					bJumping = !map->collisionMoveDown(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y, 0);
					if (!bJumping) coyoteTime = 0;
				}
			}
		}
		else
		{
			posPlayer.y += FALL_STEP;
			bool collisionDown = map->collisionMoveDown(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT), &posPlayer.y, 0);
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
		map->collisionRajola(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT));
		
		if (map->collisionSpikes(posPlayer + glm::vec2(PLAYER_WIDTH_OFFSET, 0), glm::ivec2(PLAYER_WIDTH, PLAYER_HEIGHT)) && !inmune) {
			sprite->changeAnimation(DYING);
			if (hearts > 0) --hearts;
			posPlayer.y += 4;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
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

glm::ivec2 Player::getBoundingBoxMin()
{
	return glm::ivec2(posPlayer.x + PLAYER_WIDTH_OFFSET, posPlayer.y);
}

glm::ivec2 Player::getBoundingBoxMax()
{
	return glm::ivec2(posPlayer.x + PLAYER_WIDTH_OFFSET + PLAYER_WIDTH, posPlayer.y + PLAYER_HEIGHT);
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
	return inmune;
}

int Player::getNumHearts() {
	return hearts;
}
