사용 환경: Microsoft Visual Studio Community 2022 (64-bit) - Current, 버전 17.1.1
	 OpenCV 2.4.13.6

이미지를 읽어온 뒤, myrotate 함수를 이용해 주어진 각도만큼 회전한 결과물을 화면에 출력한다.

Mat myrotate(const Mat input, float angle, const char* opt)
- 입력으로 받은 이미지를 angle만큼 회전한 결과물을 반환한다. 
- Parameters:
    input: 입력으로 읽어온 이미지
    angle: 회전할 각도
    opt: 사용할 interpolation 방식 선택. "nearest"와 "bilinear" 두 가지 가능

