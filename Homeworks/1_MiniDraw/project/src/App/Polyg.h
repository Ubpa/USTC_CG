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
	void set_mid(QPoint m);

private:
	std::vector<QPoint> points_;
};

