#pragma once
#include "ResourceManager.h"
class SpriteRenderer
{
public:

	
	SpriteRenderer(Shader &shader);
	void drawSprite(Texture2D &tex, glm::vec2 position,
					glm::vec2 size = glm::vec2(10.f, 10.f),
					GLfloat roate = 0.0f, glm::vec3 color = glm::vec3(1.0f));
	~SpriteRenderer();
private:
	Shader shader;
	GLuint quadVAO;
	void initRenderData();
};

