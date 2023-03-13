#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "TileMap.h"
#include "Player.h"
#include "Text.h"
#include "Rajola.h"
#include <vector>


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene * getInstance();
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	

private:
	static Scene *instance;

	void initShaders();
	void addRajola(int x, int y);
	TileMap *map;
	TileMap *map2;
	Player *player;
	Rajola *rajola;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	vector<vector<bool>> rajoles;
	

	// Text text;

};


#endif // _SCENE_INCLUDE

