#include "Warp.h"

void Warp::SetBeginPoint(QPoint qb)
{
	begin_points_.push_back(qb);
}

void Warp::SetEndPoint(QPoint qe)
{
	end_points_.push_back(qe);
}

void Warp::DrawControlPoints(QPainter *painter, QPoint *image_pos)
{
	for (int i = 0; i < begin_points_.size(); i++)
	{
		QPen pen;
		pen.setCapStyle(Qt::RoundCap);

		pen.setWidth(3);
		pen.setColor(Qt::red);
		painter->setPen(pen);
		painter->drawEllipse(begin_points_[i] + (*image_pos), 3, 3);

		pen.setWidth(2);
		pen.setColor(Qt::darkYellow);
		painter->setPen(pen);
		painter->drawLine(begin_points_[i] + *(image_pos), 
			end_points_[i] + *(image_pos));

		pen.setWidth(3);
		pen.setColor(Qt::blue);
		painter->setPen(pen);
		painter->drawEllipse(end_points_[i] + (*image_pos), 3, 3);
	}
}
