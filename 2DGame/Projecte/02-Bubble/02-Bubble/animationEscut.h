#ifndef ANIMATIONESCUT_INCLUDE
#define ANIMATIONESCUT_INCLUDE

#include "Entity.h"

class AnimationEscut : public Entity
{

public:
	//AnimationEscut();
	AnimationEscut(int x, int y);
	AnimationEscut(int x, int y, vector<pair<int, int>> movement);

	void update(int deltaTime);

	void setAnimationTime(float time);

	

private:



};


#endif