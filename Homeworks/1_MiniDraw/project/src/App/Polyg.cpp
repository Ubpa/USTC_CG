#include "Polyg.h"

Polyg::Polyg()
{
}

Polyg::~Polyg()
{
}

void Polyg::Draw(QPainter& painter)
{
	if (points_.size() == 0)
		painter.drawLine(start, end);
	else
	{
		painter.drawLine(start, points_[0]);

		for (int i = 0; i < points_.size() - 1; i++)
			painter.drawLine(points_[i], points_[i + 1]);

		painter.drawLine(points_.back(), end);
		painter.drawLine(end, start);
	}
}

void Polyg::set_mid(QPoint m)
{
	points_.push_back(m);
}
