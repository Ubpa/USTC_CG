#include "WarpIDW.h"


WarpIDW::WarpIDW() : u_(1)
{
}

void WarpIDW::Render(QImage* ptr_image)
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
		}
	}
}

QPoint WarpIDW::Output(QPoint p)
{
	QVector2D pf(p.x(), p.y());
	QVector<QVector2D> vecs;
	QVector<float> weights;
	float sum_weight = 0.0;

	for (int i = 0; i < begin_points_.size(); i++)
	{
		if (pf == QVector2D(begin_points_[i].x(), begin_points_[i].y()))
			return QPoint(end_points_[i].x(), end_points_[i].y());

		QVector2D vec(end_points_[i].x() - begin_points_[i].x(),
			end_points_[i].y() - begin_points_[i].y());

		float weight = 1.0 / powf((pf - QVector2D(begin_points_[i].x(), begin_points_[i].y())).length(), u_);
		sum_weight += weight;

		vecs.push_back(vec);
		weights.push_back(weight);
	}

	for (int i = 0; i < begin_points_.size(); i++)
	{
		pf += vecs[i] * weights[i] / sum_weight;
	}

	return QPoint(pf.x(), pf.y());
}