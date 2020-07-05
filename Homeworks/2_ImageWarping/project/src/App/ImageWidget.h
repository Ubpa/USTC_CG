#pragma once
#include <QWidget>
#include "WarpIDW.h"
#include "WarpRBF.h"

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
	void mouseMoveEvent(QMouseEvent* event);
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
	void SetIDW();                                              // Warp image using IDW
	void SetRBF();                                              // warp image using RBF
	void SetWarpDrawMode(int state);                            // control the visibility of control points

private:
	Warp        *warp_;
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;
	QPoint      *image_pos_;

	bool warp_set_mode_;                    // set control points
	bool warp_draw_mode_;                   // draw control points

	QVector<QPoint> begin_points_buffer_;
	QVector<QPoint> end_points_buffer_;

	void DrawControlPoints(QPainter* painter);
};

