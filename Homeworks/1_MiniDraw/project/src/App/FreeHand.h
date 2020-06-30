#pragma once

#include "Shape.h"

#include <vector>

class FreeHand : public Shape
{
public:
	FreeHand();
	~FreeHand();

	void Draw(QPainter &painter);
	void set_mid(QPoint m);

private:
	std::vector<QPoint> points_;
};

