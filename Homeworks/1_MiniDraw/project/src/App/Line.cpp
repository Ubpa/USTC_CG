#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::Draw(QPainter& painter)
{
	painter.setPen(pen_);
	painter.drawLine(start_, end_);
}
