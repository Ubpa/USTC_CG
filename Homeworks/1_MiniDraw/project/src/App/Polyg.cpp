#include "Polyg.h"

Polyg::Polyg()
{
}

Polyg::~Polyg()
{
}

void Polyg::Draw(QPainter& painter)
{
	painter.drawRect(start.x(), start.y(),
		end.x() - start.x(), end.y() - start.y());
}

void Polyg::set_mid(QPoint m)
{
	mid.push_back(m);
}
