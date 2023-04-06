#include "AnimationEscut.h"
#include <iostream>

using namespace std;

#define WAIT_TIME 100


AnimationEscut::AnimationEscut(int x, int y)
{
	initialPosition = glm::vec2(x, y);
	animationLength = 12;
	animationsUsed = { MOVE_LEFT };
	speed = 0.2;
}

AnimationEscut::AnimationEscut(int x, int y, vector<pair<int, int>> movement)
{
	initialPosition = glm::ivec2(x, y);
}


void AnimationEscut::update(int deltaTime)
{
	time += deltaTime;
	sprite->update(deltaTime);
}

void AnimationEscut::setAnimationTime(float time) {
	sprite->setAnimationSpeed(0, 11/time);
}

