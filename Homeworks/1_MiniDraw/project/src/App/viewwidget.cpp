#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;	shape_ = NULL;	type_ = shape::kDefault;
}

ViewWidget::~ViewWidget()
{

}

void ViewWidget::setLine()
{
	type_ = shape::kLine;
}

void ViewWidget::setRect()
{
	type_ = shape::kRect;
}

void ViewWidget::mousePressEvent(QMouseEvent *event){			if (Qt::LeftButton == event->button())	{		switch (type_)		{case shape::kLine:
			shape_ = new Line();
			break;		case shape::kDefault:
			break;
		
		case shape::kRect:
			shape_ = new Rect();
			break;		}		if (shape_ != NULL)		{			draw_status_ = true;			start_point_ = end_point_ = event->pos();			shape_->set_start(start_point_);			shape_->set_end(end_point_);		}	}	update();}void ViewWidget::mouseMoveEvent(QMouseEvent *event){	if (draw_status_&&shape_!=NULL)	{		end_point_ = event->pos();		shape_->set_end(end_point_);	}	}void ViewWidget::mouseReleaseEvent(QMouseEvent *event){	if (shape_!=NULL)	{		draw_status_ = false;		shape_list_.push_back(shape_);		shape_ = NULL;	}}void ViewWidget::paintEvent(QPaintEvent *){	QPainter painter(this);	painter.begin(this);	for (int i = 0; i < shape_list_.size(); i++)	{		shape_list_[i]->Draw(painter);	}	if (shape_!=NULL) {
		shape_->Draw(painter);
	}	update();}
