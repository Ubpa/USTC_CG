#pragma once
#include <QWidget>
#include <QLine>
#include "WarpIDW.h"

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(void);
	~ImageWidget(void);

protected:
	void paintEvent(QPaintEvent *paintevent);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent* event);

public slots:
	// File IO
	void Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	void SetIDW();                                            // Warp image using IDW
	void SetRBF();                                            // warp image using RBF

private:
	Warp        *warp_;
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;
	QPoint      *image_pos_;

	bool warp_set_mode_;
};

