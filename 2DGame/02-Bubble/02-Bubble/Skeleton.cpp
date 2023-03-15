#include "Skeleton.h"
#include <iostream>

using namespace std;

Skeleton::Skeleton() {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(20, 7);
	position = initialPosition;
	speed = 0.5;
	movement = { { LEFT,150 },{ STOP, 50 },{ RIGHT, 150 },{ STOP, 50 } };
	WIDTH = 10;
	HEIGHT = 13;
	HEIGHT_OFFSET = 3;
}