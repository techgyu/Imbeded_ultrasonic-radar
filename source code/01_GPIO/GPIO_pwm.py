
# 파일명: GPIO_pwm.py
# -------------------
# 라즈베리파이 GPIO 핀에 PWM 신호를 출력하는 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 5개 핀에 각각 다른 듀티사이클로 PWM 출력
# - 각 줄마다 기능 설명 주석 추가

import RPi.GPIO as GPIO           # 라즈베리파이 GPIO 제어 라이브러리 임포트
GPIO.setmode(GPIO.BCM)            # GPIO 핀 번호를 BCM 모드로 설정

GPIO.setup(2,GPIO.OUT)            # GPIO2를 출력 모드로 설정
GPIO.setup(3,GPIO.OUT)            # GPIO3를 출력 모드로 설정
GPIO.setup(4,GPIO.OUT)            # GPIO4를 출력 모드로 설정
GPIO.setup(17,GPIO.OUT)           # GPIO17을 출력 모드로 설정
GPIO.setup(27,GPIO.OUT)           # GPIO27을 출력 모드로 설정

p1=GPIO.PWM(2, 1000)              # GPIO2에 1000Hz PWM 인스턴스 생성
p2=GPIO.PWM(3, 1000)              # GPIO3에 1000Hz PWM 인스턴스 생성
p3=GPIO.PWM(4, 1000)              # GPIO4에 1000Hz PWM 인스턴스 생성
p4=GPIO.PWM(17, 1000)             # GPIO17에 1000Hz PWM 인스턴스 생성
p5=GPIO.PWM(27, 1000)             # GPIO27에 1000Hz PWM 인스턴스 생성

p1.start(100)                     # GPIO2에 듀티사이클 100%로 PWM 시작
p2.start(60)                      # GPIO3에 듀티사이클 60%로 PWM 시작
p3.start(30)                      # GPIO4에 듀티사이클 30%로 PWM 시작
p4.start(15)                      # GPIO17에 듀티사이클 15%로 PWM 시작
p5.start(7)                       # GPIO27에 듀티사이클 7%로 PWM 시작

# Duty Cycle 100%, 60%, 30%, 15%, 7%로 각각 출력됨
# p1=GPIO.PWM(2, 1000) # 예시: GPIO2를 사용, 주파수 1000Hz, TH=TL=1ms인 PWM 인스턴스