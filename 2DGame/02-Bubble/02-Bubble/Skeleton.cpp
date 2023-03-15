#include "Skeleton.h"


#define SKELETON_WIDTH 10
#define SKELETON_HEIGHT 14
#define SKELETON_SPEED 0.5
#define INIT_SKELETON_X_TILES 20
#define INIT_SKELETON_Y_TILES 6

enum SkeletonAnims
{
	MOVE_LEFT, MOVE_RIGHT
};

Skeleton::Skeleton()
{
	initialPosition = glm::ivec2(INIT_SKELETON_X_TILES, INIT_SKELETON_Y_TILES);
	actualMovement = {0, movement[0].second};
}

void Skeleton::init(const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/skeleton.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25f, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0, 1 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0, 2 / 8.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0, 3 / 8.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 1 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 2 / 8.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 3 / 8.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(positionFloat.x)), float(tileMapDispl.y + int(positionFloat.y))));

}

void Skeleton::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == LEFT)
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		positionFloat.x -= SKELETON_SPEED;
		if (map->collisionMoveLeft(positionFloat, glm::ivec2(SKELETON_WIDTH, SKELETON_HEIGHT)))
		{
			positionFloat.x += SKELETON_SPEED;
			//sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if (movement[actualMovement.first].first == RIGHT)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		positionFloat.x += SKELETON_SPEED;
		if (map->collisionMoveRight(positionFloat, glm::ivec2(SKELETON_WIDTH, SKELETON_HEIGHT)))
		{
			positionFloat.x -= SKELETON_SPEED;
			//sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else if (movement[actualMovement.first].first == STOP)
	{
		//sprite->changeAnimation(STAND);
	}

	--actualMovement.second;
	if (actualMovement.second == -1) {
		++actualMovement.first;
		if (actualMovement.first == movement.size()) actualMovement.first = 0;
		actualMovement.second = movement[actualMovement.first].second;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(positionFloat.x)), float(tileMapDispl.y + int(positionFloat.y))));
}

void Skeleton::render()
{
	sprite->render();
}

void Skeleton::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Skeleton::setPosition(const glm::vec2 & pos)
{
	positionFloat = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(positionFloat.x)), float(tileMapDispl.y + int(positionFloat.y))));

}

glm::ivec2 Skeleton::getInitialPosition()
{
	return initialPosition;
}
