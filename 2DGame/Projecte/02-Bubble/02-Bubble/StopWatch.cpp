#include "StopWatch.h"
#include <iostream>

using namespace std;

StopWatch::StopWatch(int x, int y) {
	//cout << initialPosition.x<<' '<< initialPosition.y << endl;
	initialPosition = glm::ivec2(x, y);
	speed = 0.5;
	WIDTH = 14;
	HEIGHT = 14;
	HEIGHT_OFFSET = 0;
	animationsUsed = { MOVE_UP };
	animationLength = 1;
	movement = { { MOVE_UP, 50 } };
}