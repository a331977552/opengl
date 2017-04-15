#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <GL\glew.h>

using std::string;
using std::fstream;
using std::stringstream;
class Shader {
private :
	GLuint program;

public :
	Shader(const string vertexShaderFilePath, const string  fragmentShaderFilePath) {
		if (vertexShaderFilePath == ""  ||fragmentShaderFilePath=="" )
		{
			std::cout << " the path of vertexShader  or fragmentShader  is NULL "<<std::endl;
			return;
		}
		string verCode;
		string fragCode;
		std::ifstream verFile(vertexShaderFilePath,std::ios::binary);
		std::ifstream fragFile(fragmentShaderFilePath,std::ios::binary);
		verFile.exceptions(std::ifstream::badbit);
		fragFile.exceptions(std::ifstream::badbit);
		try {
			std::stringstream  verStringStream,fragStringStream;
			 verStringStream<< verFile.rdbuf();
			 fragStringStream<< verFile.rdbuf();
			 verCode = verStringStream.str();
			fragCode = verStringStream.str();



		}
		catch (std::ifstream::failure e) {
			std::cout << "fail to operate the files "<<e.code()<<std::endl;
			return;
		}
		generateProgram(verCode.c_str(), fragCode.c_str());
	}
	void generateProgram(string &cVerCode,string &cFragCode) {
		generateProgram(cVerCode.c_str(), cFragCode.c_str());
	}
	void generateProgram(const GLchar const *cVerCode, const GLchar const *cFragCode) {
		 GLuint vertexShader= glCreateShader(GL_VERTEX_SHADER);
		GLuint  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vertexShader, 1, &cVerCode, NULL);
		glShaderSource(fragmentShader, 1, &cFragCode, NULL);
		glCompileShader(vertexShader);
		glCompileShader(fragmentShader);
		GLint success;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		char info[512];
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, 0, info);
			std::cout << "fail to compile the vertexShader :\n"<< info <<std::endl;
			return;
		}
		else {
				std::cout << "successfully compile vertexShader  "<<std::endl;
		
		}
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, 0, info);
			std::cout << "fail to compile the fragmentShader :\n"<< info <<std::endl;
			return;
		}
		else {
			std::cout << "successfully compile fragmentShader  "<<std::endl;
		}
		program =glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, 0, info);
			std::cout << "fail to link  the program :\n"<< info <<std::endl;
			return;
		}
		else {
			std::cout << " successfully link program  "<<std::endl;
		}
			std::cout << "build shader successfully!!!"<<std::endl;
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

	}

	GLuint getProgram() {
		return program;
	}
	void useProgram() {
		glUseProgram(program);
	}
	


};