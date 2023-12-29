#include "Poly.h"

Poly::Poly() :Figure(kPolygon) {

}

Poly::~Poly() {

}

void Poly::Draw(QPainter& painter) {
	int s = size;
	if (type == kPolyline) {
		for (int i = 0; i < s - 1; i++) {		
			painter.drawLine(ps[i],ps[i+1]);
		}
	}
	else if (type == kPolygon) {
		painter.drawPolygon(ps, s);
	}
}

