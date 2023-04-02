#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include <vector>
#include "Skeleton.h"
#include "Rata.h"
#include "Vampire.h"
#include "Key.h"
#include "Chest.h"
#include "MovingSlab.h"
#include "StopWatch.h"
#include "Item.h"

#include <iostream>
#include <fstream>
#include <sstream>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void loadScene();

	void init();
	void update(int deltaTime);
	void render(bool playing, bool changingLevel);
	bool isGameOver();
	bool isWin();
	void changeLevel(int n);
	int getNumLevel();
	void makeKeyAppear();

	int getNumHearts();

	void setNumHearts(int num);

	int getNumCoins();

	void setNumCoins(int num);

	void togglePlayerInmunity();

	glm::vec2 getPosPlayer();

	glm::vec2 getChestPosition();
	

private:
	void initShaders();
	void printHearts();
	void checkCollisions();
	void finishLevel();
	TileMap *map;
	TileMap *map2;
	Player *player;
	ShaderProgram texProgram;
	float currentTime, iniFreezeTime;
	glm::mat4 projection;	

	Quad *quad;
	Texture texs[3];
	TexturedQuad *texQuad[3];

	Text text;

	vector<Entity *> enemies;

	int numMovingPlatforms;
	vector<MovingSlab *> movingPlatforms;

	int numLevel, numRajoles;
	int numSkeletons, numRates, numVampires;
	glm::vec2 posCofre, posClau, posJugador;

	Chest *chest;
	Item *key;
	Item *stopwatch;
	Item *gem;

	bool allPressed;
	bool unlockChest;
	bool openChest;
	bool win;
	bool takenStopwatch;
	bool takenGem;

};


#endif // _SCENE_INCLUDE

