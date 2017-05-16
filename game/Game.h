#pragma once
#define GLEW_STATCI
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "ResourceManager.h"

enum GameState {
	ACTIVE,
	MENU,
	WIN

};
class Game
{
public:
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;

	Game(GLuint widht,GLuint height);
	void init();
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
	~Game();
};

