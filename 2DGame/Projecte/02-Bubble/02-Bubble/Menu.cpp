#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"

#include <GL/glut.h>


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26

enum State
{
	PLAYING, MENU, CREDITS, LOSE, PAUSE, LEVELS, INSTRUCTIONS, WIN
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
	finalScore = 0;

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

	texs[2].loadFromFile("images/Pause2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);

	texs[3].loadFromFile("images/levels2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);

	texs[4].loadFromFile("images/credits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].setMagFilter(GL_NEAREST);

	texs[5].loadFromFile("images/win.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].setMagFilter(GL_NEAREST);

	levelSelector = new LevelSelector();
	levelSelector->init();

	mainMenu = new MainMenu();
	mainMenu->init();

	if (!text.init("fonts/Supply Center.ttf"))
		cout << "Could not load font!!!" << endl;

}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	levelSelector->update(deltaTime);
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
	if (menu == MENU) {
		texQuad[0]->render(texs[0]);
		mainMenu->render();
	}
	else if (menu == LOSE) texQuad[0]->render(texs[1]);
	else if (menu == PAUSE)  texQuad[0]->render(texs[2]);
	else if (menu == LEVELS) {
		texQuad[0]->render(texs[3]);
		levelSelector->render();
	}
	else if (menu == CREDITS) texQuad[0]->render(texs[4]);
	else if (menu == WIN) {
		texQuad[0]->render(texs[5]);
		text.render(std::to_string(finalScore), glm::vec2(0.45*glutGet(GLUT_WINDOW_WIDTH) + 3, 0.63*glutGet(GLUT_WINDOW_HEIGHT) + 3), 40, glm::vec4(0, 0, 0, 1));
		text.render(std::to_string(finalScore), glm::vec2(0.45*glutGet(GLUT_WINDOW_WIDTH), 0.63*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(1, 1, 1, 1));

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

bool Menu::animationLevelSelectedFinished()
{
	return levelSelector->animationLevelSelectedFinished();
}

void Menu::expandLevelSelector()
{
	levelSelector->expandLevelSelector();
}

void Menu::setOptionFocus(int x)
{
	mainMenu->setOptionFocus(x);
}

int Menu::getOptionFocus()
{
	return mainMenu->getOptionFocus();
}

void Menu::setFinalScore(int num) {
	finalScore = num;
}
