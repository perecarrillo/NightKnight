#include "Chest.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	movement = { { LOCKED,150 },{ UNLOCKED, 150 },{ OPENING, 150 }, {OPENED, 150} };
	WIDTH = 16;
	HEIGHT = 16;
	HEIGHT_OFFSET = 0;
	animationsUsed = { LOCKED, UNLOCKED, OPENING, OPENED };
	animationLength = 6;
	open = false;
	unlock = false;
	opened = false;
}

void Chest::update(int deltaTime)
{
	sprite->update(deltaTime);
	time += deltaTime;
	if (sprite->animation() == LOCKED) cout << "hola";
	if (sprite->animation() == LOCKED && unlock) {
		cout << "unlock" << endl;
		sprite->changeAnimation(UNLOCKED);
	}
	else if (sprite->animation() == UNLOCKED && open) {
		cout << "open" << endl;
		sprite->changeAnimation(OPENING);
	}
	else if (sprite->animation() == OPENING && sprite->getKeyFrame() > 4) {
		cout << "opened" << endl;
		sprite->changeAnimation(OPENED);
		opened = true;
	}
}


void Chest::openChest() {
	open = true;
}

void Chest::unlockChest() {
	unlock = true;
}

bool Chest::isOpened() {
	return opened;
}