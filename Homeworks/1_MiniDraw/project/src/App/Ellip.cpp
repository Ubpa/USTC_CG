#include "Ellip.h"

Ellip::Ellip()
{
}

Ellip::~Ellip()
{
}

void Ellip::Draw(QPainter& painter)
{
	painter.drawEllipse(start, 
		end.x() - start.x(), end.y() - start.y());
}