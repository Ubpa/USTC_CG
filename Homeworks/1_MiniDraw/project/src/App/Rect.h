#pragma once

#include"figure.h"

class Rect:public Figure
{
public:
	Rect();
	~Rect();

	void Draw(QPainter& painter);
};

