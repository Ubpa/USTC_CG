#pragma once

#include"shape.h"
class Rect:public shape
{
public:
	Rect();
	~Rect();

	void Draw(QPainter &painter);
};

