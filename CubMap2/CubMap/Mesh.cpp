#include "stdafx.h"
#include "Mesh.h"


Mesh::Mesh()
{
}
Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<texture> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    this->setupMesh();
} 

void Mesh::Draw(Shader shader) {
	GLuint specular = 0;
	GLuint diffuse = 0;
	shader.useProgram();
	for (int i = 0; i < textures.size(); i++) {
		texture tex =textures[i];
		stringstream str;
		string number;
		string name=tex.type;
		if (tex.type == "texture_diffuse") {
			str<<diffuse++;
		}else{
			str<<specular++;
		}
		number =str.str();
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(glGetUniformLocation(shader.program, (name + number).c_str()),i);
		glBindTexture(GL_TEXTURE_2D, tex.texid);
	}
	    glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT,0);
		glBindVertexArray(0);
 }
void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &this->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &this->indices[0], GL_STATIC_DRAW);
	 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3 * sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(0));
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3 * sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,normal)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2 * sizeof(GL_FLOAT), GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,TexCoords)));
	glBindVertexArray(0);

}

Mesh::~Mesh()
{

}
