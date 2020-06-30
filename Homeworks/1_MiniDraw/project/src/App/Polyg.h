#pragma once

#include "Shape.h"
#include "Line.h"
#include <vector>

class Polyg : public Shape
{
public:
	Polyg();
	~Polyg();

	void Draw(QPainter &painter);

private:
	std::vector<Line> line_list;
};

