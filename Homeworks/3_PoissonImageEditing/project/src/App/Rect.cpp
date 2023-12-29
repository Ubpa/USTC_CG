#include "Rect.h"

Rect::Rect()
{
	type_ = kRect;
}

Rect::~Rect()
{
}

void Rect::Draw(QPainter& painter)
{
	painter.drawRect(start.x(), start.y(),
		end.x() - start.x(), end.y() - start.y());
}