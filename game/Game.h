#pragma once
#define GLEW_STATCI
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
enum GameState {
	ACTIVE,
	MENU,
	WIN

};
const glm::vec2 playerSize(100,20);
const float VELOCITY = 500.f; 
class Game
{
public:
	vector<GameLevel> gameLevels;
	int currentLevel = 0;
	SpriteRenderer *sprite;
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	GameObject *paddle;
	Game(GLuint widht,GLuint height);
	void init();
	void processInput(GLfloat dt);
	void update(GLfloat dt);
	void render();
	~Game();
};

