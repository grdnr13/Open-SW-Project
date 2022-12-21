사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.4
	 OpenCV 2.4.13.6

1. SIFT.cpp
  - Purpose of this code:
    SIFT를 이용하여 주어진 두 이미지 사이에 일치하는 지점을 찾는다.
  1) int main()
    - 주어진 두 이미지에 SIFT를 적용하여 image matching을 수행한다. 이 때, crossCheck과 ratio_threshold 옵션을 선택할 수 있다. 주어진 두 이미지를 연결하여 'matchingImage'를 만들고, 일치하는 지점을 연결하여 표시한다. 
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


2. SURF_Homography.cpp
  1) int main()
    - 두 이미지를 읽어온 뒤, SURF를 수행한 결과물을 출력한다.
  2) void readme()
    - 입력을 제대로 받지 못한 경우, 주어진 문구를 출력

