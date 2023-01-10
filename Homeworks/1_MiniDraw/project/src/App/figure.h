#pragma once

#include <QtGui>
#include <QDebug>
#include <vector>
#include <iostream>
class Figure {
public:
	enum Type {
		kDefault = 0,
		kLine = 1,
		kRect = 2,
		kEllipse = 3,
		kPolygon = 4,
		kPolyline = 5,
		kFreehand = 6,
	};
	
public:
	Figure();
	Figure(Type t);
	virtual ~Figure();
	virtual void Draw(QPainter& paint) = 0;
	
	void set_start(QPoint s);
	void set_end(QPoint e);
	void set_type(Type t);
	void push_point(QPoint p);
	QPoint get_start_point();
	QPoint get_end_point();


protected:
	int size;
	int capacity ;
	Type type;
	QPoint start;
	QPoint end;
	QPoint* ps;
};

