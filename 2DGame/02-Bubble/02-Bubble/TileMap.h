#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <vector>
#include <set>
#include "Rajola.h"
#include "TexturedQuad.h"
#include "Quad.h"
#include <glm/gtc/matrix_transform.hpp>



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{
	const static set<int> NO_COLLISION_BELOW;

private:
	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int offset_y, bool isOnPlatform) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, float *posY, int offset_y) const;
	bool collisionSpikes(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	void collisionRajola(const glm::ivec2 & pos, const glm::ivec2 & size);
	pair<int, int> closestJumpPosition(const glm::ivec2 &pos, const glm::ivec2 &size, bool left) const;

	int numRajolesPressed();
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	bool hasCollision(int tile, int tileBelow) const;
	void printRajoles();

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	Rajola *rajola;
	vector<vector<bool>> rajoles;

	ShaderProgram texProgram;
	Quad *quad;
	Texture texs[2];
	TexturedQuad *texQuad[3];

	int contRajoles;

};


#endif // _TILE_MAP_INCLUDE


