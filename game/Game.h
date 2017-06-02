#pragma once
#define GLEW_STATCI
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "GameLevel.h"
#include "BallObject.h"
#include "ParticleGenerator.h"
#include "SimpleAudioManager.h"
#include "PostProcessor.h"
#include "TextRender.h"
enum GameState {
	ACTIVE,
	MENU,
	WIN

};
const glm::vec2 playerSize(100,20);
const glm::vec2 ballSize(30, 30);

const float VELOCITY = 500.f; 
const glm::vec2 ball_velocity =glm::vec2(100.f,-300.f);

class Game
{
public:
	PostProcessor * processor;
	TextRender *texRender;
	ParticleGenerator * particleGenerator;
	vector<GameLevel*> gameLevels;
	int currentLevel = 0;
	SpriteRenderer *sprite;
	BallObject *ball;
	GameState state;
	GLboolean keys[1024];
	GLuint width, height;
	GameObject *paddle;
	Game(GLuint widht,GLuint height);
	void init();
	void processInput(GLfloat dt);
	void resetGame();
	void update(GLfloat dt);
	void render();
	

	~Game();
};

