#include "Chest.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	WIDTH = 16;
	HEIGHT = 18;
	HEIGHT_OFFSET = 1;
	animationsUsed = { LOCKED, UNLOCKING, UNLOCKED, OPENING_EMPTY , OPENED_EMPTY, OPENING_FULL, OPENED_FULL, ENTERING };
	animationLength = 8;
	spriteSize = glm::vec2(16, 32);
	open = false;
	unlock = false;
	opened = false;
	finishingTime = 0;
}

void Chest::update(int deltaTime)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (sprite->animation() == LOCKED && unlock) {
		sprite->changeAnimation(UNLOCKING);
	}
	else if (sprite->animation() == UNLOCKED && open) {
		sprite->changeAnimation(OPENING_EMPTY);
	}
	else if (sprite->animation() == UNLOCKING && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(UNLOCKED);
	}
	else if (sprite->animation() == OPENING_EMPTY && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(OPENED_EMPTY);
		opened = true;
	}
	// if the level is finished add some extra time before changing
	if (opened) {
		if (sprite->animation() != ENTERING)
			sprite->changeAnimation(ENTERING);

		finishingTime += deltaTime;
	}
}


void Chest::openChest() {
	open = true;
}

void Chest::unlockChest() {
	unlock = true;
}

bool Chest::isOpened() {
	return (opened && finishingTime > 1000); // the chest it's open after the animation finishes and passes and extra time
	//return opened;
}