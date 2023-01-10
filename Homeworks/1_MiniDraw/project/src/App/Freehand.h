#pragma once

#include"figure.h"

class Freehand :public Figure {
public:
	Freehand():Figure(kFreehand) {};
	~Freehand() {};

	void Draw(QPainter&);
};
#pragma once
