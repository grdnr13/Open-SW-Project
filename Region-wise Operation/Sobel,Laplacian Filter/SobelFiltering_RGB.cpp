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

	namedWindow("Sobel Filter", WINDOW_AUTOSIZE);
	imshow("Sobel Filter", output);

	waitKey(0);

	return 0;
} 

Mat sobelfilter(const Mat input) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int n = 1; // Sobel Filter Kernel N
	int tempa, tempb;
	int Ix_r, Ix_g, Ix_b, Iy_r, Iy_g, Iy_b;

	const float visualize = 0.7;

	// Initialiazing 2 Kernel Matrix with 3x3 size for Sx and Sy
	//Fill code to initialize Sobel filter kernel matrix for Sx and Sy (Given in the lecture notes)
	int sx[] = { -1,0,1, -2,0,2, -1,0,1 };
	int sy[] = { -1,-2,-1, 0,0,0, 1,2,1 };

	Mat Sx(3, 3, CV_32S, sx);
	Mat Sy(3, 3, CV_32S, sy);

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			//3*3 size (n=1)
			Ix_r = 0; Ix_g = 0; Ix_b = 0;
			Iy_r = 0; Iy_g = 0; Iy_b = 0;
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
					//Ix 이용 r,g,b 각각 계산
					Ix_r += Sx.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[0];
					Ix_g += Sx.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[1];
					Ix_b += Sx.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[2];
					//Iy 이용 r,g,b 각각 계산
					Iy_r += Sy.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[0];
					Iy_g += Sy.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[1];
					Iy_b += Sy.at<int>(a + n, b + n) * input.at<C>(tempa, tempb)[2];
					//결과 이미지 수정
					output.at<C>(i, j)[0] = (G)(visualize * sqrt(Ix_r * Ix_r + Iy_r * Iy_r));
					output.at<C>(i, j)[1] = (G)(visualize * sqrt(Ix_g * Ix_g + Iy_g * Iy_g));
					output.at<C>(i, j)[2] = (G)(visualize * sqrt(Ix_b * Ix_b + Iy_b * Iy_b));
				}

			}
		}
	}
	return output;
}