#include "Entity.h"


class MovingSlab : public Entity
{
public:
	MovingSlab(int firstX, int firstY, int lastX, int lastY, int tileSize);
	void update(int deltaTime);

private:
	glm::ivec2 start;
	glm::ivec2 end;
	bool movingLeft;
};

