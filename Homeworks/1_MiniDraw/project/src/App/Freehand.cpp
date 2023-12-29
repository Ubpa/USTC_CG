#include"Freehand.h"

void Freehand::Draw(QPainter& painter) {
	int s = size;
	for (int i = 0; i < s - 1; i++) {
		painter.drawLine(ps[i], ps[i + 1]);
	}
}