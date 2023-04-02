#ifndef RATA_INCLUDE
#define RATA_INCLUDE

#include "Entity.h"

class Rata : public Entity
{

public:
 	//Rata();
	Rata(int x, int y);
	Rata(int x, int y, vector<pair<int, int>> movement);
	
	void update(int deltaTime);

	glm::ivec2 Rata::getBoundingBoxMin();
	glm::ivec2 Rata::getBoundingBoxMax();

private:
	int WIDTH_OFFSET_LEFT;
	bool jumpingLeft, jumpingRight;
	int jumpAngle, startY;
	float jumpingSpeed;
	int JUMP_ANGLE_STEP;
	int JUMP_HEIGHT;
	int waiting;

	bool useAI;
	bool movingLeft;
	bool jumping;


	void setUp();
	void nextAnimation();
	void updateAI(int deltaTime);
	void updatePredefinedMovements(int deltaTime);
	void calculateJumpParameters(pair<int, int> jumpTo);
};


#endif