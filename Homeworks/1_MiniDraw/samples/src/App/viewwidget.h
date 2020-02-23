#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include <QWidget>
#include "ui_viewwidget.h"
#include <vector>
#include "Figure.h"
#include "Line.h"
#include "Ellipse.h"
#include "Rect.h"
#include "Polygon.h"
#include "FreeHand.h"
#include "BezierCurve.h"

enum class DrawMode
{
	kDefault = 0,
	kLine = 1,
	kRectangle = 2,
	kEllipse = 3,
	kPolygon = 4,
	kFreehand = 5,
	kBezierCurve = 6,
	kDrag = 7,
};


class ViewWidget : public QWidget {
	Q_OBJECT

public:
	ViewWidget(QWidget* parent = 0);
	~ViewWidget();

	void mousePressEvent(QMouseEvent* mouseEvent);
	void mouseMoveEvent(QMouseEvent* mouseEvent);
	void mouseReleaseEvent(QMouseEvent* mouseEvent);
	void paintEvent(QPaintEvent*);

	void set_drawMode_to_line();
	void set_drawMode_to_rectangle();
	void set_drawMode_to_ellipse();
	void set_drawMode_to_polygon();
	void set_drawMode_to_freehand();
	void set_drawMode_to_bezierCurve();
	void switch_drag();
	void set_color();
	void set_width();
	void undo();

private:
	Ui::ViewWidget ui;
	std::vector<Figure*> figureVec;
	DrawMode drawMode, dM_backup;
	Figure* current_figure;
	QPoint* pointHovered;
	std::vector<QPoint*> dragPoints;
	bool isDrawing;
	bool canDrag;
	bool isDragAll;
	QPen linePen;
};

#endif // VIEWWIDGET_H
