#pragma once

#include <QVector2D>
#include <QPainter>
#include <QWidget>
#include <cmath>

class Warp
{
public:
	Warp() = default;
	~Warp() = default;

	virtual void Render(QImage *ptr_image) = 0;
	void SetBeginPoint(QPoint qb);
	void SetEndPoint(QPoint qe);
	void DrawControlPoints(QPainter *painter, QPoint *image_pos);

protected:
	virtual QPoint Output(QPoint p) = 0;
	QVector<QPoint> begin_points_;
	QVector<QPoint> end_points_;
};
