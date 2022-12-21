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

Mat Unsharp(const Mat original, const Mat low_pass, const float k);
Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat output;

	const float k = 0.5;

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);


	//Mat L = gaussianfilter(input, 12, 7, 7, "zero-paddle"); 
	//Mat L = gaussianfilter(input, 12, 7, 7, "mirroring");
	Mat L = gaussianfilter(input, 12, 7, 7, "adjustkernel");

	//namedWindow("Blur with Low-pass filter", WINDOW_AUTOSIZE);
	//imshow("Blur with Low-pass filter", L);

	output = Unsharp(input, L, k);

	namedWindow("Unsharp Masking", WINDOW_AUTOSIZE);
	imshow("Unsharp Masking", output);

	waitKey(0);

	return 0;
}

Mat Unsharp(const Mat original, const Mat low_pass, const float k) {

	int row = original.rows;
	int col = original.cols;

	Mat output = Mat::zeros(row, col, original.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			float value_r = (original.at<C>(i, j)[0] - low_pass.at<C>(i, j)[0] * k) / (1 - k);
			float value_g = (original.at<C>(i, j)[1] - low_pass.at<C>(i, j)[1] * k) / (1 - k);
			float value_b = (original.at<C>(i, j)[2] - low_pass.at<C>(i, j)[2] * k) / (1 - k);
			if (value_r < 0)value_r = 0; if (value_r > 255)value_r = 255;
			if (value_g < 0)value_g = 0; if (value_g > 255)value_g = 255;
			if (value_b < 0)value_b = 0; if (value_b > 255)value_b = 255;
			output.at<C>(i, j)[0] = (G)value_r;
			output.at<C>(i, j)[1] = (G)value_g;
			output.at<C>(i, j)[2] = (G)value_b;
		}
	}
	return output;
}

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

	Mat kernel;

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int tempa;
	int tempb;
	float denom;
	float kernelvalue;

	// Initialiazing Kernel Matrix 
	kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

	denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigmaS, 2))) - (pow(b, 2) / (2 * pow(sigmaT, 2))));
			kernel.at<float>(a + n, b + n) = value1;
			denom += value1;
		}
	}

	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			kernel.at<float>(a + n, b + n) /= denom; //normalization
		}
	}

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-paddle")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {

						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) { //if the pixel is not a border pixel
							kernelvalue = kernel.at<float>(a + n, b + n);
							sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
							sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
						}
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {
						kernelvalue = kernel.at<float>(a + n, b + n);
						if (i + a > row - 1) {  //mirroring for the border pixels
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
						sum1_r += kernelvalue * (float)(input.at<C>(tempa, tempb)[0]);
						sum1_g += kernelvalue * (float)(input.at<C>(tempa, tempb)[1]);
						sum1_b += kernelvalue * (float)(input.at<C>(tempa, tempb)[2]);
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				float sum2 = 0.0;
				for (int a = -n; a <= n; a++) { // for each kernel window
					for (int b = -n; b <= n; b++) {
						if ((i + a <= row - 1) && (i + a >= 0) && (j + b <= col - 1) && (j + b >= 0)) {
							kernelvalue = kernel.at<float>(a + n, b + n);
							sum1_r += kernelvalue * (float)(input.at<C>(i + a, j + b)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + a, j + b)[1]);
							sum1_b += kernelvalue * (float)(input.at<C>(i + a, j + b)[2]);
							sum2 += kernelvalue;
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(sum1_r / sum2);
				output.at<C>(i, j)[1] = (G)(sum1_g / sum2);
				output.at<C>(i, j)[2] = (G)(sum1_b / sum2);
			}
		}
	}
	return output;
}