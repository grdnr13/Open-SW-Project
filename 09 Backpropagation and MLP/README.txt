이미 scipy와 numpy 등이 최신 버전으로 설치된 상태여서 삭제 후 재설치를 시도했으나 제대로 작동하지 않았습니다
따라서 불가피하게 data_utils.py의 from scipy.misc import imread를 from imageio import imread로 변경해서 실행했습니다

[1] neural_net.py

  1. class TwoLayerNet(object)
    1) __init__(self, input_size, hidden_size, output_size, std=1e-4)
      - 모델의 초기화
      - Parameters:
	input_size: 입력 데이터의 차원수
	hidden_size: hidden layer의 뉴런 수
	output_size: 클래스 개수 C
    2) loss(self, X, y=None, reg=0.0)
      - loss 연산
      - Parameters:
	X: (N, D) 형태의 입력 데이터. 각 X[i]은 훈련 샘플이다.
	y: training labels의 벡터
	reg: Regularization strength.
    3) train(self, X, y, X_val, y_val,
            learning_rate=1e-3, learning_rate_decay=0.95,
            reg=5e-6, num_iters=100,
            batch_size=200, verbose=False)
      - SGD를 이용한 신경망의 훈련
      - Parameters:
	X: A numpy array of shape (N, D) giving training data.
	y: A numpy array f shape (N,) giving training labels; y[i] = c means that X[i] has label c, where 0 <= c < C.
	X_val: A numpy array of shape (N_val, D) giving validation data.
	y_val: A numpy array of shape (N_val,) giving validation labels.
	learning_rate: Scalar giving learning rate for optimization.
	learning_rate_decay: Scalar giving factor used to decay the learning rate after each epoch.
	reg: Scalar giving regularization strength.
	num_iters: Number of steps to take when optimizing.
 	batch_size: Number of training examples to use per step.
	verbose: boolean; if true print progress during optimization.
    4) predict(self, X)
      - 훈련된 신경망을 이용해 예측을 수행한다
      - Parameters:
	X: A numpy array of shape (N, D) giving N D-dimensional data points to classify.


[2] two_layer_net.ipynb

  1. Implementing a Neural Network
    1) rel_error(x, y): relative error 반환
    2) init_toy_model(): TwoLayerNet을 만든다
    3) init_toy_data(): 작은 데이터 생성
  2. Forward pass: compute scores
    - class scores와 올바른 scores의 차이 연산
  3. Forward pass: compute loss
    - loss 함수 호출을 통해 loss 값 계산
  4. Backward pass
    - backward pass를 수행하고, numeric gradient checking을 통해 확인
  5. Train the network
    - train함수 호출을 통해 네트워크 훈련, loss 시각화
  6. Load the data
    1) get_CIFAR10_data
      - CIFAR-10 데이터를 읽어와서 subsample, 정규화, 형태 변환
  7. Train a network
    - 네트워크 훈련, 예측
  8. Debug the training
    - loss function과 train accuracies, validation accuracies, 네트워크의 weight 시각화
  9. Tune your hyperparameters
    - 튜닝을 할 범위를 정한 다음 해당 범위 안에서 가장 정확도가 높은 네트워크 선택, 시각화
  10. Run on the test set
    - 테스트 세트에서 정확도 측정