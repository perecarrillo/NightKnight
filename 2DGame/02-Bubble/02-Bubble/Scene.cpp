#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla


Scene::Scene()
{
	map = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	map = TileMap::createTileMap("levels/level.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap("levels/levelBackground.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(player->getInitialPosition() * map->getTileSize()));
	player->setTileMap(map);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH/(3.5) - 1), float(SCREEN_HEIGHT/(3.5) - 1), 0.f);
	currentTime = 0.0f;

	//// Select which font you want to use
	//if (!text.init("fonts/OpenSans-Regular.ttf"))
	//	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//	//if(!text.init("fonts/DroidSerif.ttf"))
	//	cout << "Could not load font!!!" << endl;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map2->render();
	map->render();
	player->render();

	//text.render("Videogames!!!", glm::vec2(10, CAMERA_HEIGHT - 20), 32, glm::vec4(1, 1, 1, 1));

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



