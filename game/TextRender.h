#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>
#include <GL\glew.h>
#include <glm\common.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <map>
#include "ResourceManager.h"
class TextRender
{
	struct Character {
		GLuint texId;
		glm::ivec2 size;
		glm::ivec2 bearing;
		GLuint advance;

	};
public:
	
	void update(float dt);
	void renderText(std::string text,GLfloat x,GLfloat y,GLfloat scale,glm::vec3 color);
	
	TextRender(GLuint width,GLuint height);
	~TextRender();
private :
	GLuint VAO;
		GLuint VBO;
	Shader textRenderShader;
	std::map<GLchar, Character> characters;
};

