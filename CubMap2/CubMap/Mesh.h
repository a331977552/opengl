#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <vector>
#include <sstream>

#include "Shader.h"

using namespace std;
class Mesh
{

	

public:


	struct Vertex {
		glm::vec3  position;
		glm::vec3  normal;
		glm::vec2  TexCoords;



	};
	
	struct Material {
		GLuint programId;
		
	};
	struct texture{
		GLuint texid;
		string type;
		string path;
	};


vector<Vertex> vertices;
vector<GLuint > indices;
vector<texture> textures;


Mesh();
  Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<texture> textures);
 void Draw(Shader shader);
 ~Mesh();
private :
	GLuint VAO, VBO, EBO;
 void setupMesh();

};

