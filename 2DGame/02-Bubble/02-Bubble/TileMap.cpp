#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include "Quad.h"


#define NUM_TILES_X 32
#define NUM_TILES_Y 26

using namespace std;

const set<int> TileMap::NO_COLLISION_BELOW({ 1615,1616,1617,1618,1619,1620,	1613,1614 });

TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	contRajoles = 0;

	loadLevel(levelFile);
	prepareArrays(minCoords, program);

	rajola = new Rajola();
	rajola->init(minCoords, program);


	rajoles = vector<vector<bool>>(NUM_TILES_Y, vector<bool>(NUM_TILES_X, false));
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);

	for (int i = 0; i < NUM_TILES_Y; ++i) {
		for (int j = 0; j < NUM_TILES_X; ++j) {
			if (rajoles[i][j]) {
				rajola->changePosIni(glm::vec2(j, i));
				rajola->setPosition(glm::vec2(rajola->getInitialPosition() * this->getTileSize()));
				rajola->render();
			}
		}
	}

}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			fin >> tile;
			//if(tile == ' ')
			//	map[j*mapSize.x+i] = 0;
			//else
			map[j*mapSize.x + i] = tile;
		}
		char aux;
		fin.get(aux);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if (tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (hasCollision(map[y*mapSize.x + x], map[(y + 1)*mapSize.x + (x)]))
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (hasCollision(map[y*mapSize.x + x], map[(y + 1)*mapSize.x + (x)]))
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int offset_y) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0)
		{
			if (*posY - tileSize * y + size.y + offset_y <= 2)
			{
				*posY = tileSize * y - size.y - offset_y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = floor((pos.y) / double(tileSize));
	int y0 = pos.y;
	for (int x = x0; x <= x1; x++)
	{
		if (hasCollision(map[(y)*mapSize.x + x], map[(y + 1)*mapSize.x + x]))
		{
			if (tileSize * (y) != *posY && tileSize * (y + 1) > *posY)
			{
				*posY = tileSize * (y + 1);
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionSpikes(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y;

	x0 = (pos.x) / tileSize; 
	x1 = (pos.x + size.x) / tileSize; 
	y = (pos.y + size.y + 1) / tileSize;

	if ((map[(y)*mapSize.x + x0] == 1669 || map[(y)*mapSize.x + x0] == 1670) 
		&& (map[(y)*mapSize.x + x1] == 1669 || map[(y)*mapSize.x + x1] == 1670)) return true;

	return false;
}

void TileMap::collisionRajola(const glm::ivec2 & pos, const glm::ivec2 & size)
{
	int x0, x1, y;

	x0 = (pos.x) / tileSize;
	x1 = (pos.x + size.x) / tileSize;
	y = (pos.y + size.y + 1) / tileSize;

	if (pos.y - tileSize * y + size.y <= 2) {
		if ((map[(y)*mapSize.x + x0] == 1613 || map[(y)*mapSize.x + x0] == 1614)) {
			if (map[(y)*mapSize.x + x0] == 1614) x0 -= 1;
			if (!rajoles[y][x0]) {
				rajoles[y][x0] = true;
				++contRajoles;
			}
		}
		if ((map[(y)*mapSize.x + x1] == 1613 || map[(y)*mapSize.x + x1] == 1614)) {
			if (map[(y)*mapSize.x + x1] == 1614) x1 -= 1;
			if (!rajoles[y][x1]) {
				rajoles[y][x1] = true;
				++contRajoles;
			}
		}
	}
}


bool TileMap::hasCollision(int tile, int tileBelow) const
{
	if (tile == 0) return false;
	if (tileBelow != 0) return true;
	return NO_COLLISION_BELOW.find(tile) == NO_COLLISION_BELOW.end();
}

void TileMap::printRajoles() {
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	glm::mat4 modelview;
	for (int i = 0; i < NUM_TILES_Y; ++i) {
		for (int j = 0; j < NUM_TILES_X; ++j) {
			if (rajoles[i][j]) {
				modelview = glm::translate(glm::mat4(1.0f), glm::vec3(8.f + j*8.f, 9.f + i*8.f, 1.f));
				texProgram.setUniformMatrix4f("modelview", modelview);
				texQuad[0]->render(texs[0]);
			}
		}
	}
}

int TileMap::numRajolesPressed()
{
	return contRajoles;
}