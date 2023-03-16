#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Rajola.h"
#include "Game.h"

#include <algorithm>



#define Rajola_WIDTH 12
#define Rajola_WIDTH_OFFSET 2
#define Rajola_HEIGHT 16
#define Rajola_SIZE 16

#define INIT_RAJOLA_X_TILES 11
#define INIT_RAJOLA_Y_TILES 11


enum RajolaAnims
{
	PRESSED
};

Rajola::Rajola() {
	initialPosition = glm::ivec2(INIT_RAJOLA_X_TILES, INIT_RAJOLA_Y_TILES);
}

void Rajola::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/rajola.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 8), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(PRESSED, 1);
	sprite->addKeyframe(PRESSED, glm::vec2(0.f, 0.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRajola.x), float(tileMapDispl.y + posRajola.y)));

}


void Rajola::render()
{
	sprite->render();
}

void Rajola::setPosition(const glm::vec2 &pos)
{
	posRajola = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posRajola.x), float(tileMapDispl.y + posRajola.y)));
}

glm::ivec2 Rajola::getInitialPosition()
{
	return initialPosition;
}

void Rajola::changePosIni(const glm::vec2 & pos)
{
	initialPosition = pos;
}

