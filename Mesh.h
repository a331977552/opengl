#ifndef  __MESH__H__
#define __MESH__H__

#define GLEW_STATIC
#include <GLFW\glfw3.h>
#include <GL\glew.h>
#include "Shader.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\glm.hpp>
#include <vector>
#include "Texture.h"
using std::vector;
struct  Vertex{
	glm::vec3 position;
	glm::vec3 Normal;
	glm::vec2 texCoord;
};




class Mesh {
	vector<Vertex> vertices;
	vector<Texture> textures;
	vector<GLuint> indices;
public :

	Mesh(vector<Vertex> vertices,
		 vector<Texture> textures,
		 vector<GLuint> indices ) {
		this->vertices = vertices;
		this->textures = textures;
		this->indices = indices;
		this->setupMesh();
	}
	void draw(Shader &shader) {
		GLuint diffuseIndex  = 0;
		GLuint specularIndex = 0;
		for (int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);

			Texture  t= textures[i];
			string name;
			if (Texture::TextureType::DIFFUSE) {
				diffuseIndex++;
				name = "texDiffuse" + diffuseIndex;
			}
			else {
				specularIndex++;
				name = "texSpecular" + specularIndex;
			}
			glUniform1f(glGetUniformLocation(shader.program, "material.shininess"), 16.0f);
			glUniform1i(glGetUniformLocation(shader.program,name.c_str()), i);
			glBindTexture(GL_TEXTURE_2D, t.texId);
		}
		glUniform1f(glGetUniformLocation(shader.program, "material.shininess"), 16.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		//reset to default value
		for (int i = 0; i < textures.size(); i++) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, 0);
		}

	
	}


private :
	GLuint VAO, EBO, VBO;
	void setupMesh() {
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1,&EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, sizeof(Vertex), GL_FALSE, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, sizeof(Vertex), GL_FALSE, GL_FALSE, 8 * sizeof(GLfloat), (void*)offsetof(Vertex,Normal));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, sizeof(Vertex), GL_FALSE, GL_FALSE, 8 * sizeof(GLfloat), (void*)offsetof(Vertex,texCoord));
		glBindVertexArray(0);
	}
 

};
#endif __MESH__H__// ! __MESH__H__