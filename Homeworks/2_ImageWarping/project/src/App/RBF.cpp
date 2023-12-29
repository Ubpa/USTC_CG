#include "RBF.h"
#include <cmath>
#include <limits>
#include <iostream>



RBFImageWarping::RBFImageWarping() {
}

RBFImageWarping::RBFImageWarping(int w, int h) :BaseImageWarping(w,h){

}

RBFImageWarping::~RBFImageWarping() {
}



Eigen::Vector2f RBFImageWarping::GetTransformedPoint(
	const Eigen::Vector2f& pt) {
	const int num_ctrl_pts = source_points_.size();
	Eigen::Vector2f trans_pt;
	trans_pt[0] = 0.f;
	trans_pt[1] = 0.f;
	for (int i = 0; i < num_ctrl_pts; ++i) {
		float b_i = BaseFunc(i, pt);
		trans_pt = weight_list_[i] * b_i;
		//trans_pt[0] += g_i * alpha_x_list_[i];
		//trans_pt[1] += g_i * alpha_y_list_[i];
	}
	trans_pt += pt;
	//trans_pt[0] += pt[0];
	//trans_pt[1] += pt[1];
	return trans_pt;
}

void RBFImageWarping::SolveTransformations() {
	// get min radius list
	min_radius_list_.clear();
	for (int i = 0; i < source_points_.size(); ++i) {
		min_radius_list_.push_back(CalcMinRadius(i));
	}

	// solve linear system to get coeffs
	SolveLinearSystem();
}
//计算基函数
float RBFImageWarping::BaseFunc(int i, const Eigen::Vector2f& pt) {
/*
	R(d) = (d^2+r^2)^{\nu/2}
*/
	float dist = Distance(source_points_[i], pt);
	return std::sqrt(dist * dist +
		min_radius_list_[i] * min_radius_list_[i]);
}


//找最小的r值
float RBFImageWarping::CalcMinRadius(int i) {
	float min_dist = std::numeric_limits<float>::max();
	const int num_ctrl_pts = source_points_.size();
	if (source_points_.size() <= 1) {
		return 0.f;
	}
	for (int j = 0; j < num_ctrl_pts; ++j) {
		if (i == j) {
			continue;
		}
		float dist = Distance(source_points_[j], target_points_[j]);
		if (dist < min_dist) {
			min_dist = dist;
		}
	}
	return min_dist;
}
//根据源点和目标点，计算权重alpha的值，此题采取A=I,b=0
void RBFImageWarping::SolveLinearSystem() {
	const int num_ctrl_pts = source_points_.size();
	if (num_ctrl_pts == 0) {
		return;
	}
	Eigen::MatrixXf coeff_mat(num_ctrl_pts, num_ctrl_pts);
	Eigen::VectorXf alpha_x_vec(num_ctrl_pts);
	Eigen::VectorXf alpha_y_vec(num_ctrl_pts);
	Eigen::VectorXf diff_x_vec(num_ctrl_pts);
	Eigen::VectorXf diff_y_vec(num_ctrl_pts);

	//构造 R,q,p矩阵和向量
	for (int i = 0; i < num_ctrl_pts; ++i) {
		for (int j = 0; j < num_ctrl_pts; ++j) {
			coeff_mat(i, j) = BaseFunc(i, source_points_[j]);
		}
		diff_x_vec(i) = target_points_[i][0] - source_points_[i][0];
		diff_y_vec(i) = target_points_[i][1] - source_points_[i][1];
	}
	//计算结果
	alpha_x_vec = coeff_mat.colPivHouseholderQr().solve(diff_x_vec);
	alpha_y_vec = coeff_mat.colPivHouseholderQr().solve(diff_y_vec);
	// special handling if there is only one pair of ctrl points
	if (num_ctrl_pts == 1) {
		const float coeff = coeff_mat(0, 0) + 1e-5f;
		alpha_x_vec(0) = diff_x_vec(0) / coeff;
		alpha_y_vec(0) = diff_y_vec(0) / coeff;
	}
	//alpha_x_list_.clear();
	//alpha_y_list_.clear();
	//for (int i = 0; i < num_ctrl_pts; ++i) {
	//	alpha_x_list_.push_back(alpha_x_vec(i));
	//	alpha_y_list_.push_back(alpha_y_vec(i));
	//}

	weight_list_.clear();
	for (int i = 0; i < num_ctrl_pts; ++i) {
		weight_list_.push_back(Eigen::Vector2f(alpha_x_vec(i), alpha_y_vec(i)));
	}
}