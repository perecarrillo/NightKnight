#include "Key.h"
#include <iostream>

using namespace std;

Key::Key(int x, int y) {
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0;
	movement = { { STAND_LEFT, 50 } };
	WIDTH = 7;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
	WIDTH_OFFSET = 0;
	animationLength = 1;
	animationsUsed = { STAND_LEFT };
}