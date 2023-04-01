#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26

enum State
{
	PLAYING, MENU, CREDITS, LOSE, PAUSE, LEVELS
};

Menu::Menu()
{
	menu = MENU;
}

Menu::~Menu()
{
}

void Menu::init()
{
	initShaders();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH / (3.5) - 1), float(SCREEN_HEIGHT / (3.5) - 1), 0.f);
	currentTime = 0.0f;


	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(192.f, 156.f) };

	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	//geom[1] = glm::vec2(256.f, 208.f);
	glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(256.f, 208.f) };

	//texQuad[1] = TexturedQuad::createTexturedQuad(geom2, texCoords, simpleProgram);

	// Load textures
	texs[0].loadFromFile("images/menu.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	texs[1].loadFromFile("images/gameover.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);

	texs[2].loadFromFile("images/pause.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);

	texs[3].loadFromFile("images/levels2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);

	levelSelector = new LevelSelector();
	levelSelector->init();

}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Menu::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(9.f + 32.f, 8.f + 26.f, 0.f)); //centering the menu in the viewport
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform1f("opacity", 1);
	// it will render one of the different menus
	if (menu == MENU) texQuad[0]->render(texs[0]); 
	else if (menu == LOSE) texQuad[0]->render(texs[1]);
	else if (menu == PAUSE)  texQuad[0]->render(texs[2]);
	else if (menu == LEVELS) {
		texQuad[0]->render(texs[3]);
		levelSelector->render();
	}
}


void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

void Menu::setImage(int x) {
	menu = x; //change the menu between LOSE, MENU, CREDITS...
}

void Menu::setLevelFocus(int x)
{
	levelSelector->setLevelFocus(x);
}

int Menu::getLevelFocus()
{
	return levelSelector->getLevelFocus();
}

