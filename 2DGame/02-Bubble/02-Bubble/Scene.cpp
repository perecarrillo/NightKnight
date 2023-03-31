#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26
#define TIME 120
#define FREEZETIME 5
#define NUM_LAST_LEVEL 6


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
	player->init("images/Player.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(player->getInitialPosition() * map->getTileSize()));
	player->setTileMap(map);

	// Nombre de rajoles del nivell
	getline(fin, line);
	sstream.str(line);
	sstream >> numRajoles;

	// Cofre
	getline(fin, line);
	sstream.str(line);
	sstream >> posCofre.x >> posCofre.y;
	chest = new Chest(posCofre.x, posCofre.y);
	chest->init("images/Chest.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	chest->setPosition(glm::vec2(chest->getInitialPosition() * map->getTileSize()));
	chest->setTileMap(map);

	// Clau
	getline(fin, line);
	sstream.str(line);
	sstream >> posClau.x >> posClau.y;
	key = new Item(posClau.x, posClau.y, 0, TIME);
	key->init("images/Key.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	key->setPosition(glm::vec2(key->getInitialPosition() * map->getTileSize()));
	key->setTileMap(map);


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
	stopwatch->init("images/StopWatch.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	stopwatch->setPosition(glm::vec2(stopwatch->getInitialPosition() * map->getTileSize()));
	stopwatch->setTileMap(map);
	
	// Gemma
	getline(fin, line);
	sstream.str(line);
	sstream >> x >> y;
	getline(fin, line);
	sstream.str(line);
	sstream >> appearTime >> disappearTime;
	gem = new Item(x, y, appearTime, disappearTime);
	gem->init("images/Gem.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	gem->setPosition(glm::vec2(gem->getInitialPosition() * map->getTileSize()));
	gem->setTileMap(map);

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
		movingPlatforms[i] = new MovingSlab(firstX, firstY, lastX, lastY, map->getTileSize());
		movingPlatforms[i]->init("images/MovingSlab.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		movingPlatforms[i]->setTileMap(map);
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
			enemies[i]->init("images/Skeleton.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		}
		else if (i - numSkeletons < numRates) {
			enemies[i] = new Rata(posX, posY);
			enemies[i]->init("images/Rata.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		}
		else if (i - numSkeletons - numRates < numVampires) {
			enemies[i] = new Vampire(posX, posY);
			enemies[i]->init("images/vampire.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		}
		enemies[i]->setPosition(glm::vec2(enemies[i]->getInitialPosition() * map->getTileSize()));
		enemies[i]->setTileMap(map);
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
	win = false;
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
			checkCollisions();
			key->update(deltaTime);
			if (!takenStopwatch) stopwatch->update(deltaTime);
			if (!takenGem) gem->update(deltaTime);
			if (map->numRajolesPressed() + slabsPainted >= numRajoles) allPressed = true;
		}
	}
	chest->update(deltaTime, numLevel == NUM_LAST_LEVEL);
	if (chest->isOpened()) finishLevel();
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
			iniFreezeTime = currentTime;
		}
	}
	if (!takenGem && gem->shouldCheckCollision()) {
		glm::ivec2 enemyMin = gem->getBoundingBoxMin();
		glm::ivec2 enemyMax = gem->getBoundingBoxMax();
		if ((playerMin.x < enemyMax.x && enemyMin.x < playerMax.x) && (playerMin.y < enemyMax.y && enemyMin.y < playerMax.y)) {
			takenGem = true;
		}
	}
}


void Scene::render(bool playing)
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
	else
		texProgram.setUniform1f("opacity", 0.3);
	map2->render();
	map->render();

	if (allPressed && !unlockChest) key->render();
	chest->render();
	if(!takenStopwatch) stopwatch->render();
	if (!takenGem) gem->render();
	for (MovingSlab * ms : movingPlatforms) {
		ms->render();
	}
	for (Entity * enemy : enemies) {
		enemy->render();
	}
	if (!chest->isOpened()) {
		cout << "rendering player" << endl;
		player->render();
	}

	// Print time
	string textTime = std::to_string(TIME - int(currentTime / 1000));
	if (TIME - currentTime / 1000 < 10)
	{
		textTime = std::to_string(TIME - currentTime / 1000);
		textTime = textTime.substr(0, 3);
		if (textTime[0] == '-') textTime = "0.0";
	}
	// Print shadow 
	text.render(textTime, glm::vec2(455 + 3, 100 + 3), 40, glm::vec4(0, 0, 0, 1));
	// Print number
	if (playing) {
		if (currentTime < 1000*(TIME - 10)) text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(1, 1, 1, 1));
		else if (int(currentTime) % 1000 < 500) text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(1, 0, 0, 1));
		else text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(1, 1, 1, 1));
	}
	else {
		if (currentTime < 1000 * (TIME - 10)) text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(0.3, 0.3, 0.3, 1));
		else if (int(currentTime) % 1000 < 500) text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(0.3, 0, 0, 1));
		else text.render(textTime, glm::vec2(455, 100), 40, glm::vec4(0.3, 0.3, 0.3, 1));
	}

	// Print num level
	string textLevel = "Stage " + std::to_string(numLevel);
	text.render(textLevel, glm::vec2(730 + 3, 100 + 3), 40, glm::vec4(0, 0, 0, 1));
	if (playing) text.render(textLevel, glm::vec2(730, 100), 40, glm::vec4(1, 1, 1, 1));
	else text.render(textLevel, glm::vec2(730, 100), 40, glm::vec4(0.3f, 0.3f, 0.3f, 1));

	texProgram.use();

	printHearts();

}

bool Scene::isGameOver()
{
	return (player->getNumHearts() <= 0 || currentTime > TIME*1000);
}

bool Scene::isWin()
{
	return win && chest->playerHasEntered();
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
		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(20.f + i*12.f, 18.f, 0.f));
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
	return player->getNumCoins();
}

void Scene::setNumCoins(int num)
{
	player->setNumCoins(num);
}

void Scene::togglePlayerInmunity()
{
	player->toggleInmunity();
}

void Scene::finishLevel()
{
	win = true;
}

glm::vec2 Scene::getPosPlayer()
{
	return player->getPosition();
}

glm::vec2 Scene::getChestPosition()
{
	return chest->getPosition();
}



