#pragma once

#include"figure.h"

class Line :public Figure {
public:
	Line();
	~Line();

	void Draw(QPainter& painter);
};

