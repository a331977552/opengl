#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Game.h"
const int WIDTH = 600,HEIGHT= 400;
class Main
{
public:

	
	void initEnv();
	void start();
	Main();
	~Main();
private :
	float lastTime=0;
	static Game  *game;
	GLFWwindow *window;
};

