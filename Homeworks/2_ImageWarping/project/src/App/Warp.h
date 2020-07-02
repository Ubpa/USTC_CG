#pragma once

#include <QVector2D>
#include <QWidget>

class Warp
{
public:
	Warp() = default;
	~Warp() = default;

	virtual QPointF Output(QPointF p) = 0;

	void SetControlPoint(QPointF q);
	void SetControlVec(QVector2D v);

private:
	QVector<QPointF> control_points_;
	QVector<QVector2D> control_vecs_;
};
