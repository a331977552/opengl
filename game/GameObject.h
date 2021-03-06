#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include "ResourceManager.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SpriteRenderer.h"
#include "Rect.h"
class GameObject
{
public:
	
	glm::vec2 position, size, velocity;
	glm::vec3 color;
	GLfloat rotation;
	GLboolean isSolid;
	GLboolean isDestory;
	Rect boundingBox;
	Texture2D sprite;
	GameObject();


	GameObject(glm::vec2 pos, glm::vec2  size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	~GameObject();
	virtual void draw(SpriteRenderer &renderer);
	Rect getBoundingBox();
	void setPosition(glm::vec2 position);
	glm::vec2& getPosisiton();
	void setSize(glm::vec2 size);
	glm::vec2& getSize();
private:
};

