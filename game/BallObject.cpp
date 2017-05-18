#include "BallObject.h"




BallObject::BallObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color /*= glm::vec3(1.0f)*/, glm::vec2 velocity /*= glm::vec2(0.0f, 0.0f)*/)
	:GameObject(pos,size,sprite,color,velocity)
{


}

glm::vec2 BallObject::move(GLfloat dt, GLfloat windowWidth)
{

	if (!isStuck)
	{
		glm::vec2 speed=  velocity*dt;
		position += speed;
		if (position.x < 0) {
			velocity.x = -velocity.x;
			position.x = 0;
		}
		if (position.x >   windowWidth-size.x) {
			velocity.x = -velocity.x;
			position.x =  windowWidth-size.x;
		}
		if (position.y  < 0.0f) {
			position.y = 0;
			velocity.y = -velocity.y;
		}
		boundingBox.x = position.x;
		boundingBox.y = position.y;




	}

	return position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 volecity)
{
	this->velocity = volecity;
	this->position = position;
	this->isStuck = true;

}
