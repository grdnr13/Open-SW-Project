#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <algorithm>
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

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp);
Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt);
Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char *opt);

int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;

	// check for validation
	if (!input.data) {
		printf("Could not open\n");
		return -1;
	}

	cvtColor(input, input_gray, CV_RGB2GRAY);	// convert RGB to Grayscale

												// Add noise to original image
	Mat noise_Gray = Add_salt_pepper_Noise(input_gray, 0.1f, 0.1f);
	Mat noise_RGB = Add_salt_pepper_Noise(input, 0.1f, 0.1f);

	// Denoise, using median filter
	int window_radius = 2;
	Mat Denoised_Gray = Salt_pepper_noise_removal_Gray(noise_Gray, window_radius, "adjustkernel");
	//"mirroring", "zero-padding", "adjustkernel"
	Mat Denoised_RGB = Salt_pepper_noise_removal_RGB(noise_RGB, window_radius, "zero-padding");
	//"mirroring", "zero-padding", "adjustkernel"

	namedWindow("Grayscale", WINDOW_AUTOSIZE);
	imshow("Grayscale", input_gray);

	namedWindow("RGB", WINDOW_AUTOSIZE);
	imshow("RGB", input);

	namedWindow("Impulse Noise (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (Grayscale)", noise_Gray);

	namedWindow("Impulse Noise (RGB)", WINDOW_AUTOSIZE);
	imshow("Impulse Noise (RGB)", noise_RGB);

	namedWindow("Denoised (Grayscale)", WINDOW_AUTOSIZE);
	imshow("Denoised (Grayscale)", Denoised_Gray);

	namedWindow("Denoised (RGB)", WINDOW_AUTOSIZE);
	imshow("Denoised (RGB)", Denoised_RGB);

	waitKey(0);

	return 0;
}

Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp)
{
	Mat output = input.clone(); //input 이미지 복제
	RNG rng; //난수발생기

	int amount1 = (int)(output.rows * output.cols * pp); //black noise 밀도
	int amount2 = (int)(output.rows * output.cols * ps); //white noise 밀도

	int x, y;

	// Grayscale image
	if (output.channels() == 1) {
		//black noise
		for (int counter = 0; counter < amount1; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 0;
		//white noise
		for (int counter = 0; counter < amount2; ++counter)
			output.at<G>(rng.uniform(0, output.rows), rng.uniform(0, output.cols)) = 255;
	}
	// Color image
	else if (output.channels() == 3) {
		//generate random white noise
		for (int counter = 0; counter < amount1; ++counter) {
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 0; //red channel

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 0; //green channel

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 0; //blue channel
		}
		//generate random black noise
		for (int counter = 0; counter < amount2; ++counter) {
			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[0] = 255; //red channel

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[1] = 255; //green channel

			x = rng.uniform(0, output.rows);
			y = rng.uniform(0, output.cols);
			output.at<C>(x, y)[2] = 255; //blue channel
		}
	}
	return output;
}

Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int median;		// index of median value
	int tempy, tempx;
	// initialize median filter kernel
	Mat kernel = Mat::zeros(kernel_size * kernel_size, 1, input.type());
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int idx = 0; //kernel에 들어가는 값 개수
			if (!strcmp(opt, "zero-padding")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							//if the pixel is not a border pixel
							kernel.at<G>(idx) = input.at<G>(i + y, j + x);
						}
						else {
							kernel.at<G>(idx) = 0;
						}
						idx++;
					}
				}
			}

			else if (!strcmp(opt, "mirroring")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {

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
						kernel.at<G>(idx) = input.at<G>(tempy, tempx);
						idx++;
					}
				}
			}

			else if (!strcmp(opt, "adjustkernel")) {
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							kernel.at<G>(idx) = (input.at<G>(i + y, j + x));
							idx++;
						}
					}
				}
				for (int i = idx; i < kernel_size * kernel_size; i++) kernel.at<G>(idx) = 255;
			}

			// Sort the kernels in ascending order
			sort(kernel, kernel, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
			median = idx / 2;
			output.at<G>(i, j) = kernel.at<G>(median, 0);
		}
	}

	return output;
}

Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char* opt) {

	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	int median;		// index of median value
	int channel = input.channels();

	// initialize ( (TypeX with 3 channel) - (TypeX with 1 channel) = 16 )
	// ex) CV_8UC3 - CV_8U = 16
	Mat kernel = Mat::zeros(kernel_size * kernel_size, channel, input.type() - 16);
	
	Mat output = Mat::zeros(row, col, input.type());

	// convolution
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int idx = 0; //kernel에 들어가는 값 개수
			if (!strcmp(opt, "zero-padding")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							//if the pixel is not a border pixel
							kernel.at<G>(idx, 0) = input.at<C>(i + y, j + x)[0];
							kernel.at<G>(idx, 1) = input.at<C>(i + y, j + x)[1];
							kernel.at<G>(idx, 2) = input.at<C>(i + y, j + x)[2];
						}
						else {
							//border 밖: 0으로 처리
							kernel.at<G>(idx, 0) = 0;
							kernel.at<G>(idx, 1) = 0;
							kernel.at<G>(idx, 2) = 0;
						}
						idx++;
					}
				}
			}

			else if (!strcmp(opt, "mirroring")) {
				int tempy, tempx;
				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
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
						kernel.at<G>(idx, 0) = input.at<C>(tempy, tempx)[0];
						kernel.at<G>(idx, 1) = input.at<C>(tempy, tempx)[1];
						kernel.at<G>(idx, 2) = input.at<C>(tempy, tempx)[2];
						idx++;
					}
				}
			}

			else if (!strcmp(opt, "adjustkernel")) {

				for (int x = -n; x <= n; x++) { // for each kernel window
					for (int y = -n; y <= n; y++) {
						
						if ((i + y <= row - 1) && (i + y >= 0) && (j + x <= col - 1) && (j + x >= 0)) {
							kernel.at<G>(idx, 0) = input.at<C>(i + y, j + x)[0];
							kernel.at<G>(idx, 1) = input.at<C>(i + y, j + x)[1];
							kernel.at<G>(idx, 2) = input.at<C>(i + y, j + x)[2];
							idx++;
						}
					}
				}
				//sort 위해 비어있는 값 추가(최댓값 이용해서 정렬에 영향 없도록 설정)
				for (int i = idx; i < kernel_size * kernel_size; i++) {
					kernel.at<G>(idx, 0) = 255;
					kernel.at<G>(idx, 1) = 255;
					kernel.at<G>(idx, 2) = 255;
				}
			}

			// Sort the kernels in ascending order
			sort(kernel, kernel, CV_SORT_EVERY_COLUMN + CV_SORT_ASCENDING);
			median = idx / 2;
			output.at<C>(i, j)[0] = kernel.at<G>(median, 0);
			output.at<C>(i, j)[1] = kernel.at<G>(median, 1);
			output.at<C>(i, j)[2] = kernel.at<G>(median, 2);
		}
	}

	return output;
}