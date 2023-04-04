#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#include <GL/glew.h>
#include <GL/glut.h>


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26
#define TIME 120
#define FREEZETIME 5
#define ESCUT_TIME 5
#define NUM_LAST_LEVEL 6
#define COINS_ANIMATION_TIME 3000
#define COINS_PER_SECOND 33


Scene::Scene()
{
	map = NULL;
	player = NULL;
	numLevel = 1;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}

void Scene::loadScene() {
	string levelFile = "scenes/Scene" + std::to_string(numLevel) + ".txt";
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	// Posici� jugador
	getline(fin, line);
	sstream.str(line);
	sstream >> posJugador.x >> posJugador.y;
	player = new Player(posJugador.x, posJugador.y);
	player->init("images/Player.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// Nombre de rajoles del nivell
	getline(fin, line);
	sstream.str(line);
	sstream >> numRajoles;

	// Cofre
	getline(fin, line);
	sstream.str(line);
	sstream >> posCofre.x >> posCofre.y;
	chest = new Chest(posCofre.x, posCofre.y);
	chest->init("images/Chest.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// Clau
	getline(fin, line);
	sstream.str(line);
	sstream >> posClau.x >> posClau.y;
	key = new Item(posClau.x, posClau.y, 0, TIME);
	key->init("images/Key.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);


	// Stopwatch
	getline(fin, line);
	sstream.str(line);
	int x, y;
	sstream >> x >> y;
	getline(fin, line);
	sstream.str(line);
	int appearTime, disappearTime;
	sstream >> appearTime >> disappearTime;
	stopwatch = new Item(x, y, appearTime, disappearTime);
	stopwatch->init("images/StopWatch.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	
	// Gemma
	getline(fin, line);
	sstream.str(line);
	sstream >> x >> y;
	getline(fin, line);
	sstream.str(line);
	sstream >> appearTime >> disappearTime;
	gem = new Item(x, y, appearTime, disappearTime);
	gem->init("images/Gem.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// Heart
	getline(fin, line);
	sstream.str(line);
	sstream >> x >> y;
	getline(fin, line);
	sstream.str(line);
	sstream >> appearTime >> disappearTime;
	heart = new Item(x, y, appearTime, disappearTime);
	heart->init("images/heart.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// Escut
	getline(fin, line);
	sstream.str(line);
	sstream >> x >> y;
	getline(fin, line);
	sstream.str(line);
	sstream >> appearTime >> disappearTime;
	escut = new Item(x, y, appearTime, disappearTime);
	escut->init("images/escut.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// Nombre rajoles que es mouen
	getline(fin, line);
	sstream.str(line);
	sstream >> numMovingPlatforms;
	// Posicions inicials i finals de cada rajola
	movingPlatforms = vector<MovingSlab *> (numMovingPlatforms);
	for (int i = 0; i < numMovingPlatforms; ++i) {
		int firstX, firstY;
		int lastX, lastY;
		getline(fin, line);
		sstream.str(line);
		sstream >> firstX >> firstY;
		getline(fin, line);
		sstream.str(line);
		sstream >> lastX >> lastY;
		movingPlatforms[i] = new MovingSlab(i, firstX, firstY, lastX, lastY, map->getTileSize());
		movingPlatforms[i]->init("images/MovingSlab.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	}


	// Nombre d'enemics de cada tipus
	getline(fin, line);
	sstream.str(line);
	sstream >> numSkeletons >> numRates >> numVampires;

	int numEnemies = numSkeletons + numRates + numVampires;
	// Posici� inicial de cada enemics i inicialitzaci� d'aquests
	enemies = vector<Entity *> (numEnemies);
	for (int i = 0; i < numEnemies; ++i) {
		int posX, posY;
		getline(fin, line);
		sstream.str(line);
		sstream >> posX >> posY;
		if (i < numSkeletons) {
			enemies[i] = new Skeleton(posX, posY);
			enemies[i]->init("images/Skeleton.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		}
		else if (i - numSkeletons < numRates) {
			enemies[i] = new Rata(posX, posY);
			enemies[i]->init("images/Rata.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		}
		else if (i - numSkeletons - numRates < numVampires) {
			enemies[i] = new Vampire(posX, posY);
			enemies[i]->init("images/vampire.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		}
	}

	

	fin.close();
}

void Scene::init()
{
	allPressed = false;
	unlockChest = false;
	openChest = false;
	takenStopwatch = false;
	takenGem = false;
	takenHeart = false;
	takenEscut = false;
	win = false;
	animationCoinsFinished = false;
	iniAnimationCoins = false;

	iniFreezeTime = 0.f;
	iniEscutTime = 0.f;

	initialCoins = 0;
	finalCoins = 0;
	coins = 0;
	animationTime = 0;
	iniCoinsTime = 0;

	initShaders();

	string fileMap = "levels/level" + std::to_string(numLevel) + ".txt";
	string fileBackground = "levels/level" + std::to_string(numLevel) + "Background.txt";
	//map = TileMap::createTileMap("levels/level2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map2 = TileMap::createTileMap("levels/level2Background.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap(fileMap, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap(fileBackground, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	loadScene();

	projection = glm::ortho(0.f, float(SCREEN_WIDTH/(3.5) - 1), float(SCREEN_HEIGHT/(3.5) - 1), 0.f);
	currentTime = 0.0f;


	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(12.f, 12.f) };


	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	
	// Load textures
	texs[0].loadFromFile("images/heart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	texs[1].loadFromFile("images/lostHeart.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);

	texs[2].loadFromFile("images/coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);


	// Select which font you want to use
	if (!text.init("fonts/Supply Center.ttf"))
		cout << "Could not load font!!!" << endl;

}

void Scene::update(int deltaTime)
{
	if (!openChest) {
		currentTime += deltaTime; //només actualitzem el temps de l'escena si no s'ha acabat el nivell
		int slabsPainted = 0;
		for (MovingSlab * ms : movingPlatforms) {
			ms->update(deltaTime, player);
			if (ms->isPainted())
				++slabsPainted;
		}
		player->update(deltaTime);
		if (!takenStopwatch || iniFreezeTime + FREEZETIME * 1000 < currentTime) {
			for (Entity * enemy : enemies) {
				enemy->update(deltaTime);
			}
			key->update(deltaTime);
			if (!takenStopwatch) stopwatch->update(deltaTime);
			if (!takenGem) gem->update(deltaTime);
			if (!takenHeart) heart->update(deltaTime);
			if (!takenEscut) escut->update(deltaTime);
			if (map->numRajolesPressed() + slabsPainted >= numRajoles) allPressed = true;
		}
		if (!takenEscut || iniEscutTime + ESCUT_TIME * 1000 < currentTime) checkCollisions();
	}
	else {
		//player->moveToChest(deltaTime, chest->getPosition());
	}
	if (!iniAnimationCoins) chest->update(deltaTime, numLevel == NUM_LAST_LEVEL);
	if (chest->isOpened()) finishLevel();
	if (chest->playerHasEntered() && !iniAnimationCoins) {
		startAnimationCoins();
	}
	if (iniAnimationCoins) {
 		animationTime += deltaTime;
		if (animationTime < COINS_ANIMATION_TIME) {
			float m = animationTime/(COINS_ANIMATION_TIME);
			coins = initialCoins + int(m * (finalCoins - initialCoins));
			if (int(animationTime)%150 < 20) SoundController::instance().play(COIN);
			currentTime = iniCoinsTime + (TIME*1000 - iniCoinsTime) * m;
		}
		else {
			coins = finalCoins;
			iniAnimationCoins = false;
			animationCoinsFinished = true;
			currentTime = TIME*1000;
		}
	}
}

void Scene::checkCollisions()
{
	//Mirarem les colisions Player - Enemy i Player/Enemy - Boxes
	glm::ivec2 playerMin = player->getBoundingBoxMin();
	glm::ivec2 playerMax = player->getBoundingBoxMax();
	for (Entity *enemy : enemies) {
		glm::ivec2 enemyMin = enemy->getBoundingBoxMin();
		glm::ivec2 enemyMax = enemy->getBoundingBoxMax();
		if (!player->isInmune() && (playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			//cout << "Collision"<<endl;
			player->loseHeart();
		}
	}
	if (allPressed) {
		glm::ivec2 enemyMin = key->getBoundingBoxMin();
		glm::ivec2 enemyMax = key->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			unlockChest = true;
			chest->unlockChest();
		}
	}
	if (unlockChest) {
		glm::ivec2 enemyMin = chest->getBoundingBoxMin();
		glm::ivec2 enemyMax = chest->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			openChest = true;
			chest->openChest();
		}
	}
	if (!takenStopwatch && stopwatch->shouldCheckCollision()) {
		glm::ivec2 enemyMin = stopwatch->getBoundingBoxMin();
		glm::ivec2 enemyMax = stopwatch->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			takenStopwatch = true;
			SoundController::instance().play(CLOCK);
			iniFreezeTime = currentTime;
		}
	}
	if (!takenGem && gem->shouldCheckCollision()) {
		glm::ivec2 enemyMin = gem->getBoundingBoxMin();
		glm::ivec2 enemyMax = gem->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			takenGem = true;
			coins += 500; 
			SoundController::instance().play(COIN);
		}
	}
	int numHearts = player->getNumHearts();
	if (!takenHeart && heart->shouldCheckCollision() && numHearts < 5) {
		glm::ivec2 enemyMin = heart->getBoundingBoxMin();
		glm::ivec2 enemyMax = heart->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			takenHeart = true;
			player->setNumHearts(++numHearts);
			SoundController::instance().play(HEART);
		}
	}
	if (!takenEscut && escut->shouldCheckCollision()) {
		glm::ivec2 enemyMin = escut->getBoundingBoxMin();
		glm::ivec2 enemyMax = escut->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			takenEscut = true;
			//SoundController::instance().play(CLOCK);
			iniEscutTime = currentTime;
		}
	}
}


void Scene::render(bool playing, bool changingLevel)
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	if (playing)
		texProgram.setUniform1f("opacity", 1);
	else if (changingLevel)
		texProgram.setUniform1f("opacity", 0);
	else texProgram.setUniform1f("opacity", 0.3);
	map2->render();
	map->render();

	if (allPressed && !unlockChest) key->render();
	chest->render();
	if(!takenStopwatch) stopwatch->render();
	if (!takenGem) gem->render();
	if (!takenHeart && player->getNumHearts() < 5) heart->render();
	if (!takenEscut) escut->render();
	for (MovingSlab * ms : movingPlatforms) {
		ms->render();
	}
	for (Entity * enemy : enemies) {
		enemy->render();
	}
	if (!chest->isOpened()) {
		//cout << "rendering player" << endl;
		player->render();
	}

	// Print time
	string textTime = std::to_string(TIME - int(currentTime / 1000));
	if (TIME - currentTime / 1000 < 10 && !iniAnimationCoins && !animationCoinsFinished)
	{
		textTime = std::to_string(TIME - currentTime / 1000);
		textTime = textTime.substr(0, 3);
		if (textTime[0] == '-') textTime = "0.0";
	}
	// Print shadow 
	text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH) + 3, 0.125*glutGet(GLUT_WINDOW_HEIGHT) + 3), 40, glm::vec4(0, 0, 0, 1));
	// Print number
	if (playing) {
		if (currentTime < 1000*(TIME - 10) || iniAnimationCoins || animationCoinsFinished) text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(1, 1, 1, 1));
		else if (int(currentTime) % 1000 < 500) text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(1, 0, 0, 1));
		else text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(1, 1, 1, 1));
	}
	else {
		if (currentTime < 1000 * (TIME - 10) || iniAnimationCoins || animationCoinsFinished) text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(0.3, 0.3, 0.3, 1));
		else if (int(currentTime) % 1000 < 500) text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(0.3, 0, 0, 1));
		else text.render(textTime, glm::vec2(0.48*glutGet(GLUT_WINDOW_WIDTH), 0.125*glutGet(GLUT_WINDOW_HEIGHT)), 40, glm::vec4(0.3, 0.3, 0.3, 1));
	}

	// Print num level
	string textLevel = "Stage " + std::to_string(numLevel);
	text.render(textLevel, glm::vec2(0.8*glutGet(GLUT_WINDOW_WIDTH) + 3, 0.12*glutGet(GLUT_WINDOW_HEIGHT) + 3), 30, glm::vec4(0, 0, 0, 1));
	if (playing) text.render(textLevel, glm::vec2(0.8*glutGet(GLUT_WINDOW_WIDTH), 0.12*glutGet(GLUT_WINDOW_HEIGHT)), 30, glm::vec4(1, 1, 1, 1));
	else text.render(textLevel, glm::vec2(0.8*glutGet(GLUT_WINDOW_WIDTH), 0.12*glutGet(GLUT_WINDOW_HEIGHT)), 30, glm::vec4(0.3f, 0.3f, 0.3f, 1));

	// Print coins
	string textCoins;
	if (coins > 9999) textCoins = "9999"; // 9999 is the maximum value of the coins
	else textCoins = std::to_string(coins);
	text.render(textCoins, glm::vec2(0.34*glutGet(GLUT_WINDOW_WIDTH) + 3, 0.12*glutGet(GLUT_WINDOW_HEIGHT) + 3), 30, glm::vec4(0, 0, 0, 1));
	if (playing) text.render(textCoins, glm::vec2(0.34*glutGet(GLUT_WINDOW_WIDTH), 0.12*glutGet(GLUT_WINDOW_HEIGHT)), 30, glm::vec4(1, 1, 1, 1));
	else text.render(textCoins, glm::vec2(0.34*glutGet(GLUT_WINDOW_WIDTH), 0.12*glutGet(GLUT_WINDOW_HEIGHT)), 30, glm::vec4(0.3f, 0.3f, 0.3f, 1));

	texProgram.use();

	printHearts();

	// Print coin image
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(20.f + 61.f, 18.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[2]);


}

bool Scene::isGameOver()
{
	return (player->getNumHearts() <= 0 || currentTime > TIME*1000);
}

bool Scene::isWin()
{
	//return win && chest->playerHasEntered() && animationCoinsFinished;
	return animationCoinsFinished;

}

void Scene::changeLevel(int n)
{
	numLevel = n;
	init();
}

int Scene::getNumLevel()
{
	return numLevel;
}


void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::printHearts() {
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	int num = player->getNumHearts();
	glm::mat4 modelview;
	for (int i = 0; i < 5; ++i) {
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(20.f + i*10.f, 18.f, 0.f));
		texProgram.setUniformMatrix4f("modelview", modelview);
		if (i < num) texQuad[0]->render(texs[0]);
		else  texQuad[0]->render(texs[1]);
	}
}

void Scene::makeKeyAppear() {
	allPressed = true;
}

int Scene::getNumHearts() {
	return player->getNumHearts();
}

void Scene::setNumHearts(int num) {
	player->setNumHearts(num);
}

int Scene::getNumCoins()
{
	//return player->getNumCoins();
	return coins;
}

void Scene::setNumCoins(int num)
{
	player->setNumCoins(num);
	coins = num;
}

void Scene::togglePlayerInmunity()
{
	player->toggleInmunity();
}

void Scene::finishLevel()
{
	win = true;
}

void Scene::startAnimationCoins()
{
	iniCoinsTime = currentTime;
	iniAnimationCoins = true;
	initialCoins = coins;
	animationTime = 0;
	finalCoins = COINS_PER_SECOND * (TIME - int(currentTime / 1000)) + coins;
}

glm::vec2 Scene::getPosPlayer()
{
	return player->getPosition();
}

glm::vec2 Scene::getChestPosition()
{
	return chest->getPosition();
}



