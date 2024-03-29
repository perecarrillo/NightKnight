#include "Skeleton.h"
#include <iostream>

using namespace std;

Skeleton::Skeleton(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	speed = 0.5;
	movement = { { MOVE_LEFT,-1 }, { STAND_RIGHT, 50 }, { MOVE_RIGHT, -1 }, { STAND_LEFT, 50 } };
	WIDTH = 10;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT };
	animationLength = 8;
}