#pragma once

#include"shape.h"
class Line:public shape
{
public:
	Line();
	~Line();

	void Draw(QPainter &painter);
};

