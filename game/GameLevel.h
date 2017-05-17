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


private :
	vector<vector<int>> bricks;
	vector<GameObject> objects;
	void init(float width,float height);
};

