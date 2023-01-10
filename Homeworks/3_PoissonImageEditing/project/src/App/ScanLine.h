#pragma once
# include <QtGui>
#include <Eigen/Dense>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <cfloat>

typedef struct Edge
{
	double x;
	double dx;
	double y_max;
}Edge;

typedef struct EDGE
{
	QPoint up;
	QPoint down;
	Edge eg;
}EDGE;


class ScanLine
{
public:
	ScanLine();
	ScanLine(QPoint start_point, QPoint end_point);
	ScanLine(QPolygon polygon);
	~ScanLine();
public:
	QPoint get_start();
	QPoint get_end();
private:
	void InitPoints(QPoint start_point, QPoint end_point);
	void InitPoints(QPolygon polygon);
	void InitMask();	//	Get inside mask


private:

	void InitEDGEs();
	void InitNET();
	void InitAET();
	void MoveNewEdgeFromNET();
	void RemoveEdges();
	void SortAET();
	void FillScanLine();
	void UpdateDeltaX();
	void FillPolygon();


	
public:
	Eigen::MatrixXi inside_mask_;

private:
	QVector <QPoint> points_;
	EDGE* m_pEDGEs;
	int m_PtsNum;
	int scan_index;
	QList<QVector<Edge>> NET_;	//	New edge table
	QList<QVector<Edge>> AET_;	//	active edge table
	int x_min_, x_max_, y_min_, y_max_;
	int width_, height_;
	int start_x_, start_y_, end_x_, end_y_;

	
};