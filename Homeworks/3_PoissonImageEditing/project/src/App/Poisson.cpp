#include "Poisson.h"
#include <ctime>
#include <opencv2/core/eigen.hpp>
#include <iostream>
Poisson::Poisson()
{
	pixels_num_ = 0;
	index_Emat_.resize(0, 0);
	coe_sparse_Emat_.resize(0, 0);

}

Poisson::Poisson(const Eigen::MatrixXi& mask)
{
	pixels_num_ = 0;
	index_Emat_.resize(0, 0);
	coe_sparse_Emat_.resize(0, 0);
	inside_mask_ = mask;
	PoissonInit();
}

Poisson::~Poisson()
{

}

void Poisson::PoissonInit()
{
	rows_ = inside_mask_.rows();
	cols_ = inside_mask_.cols();
	index_Emat_.resize(rows_, cols_);
	index_Emat_.setZero();
	pixels_num_ = 0;
	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				index_Emat_(i, j) = pixels_num_;//index_matrix就是用来建立映射关系：i*rows+j
				pixels_num_++;
			}
		}
	}

	coe_sparse_Emat_.resize(pixels_num_, pixels_num_);
	coe_sparse_Emat_.setZero();

	QVector<Eigen::Triplet<float>> coef;
	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				//建立系数矩阵
				int index = index_Emat_(i, j);
				coef.push_back(Eigen::Triplet<float>(index, index, 4));
				//寻找周围的点是否在内部,如果是，则加入系数矩阵中，如果不是，则归类于边界值
				if (i > 0 && inside_mask_(i - 1, j) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_Emat_(i - 1, j), -1));
				}
				if (j > 0 && inside_mask_(i, j - 1) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_Emat_(i, j - 1), -1));
				}
				if (i < rows_ - 1 && inside_mask_(i + 1, j) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_Emat_(i + 1, j), -1));
				}
				if (j < cols_ - 1 && inside_mask_(i, j + 1) == 1)
				{
					coef.push_back(Eigen::Triplet<float>(index, index_Emat_(i, j + 1), -1));
				}
			}
		}
	}
	coe_sparse_Emat_.setFromTriplets(coef.begin(), coef.end());
	//压缩
	coe_sparse_Emat_.makeCompressed();
	//预处理
	Predecomposition();

}

void Poisson::Predecomposition()
{
	solver.compute(coe_sparse_Emat_);
	if (solver.info() != Eigen::Success)
	{
		throw std::exception("Compute Matrix is error");
		return;
	}
}

void Poisson::NormalPaste(QPoint target_point, QPoint source_point, cv::Mat& target_img, cv::Mat& source_img)
{
	int srow, scol;
	int trow, tcol;

	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				trow = i + target_point.y();
				tcol = j + target_point.x();
				srow = i + source_point.y();
				scol = j + source_point.x();
				
				target_img.at<cv::Vec3b>(trow, tcol)[0] = source_img.at<cv::Vec3b>(srow, scol)[0];
				target_img.at<cv::Vec3b>(trow, tcol)[1] = source_img.at<cv::Vec3b>(srow, scol)[1];
				target_img.at<cv::Vec3b>(trow, tcol)[2] = source_img.at<cv::Vec3b>(srow, scol)[2];
			}
		}
	}
}

void Poisson::PoissonPaste(QPoint target_point, QPoint source_point, cv::Mat& target_img, cv::Mat& source_img)
{
	int tpt_x = target_point.x();
	int tpt_y = target_point.y();
	int spt_x = source_point.x();
	int spt_y = source_point.y();

	if (tpt_x + cols_ >= target_img.cols || tpt_y + rows_ >= target_img.rows) {
		printf("Out of range\n");
		return;
	}

	if (spt_x + cols_ >= source_img.cols || spt_y + rows_ >= source_img.rows) {
		printf("Out of range\n");
		return;
	}
	printf("Poisson Paste\n");
	int srow, scol;
	int trow, tcol;
	Eigen::VectorXf div_red(pixels_num_);
	Eigen::VectorXf div_green(pixels_num_);
	Eigen::VectorXf div_blue(pixels_num_);
	div_red.setZero();
	div_green.setZero();
	div_blue.setZero();

	/*cv::Mat rgb_mat(pixels_num_, 1, CV_8UC3);
	std::vector<cv::Mat> channels;*/


	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				int index = index_Emat_(i, j);
				srow = spt_y + i; 
				scol = spt_x + j;
				trow = tpt_y + i;
				tcol = tpt_x + j;

				if (i == 0 || (i > 0 && inside_mask_(i - 1, j) == 0))
				{
					GetDivRGB(div_red, index, Red, target_img, trow - 1, tcol, source_img, srow - 1, scol);
					GetDivRGB(div_green, index, Green, target_img, trow - 1, tcol, source_img, srow - 1, scol);
					GetDivRGB(div_blue, index, Blue, target_img, trow - 1, tcol, source_img, srow - 1, scol);
					/*rgb_mat.at<cv::Vec3b>(index, 0) +=
						target_img.at<cv::Vec3b>(trow - 1, tcol);*/
						//- source_img.at<cv::Vec3b>(srow - 1, scol);
				}
				if (i == rows_ - 1 || (i < rows_ - 1 && inside_mask_(i + 1, j) == 0))
				{

					GetDivRGB(div_red, index, Red, target_img, trow + 1, tcol, source_img, srow + 1, scol);
					GetDivRGB(div_green, index, Green, target_img, trow + 1, tcol, source_img, srow + 1, scol);
					GetDivRGB(div_blue, index, Blue, target_img, trow + 1, tcol, source_img, srow + 1, scol);

					/*rgb_mat.at<cv::Vec3b>(index, 0) +=
						target_img.at<cv::Vec3b>(trow + 1, tcol);*/
						//- source_img.at<cv::Vec3b>(i + 1 + spt_x, j + spt_y);
				}
				if (j == 0 || (j > 0 && inside_mask_(i, j - 1) == 0))
				{
					GetDivRGB(div_red, index, Red, target_img, trow, tcol - 1, source_img, srow, scol - 1);
					GetDivRGB(div_green, index, Green, target_img, trow, tcol - 1, source_img, srow, scol - 1);
					GetDivRGB(div_blue, index, Blue, target_img, trow, tcol - 1, source_img, srow, scol - 1);
					/*rgb_mat.at<cv::Vec3b>(index, 0) +=
						target_img.at<cv::Vec3b>(trow, tcol - 1);*/
						//- source_img.at<cv::Vec3b>(i + spt_x, j - 1 + spt_y);
				}
				if (j == cols_ - 1 || (j < cols_ - 1 && inside_mask_(i, j + 1) == 0))
				{

					GetDivRGB(div_red, index, Red, target_img, trow, tcol + 1, source_img, srow, scol + 1);
					GetDivRGB(div_green, index, Green, target_img, trow, tcol + 1, source_img, srow, scol + 1);
					GetDivRGB(div_blue, index, Blue, target_img, trow, tcol + 1, source_img, srow, scol + 1);

					/*rgb_mat.at<cv::Vec3b>(index, 0) +=
						target_img.at<cv::Vec3b>(trow, tcol + 1);*/
						//- source_img.at<cv::Vec3b>(i + spt_x, j + 1 + spt_y);
				}
			}
		}
	}
	//cv::split(rgb_mat, channels);
	//cv::cv2eigen(channels.at(0), div_red);
	//cv::cv2eigen(channels.at(1), div_green);
	//cv::cv2eigen(channels.at(2), div_blue);

	Eigen::VectorXf vec_red(pixels_num_);
	Eigen::VectorXf vec_green(pixels_num_);
	Eigen::VectorXf vec_blue(pixels_num_);
	//std::cout << div_red;
	vec_red = solver.solve(div_red);
	vec_green = solver.solve(div_green);
	vec_blue = solver.solve(div_blue);
	//std::cout << vec_red;
	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				trow = i + tpt_y;
				tcol = j + tpt_x;
				srow = i + spt_y;
				scol = j + spt_x;
				int index = index_Emat_(i, j);
				int red = vec_red(index) + source_img.at<cv::Vec3b>(srow, scol)[0];
				int green = vec_green(index) + source_img.at<cv::Vec3b>(srow, scol)[1];
				int blue = vec_blue(index) + source_img.at<cv::Vec3b>(srow, scol)[2];

				target_img.at<cv::Vec3b>(trow, tcol)[0] = red > 255 ? 255 : (red < 0 ? 0 : red);
				target_img.at<cv::Vec3b>(trow, tcol)[1] = green > 255 ? 255 : (green < 0 ? 0 : green);
				target_img.at<cv::Vec3b>(trow, tcol)[2] = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
			}
		}
	}
}

void Poisson::PoissonPaste_beta(QPoint target_point, QPoint source_point, cv::Mat& target_img, cv::Mat& source_img)
{
	Eigen::VectorXf div_red;
	Eigen::VectorXf div_green;
	Eigen::VectorXf div_blue;
	div_red.resize(pixels_num_);
	div_green.resize(pixels_num_);
	div_blue.resize(pixels_num_);
	div_red.setZero();
	div_green.setZero();
	div_blue.setZero();

	for (int i = 0; i < rows_; i++)
	{
		for (int j = 0; j < cols_; j++)
		{
			if (inside_mask_(i, j) == 1)
			{
				int index = index_Emat_(i, j);
				int x = source_point.y() + i;
				int y = source_point.x() + j;
				cv::Vec3i temp_vec = source_img.at<cv::Vec3b>(x, y);
				temp_vec *= 4;
				temp_vec -= source_img.at<cv::Vec3b>(x + 1, y);
				temp_vec -= source_img.at<cv::Vec3b>(x - 1, y);
				temp_vec -= source_img.at<cv::Vec3b>(x, y - 1);
				temp_vec -= source_img.at<cv::Vec3b>(x, y + 1);

				div_red(index_Emat_(i, j)) += temp_vec[0];
				div_green(index_Emat_(i, j)) += temp_vec[1];
				div_blue(index_Emat_(i, j)) += temp_vec[2];

				if (i == 0 || (i > 0 && inside_mask_(i - 1, j) == 0))
				{
					div_red[index] += target_img.at<cv::Vec3b>(i + target_point.y() - 1, j + target_point.x())[0];
					div_green[index] += target_img.at<cv::Vec3b>(i + target_point.y() - 1, j + target_point.x())[1];
					div_blue[index] += target_img.at<cv::Vec3b>(i + target_point.y() - 1, j + target_point.x())[2];
				}
				if (i == rows_ - 1 || (i < rows_ - 1 && inside_mask_(i + 1, j) == 0))
				{
					div_red[index] += target_img.at<cv::Vec3b>(i + target_point.y() + 1, j + target_point.x())[0];
					div_green[index] += target_img.at<cv::Vec3b>(i + target_point.y() + 1, j + target_point.x())[1];
					div_blue[index] += target_img.at<cv::Vec3b>(i + target_point.y() + 1, j + target_point.x())[2];
				}
				if (j == 0 || (j > 0 && inside_mask_(i, j - 1) == 0))
				{
					div_red[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() - 1)[0];
					div_green[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() - 1)[1];
					div_blue[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() - 1)[2];
				}
				if (j == cols_ - 1 || (j < cols_ - 1 && inside_mask_(i, j + 1) == 0))
				{
					div_red[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() + 1)[0];
					div_green[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() + 1)[1];
					div_blue[index] += target_img.at<cv::Vec3b>(i + target_point.y(), j + target_point.x() + 1)[2];
				}
			}
		}
	}

	Eigen::VectorXf vec_red(pixels_num_);
	Eigen::VectorXf vec_green(pixels_num_);
	Eigen::VectorXf vec_blue(pixels_num_);

	vec_red = solver.solve(div_red);
	vec_green = solver.solve(div_green);
	vec_blue = solver.solve(div_blue);

	for (int j = 0; j < rows_; j++)
	{
		for (int k = 0; k < cols_; k++)
		{
			if (inside_mask_(j, k) == 1)
			{
				int index = index_Emat_(j, k);
				int red = vec_red(index);
				int green = vec_green(index);
				int blue = vec_blue(index);
				target_img.at<cv::Vec3b>(j + target_point.y(), k + target_point.x())[0] = red > 255 ? 255 : (red < 0 ? 0 : red);
				target_img.at<cv::Vec3b>(j + target_point.y(), k + target_point.x())[1] = green > 255 ? 255 : (green < 0 ? 0 : green);
				target_img.at<cv::Vec3b>(j + target_point.y(), k + target_point.x())[2] = blue > 255 ? 255 : (blue < 0 ? 0 : blue);
			}
		}
	}
}

double Poisson::VecLength(cv::Vec3i vec)
{
	return sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
}

int Poisson::GetDivRGB(Eigen::VectorXf& RGB, const int& index, Color color,
						const cv::Mat& target_img, const int& trow, const int& tcol, 
						const cv::Mat& source_img, const int& srow, const int& scol)
{
	RGB[index] = target_img.at<cv::Vec3b>(trow, tcol)[color] - source_img.at<cv::Vec3b>(srow, scol)[color];
	return 0;
}

