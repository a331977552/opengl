#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Model.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
const  int WIDTH = 800, HEIGHT = 600;
class Application
{
private :
	Model *modelCharacter;
	GLuint cubeVAO,skyVAO;
	Texture *skyBoxtex,*floorTex;
	Texture *cubetex;
	Shader *cubeShader;
	Shader *skyboxShader,*floorShader;

	GLFWwindow  *window;
		void onDraw();
		void initData();
		void initGL();
public:
	

	void start();



	Application();
	~Application();
};

