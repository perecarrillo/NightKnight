#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelSelector.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26
#define ROWS 2
#define COLS 3
#define NUM_LEVELS 6
#define MENU_WIDTH 32*6
#define MENU_HEIGHT 26*6
#define ANIMATION_TIME 350


LevelSelector::LevelSelector()
{
	levelFocus = 0;
	width = MENU_WIDTH / (COLS + 1);
	height = MENU_HEIGHT / (COLS + 1);
}

LevelSelector::~LevelSelector()
{
}

void LevelSelector::init()
{
	initShaders();

	expanding = false;
	iniAnimationTime = 0.f;

	projection = glm::ortho(0.f, float(SCREEN_WIDTH / (3.5) - 1), float(SCREEN_HEIGHT / (3.5) - 1), 0.f);
	currentTime = 0.0f;


	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(width, height) };

	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	glm::vec2 geom2[2] = { glm::vec2(0.f, 0.f), glm::vec2(width*1.4f, height*1.4f) };

	texQuad[1] = TexturedQuad::createTexturedQuad(geom2, texCoords, texProgram);


	// Load textures
	texs[0].loadFromFile("images/mapa4.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	texs[1].loadFromFile("images/mapa5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);

	texs[2].loadFromFile("images/mapa6.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);

	texs[3].loadFromFile("images/mapa2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);

	texs[4].loadFromFile("images/mapa1.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].setMagFilter(GL_NEAREST);

	texs[5].loadFromFile("images/mapa3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].setMagFilter(GL_NEAREST);

	texs[6].loadFromFile("images/marc.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[6].setMagFilter(GL_NEAREST);



}

void LevelSelector::update(int deltaTime)
{
	currentTime += deltaTime;
	if ((currentTime - iniAnimationTime) > ANIMATION_TIME) expanding = false;
}

void LevelSelector::render()
{
	glm::mat4 modelview;


	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(9.f + 32.f, 8.f + 26.f, 0.f)); //centering the LevelSelector in the viewport
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform1f("opacity", 1);

	
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	for (int i = 0; i < NUM_LEVELS; ++i) {
		if (i != levelFocus) {
			int row = i / COLS;
			int col = i % COLS;
			modelview = glm::translate(glm::mat4(1.0f), glm::vec3(
				// WIDTH
				(9 + 32) // offset menu screen width
				+ width / (COLS + 1) // offset between two consecutives maps. This offset will always be applied, although the var col is 0
				+ (width / (COLS + 1) + width)*col, // offset between maps. It's the sum of the offset between maps and the width of a map.

				// HEIGHT
				(9 + 26) // offset menu screen height
				+ 1.35f*height // random offset to put all the maps near the bottom of the menu screen
				+ (height / (ROWS + 1) + height) * row, 0.f)); // vertical offset between maps plus the height of a map
			texProgram.setUniformMatrix4f("modelview", modelview);
			texQuad[0]->render(texs[i]);
		}
	}


	// Put the marc
	//modelview = glm::mat4(1.0f);
	//modelview = glm::translate(modelview, glm::vec3((9 + 32) + width / (COLS + 1) + (width / (COLS + 1) + width)*(levelFocus % COLS), (9 + 26) + 1.4f*height + (height / (ROWS + 1) + height) * (levelFocus / COLS), 0.f));
	//modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
	////modelview = glm::scale(modelview, glm::vec3(1.3f, 1.3f, 0));
	//modelview = glm::translate(modelview, glm::vec3(-width*0.2f, -height*0.2f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
	//texProgram.setUniformMatrix4f("modelview", modelview);
	//texQuad[1]->render(texs[6]);

	// Expand the level focused
	if (!expanding) {
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3((9 + 32) + width / (COLS + 1) + (width / (COLS + 1) + width)*(levelFocus % COLS), (9 + 26) + 1.35f*height + (height / (ROWS + 1) + height) * (levelFocus / COLS), 0.f));
		modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
		modelview = glm::scale(modelview, glm::vec3(1.1f, 1.1f, 0));
		modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[levelFocus]);
	}

	else {
		float diffTime = currentTime - iniAnimationTime;
		float m = diffTime / ANIMATION_TIME;
		modelview = glm::mat4(1.0f);
		modelview = glm::translate(modelview, glm::vec3(glm::vec2(posFi*m + posIni * (1-m)), 0.f));
		//modelview = glm::translate(modelview, glm::vec3(width / 2.f, height / 2.f, 0.f));
		modelview = glm::scale(modelview, glm::vec3(m*26/6.f + 1.f, m*548/156.f + 1.f, 0));
		modelview = glm::translate(modelview, glm::vec3(-width / 2.f, -height / 2.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[levelFocus]);
	}
}


void LevelSelector::initShaders()
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

void LevelSelector::setLevelFocus(int x) {
	levelFocus = x;
}

int LevelSelector::getLevelFocus() {
	return levelFocus;
}

bool LevelSelector::animationLevelSelectedFinished()
{
	return (currentTime - iniAnimationTime) > ANIMATION_TIME;
}

void LevelSelector::expandLevelSelector()
{
	iniAnimationTime = currentTime;
	expanding = true;
	posIni = glm::vec2((9 + 32) + width/2.f + width / (COLS + 1) + (width / (COLS + 1) + width)*(levelFocus % COLS), (9 + 26) + 1.35f*height + height/2.f + (height / (ROWS + 1) + height) * (levelFocus / COLS));
	posFi = glm::vec2(8 + 32*8/2.f, 8 + 2*8 + 26*8/2.f);
}
