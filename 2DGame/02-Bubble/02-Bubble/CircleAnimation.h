#ifndef _CIRCLEANIMATION_INCLUDE
#define _CIRCLEANIMATION_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Quad.h"
#include "ShaderProgram.h"
#include <vector>
#include <iostream>
#include <glm/glm.hpp>


class CircleAnimation
{
public:

	CircleAnimation(int width, int height);

	void render();

	void update(int deltaTime);

	void changeCenter(int x, int y);

	void initShaders();

private:
	ShaderProgram texProgram;
	Quad *quad;
	float currentTime;
	glm::mat4 projection;
	float radius;
	glm::vec2 center;
};
#endif

