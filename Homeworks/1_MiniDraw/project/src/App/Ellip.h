#pragma once

#include "shape.h"

class Ellip : public Shape
{
public:
	Ellip();
	~Ellip();

	void Draw(QPainter& painter);
};

