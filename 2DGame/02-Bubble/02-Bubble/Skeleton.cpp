#include "Skeleton.h"


#define SKELETON_WIDTH 10
#define SKELETON_HEIGHT 14
#define SKELETON_SPEED 0.5
#define INIT_SKELETON_X_TILES 20
#define INIT_SKELETON_Y_TILES 7

enum SkeletonAnims
{
	MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT
};

Skeleton::Skeleton()
{
	initialPosition = glm::ivec2(INIT_SKELETON_X_TILES, INIT_SKELETON_Y_TILES);
	actualMovement = {0, movement[0].second};
}

void Skeleton::init(string textureFile, int numberOfAnimations, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	float disp = 1.0f / numberOfAnimations;

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(disp, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numberOfAnimations);
	for (int i = 0; i < numberOfAnimations; ++i) {
		sprite->setAnimationSpeed(SkeletonAnims(i), 8);
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 0.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 1 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 2 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 3 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 4 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 5 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 6 / 8.f));
		sprite->addKeyframe(SkeletonAnims(i), glm::vec2(disp*i, 7 / 8.f));
	}

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
		
	}
	else if (movement[actualMovement.first].first == RIGHT)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		positionFloat.x += SKELETON_SPEED;
		
	}
	else if (movement[actualMovement.first].first == STOP)
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
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
