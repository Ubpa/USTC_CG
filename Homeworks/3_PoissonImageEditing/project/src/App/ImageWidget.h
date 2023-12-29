#pragma once
#include <QWidget>
#include "shape.h"
#include "Rect.h"
#include "Polygon.h"
#include "ScanLine.h"
#include "Poisson.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <Eigen/Eigen>
class ChildWindow;
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

enum DrawStatus
{
	kChoose,
	kPaste,
	kNone
};

enum ChooseStatus
{
	kRect,
	kPoly,
	kNoneChoose
};

enum PasteStatus
{
	pNormal,
	pPoisson,
	pNonePaste
};

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(ChildWindow* relatewindow);
	~ImageWidget(void);

	int ImageWidth();											// Width of image
	int ImageHeight();											// Height of image
	void set_draw_status_to_choose();
	void set_draw_status_to_paste();
	void set_normal_paste();
	void set_poisson_paste();
	const cv::Mat& image();
	void set_source_window(ChildWindow* childwindow);

protected:
	void paintEvent(QPaintEvent* paintevent);
	void mousePressEvent(QMouseEvent* mouseevent);
	void mouseMoveEvent(QMouseEvent* mouseevent);
	void mouseReleaseEvent(QMouseEvent* mouseevent);

public slots:
	// File IO
	void Open(QString filename);								// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal = false, bool vertical = true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin

public:
	QPoint						point_start_;					// Left top point of rectangle region
	QPoint						point_end_;						// Right bottom point of rectangle region
	Shape*						shape_;
private:
	cv::Mat						image_mat_;
	cv::Mat						image_mat_backup_;
	cv::Mat						image_mat_last_;
	Eigen::MatrixXi				inside_mask_;

	// Pointer of child window
	ChildWindow* source_window_;				// Source child window

	// Signs
	DrawStatus					draw_status_;					// Enum type of draw status
	PasteStatus					paste_status_;					// Enum type of paste status
	bool						is_choosing_;
	bool						is_pasting_;

	//Poisson Image
	Poisson*					poisson_;
	ScanLine*					scanline_;

};

