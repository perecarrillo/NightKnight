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


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	

private:
	void initShaders();
	TileMap *map;
	TileMap *map2;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;	

	// Text text;

};


#endif // _SCENE_INCLUDE

