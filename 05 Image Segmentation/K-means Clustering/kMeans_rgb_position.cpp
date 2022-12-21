#include <iostream>
#include <opencv2/opencv.hpp>

#define IM_TYPE	CV_8UC3

using namespace cv;

// Note that this code is for the case when an input data is a color value.
int main() {

	Mat input = imread("lena.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input_gray;
	cvtColor(input, input_gray, CV_RGB2GRAY); // Converting image to gray

	if (!input.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}
	const float sigma_x = 5, sigma_y = 5;
	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", input);

	// Clustering is performed for each channel (RGB) & position

	Mat samples_rgb_position(input.rows * input.cols, 5, CV_32F);
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++) {
			for (int z = 0; z < 3; z++)
				samples_rgb_position.at<float>(x + y * input.rows, z) = input.at<Vec3b>(y, x)[z];
			samples_rgb_position.at<float>(x + y * input.rows, 3) = x / sigma_x;
			samples_rgb_position.at<float>(x + y * input.rows, 4) = y / sigma_y;
		}

	int clusterCount = 10;
	Mat labels;
	int attempts = 5;
	Mat centers;
	kmeans(samples_rgb_position, clusterCount, labels, TermCriteria(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers);
	Mat new_image(input.size(), input.type());
	//using inten
	for (int y = 0; y < input.rows; y++)
		for (int x = 0; x < input.cols; x++)
		{
			int cluster_idx = labels.at<int>(x + y * input.rows, 0);
			//for each pixel of each channel, Fill code that finds the intensity of the cluster center of the output image
			for (int z = 0; z < 3; z++) {
				int val = centers.at<float>(cluster_idx, z);
				if (val > 255)val = 255;
				if (val < 0)val = 0;
				new_image.at<Vec3b>(y, x)[z] = val;
			}
		}

	//namedWindow("clustered image", WINDOW_AUTOSIZE);
	imshow("clustered image - rgb & position", new_image);


	waitKey(0);

	return 0;
}

