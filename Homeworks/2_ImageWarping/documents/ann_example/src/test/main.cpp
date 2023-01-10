#include <vector>
#include <array>

#include <ANN/ANN.h>					// ANN declarations
#include <ANN/ANNx.h>					// more ANN declarations
#include <ANN/ANNperf.h>				// performance evaluation

using namespace std;

int main() {
	constexpr size_t POINT_NUM = 4;
	constexpr size_t DIM = 2;
	constexpr size_t K = 2;
	//坐标
	ANNcoord coords[POINT_NUM * DIM] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};
	//为ANNpoint分配存储空间
	//是一个[num][dim]的数组,每行记录当前点的的坐标位置
	ANNpointArray ptsArr = annAllocPts(POINT_NUM, DIM);
	for (size_t i = 0; i < POINT_NUM; i++) {
		for (size_t j = 0; j < DIM; j++)
			ptsArr[i][j] = coords[i * DIM + j];
	}
	//给待处理数据构建一个数据结构（kd树）
	ANNbd_tree tree(ptsArr, POINT_NUM, DIM);
	//申请一个点的空间
	ANNpoint queryPt = annAllocPt(2);
	queryPt[0] = 0.2;
	queryPt[1] = 0.1;

	//存储近邻点的坐标
	ANNidx idxArr[K];
	//存储点到邻居的距离
	ANNdist distArr[K];


	//k近邻搜索两种方法
	/*annkSearch
		virtual void ANNkd_tree::annkSearch(
			ANNpoint            q,       //查询点（目的就是要查询数据点中，点q的最近邻点）
			int                 k,       //要查找的邻近点数（可以自定义也可以利用法二计算）
			ANNidxArray    nn_idx,       //查询到的q的最邻近点组成的数组（任选参数）
			ANNdistArray    dists,       //最邻近点与点q的距离（任选参数）
			double          eps = 0.0);    //误差限
		对于数组nn_idx：查询到的q的最邻近点存储在数组nn_idx数组中，
		其中最近的存储在nn_idx[0]，第二近的在nn_idx[1]中...以此类推
		对于数组 dists:存储的为相关点到点q的平方距离
	*/
	tree.annkSearch(queryPt, K, idxArr, distArr);
	
	cout << "idx:" << endl;
	for (size_t i = 0; i < K; i++) {
		cout << "- ptsArr[" << idxArr[i] << "]:";
		for (size_t j = 0; j < DIM; j++)
			cout << " " << ptsArr[i][j];
		cout << endl;
	}

	cout << "dist: ";
	for (size_t i = 0; i < K; i++)
		cout << " " << distArr[i];
	cout << endl;

	annDeallocPts(ptsArr);

	return 0;
}
