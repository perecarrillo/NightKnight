#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "MainMenu.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26
#define ROWS 3
#define COLS 1
#define NUM_OPTIONS 4
#define MENU_WIDTH 32*8
#define MENU_HEIGHT 26*8


MainMenu::MainMenu()
{
	optionFocus = 0;
	width = 100.f;
	height = 20.f;
}

MainMenu::~MainMenu()
{
}

void MainMenu::init()
{
	initShaders();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH / (3.5) - 1), float(SCREEN_HEIGHT / (3.5) - 1), 0.f);
	currentTime = 0.0f;


	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(width, height) };

	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(32*8,26*8) };

	texQuad[1] = TexturedQuad::createTexturedQuad(geom2, texCoords, texProgram);


	// Load textures
	texs[0].loadFromFile("images/textPlay.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	texs[1].loadFromFile("images/textLevels.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);

	texs[2].loadFromFile("images/textInstructions.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);

	texs[3].loadFromFile("images/textCredits.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);

	texs[4].loadFromFile("images/MenuBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].setMagFilter(GL_NEAREST);

	/*texs[5].loadFromFile("images/mapa3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].setMagFilter(GL_NEAREST);

	texs[6].loadFromFile("images/marc.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[6].setMagFilter(GL_NEAREST);*/



}

void MainMenu::update(int deltaTime)
{
	currentTime += deltaTime;
}

void MainMenu::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(9.f, 8.f , 0.f)); //centering the Bakcground
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	// Render Background of the menu
	texQuad[1]->render(texs[4]);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(9.f + 32.f, 8.f + 26.f, 0.f)); //centering the MainMenu in the viewport
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform1f("opacity", 1);


	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	for (int i = 0; i < NUM_OPTIONS; ++i) {
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(8.f + 32*8 / 2.f, (26*8 / 2.f) -2 + 1.3f * height * (i), 0.f));
		if (optionFocus == i) modelview = glm::scale(modelview, glm::vec3(1.2f, 1.2f, 0));
		modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[i]);
	}


	// Put the marc
	//modelview = glm::mat4(1.0f);
	//modelview = glm::translate(modelview, glm::vec3((9 + 32) + width / (COLS + 1) + (width / (COLS + 1) + width)*(optionFocus % COLS), (9 + 26) + 1.4f*height + (height / (ROWS + 1) + height) * (optionFocus / COLS), 0.f));
	//modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
	////modelview = glm::scale(modelview, glm::vec3(1.3f, 1.3f, 0));
	//modelview = glm::translate(modelview, glm::vec3(-width*0.2f, -height*0.2f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
	//texProgram.setUniformMatrix4f("modelview", modelview);
	//texQuad[1]->render(texs[6]);

	// Expand the level focused
	/*if (!expanding) {
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3((9 + 32) + width / (COLS + 1) + (width / (COLS + 1) + width)*(optionFocus % COLS), (9 + 26) + 1.35f*height + (height / (ROWS + 1) + height) * (optionFocus / COLS), 0.f));
		modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
		modelview = glm::scale(modelview, glm::vec3(1.1f, 1.1f, 0));
		modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[optionFocus]);
	}*/

	//else {
	//	float diffTime = currentTime - iniAnimationTime;
	//	float m = diffTime / ANIMATION_TIME;
	//	texProgram.setUniform1f("opacity", 0.3 + 0.7*(1 - m));
	//	modelview = glm::mat4(1.0f);
	//	modelview = glm::translate(modelview, glm::vec3(glm::vec2(posFi*m + posIni * (1 - m)), 0.f));
	//	//modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
	//	modelview = glm::scale(modelview, glm::vec3(m * 26 / 6.f + 1.f, m * 548 / 156.f + 1.f, 0));
	//	modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
	//	texProgram.setUniformMatrix4f("modelview", modelview);
	//	texQuad[0]->render(texs[optionFocus]);
	//}
}


void MainMenu::initShaders()
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

void MainMenu::setOptionFocus(int x) {
	optionFocus = x;
}

int MainMenu::getOptionFocus() {
	return optionFocus;
}


