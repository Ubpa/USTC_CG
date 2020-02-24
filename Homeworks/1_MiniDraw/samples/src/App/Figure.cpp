#include "Figure.h"

#include <cmath>

Figure::Figure() {
	linePen = nullptr;

	pointPen = new QPen();
	pointPen->setWidth(10);
	pointPen->setColor(QColor(0xED, 0x1C, 0x24));
}

Figure::~Figure() {
	delete pointPen;
	delete linePen;
}

void Figure::update(const QPoint _end_point) {
}

std::vector<QPoint*> Figure::getPoint(QPoint& pos, bool all) {
	return std::vector<QPoint*>();
}

bool Figure::isClose(QPoint& p1, QPoint& p2, int d) {
	return (std::abs(p1.x() - p2.x()) > d ? false
		: (std::abs(p1.y() - p2.y()) > d ? false
			: true));
}

void Figure::setPen(const QPen* pen_, const int mode) {
	if (mode & PEN::LINE) {
		delete linePen;
		linePen = (pen_ == nullptr ? nullptr : new QPen(*pen_));
	}
	if (mode & PEN::POINT) {
		delete pointPen;
		pointPen = (pen_ == nullptr ? nullptr : new QPen(*pen_));
	}
}

void Figure::setCanDrag(bool canDrag_) {
	canDrag = canDrag_;
}