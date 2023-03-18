#include "Entity.h"


Entity::Entity()
{
}


Entity::Entity(int x, int y)
{
	initialPosition = glm::vec2(x, y);
}

void Entity::init(string textureFile, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{

	spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	float disp = 1.0f / numberOfAnimations;

	sprite = Sprite::createSprite(spriteSize, glm::vec2(disp, 1.0f / animationLength), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numberOfAnimations);
	for (int i = 0; i < numberOfAnimations; ++i) {
		sprite->setAnimationSpeed(Animations(i), animationLength);
		for (int j = 0; j < animationLength; ++j) {
			sprite->addKeyframe(Animations(i), glm::vec2(disp*i, float(j) / animationLength));
		}
	}

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;

	actualMovement = { 0, movement[0].second };
	position = initialPosition;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));

}

void Entity::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (movement[actualMovement.first].first == LEFT)
	{
		if (sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		position.x -= speed;
		if (map->collisionMoveLeft(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x += speed;
			sprite->changeAnimation(STAND_LEFT);
		}

	}
	else if (movement[actualMovement.first].first == RIGHT)
	{
		if (sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		position.x += speed;
		if (map->collisionMoveRight(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT)))
		{
			position.x -= speed;
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

	position.y += FALL_STEP;
	map->collisionMoveDown(position + glm::vec2(WIDTH_OFFSET, HEIGHT_OFFSET), glm::ivec2(WIDTH, HEIGHT), &position.y, HEIGHT_OFFSET);


	--actualMovement.second;
	if (actualMovement.second == -1) {
		++actualMovement.first;
		if (actualMovement.first == movement.size()) actualMovement.first = 0;
		actualMovement.second = movement[actualMovement.first].second;
	}
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
