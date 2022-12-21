사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.4
	 OpenCV 2.4.13.6


1. ImageStitching_AffineTransform.cpp
  1) int main()
    - 주어진 두 이미지를 이용하여 SIFT를 수행한 뒤, affine transform estimation을 수행하고 image stitching을 수행한 결과물을 출력한다. 이 때, RANSAC을 사용할지 여부를 선택할 수 있다.
  2) double euclidDistance(Mat& vec1, Mat& vec2)
    - vec1와 vec2 사이의 Euclidean 거리를 연산하여 반환한다.
    - Parameters:
	Mat& vec1: euclidean distance를 구하기 위한 첫 번째 Mat
	Mat& vec2: euclidean distance를 구하기 위한 두 번째 Mat
  3) int nearestNeighbor(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors)
    - 주어진 vec와 descriptor의 각 열 사이의 Euclidean distance를 구하고, 가장 최소 거리를 갖는 nearest neighbor의 인덱스 값을 반환한다.
    - Parameters:
	Mat& vec: 찾을 descriptor 값
	vector<KeyPoint>& keypoints: 대상 이미지의 keypoints
	Mat& descriptors: 대상 이미지의 descriptors
  4) int nearestNeighbor2(Mat& vec, vector<KeyPoint>& keypoints, Mat& descriptors, int NN)
    - 기존 nearest neighbor값인 NN을 제외하고, 두 번째 nearest neighbor 값을 찾아 반환한다.
    - Parameters:
	Mat& vec: 찾을 descriptor 값
	vector<KeyPoint>& keypoints: 대상 이미지의 keypoints
	Mat& descriptors: 대상 이미지의 descriptors
	int NN: 기존 nearest neighbor 값
  5) void findPairs(vector<KeyPoint>& keypoints1, Mat& descriptors1,
	vector<KeyPoint>& keypoints2, Mat& descriptors2,
	vector<Point2f>& srcPoints, vector<Point2f>& dstPoints, bool crossCheck, bool ratio_threshold)
    - 가장 가까운 points들의 쌍들을 찾는다.
    - Parameters:
	vector<KeyPoint>& keypoints1: 첫 번째 이미지의 keypoints
	Mat& descriptors1: 첫 번째 이미지의 descriptors
	vector<KeyPoint>& keypoints2: 두 번째 이미지의 keypoints
	Mat& descriptors2: 두 번째 이미지의 descriptors
	vector<Point2f>& srcPoints: 첫 번째 이미지에서 matching된 keypoints 저장
	vector<Point2f>& dstPoints: 두 번째 이미지에서 matching된 keypoints 저장
	bool crossCheck: cross check 여부
	bool ratio_threshold: ratio threshold 여부
  6) Mat cal_affine(vector<Point2f> srcPoints, vector<Point2f> dstPoints)
    - srcPoints와 dstPoints를 이용해 blend stitching을 수행한다
    - Parameters:
	vector<Point2f> srcPoints: 첫 번째 이미지의 keypoints
	vector<Point2f> dstPoints: 두 번째 이미지의 keypoints	
  7) Mat cal_affine_RANSAC(vector<Point2f> srcPoints, vector<Point2f> dstPoints, vector<int> rand_points)
    - rand_points로 주어진 랜덤한 몇 개의 점을 이용해 affine을 한다
    - Parameters:
	vector<Point2f> srcPoints: 첫 번째 이미지의 keypoints
	vector<Point2f> dstPoints: 두 번째 이미지의 keypoints
	vector<int> rand_points: affine할 때 쓸 랜덤한 점들
  8) void blend_stitching(const Mat I1, const Mat I2, Mat& I_f, int dx, int dy, float alpha)
    - I1과 I2를 blend stitching한 결과를 I_f에 저장한다
    - Parameters:
	const Mat I1: 첫 번째 이미지
	const Mat I2: 두 번째 이미지
	Mat& I_f: blend stitching 결과를 저장
	int dx: x의 변화량
	int dy: y의 변화량
	float alpha: blend 비율

2. Hough.cpp
  1) int main()
    - 이미지를 읽어온 뒤, Hough Transform을 통해 line fitting을 한 결과를 출력한다.
