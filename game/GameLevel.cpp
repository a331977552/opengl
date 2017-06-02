#include "GameLevel.h"






GameLevel::GameLevel(const char * file, float width, float height,PostProcessor &postProcessor)
:postProcessor(postProcessor),listener(NULL),score(0){

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

void GameLevel::setOnBrickDestoriedListener(OnBrickDestoriedListener listener)
{
	this->listener = listener;
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

GameLevel::collision GameLevel::checkCollision(GameObject &obj, GameObject &ball)
{
	
	glm::vec2 size=	obj.getSize();
	glm::vec2 half_size(size.x / 2,size.y / 2);

	glm::vec2 objCenter =	obj.position + half_size;
	glm::vec2 ballCenter=  ball.position + glm::vec2(ball.getSize().x / 2, ball.getSize().x/2);
	glm::vec2 difference =   ballCenter-objCenter;
	glm::vec2 diff= glm::clamp(difference, -half_size, half_size);
	glm::vec2 closest=	diff + objCenter;
	difference =	closest - ballCenter;
	float  len=glm::length(difference);
	
	if (glm::length(difference) <= ball.size.x / 2)
	{
	
		return std::make_tuple(GL_TRUE, getVectorDirection(difference), difference);
	}
		return std::make_tuple(GL_FALSE,Direction::UP,glm::vec2(0.f,0.f));
}
void  GameLevel::doPaddCollision(GameObject &ball, GameObject &paddle) {
	BallObject balla= static_cast<BallObject&>(ball);
	if (balla.isStuck) {
		return;
	}
	collision c = checkCollision(paddle, ball);


	float radius = ball.size.x / 2;	
	if (std::get<0>(c)) {
	
		Direction dir = std::get<1>(c);
		if (dir == Direction::UP || dir == Direction::DOWN) {
			ball.velocity.y = -ball.velocity.y;
			glm::vec2 difference = std::get<2>(c);

			glm::vec2 Paddlecenter= paddle.getPosisiton() + glm::vec2(paddle.getSize().x / 2,paddle.getSize().y / 2);
			glm::vec2 ballCenter = ball.getPosisiton() + glm::vec2(ball.getSize().x / 2, ball.getSize().y / 2);
			float percentage = (ballCenter.x - Paddlecenter.x)/paddle.getSize().x;

			glm::vec2 oldVelocity = ball.velocity;


			float strength = 2.0f;
			ball.velocity.x =  100.f*percentage*strength;
			GLfloat penetration = glm::length(radius - difference);
			ball.velocity=glm::normalize(ball.velocity)*glm::length(oldVelocity);


			if (dir == Direction::UP) {
				ball.position.y -= penetration;			
			}
			else {
				ball.position.y += penetration;
			}
		}
		else {
			ball.velocity.x = -ball.velocity.x;
			glm::vec2 difference = std::get<2>(c);
			GLfloat penetration = glm::length(radius - difference);
			if (dir == Direction::LEFT) {
				ball.position.x += penetration;
			}
			else {
				ball.position.y -= penetration;
			}
		}
	}


}

void GameLevel::doCircleCollision(GameObject &ball)
{
	
	
	float radius=ball.size.x / 2;
	for (GameObject &obj : objects)
	{

		if (!obj.isDestory) {
			
			collision c=checkCollision(obj, ball);
			if (std::get<0>(c)) {
				postProcessor.shake = true;
				postProcessor.shakeTime = 0.05f;
					std::cout << "success" << std::endl;
					if (!obj.isSolid)
					{
						SimpleAudioManager::getInstance().getSoundEngine()->play2D("resources/bleep.mp3");
						obj.isDestory = true;
						score++;
						if (listener ) {
							listener();
						}
					}
					else {

						SimpleAudioManager::getInstance().getSoundEngine()->play2D("resources/solid.wav");
					}
				
	Direction																																																		dir = std::get<1>(c);
				if (dir == Direction::UP ||dir == Direction::DOWN ) {
					ball.velocity.y = -ball.velocity.y;
					glm::vec2 difference = std::get<2>(c);
					GLfloat penetration = glm::length(radius - difference);
					if (dir == Direction::UP) {
						ball.position.y -= penetration;
					}
					else {
						ball.position.y += penetration;
					}
				}
				else {
					ball.velocity.x = -ball.velocity.x;
					glm::vec2 difference = std::get<2>(c);
					GLfloat penetration = glm::length(radius - difference);
					if (dir == Direction::LEFT) {
						ball.position.x += penetration;
					}
					else {
						ball.position.y -= penetration;
					}
				}			
			}							
		}
	}

}

Direction GameLevel::getVectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f,1.f),
		glm::vec2(1.0f,0.f),
		glm::vec2(0.0f,-1.f),
		glm::vec2(-1.0f,0.f)
	};

	glm::vec2  t= glm::normalize(target);
	float max = 0;
	int bestMatch = -1;
	for (int i = 0; i < 4; i++) {


		float dotProduct= glm::dot(t, compass[i]);
		if (dotProduct > max) {
			max = dotProduct;
			bestMatch = i;
		}

	}
	return (Direction)bestMatch;
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

void GameLevel::reload(const char * file, float width, float height)
{
	score = 0;
	ifstream open(file);
	if (!open.is_open()) {

		std::cout << "fail to open  file:  " << file << std::endl;
		return;
	}
	int code;
	std::string line;
	objects.clear();
	bricks.clear();
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
