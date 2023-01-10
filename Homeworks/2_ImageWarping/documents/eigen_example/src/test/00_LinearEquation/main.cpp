#include <iostream>

#include <Eigen/Dense>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace Eigen;

int main() {
	MatrixXf A(4, 1);
	Matrix2d B;

	Vector2d a;
	Vector2d b;
	Vector2d c;

	Vector4f d;
	cv::Mat m(cv::Size(4, 1), CV_32F);
	m.at<float>(0, 0) = 1;
	m.at<float>(0, 1) = 2;
	m.at<float>(0, 2) = 3;
	m.at<float>(0, 3) = 4;
	cout << m;
	//
	//A << 1, 2, 3, 4;
	//d = A;
	//cout << d << endl;
	
	//a << 1, 2;
	//b << 3, 4;
	//c = a * 2;
	//B = a * b.transpose();
	//cout << B << endl;
	//cout << c << endl;
	/*float data[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}
	};

	for (size_t row = 0; row < 4; row++) {
		for (size_t col = 0; col < 4; col++)
			A(row, col) = data[row][col];
	}

	MatrixXf b(4, 2);
	for (size_t row = 0; row < 4; row++) {
		b(row, 0) = 1;
		b(row, 1) = 2;
	}

	MatrixXf x = A.colPivHouseholderQr().solve(b);

	cout << x << endl;*/

	return 0;
}
