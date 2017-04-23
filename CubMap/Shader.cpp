#include "stdafx.h"
#include "Shader.h"


Shader::Shader(string vertexPath , string fragmentPath ) {
	if (vertexPath == "" || fragmentPath == "") {
		REMINDER("vertexPath or fragmentPath is null", vertexPath +"   "+fragmentPath);
		return;
	}
	ifstream file(vertexPath,std::ios::binary);
	ifstream file2(fragmentPath,std::ios::binary);
	if (!file.is_open()) {
			REMINDER("fail to open vertexPath", vertexPath);
		return;
	}
		
		if (!file2.is_open()) {
			REMINDER("fail to open fragmentPath", fragmentPath);
		return;
	}
		stringstream stream1, stream2;
		stream1<<file.rdbuf();
		stream2<<file2.rdbuf();
		gengerateProgram(stream1.str(), stream2.str());



}
void Shader::gengerateProgram(string vertexSource , string fragmetSource ) {
	GLuint  vertexShader=glCreateShader(GL_VERTEX_SHADER);
	GLuint  fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
	const char * const vertex=  vertexSource.c_str();
	const char * const fragmet=  fragmetSource.c_str();

	glShaderSource(vertexShader, 1, &vertex,0);
	glShaderSource(fragmentShader, 1, &fragmet,0);
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char errInfo[512];
	if (!status) {
		glGetShaderInfoLog(vertexShader, 512, 0, errInfo);
		REMINDER("fail to compile the vertexShader", errInfo);
	}
	else {
		REMINDER("successfully  compile the vertexShader", "");
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fragmentShader, 512, 0, errInfo);
		REMINDER("fail to compile the vertexShader", errInfo);
	}
	else {
		REMINDER("successfully  compile the fragmentShader", "");
	}

	program=glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(program, 512, 0, errInfo);
		REMINDER("fail to link the program", errInfo);
	}
	else {
		REMINDER("successfully  link  the program ",  program);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	REMINDER("build shader program successfully", "");




}
void Shader::useProgram() {
	glUseProgram(program);
	}
Shader::~Shader() {

	}