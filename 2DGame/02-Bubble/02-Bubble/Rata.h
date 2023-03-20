#ifndef RATA_INCLUDE
#define RATA_INCLUDE

#include "Entity.h"

class Rata : public Entity
{

public:
 	//Rata();
	Rata(int x, int y);
	
	void update(int deltaTime);

	glm::ivec2 Rata::getBoundingBoxMin();
	glm::ivec2 Rata::getBoundingBoxMax();

private:
	int WIDTH_OFFSET_LEFT;
	bool jumpingLeft, jumpingRight;
	int jumpAngle, startY;
	float jumpingSpeed;

	void nextAnimation();
};


#endif