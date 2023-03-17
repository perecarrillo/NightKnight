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
	void render();
	bool isGameOver();
	void changeLevel(int n);
	int getNumLevel();
	

private:
	void initShaders();
	void printHearts();
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

	Skeleton *skeleton;

	Rata *rata;
	vector<Skeleton> skeletons;

	int numLevel, numRajoles, numSkeletons;
	glm::vec2 posCofre, posClau, posJugador;

	bool allPressed;

};


#endif // _SCENE_INCLUDE

