#include "Freehand.h"

Freehand::Freehand(const QPoint _start_point) {
	path = new QPainterPath();
	start_point = _start_point;
}

Freehand::~Freehand() {
	delete path;
}

void Freehand::update(const QPoint _end_point) {
	if (path->elementCount() == 0)
		path->moveTo(_end_point);
	else
		path->lineTo(_end_point);
}

void Freehand::Draw(QPainter& paint) {
	QPen oldPen = paint.pen();
	if (linePen != nullptr)
		paint.setPen(*linePen);

	if (start_point != old_start_point) {
		QPoint diff = start_point - old_start_point;
		QPainterPath* newPath = new QPainterPath();
		newPath->moveTo(start_point);
		for (int i = 0; i < path->elementCount(); i++) {
			int x = path->elementAt(i).x + diff.x();
			int y = path->elementAt(i).y + diff.y();
			newPath->lineTo(QPoint(x, y));
		}
		old_start_point = start_point;
		delete path;
		path = newPath;
	}

	paint.drawPath(*path);

	if (canDrag) {
		if (pointPen != nullptr)
			paint.setPen(*pointPen);
		paint.drawPoint(start_point);
	}

	paint.setPen(oldPen);
}

std::vector<QPoint*> Freehand::getPoint(QPoint& pos, bool all) {
	std::vector<QPoint*> rst;
	if (isClose(start_point, pos, 5))
		rst.push_back(&start_point);

	return rst;
}