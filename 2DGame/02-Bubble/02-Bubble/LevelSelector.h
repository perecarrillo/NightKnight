#ifndef _LEVELSELECTOR_INCLUDE
#define _LEVELSELECTOR_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Text.h"
#include <vector>


// LevelSelector contains all the entities of our game.
// It is responsible for updating and render them.


class LevelSelector
{

public:
	LevelSelector();
	~LevelSelector();

	void init();
	void update(int deltaTime);
	void render();
	void setLevelFocus(int x);
	int getLevelFocus();


private:
	void initShaders();
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Quad *quad;
	Texture texs[7];
	TexturedQuad *texQuad[2];
	
	float width, height;

	int levelFocus;
};


#endif 

