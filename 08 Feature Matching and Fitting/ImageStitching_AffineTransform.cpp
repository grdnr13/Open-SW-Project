// stitching.cpp : Defines the entry point for the console application.
//
#include <opencv2/opencv.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#define RATIO_THR 0.4

using namespace std;
using namespace cv;

double euclidDistance(Mat& vec1, Mat& vec2);
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors);
int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int NN);
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold);

Mat cal_affine(vector<Point2f> srcPoints, vector<Point2f> dstPoints);
Mat cal_affine_RANSAC(vector<Point2f> srcPoints, vector<Point2f> dstPoints, vector<int> ki);
void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int diff_x, int diff_y, float alpha);


int main() {

	Mat input1 = imread("input1.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input2 = imread("input2.jpg", CV_LOAD_IMAGE_COLOR);
	Mat input1_gray, input2_gray;

	if (!input1.data || !input2.data)
	{
		std::cout << "Could not open" << std::endl;
		return -1;
	}

	//resize(input1, input1, Size(input1.cols / 2, input1.rows / 2));
	//resize(input2, input2, Size(input2.cols / 2, input2.rows / 2));

	cvtColor(input1, input1_gray, CV_RGB2GRAY);
	cvtColor(input2, input2_gray, CV_RGB2GRAY);

	FeatureDetector* detector = new SiftFeatureDetector(
		0,		// nFeatures
		4,		// nOctaveLayers
		0.04,	// contrastThreshold
		10,		// edgeThreshold
		1.6		// sigma
	);
	//https://physics.nyu.edu/grierlab/manuals/opencv/classcv_1_1SiftDescriptorExtractor.html
	DescriptorExtractor* extractor = new SiftDescriptorExtractor();

	// Create a image for displaying mathing keypoints
	Size size2 = input2.size();
	Size sz2 = Size(size2.width + input1_gray.size().width, max(size2.height, input1_gray.size().height));
	Mat matchingImage2 = Mat::zeros(sz2, CV_8UC3);


	input1.copyTo(matchingImage2(Rect(size2.width, 0, input1_gray.size().width, input1_gray.size().height)));
	input2.copyTo(matchingImage2(Rect(0, 0, size2.width, size2.height)));

	// Compute keypoints and descriptor from the source image in advance
	vector<KeyPoint> keypoints1;
	Mat descriptors1;

	//https://docs.opencv.org/3.4/d0/d13/classcv_1_1Feature2D.html#ab3cce8d56f4fc5e1d530b5931e1e8dc0
	detector->detect(input1_gray, keypoints1);
	extractor->compute(input1_gray, keypoints1, descriptors1);
	printf("input1 : %d keypoints are found.\n", (int)keypoints1.size());

	vector<KeyPoint> keypoints2;
	Mat descriptors2;

	// Detect keypoints
	detector->detect(input2_gray, keypoints2); //keypoints2에 detect
	extractor->compute(input2_gray, keypoints2, descriptors2);
	printf("input2 : %zd keypoints are found.\n", keypoints2.size());

	for (int i = 0; i < keypoints1.size(); i++) {
		KeyPoint kp = keypoints1[i];
		kp.pt.x += size2.width; //matchingImage에서의 keypoint X좌표
		circle(matchingImage2, kp.pt, cvRound(kp.size * 0.25), Scalar(255, 255, 0), 1, 8, 0);
		//matchingImage에 동그라미로 표시
	}

	for (int i = 0; i < keypoints2.size(); i++) {
		KeyPoint kp = keypoints2[i];
		circle(matchingImage2, kp.pt, cvRound(kp.size * 0.25), Scalar(255, 255, 0), 1, 8, 0);
		//matchingImage에 동그라미로 표시
	}

	vector<Point2f> srcPoints;
	vector<Point2f> dstPoints;
	bool crossCheck = true;
	bool ratio_threshold = true;
	findPairs(keypoints2, descriptors2, keypoints1, descriptors1, srcPoints, dstPoints, crossCheck, ratio_threshold);
	printf("%zd keypoints are matched.\n", srcPoints.size());




	///affine............
	bool RANSAC = true;

	input1.convertTo(input1, CV_32FC3, 1.0 / 255);
	input2.convertTo(input2, CV_32FC3, 1.0 / 255);

	// height(row), width(col) of each image
	const float I1_row = input1.rows;
	const float I2_row = input2.rows;
	const float I1_col = input1.cols;
	const float I2_col = input2.cols;


	Mat A12, A21;
	if (RANSAC) {
		int s = 7;
		int maxInliers = 0;
		int delta = 400;
		vector<int> inliers;

		//RANSAC - calculate inliers
		while(s--) {
			int inliers_num = 0;
			int u = 0;
			vector<int> list;

			///random 3 points
			srand(time(NULL));
			vector<int> random_3points;
			for (int i = 0; i < 3; i++) {
				random_3points.push_back(rand() % srcPoints.size());
			}

			A12 = cal_affine_RANSAC(dstPoints, srcPoints, random_3points);

			for (int j = 0; j < dstPoints.size(); j++) {
				Point2f p12(A12.at<float>(0) * dstPoints[j].y + A12.at<float>(1) * dstPoints[j].x + A12.at<float>(2), A12.at<float>(3) * dstPoints[j].y + A12.at<float>(4) * dstPoints[j].x + A12.at<float>(5));
				if ((p12.x - srcPoints[j].x) * (p12.x - srcPoints[j].x) + (p12.y - srcPoints[j].y) * (p12.y - srcPoints[j].y) < delta * delta) {
					inliers_num = inliers_num + 1;
					list.push_back(j);
					u++;
				}
			}
			if (inliers_num > maxInliers) {
				maxInliers = inliers_num;
				for (int j = u - 1; j > 0; j--) {
					inliers.push_back(list.at(j));
				}
			}
		}

		A12 = cal_affine_RANSAC(dstPoints, srcPoints, inliers);
		A21 = cal_affine_RANSAC(srcPoints, dstPoints, inliers);
	}
	else {

		A12 = cal_affine(dstPoints, srcPoints);
		A21 = cal_affine(srcPoints, dstPoints);

	}

	// compute corners 
	Point2f p1(A21.at<float>(0) * 0 + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * 0 + A21.at<float>(5));
	Point2f p2(A21.at<float>(0) * 0 + A21.at<float>(1) * I2_col + A21.at<float>(2), A21.at<float>(3) * 0 + A21.at<float>(4) * I2_col + A21.at<float>(5));
	Point2f p3(A21.at<float>(0) * I2_row + A21.at<float>(1) * 0 + A21.at<float>(2), A21.at<float>(3) * I2_row + A21.at<float>(4) * 0 + A21.at<float>(5));
	Point2f p4(A21.at<float>(0) * I2_row + A21.at<float>(1) * I2_col + A21.at<float>(2), A21.at<float>(3) * I2_row + A21.at<float>(4) * I2_col + A21.at<float>(5));

	// inverse warping
	Point2f p1_(A12.at<float>(0) * 0 + A12.at<float>(1) * 0 + A12.at<float>(2), A12.at<float>(3) * 0 + A12.at<float>(4) * 0 + A12.at<float>(5));
	Point2f p2_(A12.at<float>(0) * 0 + A12.at<float>(1) * I1_col + A12.at<float>(2), A12.at<float>(3) * 0 + A12.at<float>(4) * I1_col + A12.at<float>(5));
	Point2f p3_(A12.at<float>(0) * I1_row + A12.at<float>(1) * 0 + A12.at<float>(2), A12.at<float>(3) * I1_row + A12.at<float>(4) * 0 + A12.at<float>(5));
	Point2f p4_(A12.at<float>(0) * I1_row + A12.at<float>(1) * I1_col + A12.at<float>(2), A12.at<float>(3) * I1_row + A12.at<float>(4) * I1_col + A12.at<float>(5));

	//I_f의 경계면 연산
	int up = round(min(0.0f, min(p1.x, p2.x)));
	int down = round(max(I1_row, max(p3.x, p4.x)));
	int left = round(min(0.0f, min(p1.y, p3.y)));
	int right = round(max(I1_col, max(p2.y, p4.y)));

	//inverse warping용 경계면 연산
	int inv_u = round(min(0.0f, min(p1_.x, p2_.x)));
	int inv_d = round(max(I2_row, std::max(p3_.x, p4_.x)));
	int inv_l = round(min(0.0f, min(p1_.y, p3_.y)));
	int inv_r = round(max(I2_col, std::max(p2_.y, p4_.y)));

	int dx = abs(up);
	int dy = abs(left);
	int d_x = abs(inv_u);
	int d_y = abs(inv_l);

	// merged image
	Mat I_f(down + -up + 1, right - left + 1, CV_32FC3, Scalar(0));
	// inverse warping with bilinear interplolation
	for (int i = -d_x; i < I_f.rows - d_x; i++) {
		for (int j = -d_y; j < I_f.cols - d_y; j++) {

			float x = A12.at<float>(0) * i + A12.at<float>(1) * j + A12.at<float>(2) + d_x;
			float y = A12.at<float>(3) * i + A12.at<float>(4) * j + A12.at<float>(5) + d_y;

			float y1 = floor(y);
			float y2 = floor(y) + 1;
			float x1 = floor(x);
			float x2 = floor(x) + 1;

			float mu = y - y1;
			float lambda = x - x1;

			if (x1 >= 0 && x2 < I2_row && y1 >= 0 && y2 < I2_col){			
				I_f.at<Vec3f>(i + d_x, j + d_y) = (1 - mu) * ((1 - lambda) * input2.at<Vec3f>(x1, y1) + (lambda)*input2.at<Vec3f>(x2, y1)) + (mu) * ((1 - lambda) * input2.at<Vec3f>(x1, y2) + (lambda)*input2.at<Vec3f>(x2, y2));
			}

		}
	}


	blend_stitching(input1, input2, I_f, dx, dy, 0.5);

	namedWindow("Left Image");
	imshow("Left Image", input1);
	namedWindow("Right Image");
	imshow("Right Image", input2);

	namedWindow("result");
	imshow("result", I_f);

	waitKey(0);

	return 0;
}

/**
* Calculate euclid distance
*/
double euclidDistance(Mat& vec1, Mat& vec2) {
	double sum = 0.0;
	int dim = vec1.cols;
	for (int i = 0; i < dim; i++) {
		sum += (vec1.at<float>(0, i) - vec2.at<float>(0, i)) * (vec1.at<float>(0, i) - vec2.at<float>(0, i));
	}
	return sqrt(sum);
}

/**
* Find the index of nearest neighbor point from keypoints.
*/
int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		Mat v = descriptors.row(i);		// each row of descriptor

		//	Fill the code
		double distance = euclidDistance(vec, v);
		if (minDist > distance) {
			minDist = distance;
			neighbor = i;
		}
	}

	return neighbor;

}

int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int NN) {
	int neighbor = -1;
	double minDist = 1e6;

	for (int i = 0; i < descriptors.rows; i++) {
		if (i == NN)continue; //기존 nn 넘어가고 두번째 nn 찾아야함
		Mat v = descriptors.row(i);		// each row of descriptor

		//	Fill the code
		double distance = euclidDistance(vec, v);

		if (minDist > distance) {
			minDist = distance;
			neighbor = i;
		}
	}

	return neighbor;

}
void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold) {
	for (int i = 0; i < descriptors1.rows; i++) {
		KeyPoint pt1 = keypoints1[i];
		Mat desc1 = descriptors1.row(i);

		int nn = nearestNeighbor(desc1, keypoints2, descriptors2);

		// Refine matching points using ratio_based thresholding
		if (ratio_threshold) {
			//	Fill the code
			Mat v1 = descriptors2.row(nn);
			double dist1 = euclidDistance(desc1, v1);

			int nn_2 = nearestNeighbor2(desc1, keypoints2, descriptors2, nn);
			if (nn_2 == -1) continue;

			Mat v2 = descriptors2.row(nn_2);
			double dist2 = euclidDistance(desc1, v2);

			if (dist1 / dist2 >= RATIO_THR) continue;
		}

		// Refine matching points using cross-checking
		if (crossCheck) {
			//	Fill the code
			Mat desc2 = descriptors2.row(nn);
			int nn_cross = nearestNeighbor(desc2, keypoints1, descriptors1);
			if (nn_cross != i) continue;
		}

		KeyPoint pt2 = keypoints2[nn];
		srcPoints.push_back(pt1.pt);
		dstPoints.push_back(pt2.pt);
	}
}



Mat cal_affine(vector<Point2f> srcPoints, vector<Point2f> dstPoints) {

	int number_of_points = srcPoints.size();

	Mat M(2 * srcPoints.size(), 6, CV_32F, Scalar(0));
	Mat b(2 * srcPoints.size(), 1, CV_32F, Scalar(0));

	Mat M_trans, temp, affineM;
	
	for (int i = 0; i < number_of_points; i++) {
		M.at<float>(2 * i, 0) = srcPoints[i].y;
		M.at<float>(2 * i, 1) = srcPoints[i].x;
		M.at<float>(2 * i, 2) = 1;
		M.at<float>(2 * i + 1, 3) = srcPoints[i].y;
		M.at<float>(2 * i + 1, 4) = srcPoints[i].x;
		M.at<float>(2 * i + 1, 5) = 1;

		b.at<float>(2 * i) = dstPoints[i].y;
		b.at<float>(2 * i + 1) = dstPoints[i].x;

	}

	transpose(M, M_trans);
	invert(M_trans * M, temp);
	affineM = temp * M_trans * b;

	return affineM;
}

Mat cal_affine_RANSAC(vector<Point2f> srcPoints, vector<Point2f> dstPoints, vector<int> rand_points) {

	int number_of_points = rand_points.size();

	Mat M(2 * number_of_points, 6, CV_32F, Scalar(0));
	Mat b(2 * number_of_points, 1, CV_32F, Scalar(0));
	Mat M_trans, temp, affineM;

	// initialize matrix
	for (int i = 0; i < number_of_points; i++) {
		M.at<float>(2 * i, 0) = srcPoints[rand_points.at(i)].y;
		M.at<float>(2 * i, 1) = srcPoints[rand_points.at(i)].x;
		M.at<float>(2 * i, 2) = 1;
		M.at<float>(2 * i + 1, 3) = srcPoints[rand_points.at(i)].y;
		M.at<float>(2 * i + 1, 4) = srcPoints[rand_points.at(i)].x;
		M.at<float>(2 * i + 1, 5) = 1;
		b.at<float>(2 * i) = dstPoints[rand_points.at(i)].y;
		b.at<float>(2 * i + 1) = dstPoints[rand_points.at(i)].x;
	}

	transpose(M, M_trans);
	invert(M_trans * M, temp);
	affineM = temp * M_trans * b;

	return affineM;
}

void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int dx, int dy, float alpha) {
	//경계면
	int bound_x = I1.rows + dx;
	int bound_y = I1.cols + dy;

	int col = I_f.cols;
	int row = I_f.rows;

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			bool cond1, cond2;
			if ((i < bound_x && i > dx) && (j < bound_y && j > dy))	cond1 = true;
			else cond1 = false;

			if (I_f.at<Vec3f>(i, j) != Vec3f(0, 0, 0))	cond2 = true;
			else cond2 = false;

			if (cond1 && cond2)
				I_f.at<Vec3f>(i, j) = alpha * I1.at<Vec3f>(i - dx, j - dy) + (1 - alpha) * I_f.at<Vec3f>(i, j);
			else if (cond1)
				I_f.at<Vec3f>(i, j) = I1.at<Vec3f>(i - dx, j - dy);
		}
	}
}