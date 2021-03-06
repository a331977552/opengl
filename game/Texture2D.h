#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
#include <SOIL\SOIL.h>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>
class Texture2D
{
public:

	GLuint ID;
	GLint channels;
	GLint width, height;
	GLuint wrap_S;
	GLuint wrap_T;
	GLuint filter_min;
	GLuint filter_max;
	void generate( const  GLchar *const str);
	void bind()const;
	void generateEmpty(GLuint width, GLuint height);
	Texture2D();
	~Texture2D();
};

