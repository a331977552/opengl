#pragma once
#include "GameObject.h"
class BallObject :
	public GameObject
{
public:
	GLboolean isStuck;
	glm::vec2 radius;
	BallObject(glm::vec2 pos, glm::vec2  size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	glm::vec2 move(GLfloat dt,GLfloat windowWidth);
	void reset(glm::vec2 position, glm::vec2 volecity);

	
	
};

