#include "Entity.h"


Entity::Entity()
{
}
	

void Entity::init(string textureFile, int numberOfAnimations, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram)
{

	spritesheet.loadFromFile(textureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	float disp = 1.0f / numberOfAnimations;

	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(disp, 1 / 8.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(numberOfAnimations);
	for (int i = 0; i < numberOfAnimations; ++i) {
		sprite->setAnimationSpeed(Animations(i), 8);
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 0.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 1 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 2 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 3 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 4 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 5 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 6 / 8.f));
		sprite->addKeyframe(Animations(i), glm::vec2(disp*i, 7 / 8.f));
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
