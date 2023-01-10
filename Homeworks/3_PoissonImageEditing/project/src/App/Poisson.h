#pragma once
#include "ScanLine.h"
#include <QImage>
#include <Eigen/Sparse>
#include<Eigen/IterativeLinearSolvers>

enum Color
{
	Red=0,
	Green=1,
	Blue=2
};

class Poisson {
public:
	Poisson();
	Poisson(const Eigen::MatrixXi&);

	~Poisson();
	void NormalPaste(QPoint paste_point, QPoint source_point, cv::Mat& paste_img_, cv::Mat& source_img_);

	void PoissonPaste(QPoint paste_point, QPoint source_point, cv::Mat& paste_img_, cv::Mat& source_img_);
	void PoissonPaste_beta(QPoint paste_point, QPoint source_point, cv::Mat& paste_img_, cv::Mat& source_img_);

private:
	void PoissonInit();
	void Predecomposition();
	double VecLength(cv::Vec3i vec);
	int GetDivRGB(Eigen::VectorXf&,const int& index, Color c,
					const cv::Mat& target_img, const int& trow, const int& tcol, 
					const cv::Mat& source_img_, const int& srow, const int& scol);
private:
	//cv::Mat source_img_;
	int pixels_num_;
	Eigen::MatrixXi index_Emat_;
	int rows_, cols_;
	Eigen::SparseMatrix<float> coe_sparse_Emat_;
	//适合用于对称正定矩阵的分解求解线性方程操作
	Eigen::SimplicialLLT<Eigen::SparseMatrix<float>> solver;
	Eigen::MatrixXi inside_mask_;


};