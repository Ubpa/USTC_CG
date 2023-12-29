#include "IDW.h"
#include <cmath>
#include <iostream>

IDWImageWarping::IDWImageWarping() :BaseImageWarping() {
	mu = -2;
}

IDWImageWarping::IDWImageWarping(int width, int height) : BaseImageWarping(width, height) {
	mu = -2;
}


IDWImageWarping::IDWImageWarping(int width, int height, int mu) : BaseImageWarping(width, height) {
	this->mu = mu;
}

IDWImageWarping::~IDWImageWarping() {
}



//根据源点和目标点计算图像warp后的坐标
Eigen::Vector2f IDWImageWarping::GetTransformedPoint(const Eigen::Vector2f& x) {
/*
    f(x)=sum(wi*fi(x))
    fi(x) = target_i + Di*(x-source_i)
    
 */
    const int num_ctrl_pts = source_points_.size();
    if (num_ctrl_pts == 0) {
        return x;
    }
    else {
        Eigen::Vector2f trans_pt;
        trans_pt[0] = 0.f;
        trans_pt[1] = 0.f;
        CalcWeights(x);
        for (int i = 0; i < num_ctrl_pts; ++i) {
            trans_pt += weights_[i] *
                (target_points_[i] +
                    transform_mat_list_[i] * (x - source_points_[i]));
        }
        return trans_pt;
    }
}
//对输入的点 x ,根据源点计算其对应的权重，不同的点，权重不一样
void IDWImageWarping::CalcWeights(const Eigen::Vector2f& x) {
/*
    w_i = sigma_i/sum(sigma_i)
	sigma_i(x)= |x-x_i|^mu

*/
    const int num_ctrl_pts = source_points_.size();
    float weights_sum = 0.f;
    weights_.clear();
    for (int i = 0; i < num_ctrl_pts; ++i) {
        float sigma = std::pow(Distance(x, source_points_[i]), mu);//sigma_i(x)= 1/|x-x_i|^2
        weights_.push_back(sigma);
        weights_sum += sigma;
    }
    for (int i = 0; i < num_ctrl_pts; ++i) {
        weights_[i] /= weights_sum;
    }
}


void IDWImageWarping::SolveTransformations() {
    SolveOptimalTransformations();
}

// 计算最优变换矩阵
void IDWImageWarping::SolveOptimalTransformations() {
/*
权重，源点，目标点,来最小化误差函数,对于每对源和目标点，都有固定的变换矩阵
*/
    const int num_ctrl_pts = source_points_.size();
    transform_mat_list_.clear();
    transform_mat_list_.resize(num_ctrl_pts);
    for (int i = 0; i < num_ctrl_pts; ++i) {
        transform_mat_list_[i] = Eigen::Matrix2f::Identity();
    }
    if (num_ctrl_pts == 1) {
        return;
	}
    else if (num_ctrl_pts == 2) {
        Eigen::Vector2f delta_p = source_points_[1] - source_points_[0];

        Eigen::Vector2f delta_q = target_points_[1] - target_points_[0];
        for (int i = 0; i < 2; i++) {
            transform_mat_list_[i](0,0) = delta_q[0] / delta_p[0];
            transform_mat_list_[i](0,1) = 0;
            transform_mat_list_[i](1,0) = 0;
            transform_mat_list_[i](1,1) = delta_q[1] / delta_p[1];
        }
    }
    else {
        for (int i = 0; i < num_ctrl_pts; ++i) {
            Eigen::Matrix2f coeff_1_mat = Eigen::Matrix2f::Zero(); //系数矩阵
            Eigen::Matrix2f coeff_2_mat = Eigen::Matrix2f::Zero();
            //Eigen::Matrix2f coeff_mat = Eigen::Matrix2f::Zero();
            Eigen::Vector2f delta_p;
            Eigen::Vector2f delta_q;
            //Eigen::VectorXf b_vec_1(2);
            //Eigen::VectorXf b_vec_2(2);
            //b_vec_1.setZero();
            //b_vec_2.setZero();
            for (int j = 0; j < num_ctrl_pts; ++j) {
                if (i == j) {
                    continue;
                }
                float sigma = std::pow(
                    Distance(source_points_[i], source_points_[j]), 
                    mu);
                delta_p = source_points_[j] - source_points_[i];
                delta_q = target_points_[j] - target_points_[i];

                coeff_1_mat += sigma * delta_p * delta_p.transpose();
                coeff_2_mat += sigma * delta_q * delta_p.transpose();


                //coeff_mat(0, 0) += sigma * (source_points_[j][0] - source_points_[i][0]) *
                //    (source_points_[j][0] - source_points_[i][0]);
                //coeff_mat(0, 1) += sigma * (source_points_[j][0] - source_points_[i][0]) *
                //    (source_points_[j][1] - source_points_[i][1]);
                //coeff_mat(1, 0) += sigma * (source_points_[j][1] - source_points_[i][1]) *
                //    (source_points_[j][0] - source_points_[i][0]);
                //coeff_mat(1, 1) += sigma * (source_points_[j][1] - source_points_[i][1]) *
                //    (source_points_[j][1] - source_points_[i][1]);

                //b_vec_1(0) += sigma * (source_points_[j][0] - source_points_[i][0]) *
                //    (target_points_[j][0] - target_points_[i][0]);
                //b_vec_1(1) += sigma * (source_points_[j][1] - source_points_[i][1]) *
                //    (target_points_[j][0] - target_points_[i][0]);
                //b_vec_2(0) += sigma * (source_points_[j][0] - source_points_[i][0]) *
                //    (target_points_[j][1] - target_points_[i][1]);
                //b_vec_2(1) += sigma * (source_points_[j][1] - source_points_[i][1]) *
                //    (target_points_[j][1] - target_points_[i][1]);
            }
            transform_mat_list_[i] = coeff_2_mat * coeff_1_mat.inverse();


            //b_vec_1 = coeff_mat.colPivHouseholderQr().solve(b_vec_1);
            //b_vec_2 = coeff_mat.colPivHouseholderQr().solve(b_vec_2);
            //transform_mat_list_[i](0, 0) = b_vec_1(0);
            //transform_mat_list_[i](0, 1) = b_vec_1(1);
            //transform_mat_list_[i](1, 0) = b_vec_2(0);
            //transform_mat_list_[i](1, 1) = b_vec_2(1);
        }
    }
}
