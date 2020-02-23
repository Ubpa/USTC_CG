#include "Ellipse.h"
#include "Rect.h"

Ellipse::Ellipse(const QPoint _start_point, const QPoint _end_point) {
	start_point.setX(_start_point.x());
	start_point.setY(_start_point.y());
	end_point.setX(_end_point.x());
	end_point.setY(_end_point.y());
	boundPen = new QPen();
	boundPen->setWidth(3);
	boundPen->setColor(QColor(0x00, 0xA2, 0xE8));
}

Ellipse::~Ellipse() {
	delete boundPen;
}

void Ellipse::update(const QPoint _end_point) {
	end_point.setX(_end_point.x());
	end_point.setY(_end_point.y());
}

void Ellipse::Draw(QPainter& paint) {
	QPen oldPen = paint.pen();

	// draw circle
	if (linePen != nullptr)
		paint.setPen(*linePen);

	paint.drawEllipse(start_point.x(), start_point.y(),
		end_point.x() - start_point.x(), end_point.y() - start_point.y());

	if (canDrag) {
		Rect boundRect(start_point, end_point);
		boundRect.setCanDrag(true);
		boundRect.setPen(boundPen, PEN::LINE);
		boundRect.Draw(paint);

		if (pointPen != nullptr)
			paint.setPen(*pointPen);
		paint.drawPoint(start_point);
		paint.drawPoint(end_point);
	}

	// restore pen
	paint.setPen(oldPen);
}

std::vector<QPoint*> Ellipse::getPoint(QPoint& pos, bool all) {
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

void Ellipse::setPen(const QPen* pen_, const int mode) {
	if (mode & PEN::BOUND) {
		delete boundPen;
		boundPen = (pen_ == nullptr ? nullptr : new QPen(*pen_));
	}
	Figure::setPen(pen_, mode);
}