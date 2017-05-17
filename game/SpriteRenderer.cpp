#include "SpriteRenderer.h"





SpriteRenderer::SpriteRenderer(Shader &shader)
{
	this->shader = shader;
	initRenderData();

}

void SpriteRenderer::drawSprite(Texture2D &tex, glm::vec2 position, glm::vec2 size /*= glm::vec2(10.f, 10.f)*/, GLfloat roate /*= 0.0f*/, glm::vec3 color /*= glm::vec3(1.0f)*/)
{

	glm::mat4 model;
	model =glm::translate(model, glm::vec3(position,0.0f));
	model = glm::translate(model, glm::vec3(0.5f* size.x,0.5f* size.y, 0));
	model = glm::rotate(model, roate, glm::vec3(0.0f, 0.0f, 1.f));
	model = glm::translate(model, glm::vec3(-0.5f* size.x,-0.5f* size.y, 0));
	model = glm::scale(model, glm::vec3(size,1.0f));
	shader.use();
	shader.setInt("tex",0);
	glActiveTexture(GL_TEXTURE0);
	tex.bind();
	shader.setMatrix4("model", model);
	shader.setVector3f("spriteColor", color);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::initRenderData()
{
	GLuint VBO;
	GLfloat vertices[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}
