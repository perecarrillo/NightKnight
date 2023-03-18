#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum State
{
	PLAYING, MENU, CREDITS, LOSE, PAUSE
};

void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	menu.init();
	state = PLAYING;
}

bool Game::update(int deltaTime)
{
	if (state == PLAYING) scene.update(deltaTime);	
	if (scene.isGameOver()) {
		state = LOSE;
		menu.setImage(LOSE);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
	if (state != PLAYING) menu.render();
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if ((key == 77) && (state == LOSE || state == PAUSE)) {// key M
		scene.init();
		state = MENU;
		menu.setImage(MENU);
	}
	if ((key == 82) && (state == LOSE || state == PAUSE)) { // key R
		scene.init();
		state = PLAYING;
	}
	if ((key == 80) && state == PLAYING) { // key P
		state = PAUSE;
		menu.setImage(PAUSE);
	}
	if ((key == 67) && state == PAUSE) { // key C
		state = PLAYING;
	}
	if ((key == 97 || key == 49) && state == MENU) { // key 1
		if (scene.getNumLevel() != 1 || scene.isGameOver()) scene.changeLevel(1);
		state = PLAYING;
	}
	if ((key == 98 || key == 50) && state == MENU) { // key 2
		if (scene.getNumLevel() != 2 || scene.isGameOver())scene.changeLevel(2);
		state = PLAYING;
	}
	if ((key == 99 || key == 51) && state == MENU) { // key 3
		if (scene.getNumLevel() != 3 || scene.isGameOver())scene.changeLevel(3);
		state = PLAYING;
	}

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





