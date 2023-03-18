#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Text.h"
#include <vector>


// Menu contains all the entities of our game.
// It is responsible for updating and render them.


class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();
	void setImage(int x);


private:
	void initShaders();
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Quad *quad;
	Texture texs[4];
	TexturedQuad *texQuad[3];

	int menu;

};


#endif // _MENU_INCLUDE

