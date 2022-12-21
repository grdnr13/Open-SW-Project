#include <opencv2/opencv.hpp>
#include <stdio.h>

#define IM_TYPE	CV_64FC3

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

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma);
Mat Gaussianfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, const char* opt);
Mat Gaussianfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, const char* opt);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale

	// 8-bit unsigned char -> 64-bit floating point
	input.convertTo(input, CV_64FC3, 1.0 / 255);
	input_gray.convertTo(input_gray, CV_64F, 1.0 / 255);

	// Add noise to original image
	Mat noise_Gray = Add_Gaussian_noise(input_gray, 0, 0.1);
	Mat noise_RGB = Add_Gaussian_noise(input, 0, 0.1);

	// Denoise, using gaussian filter -> "mirroring", "zero-padding", "adjustkernel"
	Mat Denoised_Gray = Gaussianfilter_Gray(noise_Gray, 3, 10, 10, "mirroring");
	Mat Denoised_RGB = Gaussianfilter_RGB(noise_RGB, 3, 10, 10, "zero-padding");

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Gaussian Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (Grayscale)", noise_Gray);

	namedWindow("Gaussian Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Gaussian Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_Gaussian_noise(const Mat input, double mean, double sigma) {

	Mat NoiseArr = Mat::zeros(input.rows, input.cols, input.type());
	RNG rng;
	rng.fill(NoiseArr, RNG::NORMAL, mean, sigma);

	add(input, NoiseArr, NoiseArr);

	return NoiseArr;
}

Mat Gaussianfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	float kernelvalue;

	// Initialiazing Gaussian Kernel Matrix
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
	// Fill code to initialize Gaussian filter kernel matrix
	float denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2))));
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

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {
				float sum1 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							//if the pixel is not a border pixel
							kernelvalue = kernel.at<float>(y + n, x + n);
							sum1 += kernelvalue * (float)(input.at<G>(i + y, j + x));
						}

					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1 = 0.0;
				int tempy, tempx;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						// Gaussian filter with "mirroring" process: 인접한 값으로 처리
						kernelvalue = kernel.at<float>(y + n, x + n);
						if (i + y > row - 1) {
							tempy = i - y;
						}
						else if (i + y < 0) {
							tempy = -(i + y);
						}
						else {
							tempy = i + y;
						}
						if (j + x > col - 1) {
							tempx = j - x;
						}
						else if (j + x < 0) {
							tempx = -(j + x);
						}
						else {
							tempx = j + x;
						}
						sum1 += kernelvalue * (float)(input.at<G>(tempy, tempx));
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1 = 0.0; float sum2 = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							kernelvalue = kernel.at<float>(y + n, x + n);
							sum1 += kernelvalue * (float)(input.at<G>(i + y, j + x));
							sum2 += kernelvalue;
						}
					}
				}
				output.at<G>(i, j) = (G)(sum1 / sum2);
			}

		}
	}
	return output;
}

Mat Gaussianfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	float kernelvalue;

	// Initialiazing Gaussian Kernel Matrix
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
	// Fill code to initialize Gaussian filter kernel matrix
	float denom = 0.0;
	for (int a = -n; a <= n; a++) {  // Denominator in m(s,t)
		for (int b = -n; b <= n; b++) {
			float value1 = exp(-(pow(a, 2) / (2 * pow(sigma_s, 2))) - (pow(b, 2) / (2 * pow(sigma_t, 2))));
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

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {

			if (!strcmp(opt, "zero-padding")) {
				float sum1_r = 0.0;
				float sum1_g = 0.0;
				float sum1_b = 0.0;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) { //if the pixel is not a border pixel
							kernelvalue = kernel.at<float>(y + n, x + n);
							sum1_r += kernelvalue * (float)(input.at<C>(i + y, j + x)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + y, j + x)[1]);
							sum1_b += kernelvalue * (float)(input.at<C>(i + y, j + x)[2]);
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
				int tempy, tempx;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						kernelvalue = kernel.at<float>(y + n, x + n);
						if (i + y > row - 1) {  //mirroring for the border pixels
							tempy = i - y;
						}
						else if (i + y < 0) {
							tempy = -(i + y);
						}
						else {
							tempy = i + y;
						}
						if (j + x > col - 1) {
							tempx = j - x;
						}
						else if (j + x < 0) {
							tempx = -(j + x);
						}
						else {
							tempx = j + x;
						}
						sum1_r += kernelvalue * (float)(input.at<C>(tempy, tempx)[0]);
						sum1_g += kernelvalue * (float)(input.at<C>(tempy, tempx)[1]);
						sum1_b += kernelvalue * (float)(input.at<C>(tempy, tempx)[2]);
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
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							kernelvalue = kernel.at<float>(y + n, x + n);
							sum1_r += kernelvalue * (float)(input.at<C>(i + y, j + x)[0]);
							sum1_g += kernelvalue * (float)(input.at<C>(i + y, j + x)[1]);
							sum1_b += kernelvalue * (float)(input.at<C>(i + y, j + x)[2]);
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