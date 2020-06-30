#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;
	currentShape_ = NULL;
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

void ViewWidget::setFreeHand()
{
	type_ = Shape::kFreeHand;
}

void ViewWidget::setColor()
{
	QColor color = QColorDialog::getColor();
	currentPen_.setColor(color);
}

void ViewWidget::setWidth()
{
	int width = QInputDialog::getInt(this, "width setting", "input width", 0, 0, 50);
	currentPen_.setWidth(width);
}

void ViewWidget::Undo()
{
	if (shape_list_.empty())
		return;
	else
	{
		delete shape_list_.back();
		shape_list_.pop_back();
	}
}

void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		switch (type_)
		{
		case Shape::kLine:
			currentShape_ = new Line();
			break;

		case Shape::kRect:
			currentShape_ = new Rect();
			break;

		case Shape::kEllip:
			currentShape_ = new Ellip();
			break;

		case Shape::kPolyg:
			currentShape_ = new Polyg();
			break;

		case Shape::kFreeHand:
			currentShape_ = new FreeHand();
			break;

		case Shape::kDefault:
			break;
		}
		if (currentShape_ != NULL)
		{
			draw_status_ = true;
			start_point_ = end_point_ = event->pos();
			currentShape_->set_start(start_point_);
			currentShape_->set_end(end_point_);
			currentShape_->set_pen(currentPen_);
		}
	}

	if (Qt::RightButton == event->button() && Shape::kPolyg == type_)
	{
		currentShape_->set_mid(event->pos());
	}

	update();	//QPaintEvent triggered
}

void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (draw_status_ && type_ == Shape::kFreeHand)
		currentShape_->set_mid(event->pos());

	if (draw_status_ && currentShape_ != NULL)
	{
		end_point_ = event->pos();
		currentShape_->set_end(end_point_);
	}
}

void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button())
	{
		if (currentShape_ != NULL)
		{
			draw_status_ = false;
			shape_list_.push_back(currentShape_);
			currentShape_ = NULL;
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
	if (currentShape_ != NULL) {
		currentShape_->Draw(painter);
	}

	update();	//keep QPaintEvent triggered
}