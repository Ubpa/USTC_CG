#include "Polygon.h"

Poly::Poly()
{
	type_ = kPolygon;
	finish = false;
	polygon.push_back(start);
}

Poly::~Poly()
{
}

QPolygon Poly::get_polygon()
{
	return polygon;
}

void Poly::update(int mode)
{
	switch (mode)
	{
	case 0:
		finish = true;
		break;
	case 1:
		if (polygon.size() > 0)
			polygon.back() = end;
		polygon.push_back(polygon.back());
		break;
	default:
		break;
	}
}


void Poly::Draw(QPainter& painter)
{
	if (finish)
		painter.drawPolygon(polygon);
	else
		painter.drawPolyline(polygon);
}