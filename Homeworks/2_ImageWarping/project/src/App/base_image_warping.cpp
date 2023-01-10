#include "base_image_warping.h"
#include <cmath>
BaseImageWarping::BaseImageWarping() {
    source_points_.reserve(50);
    target_points_.reserve(50);
    
    image_mat_backup_ = nullptr;

    win_height = 0;
    win_width = 0;
}

BaseImageWarping::BaseImageWarping(int width, int height) {
	source_points_.reserve(100);
	target_points_.reserve(100);
	//keypoints_.reserve(10000);
	//transformed_keypoints_.reserve(10000);

	image_mat_backup_ = nullptr;

	win_height = height;
	win_width = width;
}


BaseImageWarping::~BaseImageWarping() {
}

void BaseImageWarping::SetWidthHeight(int width, int height) {
	win_width = width;
	win_height = height;
}


//设置源节点和目标结点
void BaseImageWarping::SetAnchorPoints(
    const std::vector<Eigen::Vector2f>& src_pts,
    const std::vector<Eigen::Vector2f>& tgt_pts) 
{
	Eigen::Vector2f s_pt;
	Eigen::Vector2f t_pt;
    source_points_.clear();
    target_points_.clear();

	for (int i = 0; i < src_pts.size(); ++i) {
		s_pt[0] = src_pts[i][0]-win_width;
		s_pt[1] = src_pts[i][1] - win_height;
		t_pt[0] = tgt_pts[i][0] - win_width;
		t_pt[1] = tgt_pts[i][1] - win_height;
		source_points_.push_back(s_pt);
		target_points_.push_back(t_pt);
	}
		
    SolveTransformations();
}



//主要函数，通过GetTransformedPoint() 得到转变后的坐标点
void BaseImageWarping::WarpImage(QImage* image) {
    const int width= image->width();
    const int height = image->height();

    paint_mask_.clear();
    paint_mask_.resize(width, std::vector<int>(height, 0));
	image_mat_backup_ = new QImage(*image);    //
    /**(image_mat_backup_) = *(image);*/
    image->fill(QColor(Qt::white).rgb());//may be cause problem

    Eigen::Vector2f pt;
    Eigen::Vector2f trans_pt;

    QRgb color;

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            pt[0] = i;
            pt[1] = j;
            trans_pt = GetTransformedPoint(pt);
            int trans_x = int(trans_pt[0]);
            int trans_y = int(trans_pt[1]);

            if (!IsValidImagePoint(trans_x, trans_y, width, height)) {
                continue;
            }
			//printf("i=%d, j=%d, transx=%d, transy=%d\n", i, j, trans_x, trans_y);
            color = image_mat_backup_->pixel(i, j); //用bits() 方法可能使得这个循环加速
            image->setPixel(trans_x, trans_y, color);

            paint_mask_[trans_x][trans_y] = 1;
        }
    }
    //TODO 补充填充空洞的图像
    FillHole(*image,50);

	delete image_mat_backup_;
}


void BaseImageWarping::FillHole(QImage&image, int threshold, int K) {
	int real_count = K;
	//申请空间
	ANNpointArray dataPts = annAllocPts(image.height() * image.width(), 2);
	int nPts = 0;
	//建立已被覆盖上颜色的点集
	for (int i = 0; i < image.width(); i++)
	{
		for (int j = 0; j < image.height(); j++)
		{
			if (paint_mask_[i][j] == 1)
			{
				dataPts[nPts][0] = i;
				dataPts[nPts][1] = j;
				nPts++;
			}
		}
	}
	//生成kd树
	ANNkd_tree* kdtree = new ANNkd_tree(dataPts, nPts, 2);

	for (int i = 0; i < image.width(); i++)
	{
		for (int j = 0; j < image.height(); j++)
		{


			QVector<int> colors(3);
			colors[0] = colors[1] = colors[2] = 0;
			ANNpoint pt = annAllocPt(2);
			ANNidxArray index = new ANNidx[K];
			ANNdistArray dist = new ANNdist[K];
			pt[0] = i;
			pt[1] = j;
			kdtree->annkSearch(pt, K, index, dist, 0);
			
			real_count = K;
			for (int m = 0; m < K; m++)
			{
				int x = dataPts[index[m]][0];
				int y = dataPts[index[m]][1];
				QRgb rgb = image.pixel(x, y);
				if (fabs(x - i) <= threshold && fabs(y - j) <= threshold)//如果邻居的距离差别不是太大
				{
					colors[0] += qRed(rgb);
					colors[1] += qGreen(rgb);
					colors[2] += qBlue(rgb);
				}
				else
				{
					real_count--;
				}
			}
			if (real_count > 0)//对邻居点的像素作平均近似
			{
				image.setPixel(i, j, qRgb(colors[0] / real_count, colors[1] / real_count, colors[2] / real_count));
			}
			delete[] index;
			delete[] dist;
		}
	}
	delete kdtree;
}

//欧拉距离
float BaseImageWarping::Distance(
    const Eigen::Vector2f& p1, const Eigen::Vector2f& p2) {
    float diff_x = p1[0] - p2[0];
    float diff_y = p1[1] - p2[1];
    return sqrt(diff_x * diff_x + diff_y * diff_y);
}

bool BaseImageWarping::IsValidImagePoint(int x, int y, int width, int height) {
	return x >= 0 && x < width&& y >= 0 && y < height;
}


