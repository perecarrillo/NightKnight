#include "Chest.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	WIDTH = 16;
	HEIGHT = 18;
	HEIGHT_OFFSET = 1;
	animationsUsed = { LOCKED, UNLOCKING, UNLOCKED, OPENING_FULL , OPENED_FULL, OPENING_EMPTY, OPENED_EMPTY, ENTERING };
	animationLength = 8;
	spriteSize = glm::vec2(16, 32);
	open = false;
	unlock = false;
	opened = false;
	playerIn = false;
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
			if (sprite->animation() != OPENED_EMPTY)
				sprite->changeAnimation(OPENED_EMPTY);
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
	return (opened); // the chest it's open after the animation finishes and passes and extra time
	//return opened;
}

bool Chest::playerHasEntered()
{
	return playerIn;
}

glm::vec2 Chest::getPosition()
{
	return glm::vec2(position.x + WIDTH_OFFSET + WIDTH/2., position.y + HEIGHT_OFFSET + HEIGHT/2.);
}
