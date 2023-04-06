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
#include "SoundController.h"
#include "AnimationEscut.h"

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
	void checkCollisions(bool enemyCheck);
	void finishLevel();
	void startAnimationCoins();

	TileMap *map;
	TileMap *map2;
	Player *player;
	ShaderProgram texProgram;
	float currentTime, iniFreezeTime, iniEscutTime, iniCoinsTime, animationTime; //animationTime és el temp en que comença l'animació dels coins
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

	int initialCoins;
	int finalCoins;
	int coins;

	AnimationEscut *animationEscut;

	Chest *chest;
	Item *key;
	Item *stopwatch;
	Item *gem;
	Item *heart;
	Item *escut;

	bool allPressed;
	bool unlockChest;
	bool openChest;
	bool win;
	bool takenStopwatch;
	bool takenGem;
	bool takenHeart;
	bool takenEscut;
	bool iniAnimationCoins;
	bool animationCoinsFinished;
	bool terminatorEnd;
	bool keySound;

};


#endif // _SCENE_INCLUDE

