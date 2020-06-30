#include "shape.h"

Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::set_mid(QPoint m)
{
}

void Shape::set_start(QPoint s)
{
	start_ = s;
}

void Shape::set_end(QPoint e)
{
	end_ = e;
}

void Shape::set_pen(QPen pen)
{
	pen_ = pen;
}
