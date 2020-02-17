#pragma once

#include"Shape.h"

class Rect:public Shape
{
public:
	Rect();
	~Rect();

	void Draw(QPainter &painter);
};

