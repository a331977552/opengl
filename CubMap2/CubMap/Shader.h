#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <SOIL\SOIL.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ostream>
#include <fstream>
using std::stringstream;
using std::ifstream;
using std::string;
using std::cout;
using std::endl;

class Shader
{
	string vertexName,fragmentName;
public:
	GLuint program;
	

	Shader(string vertexPath="",string fragmentPath="");
	void gengerateProgram(string vertexSource="",string fragmetSource="");
	void useProgram();
	~Shader();
};

