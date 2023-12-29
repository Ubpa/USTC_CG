#pragma once
#ifndef BASE_IMAGE_WARPING_H_
#define BASE_IMAGE_WARPING_H_
#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <ANN/ANN.h>					// ANN declarations

#include <QImage>



class BaseImageWarping {
public:
	BaseImageWarping();
	BaseImageWarping(int width, int height);
	virtual ~BaseImageWarping();

	void SetAnchorPoints(const std::vector<Eigen::Vector2f>& src_pts,
		const std::vector<Eigen::Vector2f>& tgt_pts);
	void SetWidthHeight(int width, int  height);
	void WarpImage(QImage* image);

	//void WarpImageWithTriangulation(QImage* image);


protected:
	virtual Eigen::Vector2f GetTransformedPoint(const Eigen::Vector2f& pt) = 0;
	virtual void SolveTransformations() = 0;
protected:
	void FillHole(QImage& image, int threshold, int count=4);

	float Distance(const Eigen::Vector2f& p1, const Eigen::Vector2f& p2);

	bool IsValidImagePoint(int , int , int , int );

protected:
	std::vector<Eigen::Vector2f> source_points_;
	std::vector<Eigen::Vector2f> target_points_;
	std::vector<std::vector<int> > paint_mask_;
	QImage* image_mat_backup_;


	int win_width;
	int win_height;
};

#endif  // BASE_IMAGE_WARPING_H_