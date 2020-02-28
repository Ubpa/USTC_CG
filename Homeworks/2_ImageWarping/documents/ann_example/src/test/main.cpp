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

	ANNcoord coords[POINT_NUM * DIM] = {
		0, 0,
		0, 1,
		1, 0,
		1, 1
	};

	ANNpointArray ptsArr = annAllocPts(POINT_NUM, DIM);
	for (size_t i = 0; i < POINT_NUM; i++) {
		for (size_t j = 0; j < DIM; j++)
			ptsArr[i][j] = coords[i * DIM + j];
	}

	ANNbd_tree tree(ptsArr, POINT_NUM, DIM);

	ANNpoint queryPt = annAllocPt(2);
	ANNidx idxArr[K];
	ANNdist distArr[K];
	queryPt[0] = 0.2;
	queryPt[1] = 0.1;
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
