사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.1
	 OpenCV 2.4.13.6

1. histogram_stretching.cpp
  - Purpose of this code:
    주어진 이미지에 대해 histogram matching을 수행한 뒤, 원본 이미지와 변형된 이미지를 출력한다.
  1) int main() 
    - 이미지를 읽어와서 흑백으로 변경한 뒤, histogram stretching을 수행한다.
  2) void linear_stretching(Mat &input, Mat &stretched, G *trans_func, G x1, G x2, G y1, G y2)
    - 이미지를 x범위에 따라 histogram stretching 연산을 한다.
    - Parameters:
	&input: 입력으로 받은 이미지
	&stretched: 연산 결과를 저장할 이미지
	x1, x2: stretching 연산의 범위 구분
	y1, y2: stretching 연산 범위 양끝의 픽셀 값

2. hist_eq.cpp
  - Purpose of this code:
    주어진 이미지를 흑백으로 변경한 뒤, histogram equalization을 수행한 결과를 출력한다.
  1) int main()
    - 주어진 이미지를 흑백으로 변경한 뒤, hist_eq함수를 이용해 histogram equalization을 수행한 결과를 출력한다.
  2) void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF)
    - trans_func[i] = (G)((L - 1) * CDF[i]) 연산을 수행한 뒤, 해당 연산 결과에 따라 이미지 픽셀 값을 변경해준다.
    - Parameters:
	&input: input 이미지
	&equalized: 연산 수행한 이미지 결과물
	*trans_func: equalization 함수 관계
	*CDF: input 이미지의 CDF

3. hist_eq_RGB.cpp
  - Purpose of this code:
    주어진 이미지의 R,G,B에 대해 각각 equalization을 수행한 결과를 출력한다.
  1) int main()
    - 주어진 이미지에 대해 hist_eq_Color 함수를 이용해 R,G,B 각각 equlization 수행 후 이미지 출력
  2) void hist_eq_Color(Mat &input, Mat &equalized, G(*trans_func)[3], float **CDF) 
    - trans_func를 이용해 주어진 이미지에 대해 equalize 연산을 수행한다.
    - Parameters:
	&input: input 이미지
	&equalized: 연산 수행 이미지 결과물
	(*trans_func)[3]: equalization 함수 관계
	**CDF: input 이미지의 CDF

4. hist_eq_YUV.cpp
  - Purpose of this code:
    주어진 이미지의 Y 채널에 대해 equalization을 수행한 결과를 출력한다.
  1) int main()
    - 입력으로 주어진 이미지를 YUV 형식으로 바꿔 Y채널에 대해 equalization을 수행한 뒤, 채널을 합쳐 다시 RGB 형식으로 바꿔준다. 주어진 이미지와 연산 결과 이미지를 출력한다.
  2) void hist_eq(Mat &input, Mat &equalized, G *trans_func, float *CDF)
    -  Y채널에 대해 equalization을 수행한다.
    - Parameters:
	&input: input 이미지
	&equalized: 연산 수행 결과물을 저장
	*trans_func: equlization 함수 관계
	*CDF: input 이미지의 CDF

5. hist_matching.cpp
  - Purpose of this code:
    흑백의 input이미지에 대해, reference 이미지를 이용한 histogram matching을 수행한다.
  1) int main()
    - 흑백의 input이미지에 대해, reference 이미지를 이용해 matching 함수 관계를 만들어낸 뒤, 이를 이용해 연산한 이미지를 출력한다.
  2) void change_one_to_one(G* trans_func_eq2, G* trans_func_match)
    - 함수관계를 일대일로 바꿔 matching 함수를 만들어낸다.
    - Parameters:
	trans_func_eq2: 일대일로 만들 함수 관계
	trans_func_match: matching 함수 관계
  3) void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF) 
    - equalization 연산을 수행한다.
    - Parameters:
	input: input 이미지
	equalized: equalize된 이미지
	trans_func: equalize 함수 관계
	CDF: input 이미지의 CDF
  4) void hist_match(Mat& input, Mat& output, G* trans_func)
    - histogram matching 연산을 수행한다.
    - Parameters:
	input: input 이미지
	output: output 이미지
	trans_func: matching 함수 관계

6. hist_matching_YUV.cpp
  - Purpose of this code:
    input 이미지의 Y채널에 대해 histogram matching을 수행한다.
  1) int main()
    - input 이미지와 reference 이미지의 Y채널에 대해 equalization을 수행한 뒤, 이를 이용해 histogram matching 함수 관계를 찾아낸다. 이를 이용해 Y채널에 대해 연산한 뒤, 합쳐서 RGB 형식으로 변환하여 출력한다.
  2) void hist_eq(Mat& input, Mat& equalized, G* trans_func, float* CDF)
    - histogram equalization을 수행한다.
    - Parameters:
	input: input 이미지
	equalized: equalize된 결과물
	trans_func: eqaulize 함수 관계
	CDF: input 이미지의 CDF
  3) void change_one_to_one(G* trans_func_eq2, G* trans_func_match)
    - 함수관계를 일대일로 바꿔 matching 함수를 만들어낸다.
    - Parameters:
	trans_func_eq2: 일대일로 만들 함수 관계
	trans_func_match: matching 함수 관계
  4) void hist_match(Mat& input, Mat& output, G* trans_func)
    - histogram matching 연산을 수행한다.
    - Parameters:
	input: input 이미지
	output: output 이미지
	trans_func: matching 함수 관계	

7. hist_func.h
  1) float* cal_PDF(Mat& input)
    - single channel 이미지에 대해 PDF를 만든다.
    - Parameters:
	input: PDF를 만들 이미지
  2) float** cal_PDF_RGB(Mat& input)
    - color channel 이미지에 대해 PDF를 만든다.
    - Parameters:
	input: PDF를 만들 이미지
  3) float* cal_CDF(Mat& input)
    - single channel 이미지에 대해 CDF를 만든다.
    - Parameters:
	input: CDF를 만들 이미지
  4) float** cal_CDF_RGB(Mat& input)
    - color channel 이미지에 대해 CDF를 만든다.
    - Parameters:
	input: CDF를 만들 이미지