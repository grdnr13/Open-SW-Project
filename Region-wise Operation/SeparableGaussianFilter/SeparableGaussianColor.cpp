#include <iostream>
#include <opencv2/opencv.hpp>
#include <math.h>       /* exp */
#include <ctime>
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

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt);

int main() {
    time_t start, finish; double duration; // 시간 측정용

    Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
    Mat output;

    if (!input.data)
    {
        std::cout << "Could not open" << std::endl;
        return -1;
    }

    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", input);

    //이미지 연산 시간 측정 시작
    start = clock();

    //Boundary process: zero-paddle, mirroring, adjustkernel
    //output = gaussianfilter(input, 1, 1, 1, "mirroring"); std::cout << "using mirroring\n";
    //output = gaussianfilter(input, 10, 1, 1, "zero-paddle"); std::cout << "using zero-paddle\n";
//    output = gaussianfilter(input, 1, 1, 1, "adjustkernel"); std::cout << "using adjust kernel\n";

    //이미지 연산 시간 측정 종료
    finish = clock();  duration = (double)(finish - start);

    namedWindow("Gaussian Separation Filter", WINDOW_AUTOSIZE);
    imshow("Gaussian Separation Filter", output);

    std::cout << "Gaussian Separable at n=10\ntime spent: " << duration << 'ms\n';

    waitKey(0);

    return 0;
}

Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt) {

    //Mat kernel;

    int row = input.rows;
    int col = input.cols;
    int kernel_size = (2 * n + 1);
    int temp;
    float denom_s, denom_t;
    float kernelvalue;

    // Initialiazing Kernel Matrix 
    Mat kernel_s = Mat::zeros(kernel_size, 1, CV_32F);
    Mat kernel_t = Mat::zeros(1, kernel_size, CV_32F);

    denom_s = 0.0; denom_t = 0.0;
    for (int idx = -n; idx <= n; idx++) {
        //calculate X-Gaussian
        float value1_s = exp(-(pow(idx, 2) / (2 * pow(sigmaS, 2))));
        kernel_s.at<float>(idx + n, 0) = value1_s;
        denom_s += value1_s;
        //calculate Y-Gaussian
        float value1_t = exp(-(pow(idx, 2) / (2 * pow(sigmaT, 2))));
        kernel_t.at<float>(0, idx + n) = value1_t;
        denom_t += value1_t;
    }

    for (int idx = -n; idx <= n; idx++) {
        kernel_s.at<float>(idx + n, 0) /= denom_s; //normalization
        kernel_t.at<float>(0, idx + n) /= denom_t; //normalization
    }

    Mat tmp = Mat::zeros(row, col, input.type());
    Mat output = Mat::zeros(row, col, input.type());

    //kernel_t 이용 연산 : Y-Gaussian
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                for (int idx = -n; idx <= n; idx++) {
                    //Gaussian filter with Zero-paddle boundary process: 무조건 0으로 처리
                    if ((j + idx <= col - 1) && (j + idx >= 0)) {
                        //if the pixel is not a border pixel
                        kernelvalue = kernel_t.at<float>(0, idx + n);
                        sum1_r += kernelvalue * (float)(input.at<C>(i, j + idx)[0]);
                        sum1_g += kernelvalue * (float)(input.at<C>(i, j + idx)[1]);
                        sum1_b += kernelvalue * (float)(input.at<C>(i, j + idx)[2]);
                    }
                }
                tmp.at<C>(i, j)[0] = (G)sum1_r;
                tmp.at<C>(i, j)[1] = (G)sum1_g;
                tmp.at<C>(i, j)[2] = (G)sum1_b;
            }

            else if (!strcmp(opt, "mirroring")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                for (int idx = -n; idx <= n; idx++) {
                    // Gaussian filter with "mirroring" process: 인접한 값으로 처리
                    kernelvalue = kernel_t.at<float>(0, idx + n);
                    if (j + idx > col - 1) {
                        temp = j - idx;
                    }
                    else if (j + idx < 0) {
                        temp = -(j + idx);
                    }
                    else {
                        temp = j + idx;
                    }
                    sum1_r += kernelvalue * (float)(input.at<C>(i, temp)[0]);
                    sum1_g += kernelvalue * (float)(input.at<C>(i, temp)[1]);
                    sum1_b += kernelvalue * (float)(input.at<C>(i, temp)[2]);
                }
                tmp.at<C>(i, j)[0] = (G)sum1_r;
                tmp.at<C>(i, j)[1] = (G)sum1_g;
                tmp.at<C>(i, j)[2] = (G)sum1_b;
            }
            else if (!strcmp(opt, "adjustkernel")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                float sum2 = 0.0;
                for (int idx = -n; idx <= n; idx++) {
                    // Gaussian filter with "adjustkernel" process: 테두리 바깥은 연산에서 배제
                    if ((j + idx <= col - 1) && (j + idx >= 0)) {
                        kernelvalue = kernel_t.at<float>(0, idx + n);
                        sum1_r += kernelvalue * (float)(input.at<C>(i, j + idx)[0]);
                        sum1_g += kernelvalue * (float)(input.at<C>(i, j + idx)[1]);
                        sum1_b += kernelvalue * (float)(input.at<C>(i, j + idx)[2]);
                        sum2 += kernelvalue;
                    }
                }
                tmp.at<C>(i, j)[0] = (G)(sum1_r / sum2);
                tmp.at<C>(i, j)[1] = (G)(sum1_g / sum2);
                tmp.at<C>(i, j)[2] = (G)(sum1_b / sum2);
            }
        }
    }

    //kernel_s 이용 연산: X_Gaussian
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            if (!strcmp(opt, "zero-paddle")) {
                float sum1_r = 0.0;
                float sum1_g = 0.0;
                float sum1_b = 0.0;
                for (int idx = -n; idx <= n; idx++) {
                    //Gaussian filter with Zero-paddle boundary process: 무조건 0으로 처리
                    if ((i + idx <= row - 1) && (i + idx >= 0)) {
                        //if the pixel is not a border pixel
                        kernelvalue = kernel_s.at<float>(idx + n, 0);
                        sum1_r += kernelvalue * (float)(tmp.at<C>(i + idx, j)[0]);
                        sum1_g += kernelvalue * (float)(tmp.at<C>(i + idx, j)[1]);
                        sum1_b += kernelvalue * (float)(tmp.at<C>(i + idx, j)[2]);
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
                for (int idx = -n; idx <= n; idx++) {
                    // Gaussian filter with "mirroring" process: 인접한 값으로 처리
                    kernelvalue = kernel_s.at<float>(idx + n, 0);
                    if (i + idx > row - 1) {
                        temp = i - idx;
                    }
                    else if (i + idx < 0) {
                        temp = -(i + idx);
                    }
                    else {
                        temp = i + idx;
                    }
                    sum1_r += kernelvalue * (float)(tmp.at<C>(temp, j)[0]);
                    sum1_g += kernelvalue * (float)(tmp.at<C>(temp, j)[1]);
                    sum1_b += kernelvalue * (float)(tmp.at<C>(temp, j)[2]);
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
                for (int idx = -n; idx <= n; idx++) {
                    // Gaussian filter with "adjustkernel" process: 테두리 바깥은 연산에서 배제
                    if ((i + idx <= row - 1) && (i + idx >= 0)) {
                        kernelvalue = kernel_s.at<float>(idx + n, 0);
                        sum1_r += kernelvalue * (float)(tmp.at<C>(i + idx, j)[0]);
                        sum1_g += kernelvalue * (float)(tmp.at<C>(i + idx, j)[1]);
                        sum1_b += kernelvalue * (float)(tmp.at<C>(i + idx, j)[2]);
                        sum2 += kernelvalue;
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