#pragma once

#include "Warp.h"

#include <QVector2D>
#include <QWidget>
#include <cmath>
#include <Eigen/Dense>

using namespace Eigen;

class WarpRBF : public Warp
{
public:
	WarpRBF();
	~WarpRBF() = default;
	void Render(QImage* ptr_image);

protected:
	QPoint Output(QPoint p);

private:
	float RBF(QPoint p, int i);
	void CalculateR();
	void CalculateA();

	float u_;
	QVector<float> r_;	// pre calculate r 
	QVector<float> ax_; // pre calculate ax
	QVector<float> ay_; // pre calculate ay
};
