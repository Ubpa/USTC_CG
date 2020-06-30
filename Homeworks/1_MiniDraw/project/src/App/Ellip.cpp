#include "Ellip.h"

Ellip::Ellip()
{
}

Ellip::~Ellip()
{
}

void Ellip::Draw(QPainter& painter)
{
	painter.setPen(pen_);
	painter.drawEllipse(start_, 
		end_.x() - start_.x(), end_.y() - start_.y());
}