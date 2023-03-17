#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 9
#define SCREEN_Y 8 //Offset pantalla
#define NUM_TILES_X 32
#define NUM_TILES_Y 26


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
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(player->getInitialPosition() * map->getTileSize()));
	player->setTileMap(map);

	// Nombre de rajoles del nivell
	getline(fin, line);
	sstream.str(line);
	sstream >> numRajoles;

	// Posici� del cofre
	getline(fin, line);
	sstream.str(line);
	sstream >> posCofre.x >> posCofre.y;

	// Posici� de la rajola
	getline(fin, line);
	sstream.str(line);
	sstream >> posClau.x >> posClau.y;

	// Nombre d'enemics de cada tipus
	getline(fin, line);
	sstream.str(line);
	sstream >> numSkeletons;

	// Posici� inicial de cada enemics i inicialitzaci� d'aquests
	/*for (int i = 0; i < numSkeletons; ++i) {
		getline(fin, line);
		sstream.str(line);
		int posX, posY;
		sstream >> posX >> posY;
		Skeleton sk = Skeleton(posX, posY);
		sk.init("images/Skeleton.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		sk.setPosition(glm::vec2(sk.getInitialPosition() * map->getTileSize()));
		sk.setTileMap(map);
		skeletons.push_back(sk);
	}*/
	skeleton = new Skeleton();
	skeleton->init("images/skeleton.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	skeleton->setPosition(glm::vec2(skeleton->getInitialPosition() * map->getTileSize()));
	skeleton->setTileMap(map);

	

	fin.close();
}

void Scene::init()
{
	allPressed = false;
	initShaders();
	string fileMap = "levels/level" + std::to_string(numLevel) + ".txt";
	string fileBackground = "levels/level" + std::to_string(numLevel) + "Background.txt";
	//map = TileMap::createTileMap("levels/level2.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	//map2 = TileMap::createTileMap("levels/level2Background.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map = TileMap::createTileMap(fileMap, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map2 = TileMap::createTileMap(fileBackground, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	skeleton = new Skeleton();
	skeleton->init("images/skeleton.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	skeleton->setPosition(glm::vec2(skeleton->getInitialPosition() * map->getTileSize()));
	skeleton->setTileMap(map);

	rata = new Rata();
	rata->init("images/Rata.png", glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	rata->setPosition(glm::vec2(rata->getInitialPosition() * map->getTileSize()));
	rata->setTileMap(map);

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
	if (!text.init("fonts/OpenSans-Regular.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	player->update(deltaTime);
	//for (int i = 0; i < numSkeletons; ++i) skeletons[i].update(deltaTime);
	skeleton->update(deltaTime);
	rata->update(deltaTime);
	if (map->numRajolesPressed() >= numRajoles) allPressed = true;
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

	skeleton->render();
	rata->render();
	player->render();
	//for (int i = 0; i < numSkeletons; ++i) skeletons[i].render();

	text.render("Videogames!!!", glm::vec2(200, 300), 32, glm::vec4(1, 1, 1, 1));

	printHearts();

}

bool Scene::isGameOver()
{
	return player->getNumHearts() <= 0;
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


