#include "FreeHand.h"

FreeHand::FreeHand()
{
}

FreeHand::~FreeHand()
{
}

void FreeHand::Draw(QPainter& painter)
{
	QPainterPath path;
	path.moveTo(start_);

	for (int i = 0; i < points_.size(); i++)
		path.lineTo(points_[i]);
	
	painter.setPen(pen_);
	painter.drawPath(path);
}

void FreeHand::set_mid(QPoint m)
{
	points_.push_back(m);
}
