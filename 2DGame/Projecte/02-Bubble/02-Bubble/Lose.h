#ifndef _LOSE_INCLUDE
#define _LOSE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Text.h"
#include <vector>


// Lose contains all the entities of our game.
// It is responsible for updating and render them.


class Lose
{

public:
	Lose();
	~Lose();

	void init();
	void update(int deltaTime);
	void render();


private:
	void initShaders();
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Quad *quad;
	Texture texs[2];
	TexturedQuad *texQuad[3];

	Text text;

};


#endif // _LOSE_INCLUDE

