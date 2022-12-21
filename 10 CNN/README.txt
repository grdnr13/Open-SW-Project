NVDIA 그래픽카드가 없어서 CUDA 실행이 원활하게 되지 않아 main.py의 50번째 줄을 checkpoint = torch.load(PATH,map_location=torch.device('cpu'))으로 변경했습니다.

1.main.py
  1) device를 cpu로 설정
  2) 이미지 전처리
  3) CIFAR-10 데이터셋 가져옴
  4) 모델 선택 - ResNet 또는 VGG
  5) 모델 훈련 준비
    - 하이퍼파라미터, loss, optimizer 설정
    - learning rate 업데이트 설정
  6) epoch 수만큼 반복문을 돌며 모델 훈련
    - device에 image와 label 입력
    - forward pass 수행
    - backward pass 수행, optimize
    - 학습률 decay
  7) 모델 체크포인트 저장
  8) 모델 평가

2. vgg16_full.py
  1) class VGG(nn.Module)
    - features: 매개변수로 받아옴
    - classifier 설정
    - weight 초기화
    - forward(self, x): forward path 정의
  2) make_layers(cfg, batch_norm=False)
    - cfg의 값에 따라 해당하는 레이어 생성
  3) vgg16()
    - cfg 생성 및 VGG(make_layers(cfg)) 호출

3. resnet50_skeleton.py
  1) conv1x1(in_channels, out_channels, stride, padding)
    - 1x1 convolution layer를 정의한다
  2) conv3x3(in_channels, out_channels, stride, padding)
    - 3x3 convolution layer를 정의한다
  3) class ResidualBlock(nn.Module)
    - bottleneck 부분의 정의
    - downsample 값에 따라서 다른 레이어 생성
    - forward(self, x): downsample 값에 따라서 forward path 정의
  4) class ResNet50_layer4(nn.Module)
    - 각 층 정의
    - forward(self, x): forward path 정의
