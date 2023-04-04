#ifndef _MAINMENU_INCLUDE
#define _MAINMENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Text.h"
#include <vector>


// MainMenu contains all the entities of our game.
// It is responsible for updating and render them.


class MainMenu
{

public:
	MainMenu();
	~MainMenu();

	void init();
	void update(int deltaTime);
	void render();
	void setOptionFocus(int x);
	int getOptionFocus();

	bool animationLevelSelectedFinished();


private:
	void initShaders();
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Quad *quad;
	Texture texs[7];
	TexturedQuad *texQuad[2];

	float width, height;

	int optionFocus;
};


#endif 

