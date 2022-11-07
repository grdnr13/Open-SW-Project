# Image Rotation
## Explanation of the codes
- “nearest”를 입력으로 받은 경우, nearest neighborhood interpolation을 이용해 이미지를 회전한다. y좌표와 x좌표를 반올림해 가장 가까운 정수를 찾고, input에서 해당 위치의 픽셀을 output의 `(i,j)`위치에 복사한다.
- “bilinear”를 입력으로 받은 경우, bilinear neighborhood interpolation을 이용해 이미지를 회전한다. x, y좌표의 버림값과 올림값을 이용해 mu와 lamda를 구해 각 좌표축에 대해 linear interpolation을 수행한다.
## Experimental Results
original image:

![image](https://user-images.githubusercontent.com/77968137/200229560-781b6cf9-b984-42e4-bfd7-380858d79719.png)

rotated image:

![image](https://user-images.githubusercontent.com/77968137/200229612-51561952-e02b-4e18-86f3-f5c8b7d80815.png)

# Image Stitching
## Explanation of the codes
두 이미지에 대해 28개의 corresponding pixels이 주어졌을 때, 이들을 이용해 stitching 작업을 수행한다:
1.	stitching 할 두 개의 이미지 "stitchingL.jpg”와 “stitchingR.jpg”를 각각 I1, I2로 읽어온다
2.	연산을 위해 I1, I2를 32비트 float, 3채널로 변환하고, 이미지를 제대로 읽지 못했을 때에는 오류 메시지를 출력하고 종료한다. 
3.	cal_affine 함수를 이용해 affine Matrix A12, A21을 구한다
    - 32비트 실수를 값으로 가진 `(2 * number_of_points, 6)` 크기의 Mat M을 만들며 0으로 초기화하고, 마찬가지로 32비트 실수를 값으로 가진 `(2 * number_of_points, 1)` 크기의 Mat b를 하나 더 만든다.
    - Mat M, b를 각자 초기화해준다.
    - `(M^T * M)^(−1) * M^T * b`을 연산해서 반환한다.
4.	4개의 모퉁이 p1, p2, p3, p4를 구한다
5.	앞서 구한 모퉁이를 이용해서 합쳐진 이미지의 모서리 4개`(bound_u, bound_b, bound_l, bound_r)`를 구한다
6.	`(bound_b - bound_u + 1, bound_r - bound_l + 1)` 사이즈를 갖는, 32비트 float, 3채널의 I_f를 0으로 초기화한다.
7.	모서리 안의 각 픽셀에 대해 bilinear interpolation을 이용한 inverse warp를 수행한다:
실수형 변수 x, y에 A12 곱셈결과를 저장하고, 이를 이용해 뮤 값과 람다 값을 구해 각 좌표축에 대해 linear interpolation을 수행한다.
8.	`blend_stitching` 함수를 이용해 두 이미지를 합친다:
  I2는 이미 I_f에 있으므로 I1만 추가해주면 된다.
  I_f의 `(i - bound_u, j - bound_l)`위치 픽셀이 비어있는 경우, I1의`(i,j)`위치 픽셀에 alpha값을 곱한 값과 I2의 `(i - bound_u, j - bound_l)` 위치 픽셀에 `(1-alpha)`값을 곱한 값을 더해 I_f에 복사한다.
아닌 경우, I1의 픽셀을 그대로 복사한다.
9.	left image, right image, result를 모두 출력한다.

## Experimental Results
original images:

![image](https://user-images.githubusercontent.com/77968137/200230047-a3162ce1-1a3e-4f01-8610-eb1ed798ce65.png)
![image](https://user-images.githubusercontent.com/77968137/200230061-0f8b3635-0c35-4716-98eb-e6d1620b0f94.png)

Stitched image:
![image](https://user-images.githubusercontent.com/77968137/200230103-2a8b35ad-31e9-460b-bd2f-541b917de396.png)



