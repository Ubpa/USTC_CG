#include "base_image_warping.h"
#include <cmath>
BaseImageWarping::BaseImageWarping() {
    source_points_.reserve(100);
    target_points_.reserve(100);
    keypoints_.reserve(10000);
    transformed_keypoints_.reserve(10000);

    image_mat_backup_ = new QImage();
}

BaseImageWarping::~BaseImageWarping() {
}

//设置源节点和目标结点
void BaseImageWarping::SetAnchorPoints(
    const std::vector<Eigen::Vector2f>& src_pts,
    const std::vector<Eigen::Vector2f>& tgt_pts) {
    source_points_.clear();
    source_points_.assign(src_pts.begin(), src_pts.end());
    target_points_.clear();
    target_points_.assign(tgt_pts.begin(), tgt_pts.end());

    SolveTransformations();
}
//主要函数，通过GetTransformedPoint() 得到转变后的坐标点
void BaseImageWarping::WarpImage(QImage* image) {
    const int width = image->width();
	const int height = image->height();

    paint_mask_.clear();
    paint_mask_.resize(height, std::vector<int>(width, 0));
    *(image_mat_backup_) = *(image);
    image->fill(QColor(Qt::white).rgb());//may be cause problem
	
    Eigen::Vector2f pt;
    Eigen::Vector2f trans_pt;

    QRgb color;
	
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            pt[0] = j;
            pt[1] = i;
            trans_pt = GetTransformedPoint(pt);
            if (!IsValidImagePoint(trans_pt, width, height)) {
                continue;
            }
            int trans_x = static_cast<int>(trans_pt[0]);
            int trans_y = static_cast<int>(trans_pt[1]);
            color = image_mat_backup_->pixel(i, j); //用bits() 方法可能使得这个循环加速
            image->setPixel(trans_y, trans_x, color);
            /*image->at<cv::Vec3b>(trans_y, trans_x) =
                image_mat_backup_.at<cv::Vec3b>(i, j);*/
            paint_mask_[trans_y][trans_x] = 1;
        }
    }
	//TODO 补充填充空洞的图像
    //FillHole(image);
}

//欧拉距离
float BaseImageWarping::Distance(
    const Eigen::Vector2f& p1, const Eigen::Vector2f& p2) {
    float diff_x = p1[0] - p2[0];
    float diff_y = p1[1] - p2[1];
    return sqrt(diff_x * diff_x + diff_y * diff_y);
}


