#pragma once

#include"shape.h"


class Rect :
	public Shape
{
public:
	Rect();
	~Rect();

	void Draw(QPainter& painter);
};


