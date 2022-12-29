#pragma once
#include <vector>
#include <memory>
#include <Eigen/Dense>
#include <QImage>



class BaseImageWarping {
public:
	BaseImageWarping();
	virtual ~BaseImageWarping();

	void SetAnchorPoints(const std::vector<Eigen::Vector2f>& src_pts,
		const std::vector<Eigen::Vector2f>& tgt_pts);

	void WarpImage(QImage* image);

	//void WarpImageWithTriangulation(QImage* image);


protected:
	virtual Eigen::Vector2f GetTransformedPoint(const Eigen::Vector2f& pt) = 0;
	virtual void SolveTransformations() = 0;
	
protected:
	//void FillHole(cv::Mat* image);

	float Distance(const Eigen::Vector2f& p1, const Eigen::Vector2f& p2);

	bool IsValidImagePoint(const Eigen::Vector2f& pt, int width, int height) {
		int x = static_cast<int>(pt[0]);
		int y = static_cast<int>(pt[1]);
		return x >= 0 && x < width&& y >= 0 && y < height;
	}

	//void GenerateRandomKeyPointsAndDoTriangulation(int width, int height);

	/*void GetBarycentricCoordinates(const Eigen::Vector2f& p1,
		const Eigen::Vector2f& p2, const Eigen::Vector2f& p3,
		const Eigen::Vector2f& p, Eigen::Vector3f* bc_coords);*/

protected:
	std::vector<Eigen::Vector2f> source_points_;
	std::vector<Eigen::Vector2f> target_points_;
	// paint_mask_[i][j] = 1 means pixel (i, j) is painted, else in 'hole'
	std::vector<std::vector<int> > paint_mask_;
	QImage *image_mat_backup_;

	std::vector<Eigen::Vector2f> keypoints_;
	std::vector<Eigen::Vector2f> transformed_keypoints_;
	//std::shared_ptr<cv::Subdiv2D> subdiv_2d_;
};

