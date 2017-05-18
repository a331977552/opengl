#include "GameLevel.h"






GameLevel::GameLevel(const char * file, float width, float height)
{
	ifstream open(file);
	if (!open.is_open()) {

		std::cout << "fail to open  file:  " << file << std::endl;
		return;
	}
	int code;
	std::string line;
	while (std::getline(open, line)) {

		vector<int > row;
		stringstream  ststream;
		
		ststream << line;
		while (ststream >> code) {
			row.push_back(code);
		}
		bricks.push_back(row);
	}

	if (bricks.size() > 0)
		init(width, height);

}

void GameLevel::doCollision(GameObject &ball)
{

	for (GameObject &obj :objects)
	{
		bool collided = obj.boundingBox.isContain(ball.boundingBox);

		if (!obj.isSolid && !obj.isDestory  &&collided) {

			obj.isDestory = true;
		}

	}

}

void GameLevel::doCircleCollision(GameObject &ball)
{
	glm::vec2 	ballCenter=  ball.size.x/2 + ball.position;
	for (GameObject &obj : objects)
	{


		if (!obj.isSolid && !obj.isDestory) {

			glm::vec2 aabbHalfExtents(obj.size.x / 2, obj.size.y / 2);
			glm::vec2 center = obj.position + aabbHalfExtents;

			glm::vec2  difference = ballCenter - center;
			glm::vec2 clamped = glm::clamp(difference, -aabbHalfExtents, aabbHalfExtents);
			glm::vec2 closest= center + clamped;
			if (glm::length(ballCenter-closest)<=ball.size.x/2  )
			{

				obj.isDestory = true;
			}

			
		}

	}


}

void GameLevel::init(float width, float height)
{
	float unitWidth =  width/ static_cast<float> (bricks.at(0).size());
	float unitHeight = height/ static_cast<float> (bricks.size());
	for (int x = 0; x < bricks.size(); x++) {
		 vector<int> row=bricks[x];

		for (int y = 0; y < row.size(); y++) {
			int code =  row[y];
			GameObject  obj;
			glm::vec2 pos(y*unitWidth, x*unitHeight);
			glm::vec2 size(unitWidth, unitHeight);
			obj.setSize( size);
			obj.setPosition(pos);

			if (code == 1) {
				obj.sprite = ResourceManager::getInstance().getTexture2D("brick_solid");
				obj.color = glm::vec3(0.8f, 0.8f, 0.7f);
				obj.isSolid = true;
				objects.push_back(obj);
			}
			else if (code >= 2) {
				obj.sprite = ResourceManager::getInstance().getTexture2D("brick");
				obj.isSolid = false;
				if (code == 2) {				
					obj.color =  glm::vec3(0.2f, 0.6f, 1.0f);
				}
				else if (code == 3) {
					obj.color = glm::vec3(0.0f, 0.7f, 0.0f);
				}
				else if (code == 4) {
					obj.color = glm::vec3(0.8f, 0.8f, 0.4f);
				}
				else if (code ==5) {
					obj.color = glm::vec3(1.0f, 0.5f, 0.0f);
				}
				objects.push_back(obj);
			}
		

		}

	}


}

void GameLevel::draw(SpriteRenderer &renderer)
{
	for (GameObject &obj : objects) {
		if (!obj.isDestory) {
			obj.draw(renderer);
		}
	}


}

GLboolean GameLevel::isComplete()
{
	for (GameObject &obj : objects) {
		if (!obj.isSolid &&!obj.isDestory) {
			return GL_FALSE;
		}
	}
	return GL_TRUE;
}

GameLevel::~GameLevel()
{
}

 vector<GameObject>& GameLevel::getObjects()
{

	return objects;
}
