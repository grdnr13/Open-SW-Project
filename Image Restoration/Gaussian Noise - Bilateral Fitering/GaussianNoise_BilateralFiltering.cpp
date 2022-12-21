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
Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);
Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt);

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

	// Denoise, using bilateral filter -> "mirroring", "zero-padding", "adjustkernel"
	Mat Denoised_Gray = Bilateralfilter_Gray(noise_Gray, 3, 10, 10, 0.13, "zero-padding");
	Mat Denoised_RGB = Bilateralfilter_RGB(noise_RGB, 3, 10, 10, 0.1, "adjustkernel");

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

Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {
	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);
	float kernelvalue;

	// Initialiazing Kernel Matrix
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);
	
	// Fill code to initialize filter kernel matrix
	for (int s = -n; s <= n; s++) {
		for (int t = -n; t <= n; t++) {
			float value1 = exp(-(pow(s, 2) / (2 * pow(sigma_s, 2))) - (pow(t, 2) / (2 * pow(sigma_t, 2))));
			kernel.at<float>(s + n, t + n) = value1;
		}
	}

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			Mat kernel_W = Mat::zeros(kernel_size, kernel_size, CV_32F);

			if (!strcmp(opt, "zero-padding")) {
				float sum1 = 0.0;
				float W = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) {
					for (int t = -n; t <= n; t++) {
						float kernelvalue = kernel.at<float>(s + n, t + n);

						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							float kernel_cal = kernelvalue * exp(-(pow(input.at<G>(i, j) - input.at<G>(i + s, j + t), 2) / (2 * pow(sigma_r, 2))));
							kernel_W.at<float>(s + n, t + n) = kernel_cal;
							W += kernel_cal;
						}
						else {
							float kernel_cal = kernelvalue * exp(-(pow(input.at<G>(i, j) - 0, 2) / (2 * pow(sigma_r, 2))));
							kernel_W.at<float>(s + n, t + n) = kernel_cal;
							W += kernel_cal;
						}
					}
				}
				//이미지 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							//if the pixel is not a border pixel
							kernelvalue = kernel_W.at<float>(t + n, s + n);
							sum1 += kernelvalue / W * input.at<G>(i + s, j + t);
						}
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1 = 0.0;
				int tempy, tempx;
				float W = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						kernelvalue = kernel.at<float>(s + n, t + n);

						if (i + s > row - 1) tempy = i - s;
						else if (i + s < 0) tempy = -(i + s);
						else tempy = i + s;

						if (j + t > col - 1) tempx = j - t;
						else if (j + t < 0)	tempx = -(j + t);
						else tempx = j + t;

						float kernel_cal = kernelvalue * exp(-(pow(input.at<G>(i, j) - input.at<G>(tempy, tempx), 2) / (2 * pow(sigma_r, 2))));
						kernel_W.at<float>(s + n, t + n) = kernel_cal;
						W += kernel_cal;
					}
				}
				//이미지 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if (i + s > row - 1) tempy = i - s;
						else if (i + s < 0) tempy = -(i + s);
						else tempy = i + s;

						if (j + t > col - 1) tempx = j - t;
						else if (j + t < 0)	tempx = -(j + t);
						else tempx = j + t;

						kernelvalue = kernel_W.at<float>(s + n, t + n);
						sum1 += kernelvalue / W * input.at<G>(tempy, tempx);
					}
				}
				output.at<G>(i, j) = (G)sum1;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1 = 0.0; float sum2 = 0.0;
				float W = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							float kernelvalue = kernel.at<float>(s + n, t + n);
							float kernel_cal = kernelvalue * exp(-(pow(input.at<G>(i, j) - input.at<G>(i + s, j + t), 2) / (2 * pow(sigma_r, 2))));
							kernel_W.at<float>(s + n, t + n) = kernel_cal;
							W += kernel_cal;
						}
						else {
							kernel_W.at<float>(s + n, t + n) = 0;
						}
					}
				}
				//image 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							float kernelvalue = kernel_W.at<float>(s + n, t + n);
							sum1 += kernelvalue / W * input.at<G>(i + s, j + t);
							sum2 += kernelvalue / W;
						}
					}
				}
				output.at<G>(i, j) = (G)(sum1 / sum2);
			}

		}
	}
	return output;
}

Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt) {
	int row = input.rows;
	int col = input.cols;
	int kernel_size = (2 * n + 1);

	// Initialiazing Kernel Matrix
	Mat kernel = Mat::zeros(kernel_size, kernel_size, CV_32F);

	// Fill code to initialize filter kernel matrix
	for (int s = -n; s <= n; s++) {
		for (int t = -n; t <= n; t++) {
			float value1 = exp(-(pow(s, 2) / (2 * pow(sigma_s, 2))) - (pow(t, 2) / (2 * pow(sigma_t, 2))));
			kernel.at<float>(s + n, t + n) = value1;
		}
	}

	Mat output = Mat::zeros(row, col, input.type());

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			Mat kernel_W_r = Mat::zeros(kernel_size, kernel_size, CV_32F);
			Mat kernel_W_g = Mat::zeros(kernel_size, kernel_size, CV_32F);
			Mat kernel_W_b = Mat::zeros(kernel_size, kernel_size, CV_32F);

			if (!strcmp(opt, "zero-padding")) {
				float sum1_r = 0.0, sum1_g = 0.0, sum1_b = 0.0;
				float W_r = 0.0, W_g = 0.0, W_b = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) {
					for (int t = -n; t <= n; t++) {
						float kernelvalue = kernel.at<float>(s + n, t + n);
						float kernel_cal_r, kernel_cal_g, kernel_cal_b;
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							kernel_cal_r = kernelvalue * exp(-(pow(input.at<C>(i, j)[0] - input.at<C>(i + s, j + t)[0], 2) / (2 * pow(sigma_r, 2))));
							kernel_cal_g = kernelvalue * exp(-(pow(input.at<C>(i, j)[1] - input.at<C>(i + s, j + t)[1], 2) / (2 * pow(sigma_r, 2))));
							kernel_cal_b = kernelvalue * exp(-(pow(input.at<C>(i, j)[2] - input.at<C>(i + s, j + t)[2], 2) / (2 * pow(sigma_r, 2))));
						}
						else {
							kernel_cal_r = kernelvalue * exp(-(pow(input.at<C>(i, j)[0] - 0, 2) / (2 * pow(sigma_r, 2))));
							kernel_cal_g = kernelvalue * exp(-(pow(input.at<C>(i, j)[1] - 0, 2) / (2 * pow(sigma_r, 2))));
							kernel_cal_b = kernelvalue * exp(-(pow(input.at<C>(i, j)[2] - 0, 2) / (2 * pow(sigma_r, 2))));
						}
						kernel_W_r.at<float>(s + n, t + n) = kernel_cal_r;
						kernel_W_g.at<float>(s + n, t + n) = kernel_cal_g;
						kernel_W_b.at<float>(s + n, t + n) = kernel_cal_b;
						W_r += kernel_cal_r;
						W_g += kernel_cal_g;
						W_b += kernel_cal_b;
					}
				}
				//픽셀 값 계산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							//if the pixel is not a border pixel
							float kernelvalue_r = kernel_W_r.at<float>(t + n, s + n);
							float kernelvalue_g = kernel_W_g.at<float>(t + n, s + n);
							float kernelvalue_b = kernel_W_b.at<float>(t + n, s + n);
							sum1_r += kernelvalue_r / W_r * input.at<C>(i + s, j + t)[0];
							sum1_g += kernelvalue_g / W_g * input.at<C>(i + s, j + t)[1];
							sum1_b += kernelvalue_b / W_b * input.at<C>(i + s, j + t)[2];
						}
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "mirroring")) {
				float sum1_r = 0.0, sum1_g = 0.0, sum1_b = 0.0;
				int tempy, tempx;
				float W_r = 0.0, W_g = 0.0, W_b = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						float kernelvalue = kernel.at<float>(s + n, t + n);

						if (i + s > row - 1) tempy = i - s;
						else if (i + s < 0) tempy = -(i + s);
						else tempy = i + s;

						if (j + t > col - 1) tempx = j - t;
						else if (j + t < 0)	tempx = -(j + t);
						else tempx = j + t;

						float kernel_cal_r = kernelvalue * exp(-(pow(input.at<C>(i, j)[0] - input.at<C>(tempy, tempx)[0], 2) / (2 * pow(sigma_r, 2))));
						float kernel_cal_g = kernelvalue * exp(-(pow(input.at<C>(i, j)[1] - input.at<C>(tempy, tempx)[1], 2) / (2 * pow(sigma_r, 2))));
						float kernel_cal_b = kernelvalue * exp(-(pow(input.at<C>(i, j)[2] - input.at<C>(tempy, tempx)[2], 2) / (2 * pow(sigma_r, 2))));
						kernel_W_r.at<float>(s + n, t + n) = kernel_cal_r;
						kernel_W_g.at<float>(s + n, t + n) = kernel_cal_g;
						kernel_W_b.at<float>(s + n, t + n) = kernel_cal_b;
						W_r += kernel_cal_r; W_g += kernel_cal_g; W_b += kernel_cal_b;
					}
				}
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if (i + s > row - 1) tempy = i - s;
						else if (i + s < 0) tempy = -(i + s);
						else tempy = i + s;

						if (j + t > col - 1) tempx = j - t;
						else if (j + t < 0)	tempx = -(j + t);
						else tempx = j + t;

						float kernelvalue_r = kernel_W_r.at<float>(s + n, t + n);
						float kernelvalue_g = kernel_W_g.at<float>(s + n, t + n);
						float kernelvalue_b = kernel_W_b.at<float>(s + n, t + n);
						sum1_r += kernelvalue_r / W_r * input.at<C>(tempy, tempx)[0];
						sum1_g += kernelvalue_g / W_g * input.at<C>(tempy, tempx)[1];
						sum1_b += kernelvalue_b / W_b * input.at<C>(tempy, tempx)[2];
					}
				}
				output.at<C>(i, j)[0] = (G)sum1_r;
				output.at<C>(i, j)[1] = (G)sum1_g;
				output.at<C>(i, j)[2] = (G)sum1_b;
			}

			else if (!strcmp(opt, "adjustkernel")) {
				float sum1_r = 0.0, sum1_g = 0.0, sum1_b = 0.0;
				float sum2_r = 0.0, sum2_g = 0.0, sum2_b = 0.0;
				float W_r = 0.0, W_g = 0.0, W_b = 0.0;
				//kernel 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							float kernelvalue = kernel.at<float>(s + n, t + n);
							float kernel_cal_r = kernelvalue * exp(-(pow(input.at<C>(i, j)[0] - input.at<C>(i + s, j + t)[0], 2) / (2 * pow(sigma_r, 2))));
							float kernel_cal_g = kernelvalue * exp(-(pow(input.at<C>(i, j)[1] - input.at<C>(i + s, j + t)[1], 2) / (2 * pow(sigma_r, 2))));
							float kernel_cal_b = kernelvalue * exp(-(pow(input.at<C>(i, j)[2] - input.at<C>(i + s, j + t)[2], 2) / (2 * pow(sigma_r, 2))));
							kernel_W_r.at<float>(s + n, t + n) = kernel_cal_r;
							kernel_W_g.at<float>(s + n, t + n) = kernel_cal_g;
							kernel_W_b.at<float>(s + n, t + n) = kernel_cal_b;
							W_r += kernel_cal_r; W_g += kernel_cal_g; W_b += kernel_cal_b;
						}
						else {
							kernel_W_r.at<float>(s + n, t + n) = 0;
							kernel_W_g.at<float>(s + n, t + n) = 0;
							kernel_W_b.at<float>(s + n, t + n) = 0;
						}
					}
				}
				//image 연산
				for (int s = -n; s <= n; s++) { // for each kernel window
					for (int t = -n; t <= n; t++) {
						if ((i + s <= row - 1) && (i + s >= 0) && (j + t <= col - 1) && (j + t >= 0)) {
							float kernelvalue_r = kernel_W_r.at<float>(s + n, t + n);
							float kernelvalue_g = kernel_W_g.at<float>(s + n, t + n);
							float kernelvalue_b = kernel_W_b.at<float>(s + n, t + n);
							sum1_r += kernelvalue_r / W_r * input.at<C>(i + s, j + t)[0];
							sum1_g += kernelvalue_g / W_g * input.at<C>(i + s, j + t)[1];
							sum1_b += kernelvalue_b / W_b * input.at<C>(i + s, j + t)[2];
							sum2_r += kernelvalue_r / W_r;
							sum2_g += kernelvalue_g / W_g;
							sum2_b += kernelvalue_b / W_b;
						}
					}
				}
				output.at<C>(i, j)[0] = (G)(sum1_r / sum2_r);
				output.at<C>(i, j)[1] = (G)(sum1_g / sum2_g);
				output.at<C>(i, j)[2] = (G)(sum1_b / sum2_b);
			}

		}
	}
	return output;
}
