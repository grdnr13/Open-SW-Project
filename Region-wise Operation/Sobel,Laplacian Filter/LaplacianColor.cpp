#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#define IM_TYPE	CV_8UC3

using namespace cv;

// Image Type
// "G" for GrayScale Image, "C" for Color Image
#if (IM_TYPE == CV_8UC3)
typedef uchar G;
typedef cv::Vec3b C;
#elif (IM_TYPE == CV_16SC3)
typedef short G;
typedef Vec3s C;
#elif (IM_TYPE == CV_32SC3)
typedef int G;
typedef Vec3i C;
#elif (IM_TYPE == CV_32FC3)
typedef float G;
typedef Vec3f C;
#elif (IM_TYPE == CV_64FC3)
typedef double G;
typedef Vec3d C;
#endif

Mat sobelfilter(const Mat input);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat output;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);
	output = sobelfilter(input); //Boundary process: zero-paddle, mirroring, adjustkernel

	namedWindow("Laplacian Filter", WINDOW_AUTOSIZE);
	imshow("Laplacian Filter", output);

	waitKey(0);

	return 0;
}

Mat sobelfilter(const Mat input) {

	//Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N
	int tempa, tempb;
	int sum_r, sum_g, sum_b;

	const float visualize = 8;

	// Initialiazing Kernel Matrix with 3x3 size
	int filter[] = { 0,1,0, 1,-4,1, 0,1,0 };
	Mat kernel(3, 3, CV_32S, filter);

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//3*3 size (n=1)
			sum_r = 0; sum_g = 0; sum_b = 0;
			for (int a = -n; a <= n; a++) {
				for (int b = -n; b <= n; b++) {
					// Use mirroring boundary process
					if (i + a > row - 1) {
						tempa = i - a;
					}
					else if (i + a < 0) {
						tempa = -(i + a);
					}
					else {
						tempa = i + a;
					}
					if (j + b > col - 1) {
						tempb = j - b;
					}
					else if (j + b < 0) {
						tempb = -(j + b);
					}
					else {
						tempb = j + b;
					}
					// Find output M(x,y) = sqrt( input.at<G>(x, y)*Sx + input.at<G>(x, y)*Sy ) 
					//filter 이용 r,g,b 각각 계산
					sum_r += kernel.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[0];
					sum_g += kernel.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[1];
					sum_b += kernel.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[2];

					//결과 이미지 수정
					output.at<C>(i, j)[0] = (G)(visualize * sqrt(sum_r * sum_r));
					output.at<C>(i, j)[1] = (G)(visualize * sqrt(sum_g * sum_g));
					output.at<C>(i, j)[2] = (G)(visualize * sqrt(sum_b * sum_b));
				}

			}
		}
	}
	return output;
}