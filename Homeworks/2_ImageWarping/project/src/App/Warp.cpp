#include "Warp.h"

void Warp::Render(QImage* ptr_image)
{
	QImage image_tmp(*(ptr_image));

	int width = ptr_image->width();
	int height = ptr_image->height();

	ptr_image->fill(Qt::lightGray);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			QPoint p(i, j);
			QPoint po = Output(p);

			if (po.x() >= 0 && po.x() < width && po.y() >= 0 && po.y() < height)
				ptr_image->setPixel(Output(p), image_tmp.pixel(p));

			//FixHole(ptr_image);
		}
	}
}

void Warp::SetControlPoints(QVector<QPoint> &bps, QVector<QPoint> &eps)
{
	begin_points_ = bps;
	end_points_ = eps;
}
