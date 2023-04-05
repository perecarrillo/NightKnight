#include "Chest.h"
#include <iostream>
#include <algorithm>

using namespace std;

Chest::Chest(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	WIDTH = 14;
	WIDTH_OFFSET = 1;
	HEIGHT = 9;
	HEIGHT_OFFSET = 18;
	animationsUsed = { LOCKED, UNLOCKING, UNLOCKED, OPENING_FULL , OPENED_FULL, OPENING_EMPTY, OPENED_EMPTY, ENTERING };
	animationLength = 18;
	animationSpeed = 8;
	spriteSize = glm::vec2(16, 32);
	open = false;
	unlock = false;
	opened = false;
	playerIn = false;
}

void Chest::init(string textureFile, const glm::ivec2 & tileMapPos, ShaderProgram & shaderProgram, TileMap *map)
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
	for (int i = 0; i < numberOfAnimations - 1; ++i) {
		sprite->setAnimationSpeed(animationsUsed[i], animationSpeed);
		for (int j = 0; j < 6; ++j) {
			sprite->addKeyframe(animationsUsed[i], glm::vec2(disp*i, float(j) / animationLength));
		}
	}
	// The player entering chest animation is hardcoded because it's way longer. It needs to be the last one on the file
	sprite->setAnimationSpeed(ENTERING, animationSpeed);
	for (int j = 0; j < 18; ++j) {
		sprite->addKeyframe(ENTERING, glm::vec2(disp*(numberOfAnimations-1), float(j) / animationLength));
	}

	sprite->changeAnimation(*min_element(animationsUsed.begin(), animationsUsed.end()));
	tileMapDispl = tileMapPos;

	if (movement.size() > 0)
		actualMovement = { 0, movement[0].second };
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + int(position.x)), float(tileMapDispl.y + int(position.y))));

}

void Chest::update(int deltaTime, bool finalLevel)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (sprite->animation() == LOCKED && unlock) {
		sprite->changeAnimation(UNLOCKING);
	}
	else if (sprite->animation() == UNLOCKED && open) {
		if (finalLevel) sprite->changeAnimation(OPENING_FULL);
		else sprite->changeAnimation(OPENING_EMPTY);
	}
	else if (sprite->animation() == UNLOCKING && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(UNLOCKED);
	}
	else if (sprite->animation() == OPENING_FULL && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(OPENED_FULL);
	}
	else if (sprite->animation() == OPENING_EMPTY && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(OPENED_EMPTY);
		opened = true;
	}
	// if the level is finished add some extra time before changing
	if (opened) {
		if (sprite->animation() != ENTERING)
			sprite->changeAnimation(ENTERING);
		if (sprite->getKeyFrame() >= animationLength - 1) {
			playerIn = true;
			if (sprite->animation() != UNLOCKED)
				sprite->changeAnimation(UNLOCKED);
		}
	}
}


void Chest::openChest() {
	open = true;
}

void Chest::unlockChest() {
	unlock = true;
}

bool Chest::isOpened() {
	return (opened); // the chest is open after the animation finishes and passes and extra time
	//return opened;
}

bool Chest::playerHasEntered()
{
	return (playerIn || sprite->animation() == OPENED_FULL);
}

glm::vec2 Chest::getPosition()
{
	return glm::vec2(position.x, position.y + HEIGHT_OFFSET + HEIGHT);
}
