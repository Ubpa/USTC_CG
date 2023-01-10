#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>
#include "ChildWindow.h"

using std::cout;
using std::endl;

ImageWidget::ImageWidget(ChildWindow* relatewindow)
{

	draw_status_ = kNone;
	paste_status_ = pNonePaste;
	is_choosing_ = false;
	is_pasting_ = false;

	point_start_ = QPoint(0, 0);
	point_end_ = QPoint(0, 0);

	source_window_ = NULL;

	inside_mask_.resize(0, 0);
	shape_ = nullptr;
	poisson_ = nullptr;
	scanline_ = nullptr;
}

ImageWidget::~ImageWidget(void) {
	image_mat_.release();
	image_mat_backup_.release();
	image_mat_last_.release();
	if (shape_ != NULL)
	{
		delete shape_;
	}
	if (poisson_ != NULL) {
		delete poisson_;
	}

	if (scanline_ != NULL) {
		delete scanline_;
	}
}

int ImageWidget::ImageWidth()
{
	return image_mat_.cols;
}

int ImageWidget::ImageHeight()
{
	return image_mat_.rows;
}

void ImageWidget::set_draw_status_to_choose()
{
	draw_status_ = kChoose;
}

void ImageWidget::set_draw_status_to_paste()
{
	draw_status_ = kPaste;
}


void ImageWidget::set_normal_paste()
{
	paste_status_ = pNormal;
}

void ImageWidget::set_poisson_paste()
{
	paste_status_ = pPoisson;
}

const cv::Mat& ImageWidget::image()
{
	return image_mat_;
}

void ImageWidget::set_source_window(ChildWindow* source)
{
	source_window_ = source;
}

void ImageWidget::paintEvent(QPaintEvent* paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QImage image_((unsigned char*)(image_mat_.data), image_mat_.cols, image_mat_.rows, image_mat_.step, QImage::Format_RGB888);
	QRect rect = QRect(0, 0, image_.width(), image_.height());
	painter.drawImage(rect, image_);

	// Draw choose region
	painter.setBrush(Qt::NoBrush);
	painter.setPen(Qt::red);
	if (shape_ != nullptr)
	{
		shape_->Draw(painter);
	}

	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* mouseevent)
{
	if (Qt::LeftButton == mouseevent->button())
	{
		switch (draw_status_)
		{
		case kChoose:
			is_choosing_ = true;
			point_start_ = point_end_ = mouseevent->pos();
			shape_->set_start(point_start_);
			shape_->set_end(point_end_);
			if (shape_->type_ == Shape::kPolygon)
			{
				shape_->update(1);
			}
			break;

		case kPaste:
		{
			is_pasting_ = true;

			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->point_start_.rx();
			int ysourcepos = source_window_->imagewidget_->point_start_.ry();

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->point_end_.rx()
				- source_window_->imagewidget_->point_start_.rx() + 1;
			int h = source_window_->imagewidget_->point_end_.ry()
				- source_window_->imagewidget_->point_start_.ry() + 1;

			inside_mask_ = source_window_->imagewidget_->inside_mask_;


			// Paste
			if ((xpos + w < image_mat_.cols) && (ypos + h < image_mat_.rows))
			{
				// Restore image
			//	*(image_) = *(image_backup_);

				ImageWidget* source_img_wgt = source_window_->imagewidget_;
				QPoint source_start = source_img_wgt->scanline_->get_start();
				cv::Mat source_mat = source_img_wgt->image_mat_;
				// Paste
				switch (paste_status_)
				{
				case pNormal:
					source_img_wgt->poisson_->NormalPaste(mouseevent->pos(), source_start, image_mat_, source_mat);
					break;
				case pPoisson:
					source_img_wgt->poisson_->PoissonPaste(mouseevent->pos(), source_start, image_mat_, source_mat);
					break;
				default:
					break;
				}
			}
		}

		update();
		break;

		default:
			break;
		}
	}
	if (Qt::RightButton == mouseevent->button())
	{
		if (draw_status_ == kChoose && shape_->type_ == Shape::kPolygon)
		{
			is_choosing_ = false;
			draw_status_ = kNone;
			
			shape_->update(0);
			scanline_ = new ScanLine(shape_->get_polygon());
			poisson_ = new Poisson(scanline_->inside_mask_);

			update();
		}

	}
}

void ImageWidget::mouseMoveEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
	case kChoose:
		// Store point position for rectangle region
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			shape_->set_end(point_end_);
		}
		break;

	case kPaste:
		// Paste rectangle region to object image
		if (is_pasting_)
		{
			// Start point in object image
			int xpos = mouseevent->pos().rx();
			int ypos = mouseevent->pos().ry();

			// Start point in source image
			int xsourcepos = source_window_->imagewidget_->point_start_.rx();
			int ysourcepos = source_window_->imagewidget_->point_start_.ry();

			// Width and Height of rectangle region
			int w = source_window_->imagewidget_->point_end_.rx()
				- source_window_->imagewidget_->point_start_.rx() + 1;
			int h = source_window_->imagewidget_->point_end_.ry()
				- source_window_->imagewidget_->point_start_.ry() + 1;

			// Paste
			if ((xpos > 0) && (ypos > 0) && (xpos + w < image_mat_.cols) && (ypos + h < image_mat_.rows))
			{
				// Restore image 
				image_mat_ = image_mat_last_.clone();
				ImageWidget* source_img_wgt = source_window_->imagewidget_;
				QPoint source_start = source_img_wgt->scanline_->get_start();
				cv::Mat source_mat = source_img_wgt->image_mat_;
				// Paste
				switch (paste_status_)
				{
				case pNormal:
					source_img_wgt->poisson_->NormalPaste(mouseevent->pos(), source_start, image_mat_, source_mat);
					break;
				case pPoisson:
					source_img_wgt->poisson_->PoissonPaste(mouseevent->pos(), source_start, image_mat_, source_mat);
					break;
				default:
					break;
				}

			}
		}

	default:
		break;
	}

	update();
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* mouseevent)
{
	switch (draw_status_)
	{
		
	case kChoose:
		if (is_choosing_)
		{
			point_end_ = mouseevent->pos();
			if (shape_->type_ == Shape::kRect)
			{

				is_choosing_ = false;
				draw_status_ = kNone;
				scanline_ = new ScanLine(point_start_, point_end_);
				poisson_ = new Poisson(scanline_->inside_mask_);

			}
		}
		break;
	case kPaste:
		if (is_pasting_) {
			is_pasting_ = false;
			draw_status_ = kNone;
			image_mat_last_ = image_mat_.clone();
		}
		break;
	default:
		break;
	}

	update();
}

void ImageWidget::Open(QString filename)
{
	// Load file
	if (!filename.isEmpty())
	{
		image_mat_ = cv::imread(filename.toStdString());
		cv::cvtColor(image_mat_, image_mat_, cv::COLOR_BGR2RGB);
		image_mat_backup_ = image_mat_.clone();
		image_mat_last_ = image_mat_.clone();
		std::cout << image_mat_.cols << " " << image_mat_.rows << " " << image_mat_.step << std::endl;
	}

	//	setFixedSize(image_->width(), image_->height());
	//	relate_window_->setWindowFlags(Qt::Dialog);
	//	relate_window_->setFixedSize(QSize(image_->width(), image_->height()));
	//	relate_window_->setWindowFlags(Qt::SubWindow);

		//image_->invertPixels(QImage::InvertRgb);
		//*(image_) = image_->mirrored(true, true);
		//*(image_) = image_->rgbSwapped();
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

	cv::Mat img_save = image_mat_.clone();
	cv::cvtColor(img_save, img_save, cv::COLOR_RGB2BGR);
	cv::imwrite(filename.toStdString(), img_save);
}

void ImageWidget::Invert()
{
	for (int i = 0; i < image_mat_.rows; i++)
	{
		cv::Vec3b* p = image_mat_.ptr<cv::Vec3b>(i);
		for (int j = 0; j < image_mat_.cols; j++)
		{
			p[j][0] = 255 - p[j][0];
			p[j][1] = 255 - p[j][1];
			p[j][2] = 255 - p[j][2];
		}
	}
	image_mat_last_ = image_mat_.clone();
	// equivalent member function of class QImage
	// image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	cv::flip(image_mat_, image_mat_, 1);
	image_mat_last_ = image_mat_.clone();
	update();
}

void ImageWidget::TurnGray()
{
	for (int i = 0; i < image_mat_.rows; i++)
	{
		cv::Vec3b* p = image_mat_.ptr<cv::Vec3b>(i);
		for (int j = 0; j < image_mat_.cols; j++)
		{
			unsigned char gray = (p[j][0] + p[j][1] + p[j][2]) / 3;
			p[j] = cv::Vec3b(gray, gray, gray);
		}
	}
	image_mat_last_ = image_mat_.clone();
	update();
}

void ImageWidget::Restore()
{
	image_mat_ = image_mat_backup_.clone();
	image_mat_last_ = image_mat_backup_.clone();
	point_start_ = point_end_ = QPoint(0, 0);
	update();
}
