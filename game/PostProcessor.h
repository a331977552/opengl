#pragma once
#include "ResourceManager.h"
class PostProcessor
{
public:
	Shader processShader;
	Texture2D tex;
	GLuint width, height;
	GLboolean confuse, chaos, shake;

	
	PostProcessor(Shader processShader,GLuint width,GLuint height);
	void beginRender();
	void endRender();
	void render(GLfloat dt);
	~PostProcessor();
	float shakeTime;
private :
	GLuint MSFBO, FBO;
	GLuint RBO,VAO;
	void initRender();
	
};

