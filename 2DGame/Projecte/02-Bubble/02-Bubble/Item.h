#ifndef	ITEM_INCLUDE
#define ITEM_INCLUDE

#include "Entity.h"

class Item : public Entity
{

public:
	Item(int x, int y, float appear, float disappear);

	void update(int deltaTime);

	void render();

	bool shouldCheckCollision();

	bool firstAppear();


private:
	float time;
	float disappearTime;
	float appearTime;
	bool firstRender, secondRender;
};


#endif