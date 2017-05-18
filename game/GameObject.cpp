#include "GameObject.h"



GameObject::GameObject()
:position(0,0),size(1,1),sprite(),color(1.0f),velocity(0.0f),rotation(0),isDestory(false),isSolid(false)

{
	boundingBox.x = 0;
	boundingBox.y = 0;
	boundingBox.width = 1;
	boundingBox.height = 1;
}


GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f, 0.0f)*/)
:position(pos),size(size),sprite(sprite),color(color),velocity(velocity),rotation(0),isDestory(false),isSolid(false) {
	boundingBox.x = pos.x;
	boundingBox.y = pos.y;
	boundingBox.width = size.x;
	boundingBox.height = size.y;


}

GameObject::~GameObject()
{
}

void GameObject::draw(SpriteRenderer &renderer)
{

	renderer.drawSprite(sprite, position, size, rotation, color);

}

Rect GameObject::getBoundingBox()
{

	return boundingBox;
}

void GameObject::setPosition(glm::vec2 position)
{
	this->position = position;
	boundingBox.x = position.x;
	boundingBox.y = position.y;


}

glm::vec2& GameObject::getPosisiton()
{
	return position;
}

void GameObject::setSize(glm::vec2 size)
{
	this->size = size;
	boundingBox.width = size.x;
	boundingBox.height = size.y;


}

glm::vec2& GameObject::getSize()
{
	return size;
}
