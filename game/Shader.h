#pragma once
#include <string>
#define GLEW_STATIC
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <iostream>

class Shader
{
public:
	GLuint ID;
	Shader();
	Shader &use();
	void compile(const GLchar * vertexSource, const GLchar * fragmeSource, const GLchar * geometrySource = nullptr);
	void setFloat(const GLchar * name,GLfloat value,GLboolean useShader=false);
	void setInt(const GLchar * name,GLint value,GLboolean useShader=false);
	void setVector2f(const GLchar * name,const GLfloat x,const GLfloat y,GLboolean useShader=false);
	void setVector2f(const GLchar * name,const  glm::vec2 &value,GLboolean useShader=false);
	void setVector3f(const GLchar * name,const GLfloat x,const GLfloat y,const GLfloat z,GLboolean useShader=false);
	void setVector3f(const GLchar * name,const  glm::vec3 &value,GLboolean useShader=false);

	void setVector4f(const GLchar * name,const GLfloat x,const GLfloat y,const GLfloat z,const GLfloat w,GLboolean useShader=false);
	void setVector4f(const GLchar * name,const  glm::vec4 &value,GLboolean useShader=false);
	void setMatrix4(const GLchar * name,const  glm::mat4 &value,GLboolean useShader=false);

	~Shader();
private :
	void checkCompileErrors(GLuint object, std::string type);

};

