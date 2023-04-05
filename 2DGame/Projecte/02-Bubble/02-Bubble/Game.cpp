#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "SoundController.h"

#define READY_TIME 2000

enum State
{
	PLAYING, MENU, CREDITS, LOSE, PAUSE, LEVELS, INSTRUCTIONS, WIN, READY
};


void Game::init()
{
	circle = new CircleAnimation(SCREEN_WIDTH, SCREEN_HEIGHT);
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	menu.init();
	state = MENU;
	menu.setImage(MENU);
	changingLevel = false;
	levelExpanding = false;
	animationLevelSelected = false;
	showReady = false;
	winGame = false;
	readyIniTime = 0.f;
	currentTime = 0.f;
	// Select which font you want to use
	if (!text.init("fonts/Supply Center.ttf"))
		cout << "Could not load font!!!" << endl;
	
	SoundController::instance().init();
	SoundController::instance().play(MENUU);
}

bool Game::update(int deltaTime)
{
	currentTime += deltaTime;
	if (state == PLAYING && !changingLevel) scene.update(deltaTime);	
	if (scene.isGameOver()) {
		state = LOSE;
		menu.setImage(LOSE);
	}

	int level = scene.getNumLevel();

	if (scene.isWin() && level < 6 && !changingLevel) {
		SoundController::instance().play(WINN);
		changingLevel = true;
		circle->changeRadius(400.f);
		glm::vec2 posPlayer = scene.getPosPlayer();
		circle->changeCenter(posPlayer.x, posPlayer.y);
		circle->changeToShrink();
	}

	if (scene.isWin() && level == 6 && !winGame) {
		winGame = true;
		int score = scene.getNumCoins();
		menu.setFinalScore(score);
		state = WIN;
		menu.setImage(WIN);
		SoundController::instance().setAllSoundsPaused(true);
		SoundController::instance().play(WINFINAL);
	}

	// Circle Animation
	if (changingLevel && circle->finishShrink() && !levelExpanding) {
		levelExpanding = true;
		menu.setLevelFocus(level);
		changeLevel(++level);
		circle->changeRadius(0.f);
		glm::vec2 posPlayer = scene.getPosPlayer();
		circle->changeCenter(posPlayer.x, posPlayer.y);
		circle->changeToExpand();
	}

	if (changingLevel) circle->update(deltaTime);

	if (changingLevel && circle->finishExpand()) {
		changingLevel = false;
		levelExpanding = false;
		state = READY;
		readyIniTime = currentTime;
	}
	// End Circle Animation

	menu.update(deltaTime);

	// If we have selected a level from the level menu and the animation of expanding is finished the menu will change to 'Ready?'
	if (animationLevelSelected && menu.animationLevelSelectedFinished()) {
		animationLevelSelected = false;
		if (showReady) {
			state = READY;
			readyIniTime = currentTime;
			showReady = false;
		}
		else state = PLAYING;
		SoundController::instance().setAllSoundsPaused(true);
		SoundController::instance().play(MAINTHEME);
	}

	// If it has passed more than READY_TIME since the message appeared, the level will start
	if (state == READY && ((currentTime - readyIniTime) > READY_TIME)) {
		state = PLAYING;
		SoundController::instance().setAllSoundsPaused(true);
		SoundController::instance().play(READYY);
		SoundController::instance().play(MAINTHEME);
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render((state == PLAYING && !changingLevel) || (changingLevel && !levelExpanding), animationLevelSelected);
	if (state != PLAYING && state != READY && state) menu.render();
	if (changingLevel) circle->render();
	if (state == READY) {
		// Print Ready?
		string textLevel = "Ready?";
		text.render(textLevel, glm::vec2(0.43*glutGet(GLUT_WINDOW_WIDTH) + 3, 0.6*glutGet(GLUT_WINDOW_HEIGHT) + 3), 40, glm::vec4(0, 0, 0, 1));
		text.render(textLevel, glm::vec2(0.43*glutGet(GLUT_WINDOW_WIDTH), 0.6*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(1, 1, 1, 1));
	}
}

void Game::keyPressed(int key)
{

	if (key == 77 || key == 109 || key == 27) {// key M/m or Esc
		//scene.init();
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
		SoundController::instance().setAllSoundsPaused(true);
		SoundController::instance().play(MENUU);
	}
	if ((key == 67 || key == 99) && state == PAUSE) { // key C/c
		state = PLAYING;
		SoundController::instance().setAllSoundsPaused(false);
		SoundController::instance().play(MAINTHEME);
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
		SoundController::instance().setAllSoundsPaused(true);
		SoundController::instance().play(MENUU);
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
	if (key == 13) { // enter
		if (state == LEVELS && !animationLevelSelected) { 
			menu.expandLevelSelector();
			animationLevelSelected = true;
			int num = menu.getLevelFocus() + 1;
			if (scene.getNumLevel() != num || scene.isGameOver()) {
				scene.changeLevel(num);
				showReady = true;
			}
		}
		else if (state == MENU) {
			int option = menu.getOptionFocus();
			if (option == 0) { // play
				scene.changeLevel(1);
				state = READY;
				readyIniTime = currentTime;
				showReady = false;
			}
			else if (option == 1) {
				state = LEVELS;
				menu.setImage(LEVELS);
			}
			else if (option == 2) {
				state = INSTRUCTIONS;
				menu.setImage(INSTRUCTIONS);
			}
			else if (option == 3) {
				state = CREDITS;
				menu.setImage(CREDITS);
			}
				
		}
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
	if (key == GLUT_KEY_RIGHT && state == LEVELS && !animationLevelSelected) {
		int num = menu.getLevelFocus();
		if (num % 3 < 2) menu.setLevelFocus(++num);
		SoundController::instance().play(ARROW);
	}
	if (key == GLUT_KEY_LEFT && state == LEVELS && !animationLevelSelected) {
		int num = menu.getLevelFocus();
		if (num % 3 > 0) menu.setLevelFocus(--num);
		SoundController::instance().play(ARROW);

	}
	if (key == GLUT_KEY_DOWN) {
		if (state == LEVELS && !animationLevelSelected) {
			int num = menu.getLevelFocus();
			if (num < 3) menu.setLevelFocus(num + 3);
			SoundController::instance().play(ARROW);
		}
		else if (state == MENU) {
			int num = menu.getOptionFocus();
			if (num < 3) menu.setOptionFocus(++num);
			SoundController::instance().play(ARROW);
		}

	}
	if (key == GLUT_KEY_UP) {
		if (state == LEVELS && !animationLevelSelected) {
			int num = menu.getLevelFocus();
			if (num > 2) menu.setLevelFocus(num - 3);
			SoundController::instance().play(ARROW);
		}
		else if (state == MENU) {
			int num = menu.getOptionFocus();
			if (num > 0) menu.setOptionFocus(--num);
			SoundController::instance().play(ARROW);
		}
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
	winGame = false;
	int hearts = scene.getNumHearts();
	int coins = scene.getNumCoins();
	scene.changeLevel(level);
	scene.setNumHearts(hearts);
	scene.setNumCoins(coins);
}






