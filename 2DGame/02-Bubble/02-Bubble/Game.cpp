#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

enum State
{
	PLAYING, MENU, CREDITS, LOSE, PAUSE, LEVELS
};

void Game::init()
{
	circle = new CircleAnimation(SCREEN_WIDTH, SCREEN_HEIGHT);
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	menu.init();
	state = PLAYING;
	changingLevel = false;
	levelExpanding = false;
}

bool Game::update(int deltaTime)
{
	if (state == PLAYING && !changingLevel) scene.update(deltaTime);	
	if (scene.isGameOver()) {
		state = LOSE;
		menu.setImage(LOSE);
	}

	int level = scene.getNumLevel();

	if (scene.isWin() && level < 6 && !changingLevel) {
		changingLevel = true;
		circle->changeRadius(400.f);
		glm::vec2 posPlayer = scene.getPosPlayer();
		circle->changeCenter(posPlayer.x, posPlayer.y);
		circle->changeToShrink();
	}

	if (changingLevel && circle->finishShrink() && !levelExpanding) {
		levelExpanding = true;
		changeLevel(++level);
		circle->changeRadius(0.f);
		glm::vec2 posPlayer = scene.getPosPlayer();
		circle->changeCenter(posPlayer.x, posPlayer.y);
		circle->changeToExpand();
	}

	if (changingLevel) circle->update(deltaTime);

	if (circle->finishExpand()) {
		changingLevel = false;
		levelExpanding = false;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render(state == PLAYING);
	if (state != PLAYING) menu.render();
	if (changingLevel) circle->render();
	
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if ((key == 77 || key == 109) && (state == LOSE || state == PAUSE)) {// key M/m
		scene.init();
		state = MENU;
		menu.setImage(MENU);
	}
	if ((key == 82 || key == 114) && (state == LOSE || state == PAUSE)) { // key R/r
		scene.init();
		state = PLAYING;
	}
	if ((key == 80 || key == 112) && state == PLAYING) { // key P/p
		state = PAUSE;
		menu.setImage(PAUSE);
	}
	if ((key == 67 || key == 99) && state == PAUSE) { // key C/c
		state = PLAYING;
	}
	if ((key == 71 || key == 103) && state == PLAYING) { // key G/g
		scene.togglePlayerInmunity();
	}
	if ((key == 75 || key == 107) && state == PLAYING) { // key K/k
		scene.makeKeyAppear();
	}
	if ((key == 108 || key == 76)) { // key L/l
		state = LEVELS;
		menu.setImage(LEVELS);
	}
	if ((key == 97 || key == 49) && state == MENU) { // key 1
		if (scene.getNumLevel() != 1 || scene.isGameOver()) scene.changeLevel(1);
		state = PLAYING;
	}
	if ((key == 98 || key == 50) && state == MENU) { // key 2
		if (scene.getNumLevel() != 2 || scene.isGameOver()) scene.changeLevel(2);
		state = PLAYING;
	}
	if ((key == 99 || key == 51) && state == MENU) { // key 3
		if (scene.getNumLevel() != 3 || scene.isGameOver()) scene.changeLevel(3);
		state = PLAYING;
	}
	if ((key == 100 || key == 52) && state == MENU) { // key 4
		if (scene.getNumLevel() != 4 || scene.isGameOver()) scene.changeLevel(4);
		state = PLAYING;
	}
	if ((key == 101 || key == 53) && state == MENU) { // key 5
		if (scene.getNumLevel() != 5 || scene.isGameOver()) scene.changeLevel(5);
		state = PLAYING;
	}
	if ((key == 102 || key == 54) && state == MENU) { // key 6
		if (scene.getNumLevel() != 6 || scene.isGameOver()) scene.changeLevel(6);
		state = PLAYING;
	}
	if (key == 13 && state == LEVELS) { // enter
		cout << "enter" << endl;
		int num = menu.getLevelFocus() + 1;
		if (scene.getNumLevel() != num || scene.isGameOver()) scene.changeLevel(num);
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
	if (key == GLUT_KEY_RIGHT && state == LEVELS) {
		int num = menu.getLevelFocus();
		if (num % 3 < 2) menu.setLevelFocus(++num);
	}
	if (key == GLUT_KEY_LEFT && state == LEVELS) {
		int num = menu.getLevelFocus();
		if (num % 3 > 0) menu.setLevelFocus(--num);
	}
	if (key == GLUT_KEY_DOWN && state == LEVELS) {
		int num = menu.getLevelFocus();
		if (num < 3) menu.setLevelFocus(num + 3);
	}
	if (key == GLUT_KEY_UP && state == LEVELS) {
		int num = menu.getLevelFocus();
		if (num > 2) menu.setLevelFocus(num - 3);
	}

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

void Game::changeLevel(int level) {
	int hearts = scene.getNumHearts();
	int coins = scene.getNumCoins();
	scene.changeLevel(level);
	scene.setNumHearts(hearts);
	scene.setNumCoins(coins);
}






