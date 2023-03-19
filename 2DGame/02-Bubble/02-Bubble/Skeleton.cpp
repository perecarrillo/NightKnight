#include "Skeleton.h"
#include <iostream>

using namespace std;

Skeleton::Skeleton(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.5;
	movement = { { MOVE_LEFT,150 },{ STAND_LEFT, 150 },{ MOVE_RIGHT, 150 },{ STAND_RIGHT, 50 } };
	WIDTH = 10;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
	animationsUsed = { MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT };
	animationLength = 8;
}