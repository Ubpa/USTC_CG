#include "BezierCurve.h"
#include <cmath>
#include <vector>

BezierCurve::BezierCurve(const QPoint _start_point)
	:Polygon(_start_point)
{
	boundPen = new QPen();
	boundPen->setWidth(3);
	boundPen->setColor(QColor(0x00, 0xA2, 0xE8));
}

BezierCurve::~BezierCurve() {
	delete boundPen;
}

void BezierCurve::Draw(QPainter& paint) {
	if (canDrag) {
		done = false;
		QPen* tmp = linePen;
		linePen = boundPen;
		Polygon::Draw(paint);
		linePen = tmp;
	}

	// get bizier curve
	int n = points.size() - 1;
	Polygon bezierCurve(points[0]);
	bezierCurve.setCanDrag(false);
	std::vector<QPointF> tmpP(points.size());
	float step = 1.0f / (20.0f * n);
	for (float t = step; t <= 1.0f + 10E-6; t += step) {
		bezierCurve.AddPoint(false);
		for (int i = 0; i < points.size(); i++)
			tmpP[i] = points[i];
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n - i; j++)
				tmpP[j] = (1 - t) * tmpP[j] + t * tmpP[j + 1];
		}

		/*float PtX = 0.0f, PtY = 0.0f;
		for(int i = 0; i <= n; i++){
			float B_n_i_t = Bernstein(n, i, t);
			PtX += B_n_i_t * points[i].x();
			PtY += B_n_i_t * points[i].y();
		}*/

		bezierCurve.update(QPoint(tmpP[0].x(), tmpP[0].y()));
	}

	const QPen oldPen = paint.pen();

	// draw bizier curve
	bezierCurve.Draw(paint);

	// restore pen
	paint.setPen(oldPen);
}

void BezierCurve::setPen(const QPen* pen_, const int mode) {
	if (mode & PEN::BOUND) {
		delete boundPen;
		boundPen = (pen_ == nullptr ? nullptr : new QPen(*pen_));
	}
	Polygon::setPen(pen_, mode);
}

float BezierCurve::Bernstein(int n, int i, float t) {
	int numerator = 1, denominator = 1;
	for (int k = 1; k <= n - i; k++) {
		numerator *= i + k;
		denominator *= k;
	}
	int C_n_i = numerator / denominator;
	return C_n_i * pow(1 - t, n - i) * pow(t, i);
}