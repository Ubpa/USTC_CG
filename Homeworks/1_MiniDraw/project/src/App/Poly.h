#pragma once

#include"figure.h"

class Poly :public Figure {
public:
	QVector<QPoint> points;

	Poly();
	~Poly();

	void Draw(QPainter&);
};

#pragma once
