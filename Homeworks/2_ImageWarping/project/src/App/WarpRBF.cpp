#include "WarpRBF.h"

#include <iostream>

WarpRBF::WarpRBF() : u_(1.0f)
{
}

QPoint WarpRBF::Output(QPoint p)
{
	float xo = p.x(), yo = p.y();
	for (int i = 0; i < begin_points_.size(); i++)
	{
		xo += ax_[i] * RBF(p, i);
		yo += ay_[i] * RBF(p, i);
	}
	return QPoint(xo, yo);
}

float WarpRBF::RBF(QPoint p, int i)
{
	QVector2D vec(p.x() - begin_points_[i].x(), p.y() - begin_points_[i].y());
	return powf(vec.lengthSquared() + r_[i] * r_[i], u_ / 2.0f);
}

void WarpRBF::CalculateR()
{
	for (int i = 0; i < begin_points_.size(); i++)
	{
		float min_r_i = 99999;
		for (int j = 0; j < begin_points_.size(); j++)
		{
			if (i != j)
			{
				QVector2D vec(begin_points_[i].x() - begin_points_[j].x(),
					begin_points_[i].y() - begin_points_[j].y());

				if (vec.length() < min_r_i)
					min_r_i = vec.length();
			}
			r_.push_back(min_r_i);
		}
	}
}

void WarpRBF::CalculateA()
{
	int n = begin_points_.size();

	MatrixXf bx(n, 1), by(n, 1);

	for (int i = 0; i < n; i++)
	{
		bx(i, 1) = end_points_[i].x() - begin_points_[i].x();
		by(i, 1) = end_points_[i].y() - begin_points_[i].y();
	}

	MatrixXf A(n, n);

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			A(i, j) = RBF(begin_points_[i], j);
		}
	}

	MatrixXf ax = A.colPivHouseholderQr().solve(bx);
	MatrixXf ay = A.colPivHouseholderQr().solve(by);

	std::cout << ax << ay;

	for (int i = 0; i < n; i++)
	{
		ax_.push_back(ax(i, 1));
		ay_.push_back(ay(i, 1));
	}
}

void WarpRBF::Render(QImage* ptr_image)
{
	QImage image_tmp(*(ptr_image));

	int width = ptr_image->width();
	int height = ptr_image->height();

	ptr_image->fill(Qt::lightGray);

	CalculateR();
	CalculateA();

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			QPoint p(i, j);
			QPoint po = Output(p);

			if (po.x() >= 0 && po.x() < width && po.y() >= 0 && po.y() < height)
				ptr_image->setPixel(Output(p), image_tmp.pixel(p));

			//FixHole(ptr_image);
		}
	}
}
