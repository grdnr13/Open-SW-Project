사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.1
	 OpenCV 2.4.13.6

1-1. MeanFilterGray.cpp
  - Purpose of this code:
    주어진 이미지에 대해, 흑백으로 변형한 뒤 uniform mean filter를 수행하여 원본 이미지와 변형된 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 바꾼 뒤, meanfilter 함수를 호출하여 uniform mean filter를 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat meanfilter(const Mat input, int n, const char* opt)
    - 이미지에 uniform mean filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	n: window의 크기를 결정
	opt: boundary process 옵션

1-2. MeanFilterRGB.cpp
  - Purpose of this code:
    주어진 이미지에 대해, uniform mean filter를 수행한 뒤, 원본 이미지와 변형된 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 meanfilter 함수를 호출하여 uniform mean filter를 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat meanfilter(const Mat input, int n, const char* opt)
    - 이미지에 uniform mean filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	n: window의 크기를 결정
	opt: boundary process 옵션


2-1. GaussianGraySkeleton.cpp
  - Purpose of this code:
    주어진 이미지를 흑백으로 변형한 뒤, Gaussian 필터링을 적용한 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 변형한 뒤, Gaussian 필터링을 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	n: window의 크기를 결정
	opt: boundary process 옵션

2-2. GaussianRGB.cpp
  - Purpose of this code:
    주어진 이미지에 대해, Gaussian 필터링을 적용한 컬러 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 Gaussian 필터링을 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	n: window의 크기를 결정
	opt: boundary process 옵션


3-1.SobelGraySkeleton.cpp
  - Purpose of this code:
    주어진 이미지에 대해, sobel 필터링을 적용한 흑백 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 변형한 뒤, sobel 필터링을 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat sobelfilter(const Mat input) 
    - 이미지에 sobel filter를 적용한 결과를 반환한다.
    - Parameters:
	input: 입력으로 받은 이미지

3-2. SobelFiltering_RGB.cpp
  - Purpose of this code:
    주어진 이미지에 대해, sobel 필터링을 적용한 컬러 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 sobel 필터링을 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat sobelfilter(const Mat input) 
    - 이미지에 sobel filter를 적용한 결과를 반환한다.
    - Parameters:
	input: 입력으로 받은 이미지
* 강의자료 이론 부분에는 R,G,B채널을 더한 뒤 3으로 나누라고 되어 있는데, 과제 안내 부분에는 color image를 출력하라고 되어있어서 각 채널을 별도로 연산했습니다


4-1. LaplacianGray.cpp
  - Purpose of this code:
    주어진 이미지에 대해, laplacian 필터링을 적용한 흑백 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 laplacian 필터링을 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat laplacianfilter(const Mat input)
    - 이미지에 laplacian 필터링을 적용한 결과를 반환한다.
    - Parameters:
	input: 입력으로 받은 이미지

4-2. LaplacianColor.cpp
  - Purpose of this code:
    주어진 이미지에 대해, laplacian 필터링을 적용한 컬러 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 laplacian 필터링을 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat laplacianfilter(const Mat input)
    - 이미지에 laplacian 필터링을 적용한 결과를 반환한다.
    - Parameters:
	input: 입력으로 받은 이미지


5-1. SeparableGaussianGray.cpp
  - Purpose of this code:
    주어진 이미지를 흑백으로 변형한 뒤, Gaussian 필터링 separate하게 적용한 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 변형한 뒤, Gaussian 필터링을 separate하게 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다. 이 때, 두 개의 커널을 만들어 각각의 연산을 따로 수행한다.
    - Parameters:
	input: 입력으로 받은 이미지
	n: window의 크기를 결정
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	opt: boundary process 옵션

5-2. SeparableGaussianColor.cpp
  - Purpose of this code:
    주어진 이미지에 대해, Gaussian 필터링을 separate하게 적용한 컬러 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 Gaussian 필터링을 separate하게 적용한다. 이 때, "mirroring", "zero-padding", "adjustkernel" 중 하나의 옵션을 선택할 수 있다. 원본 이미지와 변형된 이미지를 출력한다. 
  2) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다. 이 때, 두 개의 커널을 만들어 각각의 연산을 따로 수행한다.
    - Parameters:
	input: 입력으로 받은 이미지
	n: window의 크기를 결정
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	opt: boundary process 옵션

6-1. UnsharpGray.cpp
  - Purpose of this code:
    주어진 이미지에 대해, Unsharp Filter를 적용한 흑백 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 바꾸고, 'gaussianfilter'호출을 통해 Gaussian 필터링을 적용한 결과를 'L'에 저장한다. 'Unsharp' 함수를 이용해서 기존 이미지와 Gaussian 필터를 적용한 이미지를 연산하여 Unsharp Filter를 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) Mat Unsharp(const Mat original, const Mat low_pass, const float k)
    - 이미지의 모든 픽셀에 대하여 (I-kL)/(1-k)연산을 수행하여 Unsharp filter를 적용한 결과를 반환한다.
    - Parameters:
	original: 입력으로 받은 이미지
	low_pass: Low pass 필터링(Gaussian filtering)을 적용한 이미지
	k: 스케일링할 때 쓸 실수형 상수
  3) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	n: window의 크기를 결정
	opt: boundary process 옵션

6-2. MeanFilterGray.cpp
  - Purpose of this code:
    주어진 이미지에 대해, Unsharp Filter를 적용한 컬러 이미지와 원본 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 'gaussianfilter'호출을 통해 Gaussian 필터링을 적용한 결과를 'L'에 저장한다. 'Unsharp' 함수를 이용해서 기존 이미지와 Gaussian 필터를 적용한 이미지를 연산하여 Unsharp Filter를 적용한다. 원본 이미지와 변형된 이미지를 출력한다.
  2) 
    - 이미지의 모든 픽셀의 RGB 채널에 대하여 (I-kL)/(1-k)연산을 수행하여 Unsharp filter를 적용한 결과를 반환한다.
    - Parameters:
	original: 입력으로 받은 이미지
	low_pass: Low pass 필터링(Gaussian filtering)을 적용한 이미지
	k: 스케일링할 때 쓸 실수형 상수
  3) Mat gaussianfilter(const Mat input, int n, float sigmaT, float sigmaS, const char* opt)
    - 이미지에 Gaussian filter를 적용한 결과를 반환한다
    - Parameters:
	input: 입력으로 받은 이미지
	sigmaT: x의 표준편차
	sigmaS: y의 표준편차
	n: window의 크기를 결정
	opt: boundary process 옵션