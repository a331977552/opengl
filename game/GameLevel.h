#pragma once
#include <vector>
#include "GameObject.h"
#include <fstream>
#include <sstream>
using std::stringstream;
using std::ifstream;
using std::vector;
class GameLevel
{
public:
	
	GameLevel(const char * file,float width,float height);

	void draw(SpriteRenderer &renderer);
	GLboolean isComplete();
	~GameLevel();
	 vector<GameObject>& getObjects();
	 
	 void doCollision(GameObject &obj);
	 void doCircleCollision(GameObject &obj);
private :
	vector<GameObject> objects;
	vector<vector<int>> bricks;
	
	void init(float width,float height);
};

