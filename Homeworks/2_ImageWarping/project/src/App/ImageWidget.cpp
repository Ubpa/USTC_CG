#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>

using std::cout;
using std::endl;
//public:
ImageWidget::ImageWidget(void)
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	image_warping_ = nullptr;
	warping_method_ = IDW;
}


ImageWidget::~ImageWidget(void)
{
}

//protected:
void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QRect rect = QRect( (width()-ptr_image_->width())/2, (height()-ptr_image_->height())/2, ptr_image_->width(), ptr_image_->height());
	painter.drawImage(rect, *ptr_image_); 

	if (select_mode_) {
		QPen pen(Qt::red, 4);
		painter.setPen(pen);
		const int num_ctrl_pts = source_points_.size();
		for (int i = 0; i < num_ctrl_pts; ++i) {
			QPoint p1(static_cast<int>(source_points_[i][0]),
				static_cast<int>(source_points_[i][1]));
			QPoint p2(static_cast<int>(target_points_[i][0]),
				static_cast<int>(target_points_[i][1]));
			painter.drawLine(p1, p2);
		}
		if (is_drawing_) {
			painter.drawLine(point_start_, point_end_);
		}
	}


	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* mouseevent)
{
	if (select_mode_ && mouseevent->button() == Qt::LeftButton) {
		is_drawing_ = true;
		auto pt = mouseevent->pos();
		point_start_ = pt;
		point_end_ = pt;
		update();
	}
	
}

void ImageWidget::mouseMoveEvent(QMouseEvent* mouseevent)
{
	if (select_mode_ && is_drawing_) {
		point_end_ = mouseevent->pos();
		update();
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* mouseevent)
{
	if (select_mode_ && is_drawing_) {
		Eigen::Vector2f pt;
		pt[0] = point_start_.x();
		pt[1] = point_start_.y();
		source_points_.push_back(pt);

		pt[0] = point_end_.x();
		pt[1] = point_end_.y();
		target_points_.push_back(pt);

		printf("start:(%d, %d), end: (%d, %d)\n", point_start_.x(), point_start_.y(), point_end_.x(), point_end_.y());
		is_drawing_ = false;
		update();
	}
}

//public slots:


void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
	}

	//ptr_image_->invertPixels(QImage::InvertRgb);
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	//*(ptr_image_) = ptr_image_->rgbSwapped();
	cout<<"image size: "<<ptr_image_->width()<<' '<<ptr_image_->height()<<endl;
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}	

	ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
				}
			}
		} 
		else
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
				}
			}
		}
		
	}
	else
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
				}
			}
		}
	}

	// equivalent member function of class QImage
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	update();
}

void ImageWidget::TurnGray()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
		}
	}

	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	update();
}

void ImageWidget::Warp()
{
	if (image_warping_ != nullptr)
		delete image_warping_;
	int w = (width() - ptr_image_->width()) / 2;
	int h = (height() - ptr_image_->height()) / 2;
	if (warping_method_ == RBF)
		image_warping_ = new RBFImageWarping(w,h);
	else if (warping_method_ == IDW)
		image_warping_ = new IDWImageWarping(w,h);
	image_warping_->SetAnchorPoints(source_points_, target_points_);
	image_warping_->WarpImage(ptr_image_);

	update();
}

void ImageWidget::ClearControlPoints()
{
	source_points_.clear();
	target_points_.clear();

	update();
}

void ImageWidget::UndoSelect()
{
	if (!source_points_.empty() && !target_points_.empty()) {
		source_points_.pop_back();
		target_points_.pop_back();
	}

	update();
}

void ImageWidget::SetSelectPointsMode(bool status)
{
	select_mode_ = status;
	if (!select_mode_) {
		ClearControlPoints();
	}

	update();
}

