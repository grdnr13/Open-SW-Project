사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.4
	 OpenCV 2.4.13.6


1. LoG_grayscale.cpp
  - Purpose of this code:
    주어진 이미지를 흑백으로 변형한 뒤, Gaussian 필터링을 적용한 뒤 Laplacian 필터를 적용하여 edge detection을 수행한다. 수행 결과 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 변형한 뒤, 64-bit floating point로 바꿔준다. 'Gaussianfilter'함수 호출을 통해 해당 이미지에 가우시안 필터를 적용한 결과를 h_f로 저장한다. 이후, Laplacianfilter 함수를 호출하여 h_f에 laplacian filter를 적용하고 normalize를 적용한다. 이 때, boundary에 대해서는 zero-padding을 적용한다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat Gaussianfilter(const Mat input, int n, double sigma_t, double sigma_s)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: 커널 크기 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
  3) Mat Laplacianfilter(const Mat input)
    - 흑백 이미지에 Laplacian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
  4) Mat Mirroring(const Mat input, int n)
    - 흑백 이미지의 boundary에 대해서 mirroring을 적용한다.
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: mirroring 적용할 테두리 너비
  5) Mat get_Gaussian_Kernel(int n, double sigma_t, double sigma_s, bool normalize)
    - Gaussian filter의 값을 연산한다.
    - Parameters:
	int n: 커널 크기 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
	bool normalize: normalize 여부 결정
  6) Mat get_Laplacian_Kernel()
    - Laplacian Filter의 값 결정, 커널 반환


2. LoG_color.cpp
  - Purpose of this code:
    주어진 컬러 이미지에 Gaussian 필터링을 적용한 뒤 Laplacian 필터를 적용하여 edge detection을 수행한다. 수행 결과 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 64-bit floating point로 바꿔준다. 'Gaussianfilter'함수 호출을 통해 해당 이미지에 가우시안 필터를 적용한 결과를 h_f로 저장한다. 이후, Laplacianfilter 함수를 호출하여 h_f에 laplacian filter를 적용하고 normalize를 적용한다. 이 때, boundary에 대해서는 zero-padding을 적용한다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat Gaussianfilter(const Mat input, int n, double sigma_t, double sigma_s)
    - 컬러 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: 커널 크기 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
  3) Mat Laplacianfilter(const Mat input)
    - 컬러 이미지에 Laplacian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
  4) Mat Mirroring(const Mat input, int n)
    - 컬러 이미지의 boundary에 대해서 mirroring을 적용한다.
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: mirroring 적용할 테두리 너비
  5) Mat get_Gaussian_Kernel(int n, double sigma_t, double sigma_s, bool normalize)
    - Gaussian filter의 값을 연산한다.
    - Parameters:
	int n: 커널 크기 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
	bool normalize: normalize 여부 결정
  6) Mat get_Laplacian_Kernel()
    - Laplacian Filter의 값 결정, 커널 반환


3. CannyEdge.cpp
  - Purpose of this code:
    이미지에 Canny edge detection을 수행한 결과 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 바꿔준다. 'Canny' 내장 함수 호출을 통해 주어진 이미지에 canny edge detection을 적용한다. 변형된 이미지와 원본 이미지를 출력한다.


4. HarrisCorner.cpp
  - Purpose of this code:
    이미지에 Harris Corner detection을 수행한 결과 이미지와 원본 이미지를 출력한다.
  1) main()
    - 입력으로 받은 이미지를 흑백으로 바꿔준다. cornerHarris 함수 호출을 통하여 M matrix에서 cornerness score를 얻는다. edge로 판단되는 지점마다 빨간색 원을 그려준다.
    - NonMaxSupp 옵션을 선택했다면, NonMaximum_Suppression 함수 호출을 통하여 non-maximum suppression 작업을 수행하고, 그 결과를 출력한다.
    - Subpixel 옵션을 선택했다면, Sub-pixel refinement을 적용해준다.
  2) vector<Point2f> MatToVec(const Mat input)
    - Mat을 vector 형식으로 바꿔서 반환한다.
    - Parameters:
	const Mat input: vector 형식으로 바꿔줄 Mat
  3) Mat NonMaximum_Suppression(const Mat input, Mat corner_mat, int radius)
    - non-maximum suppression 작업을 수행한다.
    - Parameters:
	const Mat input: corner detection 대상 이미지
	Mat corner_mat: 기존 corner들
	int radius: 반지름으로 이용
  4) Mat Mirroring(const Mat input, int n)
    - 이미지의 boundary에 대해서 mirroring을 적용한다.
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: mirroring 적용할 테두리 너비
  5) void type2str(int type)
     - 매개변수로 받은 타입을 문자열로 출력해준다.
     - Parameters:
	int type: 문자열로 표현할 타입