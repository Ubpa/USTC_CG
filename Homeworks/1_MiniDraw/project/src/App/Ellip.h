#pragma once

#include"figure.h"

class Ellip:public Figure {
public:
	Ellip ();
	~Ellip ();

	void Draw(QPainter& painter);
};

