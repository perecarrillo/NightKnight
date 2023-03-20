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
#include "Key.h"
#include "Chest.h"

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
	void render(bool playing);
	bool isGameOver();
	bool isWin();
	void changeLevel(int n);
	int getNumLevel();
	void makeKeyAppear();
	

private:
	void initShaders();
	void printHearts();
	void checkCollisions();
	TileMap *map;
	TileMap *map2;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;	

	Quad *quad;
	Texture texs[2];
	TexturedQuad *texQuad[3];

	Text text;

	vector<Entity *> enemies;

	int numLevel, numRajoles;
	int numSkeletons, numRates;
	glm::vec2 posCofre, posClau, posJugador;

	Key *key;
	Chest *chest;

	bool allPressed;
	bool unlockChest;
	bool openChest;
	bool win;

};


#endif // _SCENE_INCLUDE

