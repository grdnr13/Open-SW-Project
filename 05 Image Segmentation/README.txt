사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.4
	 OpenCV 2.4.13.6

1. adaptiveThreshold_movingAverages.cpp
  - Purpose of this code:
    주어진 이미지에 adaptive thresholding을 적용한다.
    1) int main()
      - 이미지를 입력으로 받아서 'adaptive_thres' 함수 호출을 통해 adaptive thresholding을 적용한 결과를 출력한다.
    2) Mat adaptive_thres(const Mat input, int n, float bnumber)
      - 입력으로 받은 이미지에 moving average를 이용해 adaptive thresholding을 적용한 결과물을 반환한다. 이 때, zero paddle을 적용한다.
      - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: 필터의 크기 조정
	float bnumber: 픽셀 intensity 연산결과 조정

2. kMeans_intensity.cpp
  - Purpose of this code:
    주어진 흑백 이미지의 intensity를 이용해 kmeans clustering을 수행한다.
    1) int main()
      - 이미지를 입력으로 받아서 흑백으로 변환시킨다. 이미지의 intensity를 이용한 samples로 kmeans 함수 호출을 통해 kmeans clustering을 적용한다. 결과물 이미지를 출력한다.

3. kMeans_intensity_position.cpp
  - Purpose of this code:
    주어진 흑백 이미지에서 각 픽셀의 intensity와 position를 이용해 kmeans clustering을 수행한다.
    1) int main()
      - 이미지를 입력으로 받아서 흑백으로 변환시킨다. 이미지의 intensity, x좌표, y좌표를 이용한 samples로 kmeans 함수 호출을 통해 kmeans clustering을 적용한다. 결과물 이미지를 출력한다.

4. kMeans_rgb.cpp
  - Purpose of this code:
    주어진 컬러 이미지의 rgb채널 값을 이용해 kmeans clustering을 수행한다.
    1) int main()
      - 이미지를 입력으로 받는다. 이미지의 R, G, B 채널을 이용한 samples로 kmeans 함수 호출을 통해 kmeans clustering을 적용한다. 결과물 이미지를 출력한다.

5. kMeans_rgb_position.cpp
  - Purpose of this code:
    주어진 컬러 이미지의 rgb채널과 x좌표, y좌표 값을 이용해 kmeans clustering을 수행한다.
    1) int main()
      - 이미지를 입력으로 받는다. 이미지의 R, G, B 채널과 x좌표, y좌표를 이용한 samples로 kmeans 함수 호출을 통해 kmeans clustering을 적용한다. 결과물 이미지를 출력한다.
