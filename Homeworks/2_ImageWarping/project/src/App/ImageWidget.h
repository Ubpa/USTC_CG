#pragma once
#include <QWidget>

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

private:
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;
};

