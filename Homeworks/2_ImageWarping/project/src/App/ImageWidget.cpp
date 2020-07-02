#include "ImageWidget.h"
#include "WarpIDW.h"

#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>

using std::cout;
using std::endl;

ImageWidget::ImageWidget(void) : warp_set_mode_(false), warp_(nullptr)
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	image_pos_ = new QPoint(0, 0);
}


ImageWidget::~ImageWidget(void)
{
}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	painter.begin(this);

	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QRect rect = QRect(image_pos_->x(), image_pos_->y(), ptr_image_->width(), ptr_image_->height());
	painter.drawImage(rect, *ptr_image_); 

	if (warp_set_mode_)
	{
		warp_->DrawControlPoints(&painter, image_pos_);
	}

	painter.end();
}

void ImageWidget::mousePressEvent(QMouseEvent* event)
{
	if (warp_set_mode_ && Qt::LeftButton == event->button())
	{
		warp_->SetBeginPoint(event->pos() - (*image_pos_));
	}

	if (warp_set_mode_ && Qt::RightButton == event->button())
	{
		warp_set_mode_ = false;
		warp_->Render(ptr_image_);

		if (!warp_)
		{
			delete warp_;
			warp_ = nullptr;
		}

		update();	//clean control points and render
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (Qt::LeftButton == event->button() && warp_set_mode_)
	{
		warp_->SetEndPoint(event->pos() - (*image_pos_));
		update();	//refresh control points
	}
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
		image_pos_ = new QPoint((width() - ptr_image_->width()) / 2,
			(height() - ptr_image_->height()) / 2);
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
	for (int i = 0; i < ptr_image_->width(); i++)
	{
		for (int j = 0; j < ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255 - qRed(color), 255 - qGreen(color), 255 - qBlue(color)));
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
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width - 1 - i, height - 1 - j));
				}
			}
		}
		else
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height - 1 - j));
				}
			}
		}

	}
	else
	{
		if (isvertical)
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width - 1 - i, j));
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
	for (int i = 0; i < ptr_image_->width(); i++)
	{
		for (int j = 0; j < ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color) + qGreen(color) + qBlue(color)) / 3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value));
		}
	}

	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	update();
}

void ImageWidget::SetIDW()
{
	warp_set_mode_ = true;

	if (!warp_)
	{
		delete warp_;
		warp_ = nullptr;
	}

	warp_ = new WarpIDW;
}

void ImageWidget::SetRBF()
{
	
}