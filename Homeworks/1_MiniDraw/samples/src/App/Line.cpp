#include "Line.h"

Line::Line(const QPoint _start_point, const QPoint _end_point) {
	start_point.setX(_start_point.x());
	start_point.setY(_start_point.y());
	end_point.setX(_end_point.x());
	end_point.setY(_end_point.y());
}

void Line::Draw(QPainter& paint) {
	QPen oldPen = paint.pen();
	if (linePen != nullptr)
		paint.setPen(*linePen);

	paint.drawLine(start_point, end_point);

	if (canDrag) {
		if (pointPen != nullptr)
			paint.setPen(*pointPen);
		paint.drawPoint(start_point);
		paint.drawPoint(end_point);
	}

	paint.setPen(oldPen);
}

void Line::update(const QPoint _end_point) {
	end_point.setX(_end_point.x());
	end_point.setY(_end_point.y());
}

std::vector<QPoint*> Line::getPoint(QPoint& pos, bool all) {
	std::vector<QPoint*> rst;
	if (isClose(start_point, pos, 5)) {
		rst.push_back(&start_point);
		if (all)
			rst.push_back(&end_point);
	}
	if (isClose(end_point, pos, 5)) {
		rst.push_back(&end_point);
		if (all)
			rst.push_back(&start_point);
	}
	return rst;
}