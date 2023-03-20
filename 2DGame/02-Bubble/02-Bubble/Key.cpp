#include "Key.h"
#include <iostream>

using namespace std;

Key::Key(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0.5;
	WIDTH = 6;
	HEIGHT = 16;
	HEIGHT_OFFSET = 0;
	animationsUsed = { MOVE_UP };
	animationLength = 6;
	movement = { {MOVE_UP, 50} };
}