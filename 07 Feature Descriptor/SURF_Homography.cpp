#include <stdio.h>
#include <iostream>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;

void readme();

/**
 * @function main
 * @brief Main function
 */
int main()
{
	//if (argc != 3)
	//{
	//	readme(); return -1;
	//}
	///SURF�� ������ �̹��� 2�� �о����
	Mat img_object = imread("input1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_scene = imread("input2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//�о���� ���� üũ
	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	/// 1. SURF Detector �̿��ؼ� keypoints ����
	int minHessian = 400;//Threshold
	
	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	//SurfFeatureDetector �̿��ؼ� �� �̹����� keypoint�� ����
	detector.detect(img_object, keypoints_object); 
	detector.detect(img_scene, keypoints_scene);

	/// 2. descriptor ����
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;
	//SurfDescriptorExtractor �̿��ؼ� �� �̹����� descriptors�� ����
	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	/// 3. FLANN matcher�� �̿��ؼ� descriptor ���� ��Ī�ϱ�
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	//descriptor ���� ��Ī ����� matches�� ����
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100; //max distance, min distance  ������ ���� ���� ����

	//keypoints ���� max distance, min distance ����
	for (int i = 0; i < descriptors_object.rows; i++) //�� descriptors_object�� rows�� ���Ͽ�
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist; //min_dist ����
		if (dist > max_dist) max_dist = dist; //max_dist ����
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	/// 4. 3*min_dist ���� ����� match�� good_matchs�� �߰�
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) //�� descriptors_object�� rows�� ���Ͽ�
	{
		if (matches[i].distance < 3 * min_dist)//3*min_dist ���� ������
		{
			good_matches.push_back(matches[i]); //good_matches�� �߰�
		}
	}

	//��Ī ��� ����� �̹��� ����
	Mat img_matches;
	//https://docs.opencv.org/4.x/d4/d5d/group__features2d__draw.html
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/// 5. ù��° �̹����� object�� �ι�° �̹������� localize
	std::vector<Point2f> obj; //ù��° �̹����� object�� keypoints ����
	std::vector<Point2f> scene; //�ι�° �̹��� localize�� �� ���� keypoints

	for (int i = 0; i < good_matches.size(); i++) // good matches���� keypoint ��������
	{
		//���� obj�� scene�� ����
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	//���� ��ȯ ��� ���ϱ� - src: obj, dst: scene, method: RANSAC
	Mat H = findHomography(obj, scene, CV_RANSAC);

	/// 6. ù��° �̹����� corner 4�� ���� -> scene���� ã�� object 
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);


	/// 7. �� �̹������� object�� corner �� line �׸���
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	/// 8. ����� ���̱�
	imshow("Good Matches & Object detection", img_matches);

	waitKey(0);

	return 0;
}

/**
 * @function readme
 */
void readme()
{
	std::cout << " Usage: ./SURF_Homography <img1> <img2>" << std::endl;
}