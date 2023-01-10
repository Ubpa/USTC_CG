#include "Line.h"

Line::Line():Figure(kLine){
}


Line::~Line() {
}

void Line::Draw(QPainter& painter) {
	painter.drawLine(start, end);
}
