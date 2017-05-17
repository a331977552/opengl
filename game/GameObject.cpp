#include "GameObject.h"



GameObject::GameObject()
:position(0,0),size(1,1),sprite(),color(1.0f),velocity(0.0f),rotation(0),isDestory(false),isSolid(false)

{
}


GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f, 0.0f)*/)
:position(pos),size(size),sprite(sprite),color(color),velocity(velocity),rotation(0),isDestory(false),isSolid(false) {


}

GameObject::~GameObject()
{
}

void GameObject::draw(SpriteRenderer &renderer)
{
	renderer.drawSprite(sprite, position, size, rotation, color);

}
