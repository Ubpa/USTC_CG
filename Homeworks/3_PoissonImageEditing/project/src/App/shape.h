#pragma once
#include <QtGui>
#include "Poisson.h"

class Shape
{
public:
	Shape();
	virtual ~Shape();
	void set_start(QPoint s);
	void set_end(QPoint e);
	QPoint get_start();
	QPoint get_end();
	virtual void Draw(QPainter& paint) = 0;
	virtual void update(int mode) {}
	virtual QPolygon get_polygon() { QPolygon polygon; return polygon; }

public:
	enum Type
	{
		kDefault,
		kRect,
		kEllipse,
		kPolygon,
		kFreedraw,
	};
	Type type_;

protected:
	QPoint start;
	QPoint end;

};