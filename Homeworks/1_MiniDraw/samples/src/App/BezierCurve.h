#pragma once

#include "Polygon.h"
#include "qpen.h"

class BezierCurve : public Polygon
{
public:
	BezierCurve(const QPoint _start_point);
	~BezierCurve();
	void Draw(QPainter& paint);
	virtual void setPen(const QPen* pen_ = nullptr, const int mode = 0);

private:
	float Bernstein(int n, int i, float t);
	QPen* boundPen;
};
