#include "Rect.h"



bool Rect::isContain(Rect &rec)
{

	if (x<= (rec.x+width) && (x+width) >= (rec.x)   && y<= (rec.y+height) && (y+height) >= (rec.y)      )
		return true;
	return false;
}

Rect::Rect()
{

}


Rect::Rect(float x, float y, float width, float height):
	x(x),y(y),width(width),height(height)
{

}

Rect::~Rect()
{
}
