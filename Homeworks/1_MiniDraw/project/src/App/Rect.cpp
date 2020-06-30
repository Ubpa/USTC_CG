#include "Rect.h"

Rect::Rect()
{
}

Rect::~Rect()
{
}

void Rect::Draw(QPainter& painter)
{
	painter.setPen(pen_);
	painter.drawRect(start_.x(), start_.y(),
		end_.x() - start_.x(), end_.y() - start_.y());
}
