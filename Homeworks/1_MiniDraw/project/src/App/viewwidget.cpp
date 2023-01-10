#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	draw_status_ = Sdone;
	figure_ = NULL;
	type_ = Figure::kDefault;
}

ViewWidget::~ViewWidget() {
	for (int i = 0; i < figure_list_.size(); i++) {
		delete figure_list_[i];
	}
}
void ViewWidget::Undo() {
	if (figure_list_.size() != 0) {
		Figure* f = figure_list_.back();
		figure_list_.pop_back();
		delete f;
	}
}

void ViewWidget::Clean() {
	Figure* f;
	while (figure_list_.size()>0) {
		f = figure_list_.back();
		figure_list_.pop_back();
		delete f;
	}
}


void ViewWidget::setLine() {
	type_ = Figure::kLine;
}

void ViewWidget::setRect() {
	type_ = Figure::kRect;
}

void ViewWidget::setEllip() {
	type_ = Figure::kEllipse;
}

void ViewWidget::setPoly() {
	type_ = Figure::kPolygon;
}

void ViewWidget::setFree() {
	type_ = Figure::kFreehand;
}

//只有当鼠标按下时才会触发(类似给了一个脉冲)，如果一直按下，一直处于“正脉冲”，则不会触发
void ViewWidget::mousePressEvent(QMouseEvent* event) {
	if (Qt::LeftButton == event->button()) {//判断按下的是否是左键
		if (draw_status_ == Sdone) {//如果之前已发出结束指令，则开始新的作画
			switch (type_) {
			case Figure::kDefault:
				figure_ = NULL;
				break;
			case Figure::kLine:
				figure_ = new Line();
				break;
			case Figure::kRect:
				figure_ = new Rect();
				break;
			case Figure::kEllipse:
				figure_ = new Ellip();
				break;
			case Figure::kPolygon:
				figure_ = new Poly();
				break;
			case Figure::kFreehand:
				figure_ = new Freehand();
				break;
			}

			if (figure_ != NULL) {
				draw_status_ = Sdraw;
				start_point_ = end_point_ = event->pos();
				figure_->set_start(start_point_);
				figure_->push_point(start_point_);
				figure_->set_end(end_point_);
				figure_->push_point(end_point_);
				if (type_ == Figure::kPolygon) {
					//这里设置了鼠标跟踪的响应参数，设置为True是使得只要鼠标有按下(无论是否松开)，则会一直跟踪鼠标位置，
					//与mouseMoveEvent事件相关联
					figure_->set_type(Figure::kPolyline);
					this->setMouseTracking(true);
				}
			}
		}
		//对于Polygen类，当再次触发左键按下操作时，处于Draw状态，则要记录下当前的点
		else if (draw_status_ == Sdraw) {
			if (type_ == Figure::kPolygon) {
				end_point_ = event->pos();
				figure_->set_end(end_point_);
				figure_->push_point(end_point_);
			}
		}
	}
	//当是右键按下时，则标志Polygen绘图的结束
	else if (Qt::RightButton == event->button()) {
		if (type_ == Figure::kPolygon) {
			draw_status_ = Sdone;
			figure_->set_type(Figure::kPolygon);
		}
	}
	update();
}

//这里是鼠标移动时触发的事件
void ViewWidget::mouseMoveEvent(QMouseEvent* event) {
	if (figure_ != NULL) {
		if (draw_status_ == Sdraw) {
			if (type_ != Figure::kFreehand) {
				end_point_ = event->pos();
				figure_->set_end(end_point_);
			}
			else {
				cur_point_ = event->pos();
				if (end_point_.x() == cur_point_.x() && start_point_.x() == cur_point_.x()) {
					end_point_ = cur_point_;
					figure_->set_end(end_point_);
				}
				else if (end_point_.y() == cur_point_.y() && start_point_.y() == cur_point_.y()) {
					end_point_ = cur_point_;
					figure_->set_end(end_point_);
				}
				else {
					start_point_ = end_point_ = cur_point_;
					figure_->set_end(cur_point_);
					figure_->push_point(cur_point_);
				}
			}
		}
		else if (draw_status_ == Sdone) {
			//设置为false:代表着只有当鼠标按下时才会跟踪位置，松开后则不再跟踪
			this->setMouseTracking(false);
		}
	}
}

//这里是鼠标松开时触发的事件
void ViewWidget::mouseReleaseEvent(QMouseEvent* event) {
	if (figure_ != NULL) {//对于Polygon类型，当右键鼠标松开时才代表绘图结束，左键松开没有状态的改变
		if (type_ == Figure::kPolygon) {
			if (Qt::RightButton == event->button()) {
				draw_status_ = Sdone;
				figure_list_.push_back(figure_);
				figure_ = NULL;
			}
		}
		else {//其他情况默认是键松开结束
			draw_status_ = Sdone;
			figure_list_.push_back(figure_);
			figure_ = NULL;
		}
	}
}

void ViewWidget::paintEvent(QPaintEvent* event) {
	QPainter painter(this);

	for (int i = 0; i < figure_list_.size(); i++) {
		figure_list_[i]->Draw(painter);
	}


	if (figure_ != NULL) {
		figure_->Draw(painter);
	}

	update();
}