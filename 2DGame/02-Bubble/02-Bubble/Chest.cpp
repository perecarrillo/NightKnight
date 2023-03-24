#include "Chest.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	movement = { { LOCKED,150 },{ UNLOCKING, 150 } , { UNLOCKED, 150 },{ OPENING, 150 }, {OPENED, 150} };
	WIDTH = 16;
	HEIGHT = 16;
	HEIGHT_OFFSET = 0;
	animationsUsed = { LOCKED, UNLOCKING, UNLOCKED, OPENING, OPENED };
	animationLength = 6;
	open = false;
	unlock = false;
	opened = false;
	speed = 0.1;
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
		sprite->changeAnimation(OPENING);
	}
	else if (sprite->animation() == UNLOCKING && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(UNLOCKED);
	}
	else if (sprite->animation() == OPENING && sprite->getKeyFrame() > 3) {
		sprite->changeAnimation(OPENED);
		opened = true;
	}
	// if the level is finished add some extra time before changing
	if (opened) {
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