#ifndef VIEWWIDGET_H
#define VIEWWIDGET_H

#include "ui_viewwidget.h"

#include "figure.h"
#include "Line.h"
#include "Rect.h"
#include "Ellip.h"
#include "Poly.h"
#include "Freehand.h"

#include <qevent.h>
#include <qpainter.h>
#include <QWidget>
#include <iostream>
#include <vector>
#include <QDebug>
class ViewWidget : public QWidget
{
	Q_OBJECT

public:
	ViewWidget(QWidget* parent = 0);
	~ViewWidget();

private:
	Ui::ViewWidget ui;
private:
	enum Statues {
		Swait = 0,
		Sdraw = 1,
		Sdone = 2
	};
private:
	Statues draw_status_;
	QPoint start_point_;
	QPoint end_point_;
	QPoint cur_point_;
	Figure::Type type_;
	Figure* figure_;
	std::vector<Figure*> figure_list_;



public:
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);
public:
	void paintEvent(QPaintEvent*);
signals:
public slots:
	void setLine();
	void setRect();
	void setEllip();
	void setPoly();
	void setFree();
	void Clean();
	void Undo();
};

#endif // VIEWWIDGET_H
