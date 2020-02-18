#pragma once

#include"shape.h"

class Line :public Shape
{
public:
	Line();
	~Line();

	void Draw(QPainter& painter);
};

