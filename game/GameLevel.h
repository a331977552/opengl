#pragma once
#include <vector>
#include "GameObject.h"
#include <fstream>
#include <sstream>
#include "BallObject.h"
using std::stringstream;
using std::ifstream;
using std::vector;
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
class GameLevel
{
public:
	
	typedef std::tuple<GLboolean, Direction, glm::vec2> collision;
	GameLevel(const char * file,float width,float height);

	void draw(SpriteRenderer &renderer);
	void reload(const char * file,float width,float height);
	GLboolean isComplete();
	~GameLevel();
	 vector<GameObject>& getObjects();
	 void doPaddCollision(GameObject &ball, GameObject &paddle);
	 void doCollision(GameObject &obj);
	 collision checkCollision(GameObject &obj,GameObject &ball);
	 void doCircleCollision(GameObject &obj);
private :
	vector<GameObject> objects;
	vector<vector<int>> bricks;
	Direction getVectorDirection(glm::vec2 dir);
	void init(float width,float height);
};

