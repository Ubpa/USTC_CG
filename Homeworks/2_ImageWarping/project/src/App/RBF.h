#pragma once
#include "base_image_warping.h"

class RBFImageWarping : public BaseImageWarping {
public:
    RBFImageWarping();
	RBFImageWarping(int w, int h);
    virtual ~RBFImageWarping();


protected:
    virtual Eigen::Vector2f GetTransformedPoint(
        const Eigen::Vector2f& pt) override;
    virtual void SolveTransformations() override;

private:
    float BaseFunc(int i, const Eigen::Vector2f& pt);
    float CalcMinRadius(int i);
    void SolveLinearSystem();

private:
    std::vector<float> min_radius_list_;
    std::vector<float> alpha_x_list_;
    std::vector<float> alpha_y_list_;
    std::vector<Eigen::Vector2f> weight_list_;
};