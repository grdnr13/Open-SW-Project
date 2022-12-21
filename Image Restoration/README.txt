사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.1
	 OpenCV 2.4.13.6

1. salt_and_pepper.cpp
  - Purpose of this code:
    주어진 이미지의 흑백&컬러버전에 각각 salt and pepper noise를 적용한 뒤, median filtering을 적용한다.
  1) int main()
    - 이미지를 읽어와서 흑백 버전과 컬러 버전 각각에 대해 'Add_salt_pepper_Noise' 함수 호출을 통하여 salt and pepper noise를 적용한다. 이후, 'Salt_pepper_noise_removal_Gray' 함수와 'Salt_pepper_noise_removal_RGB' 함수를 각각 호출하여 median filter를 적용한 결과물을 만들어낸다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 이미지 연산이 종료되면, 원본 이미지와 노이즈가 생긴 이미지, median filter를 적용한 이미지를 모두 출력한다.
  2) Mat Add_salt_pepper_Noise(const Mat input, float ps, float pp)
    - input으로 주어진 이미지에 salt and pepper noise를 적용한 결과를 반환한다.
    - Parameters:
	const Mat input: noise를 적용할 이미지
	float ps: white noise 밀도
	float pp: black noise 밀도
  3) Mat Salt_pepper_noise_removal_Gray(const Mat input, int n, const char *opt)
  - 흑백 이미지에 대하여 median filtering을 이용해 salt and pepper noise를 제거한 결과물을 반환한다.
  - Parameters:
	const Mat input: 노이즈를 제거할 이미지
	int n: 커널 크기를 결정
	const char *opt: boundary processing 옵션
  4) Mat Salt_pepper_noise_removal_RGB(const Mat input, int n, const char* opt)	
  - 컬러 이미지에 대하여 median filtering을 이용해 salt and pepper noise를 제거한 결과물을 반환한다.
  - Parameters:
	const Mat input: 노이즈를 제거할 이미지
	int n: 커널 크기를 결정
	const char *opt: boundary processing 옵션

2. GaussianNoise_GaussianFiltering.cpp
  - Purpose of this code:
    주어진 이미지의 흑백&컬러버전에 각각 Gaussian Noise를 적용한 뒤, Gaussian Filtering을 적용한다.
  1) int main()
    - 이미지를 읽어와서 흑백 버전과 컬러 버전 각각에 대해 'Add_Gaussian_noise'함수 호출을 통하여 Gaussian Noise를 적용한다. 이후, 'Gaussianfilter_Gray' 함수와 'Gaussianfilter_RGB' 함수를 각각 호출하여 Gaussian filter를 적용한 결과물을 만들어낸다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 이미지 연산이 종료되면, 원본 이미지와 노이즈가 생긴 이미지, gaussian filter를 적용한 이미지를 모두 출력한다.
  2) Mat Add_Gaussian_noise(const Mat input, double mean, double sigma)
    - 이미지에 Gaussian noise를 추가한 결과를 반환한다
    - Parameters:
	const Mat input: 노이즈를 추가할 이미지
	double mean: 사용할 가우시안 분포의 평균값
	double sigma: 사용할 가우시안 분포의 표준편차
  3) Mat Gaussianfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, const char* opt) {
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: window의 크기를 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
	const char* opt: boundary process 옵션
  4) Mat Gaussianfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	const Mat input: 입력으로 받은 이미지
	int n: window의 크기를 결정
	double sigma_t: x의 표준편차
	double sigma_s: y의 표준편차
	const char* opt: boundary process 옵션

3. GaussianNoise_BilateralFiltering.cpp
  - Purpose of this code:
    주어진 이미지의 흑백&컬러버전에 각각 Gaussian Noise를 적용한 뒤, Bilateral Filtering을 적용한다.
  1) int main()
    - 이미지를 읽어와서 흑백 버전과 컬러 버전 각각에 대해 'Add_Gaussian_noise'함수 호출을 통하여 Gaussian Noise를 적용한다. 이후, 'Bilateralfilter_Gray' 함수와 'Bilateralfilter_RGB' 함수를 각각 호출하여 Bilateral filter를 적용한 결과물을 만들어낸다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 이미지 연산이 종료되면, 원본 이미지와 노이즈가 생긴 이미지, bilateral filter를 적용한 이미지를 모두 출력한다.
  2) Mat Add_Gaussian_noise(const Mat input, double mean, double sigma)
    - input 이미지에 가우시안 노이즈를 추가한다.
    - Parameters:
	const Mat input: 노이즈를 추가할 이미지
	double mean: 사용할 가우시안 분포의 평균값
	double sigma: 사용할 가우시안 분포의 표준편차
  3) Mat Bilateralfilter_Gray(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt)
  - input으로 받은 흑백 이미지에 bilateral filtering을 적용한 결과를 반환한다.
  - Parameters:
	const Mat input: bilateral 필터링을 적용할 이미지
	double sigma_t: x좌표에 대한 표준편차
	double sigma_s: y좌표에 대한 표준편차
	double sigma_r: intensity에 대한 표준편차
	const char* opt: boundary processing 방식
  4) Mat Bilateralfilter_RGB(const Mat input, int n, double sigma_t, double sigma_s, double sigma_r, const char* opt)
  - input으로 받은 컬러 이미지에 bilateral filtering을 적용한 결과를 반환한다.
  - Parameters:
	const Mat input: bilateral 필터링을 적용할 이미지
	double sigma_t: x좌표에 대한 표준편차
	double sigma_s: y좌표에 대한 표준편차
	double sigma_r: intensity에 대한 표준편차
	const char* opt: boundary processing 방식
