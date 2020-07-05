#include "WarpIDW.h"


WarpIDW::WarpIDW() : u_(1)
{
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

		float weight = 1.0f / powf((pf - QVector2D(begin_points_[i].x(), begin_points_[i].y())).length(), u_);
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