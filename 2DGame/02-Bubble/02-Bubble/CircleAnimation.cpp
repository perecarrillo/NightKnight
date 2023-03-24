#include "CircleAnimation.h"

using namespace std;


CircleAnimation::CircleAnimation(int width, int height)
{
	initShaders();

	projection = glm::ortho(0.f, float(width / (3.5) - 1), float(height / (3.5) - 1), 0.f);
	currentTime = 0.0f;

	quad = Quad::createQuad(0.f, 0.f, width, height, texProgram);

	radius = 0;
	center = glm::vec2(150.f, 150.f);
	
}

void CircleAnimation::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("center", center.x, center.y);
	texProgram.setUniform1f("radius", radius);
	quad->render();
}


void CircleAnimation::update(int deltaTime) {
	currentTime += deltaTime;
	radius = currentTime/8;

}

void CircleAnimation::changeCenter(int x, int y)
{
	center = glm::vec2(x + 16, y + 16);
}


void CircleAnimation::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/circle.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/circle.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

