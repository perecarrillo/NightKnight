#include "Key.h"
#include <iostream>

using namespace std;

Key::Key() {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(20, 7);
	position = initialPosition;
	speed = 0.5;
	movement = {{ STOP, 50 }};
	WIDTH = 7;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
}

Key::Key(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	position = initialPosition;
	speed = 0;
	movement = { { STOP, 50 } };
	WIDTH = 7;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
	WIDTH_OFFSET = 0;
	animationLength = 1;
	numberOfAnimations = 1;
}