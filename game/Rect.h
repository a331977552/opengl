#pragma once
class Rect
{
public:
	float x, y, width, height;

	bool isContain(Rect &rec);
	Rect();
	Rect(float x,float y,float width,float height);
	~Rect();
};

