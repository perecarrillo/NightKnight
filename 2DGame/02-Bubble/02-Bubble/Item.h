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


private:
	float time;
	float disappearTime;
	float appearTime;
	
};


#endif