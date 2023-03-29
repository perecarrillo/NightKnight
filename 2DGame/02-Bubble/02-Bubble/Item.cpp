#include "Item.h"
#include <iostream>

using namespace std;



Item::Item(int x, int y, float appear, float disappear)
{
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.5;
	movement = { { MOVE_LEFT,1 } };
	WIDTH = 16;
	HEIGHT = 16;
	HEIGHT_OFFSET = 0;
	animationsUsed = { MOVE_LEFT};
	animationLength = 1;
	disappearTime = disappear;
	appearTime = appear;
	time = 0;
}

void Item::update(int deltaTime)
{
	time += deltaTime;
	float y = initialPosition.y + 0.2 * sin((time * 3.141592f) / 500);
	int size = map->getTileSize();
	setPosition(glm::vec2(initialPosition.x*size-6.f, y*size));
}

void Item::render() {
	if (time/1000 > appearTime) {
		if (time/1000 < disappearTime - 10) sprite->render();
		else if (time/1000 < disappearTime && int(time) % 400 < 200) sprite->render();
	}
}


bool Item::shouldCheckCollision() {
	return time / 1000 > appearTime && time / 1000 < disappearTime;
}