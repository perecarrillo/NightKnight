#include "Entity.h"
#include <algorithm>

#include <iostream>


Entity::Entity()
{
}


Entity::Entity(int x, int y)
{
	initialPosition = glm::vec2(x, y);
}

void Entity::init(string textureFile, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, TileMap *map)
{
	position = glm::vec2(initialPosition * map->getTileSize());
	this->map = map;
	if (animationSpeed == -1) animationSpeed = animationLength;
	spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	int numberOfAnimations = animationsUsed.size();
	float disp = 1.0f / numberOfAnimations;

	sprite = Sprite::createSprite(spriteSize, glm::vec2(disp, 1.0f / animationLength), &spritesheet, &shaderProgram);
	sprite->setNumberOfLastAnimation(*max_element(animationsUsed.begin(), animationsUsed.end()));
	for (int i = 0; i < numberOfAnimations; ++i) {
		sprite->setAnimationSpeed(animationsUsed[i], animationSpeed);
		for (int j = 0; j < animationLength; ++j) {
			sprite->addKeyframe(animationsUsed[i], glm::vec2(disp*i, float(j) / animationLength));
		}
	}

	sprite->changeAnimation(*min_element(animationsUsed.begin(), animationsUsed.end()));
	tileMapDispl = tileMapPos;

	if (movement.size() > 0)
		actualMovement = { 0, movement[0].second };
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));

}

void Entity::update(int deltaTime)
{
	float trash = 0;
	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == MOVE_LEFT)
	{
		position.x -= speed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), false))
		{
			position.x += speed;
			nextAnimation();
		}
		else if (!map->collisionMoveDown(position + glm::vec2(-map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET))
		{
			position.x += speed;
			nextAnimation();
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
			nextAnimation();
		}
		else if (!map->collisionMoveDown(position + glm::vec2(map->getTileSize(), map->getTileSize()) + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &trash, HEIGHT_OFFSET))
		{
			//cout << "collisionDown Right" << endl;
			position.x -= speed;
			nextAnimation();
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

	position.y += FALL_STEP;
	map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);


	if (actualMovement.second == 0) {
		nextAnimation();
	}
	--actualMovement.second;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));
}

void Entity::render()
{

	sprite->render();
}

void Entity::setTileMap(TileMap * tileMap)
{
	map = tileMap;
}

void Entity::setPosition(const glm::vec2 & pos)
{
	position = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));

}

glm::ivec2 Entity::getInitialPosition()
{
	return initialPosition;
}

glm::ivec2 Entity::getSpriteSize()
{
	return spriteSize;
}

//We reduce by 2 px the boundingBox because we can
glm::ivec2 Entity::getBoundingBoxMin()
{
	return glm::ivec2(position.x + WIDTH_OFFSET + 1, position.y + HEIGHT_OFFSET);
}

glm::ivec2 Entity::getBoundingBoxMax()
{
	return glm::ivec2(position.x + WIDTH_OFFSET + WIDTH + WIDTH_OFFSET - 1, position.y + HEIGHT + HEIGHT_OFFSET);
}

void Entity::nextAnimation()
{
	++actualMovement.first;
	if (actualMovement.first == movement.size()) actualMovement.first = 0;
	actualMovement.second = movement[actualMovement.first].second;
}