#include "Polyg.h"

Polyg::Polyg()
{
}

Polyg::~Polyg()
{
}

void Polyg::Draw(QPainter& painter)
{
	painter.setPen(pen_);

	if (points_.size() == 0)
		painter.drawLine(start_, end_);
	else
	{
		painter.drawLine(start_, points_[0]);

		for (int i = 0; i < points_.size() - 1; i++)
			painter.drawLine(points_[i], points_[i + 1]);

		painter.drawLine(points_.back(), end_);
		painter.drawLine(end_, start_);
	}
}

void Polyg::set_mid(QPoint m)
{
	points_.push_back(m);
}
