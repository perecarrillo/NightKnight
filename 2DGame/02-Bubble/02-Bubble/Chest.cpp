#include "Chest.h"
#include <iostream>

using namespace std;

Chest::Chest(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.5;
	movement = {{ MOVE_LEFT,150 },{ STAND_LEFT, 150 }};
	WIDTH = 16;
	HEIGHT = 16;
	HEIGHT_OFFSET = 0;
	animationsUsed = { MOVE_LEFT, STAND_LEFT };
	animationLength = 8;
}