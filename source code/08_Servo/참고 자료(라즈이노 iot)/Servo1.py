
# 파일명: Servo1.py
# ------
# 서보 모터를 0도 위치로 제어하는 라즈베리파이 예제 코드
# - PWM 펄스 폭(duty cycle)으로 각도 제어
# - 주요 코드와 연산에 상세 설명 주석 추가
#
# ※ 라즈베리파이 환경에서만 동작 (RPi.GPIO 필요)
# ※ 서보 특성에 따라 duty cycle 범위(3~12%) 조정 필요

### 【 라즈이노 IoT 】
### (실습 1) 서보모터 각도 조절해 보기 (0도 위치로 돌리기)
### 학습 자료 제공 페이지 : Rasino.tistory.com/341
### 아래 주석은 PWM/서보 동작 원리 설명


import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수

servo_pin = 21           # 서보 모터가 연결된 GPIO 번호

GPIO.setmode(GPIO.BCM)           # BCM 모드: GPIO 번호 기준
GPIO.setup(servo_pin, GPIO.OUT)  # 서보 핀 출력 설정

pwm = GPIO.PWM(servo_pin, 50)    # PWM 객체 생성, 주파수 50Hz (서보 표준)
pwm.start(3.0)                   # 0도에 해당하는 duty cycle로 시작 (3%)

time.sleep(2.0)                  # 2초 동안 0도 위치 유지
pwm.ChangeDutyCycle(0.0)         # 듀티 사이클 0으로 복귀 (모터 정지)

pwm.stop()                       # PWM 종료
GPIO.cleanup()                   # GPIO 리소스 정리

