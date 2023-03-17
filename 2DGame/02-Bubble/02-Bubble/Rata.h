#ifndef RATA_INCLUDE
#define RATA_INCLUDE

#include "Entity.h"

class Rata : public Entity
{
	enum Movements
	{
		LEFT, RIGHT, STOP, JUMP
	};

	enum Animations
	{
		MOVE_LEFT, MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, START_JUMP_LEFT, START_JUMP_RIGHT, JUMP_LEFT, JUMP_RIGHT
	};



public:
 	//Rata();
	Rata(int x, int y);
	
	void update(int deltaTime);

	glm::ivec2 Rata::getBoundingBoxMin();
	glm::ivec2 Rata::getBoundingBoxMax();

private:
	int WIDTH_OFFSET_LEFT;
};


#endif