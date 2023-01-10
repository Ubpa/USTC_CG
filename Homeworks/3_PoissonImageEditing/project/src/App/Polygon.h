#ifndef POLYGON_H
#define POLYGON_H

#include"shape.h"

class Poly :public Shape
{
public:
	Poly();
	~Poly();

	void Draw(QPainter& painter);
	void update(int mode);
	QPolygon get_polygon();

private:
	QPolygon polygon;
	bool finish;
};



#endif 