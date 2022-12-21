path 등 일부 설정을 구글 코랩에 맞춰 변경한 상태입니다. 
import torchvision.transforms as transforms가 제대로 동작하지 않아 from torchvision import transforms로 대체했습니다. 


1. Unet.py
  1) conv(in_channels, out_channels)
    - 이후 연산에서 사용할 convolution 층을 정의한다.
  2) class Unet(nn.Module)
    - __init__(self, in_channels, out_channels)
      - 각 채널 크기(64, 128, 256, 512, 1024)에 맞춰 convDown을 정의한다.  
      - maxpool과 upsample을 정의한다.
      - 각 채널 크기에 맞춰 convUp을 정의한다.
    - forward(self, x)
      - 앞서 정의한 대로 convDown을 순서대로 수행한다. 이후 단계별로 conv4, conv3, conv2, conv1과 concatenation을 수행하며 upsample을 한다.

2. modules.py
  1) train_model(trainloader, model, criterion, optimizer,scheduler, device)
    - 모델 훈련 부분을 정의한다. 모델의 output을 받고, loss 연산, optimizer 설정, back propagation을 한다.
  2) accuracy_check(label, pred)
    - 정확도를 체크한다.
  3) accuracy_check_for_batch(labels, preds, batch_size)
    - 배치 단위의 정확도를 체크한다.
  4) get_loss_train(model, trainloader, criterion, device)
    - train에서 loss를 연산한다. 
  5) val_model(model, valloader, criterion, device, dir)
    - model을 validate한다. 

3. main_skeleton.py
  - batch 크기, 데이터 directory 등을 설정해주고 데이터를 읽어와 저장한다.
  - Unet 또는 UNetWithResnet50Encoder을 이용해 모델을 초기화한다. 
  - loss function, optimizer, parameter 등을 설정해준다.
  - 기존 훈련 데이터를 Unet 또는 UNetWithResnet50Encoder에 맞게 가져온다. 
  - 주어진 epoch만큼 반복하며 훈련을 진행한다.
  - matplotlib을 이용하여 결과값을 수치화한다.

4. resnet_encoder_unet.py
  1) conv1x1(in_channels, out_channels, stride, padding)
    - 1x1 convolution layer를 정의한다
  2) conv3x3(in_channels, out_channels, stride, padding)
    - 3x3 convolution layer를 정의한다
  3) ResidualBlock(nn.Module)
    - bottleneck 부분의 정의
    - downsample 값에 따라서 다른 레이어 생성
    - forward(self, x): downsample 값에 따라서 forward path 정의
  4) conv(in_channels, out_channels)
    - conv연산 정의
  5) UNetWithResnet50Encoder(nn.Module)
    - __init__(self, n_classes=22)
      - ResNet을 이용한 Unet의 각 layer 1,2,3과 연산을 정의한다
    - def forward(self, x, with_output_feature_map=False)
      - forward path를 정의한다


