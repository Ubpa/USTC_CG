#pragma once
#include <QWidget>


#include <vector>
#include <string>
#include <memory>

#include <Eigen/Eigen>

#include "IDW.h"
#include "RBF.h"

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public :
	enum Mode {
		IDW = 0,
		RBF = 1,
	};

public:
	ImageWidget(void);
	~ImageWidget(void);

protected:
	void paintEvent(QPaintEvent* paintevent);
	void mousePressEvent(QMouseEvent* mouseevent);
	void mouseMoveEvent(QMouseEvent* mouseevent);
	void mouseReleaseEvent(QMouseEvent* mouseevent);

public slots:
	// File IO
	void Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal = false, bool vertical = true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin

	/// Image Warping
	void Warp();
	void ClearControlPoints();
	void UndoSelect();
	
	void SetWarpingMode(Mode m) {
		warping_method_ = m;
	}
	
	void SetSelectPointsMode(bool status);
	
	/*void SetRealTimeWarpingMode(bool status) {
		realtime_warping_mode_ = status;
	}*/
//private:
//	void InitWarpingInstanceMap();

private://prive member
	
	//IDW and RBF mode
	Mode warping_method_;
	//base warping class
	BaseImageWarping* image_warping_;
	// image 
	QImage* ptr_image_;				
	QImage* ptr_image_backup_;

	bool is_drawing_ = false;
	bool select_mode_ = false;
	//bool realtime_warping_mode_ = false;
	
	//Point
	QPoint point_start_;
	QPoint point_end_;

	//source and target points
	std::vector<Eigen::Vector2f> source_points_;
	std::vector<Eigen::Vector2f> target_points_;



};

