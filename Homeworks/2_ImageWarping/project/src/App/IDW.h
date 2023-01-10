#pragma once
#ifndef IDW_H_
#define IDW_H_

#include "base_image_warping.h"

class IDWImageWarping : public BaseImageWarping {
public:
	IDWImageWarping();
	IDWImageWarping(int width, int height);
	IDWImageWarping(int width, int height, int mu);
	virtual ~IDWImageWarping();

protected:

	virtual Eigen::Vector2f GetTransformedPoint(
		const Eigen::Vector2f& pt) override;
	virtual void SolveTransformations() override;

private:
	void CalcWeights(const Eigen::Vector2f& pt);
	void SolveOptimalTransformations();
	
private:
	std::vector<float> weights_;
	std::vector<Eigen::Matrix2f> transform_mat_list_;
	int mu;
};
#endif  // IDW_H_