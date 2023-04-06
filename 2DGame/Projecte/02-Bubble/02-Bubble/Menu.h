#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Text.h"
#include <vector>
#include "LevelSelector.h"
#include "MainMenu.h"



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
	void setLevelFocus(int x);
	int getLevelFocus();
	bool animationLevelSelectedFinished();
	void expandLevelSelector();

	void setOptionFocus(int x);

	int getOptionFocus();

	void setFinalScore(int num);


private:
	void initShaders();
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
	glm::mat4 projection;
	LevelSelector *levelSelector;
	MainMenu *mainMenu;

	Quad *quad;
	Texture texs[8];
	TexturedQuad *texQuad[3];

	int menu;

	int finalScore;

	int instructionsPage;

	Text text;
};


#endif // _MENU_INCLUDE

