#include "Shape.h"

Shape::Shape()
{
}

Shape::~Shape()
{
}

void Shape::set_start(QPoint s)
{
	start = s;
}

void Shape::set_end(QPoint e)
{
	end = e;
}

QPoint Shape::get_start()
{
	return start;
}

QPoint Shape::get_end()
{
	return end;
}