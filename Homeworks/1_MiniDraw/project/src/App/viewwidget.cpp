#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;
	shape_ = NULL;
	type_ = Shape::kDefault;
}

ViewWidget::~ViewWidget()
{
	for (size_t i = 0; i < shape_list_.size(); i++)
	{
		if (shape_list_[i])
		{
			delete shape_list_[i];
			shape_list_[i] = NULL;
		}
	}
}

void ViewWidget::setLine()
{
	type_ = Shape::kLine;
}

void ViewWidget::setRect()
{
	type_ = Shape::kRect;
}

void ViewWidget::setEllip()
{
	type_ = Shape::kEllip;
}

void ViewWidget::setPolyg()
{
	type_ = Shape::kPolyg;
}

void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		switch (type_)
		{
		case Shape::kLine:
			shape_ = new Line();
			break;

		case Shape::kRect:
			shape_ = new Rect();
			break;

		case Shape::kEllip:
			shape_ = new Ellip();
			break;

		case Shape::kPolyg:
			shape_ = new Polyg();
			break;

		case Shape::kDefault:
			break;
		}
		if (shape_ != NULL)
		{
			draw_status_ = true;
			start_point_ = end_point_ = event->pos();
			shape_->set_start(start_point_);
			shape_->set_end(end_point_);
		}
	}

	if (Qt::RightButton == event->button() && Shape::kPolyg == type_)
	{
		shape_->set_mid(event->pos());
	}

	update();	//QPaintEvent triggered
}

void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (draw_status_ && shape_ != NULL)
	{
		end_point_ = event->pos();
		shape_->set_end(end_point_);
	}
}

void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		if (shape_ != NULL)
		{
			draw_status_ = false;
			shape_list_.push_back(shape_);
			shape_ = NULL;
		}
	}
}

void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);

	//draw shapes that have been drawn
	for (int i = 0; i < shape_list_.size(); i++)
	{
		shape_list_[i]->Draw(painter);
	}

	//draw the shape that is being drawn
	if (shape_ != NULL) {
		shape_->Draw(painter);
	}

	update();	//keep QPaintEvent triggered
}