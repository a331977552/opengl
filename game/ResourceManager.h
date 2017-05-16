#pragma once
#include "Shader.h"
#include "Texture2D.h"
#include <sstream>
#include <ostream>
#include <fstream>

#include <map>
using std::string;
using std::stringstream;
using std::ifstream;

class ResourceManager
{
public :
	
	 Shader loadShader(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile,std::string name);
	 Texture2D loadTexture2D(const char *path,std::string name, bool alpha=true);
	 Texture2D getTexture2D(std::string name);
	 Shader getShader(std::string name);
	 void clear();
	 static ResourceManager& getInstance();

private:
	std::map<std::string, Shader> shaders;
	std::map<std::string, Texture2D> texture2Ds;

	Shader loadShaderFromFile(const char * vShaderFile, const char * fShaderFile, const char * gShaderFile);
	Texture2D loadTexture2DFromFile(const char *path);

	ResourceManager();
	
};

