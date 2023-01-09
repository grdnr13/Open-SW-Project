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
	///SURF를 수행할 이미지 2개 읽어오기
	Mat img_object = imread("input1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img_scene = imread("input2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	//읽어오기 오류 체크
	if (!img_object.data || !img_scene.data)
	{
		std::cout << " --(!) Error reading images " << std::endl; return -1;
	}

	/// 1. SURF Detector 이용해서 keypoints 추출
	int minHessian = 400;//Threshold
	
	SurfFeatureDetector detector(minHessian);

	std::vector<KeyPoint> keypoints_object, keypoints_scene;
	//SurfFeatureDetector 이용해서 각 이미지의 keypoint를 추출
	detector.detect(img_object, keypoints_object); 
	detector.detect(img_scene, keypoints_scene);

	/// 2. descriptor 연산
	SurfDescriptorExtractor extractor;

	Mat descriptors_object, descriptors_scene;
	//SurfDescriptorExtractor 이용해서 각 이미지의 descriptors를 연산
	extractor.compute(img_object, keypoints_object, descriptors_object);
	extractor.compute(img_scene, keypoints_scene, descriptors_scene);

	/// 3. FLANN matcher를 이용해서 descriptor 벡터 매칭하기
	FlannBasedMatcher matcher;
	std::vector< DMatch > matches;
	//descriptor 사이 매칭 결과를 matches에 저장
	matcher.match(descriptors_object, descriptors_scene, matches);

	double max_dist = 0; double min_dist = 100; //max distance, min distance  연산을 위한 변수 선언

	//keypoints 사이 max distance, min distance 연산
	for (int i = 0; i < descriptors_object.rows; i++) //각 descriptors_object의 rows에 대하여
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist; //min_dist 갱신
		if (dist > max_dist) max_dist = dist; //max_dist 갱신
	}

	printf("-- Max dist : %f \n", max_dist);
	printf("-- Min dist : %f \n", min_dist);

	/// 4. 3*min_dist 보다 가까운 match만 good_matchs에 추가
	std::vector< DMatch > good_matches;

	for (int i = 0; i < descriptors_object.rows; i++) //각 descriptors_object의 rows에 대하여
	{
		if (matches[i].distance < 3 * min_dist)//3*min_dist 보다 가까우면
		{
			good_matches.push_back(matches[i]); //good_matches에 추가
		}
	}

	//매칭 결과 출력할 이미지 생성
	Mat img_matches;
	//https://docs.opencv.org/4.x/d4/d5d/group__features2d__draw.html
	drawMatches(img_object, keypoints_object, img_scene, keypoints_scene,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	/// 5. 첫번째 이미지의 object를 두번째 이미지에서 localize
	std::vector<Point2f> obj; //첫번째 이미지의 object의 keypoints 저장
	std::vector<Point2f> scene; //두번째 이미지 localize할 때 쓰일 keypoints

	for (int i = 0; i < good_matches.size(); i++) // good matches에서 keypoint 가져오기
	{
		//각각 obj와 scene에 저장
		obj.push_back(keypoints_object[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints_scene[good_matches[i].trainIdx].pt);
	}
	//원근 변환 행렬 구하기 - src: obj, dst: scene, method: RANSAC
	Mat H = findHomography(obj, scene, CV_RANSAC);

	/// 6. 첫번째 이미지의 corner 4개 추출 -> scene에서 찾을 object 
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0, 0); obj_corners[1] = cvPoint(img_object.cols, 0);
	obj_corners[2] = cvPoint(img_object.cols, img_object.rows); obj_corners[3] = cvPoint(0, img_object.rows);
	std::vector<Point2f> scene_corners(4);

	perspectiveTransform(obj_corners, scene_corners, H);


	/// 7. 두 이미지에서 object의 corner 간 line 그리기
	line(img_matches, scene_corners[0] + Point2f(img_object.cols, 0), scene_corners[1] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[1] + Point2f(img_object.cols, 0), scene_corners[2] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[2] + Point2f(img_object.cols, 0), scene_corners[3] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);
	line(img_matches, scene_corners[3] + Point2f(img_object.cols, 0), scene_corners[0] + Point2f(img_object.cols, 0), Scalar(0, 255, 0), 4);

	/// 8. 결과물 보이기
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