'''
파일명: Servo2-1.py
------
서보 모터를 0도, 90도, 180도 위치로 반복 이동시키는 라즈베리파이 예제 코드
- RPi.GPIO 라이브러리 사용
- 주요 코드와 연산에 상세 설명 주석 추가

※ 라즈베리파이 환경에서만 동작 (RPi.GPIO 필요)
※ 서보 특성에 따라 duty cycle 범위(3~12%) 조정 필요
'''

### 【 라즈이노 IoT 】
### (실습 2-1) 서보모터 각도 조절해 보기 (0도, 90도, 180도 조절하기)
### 학습 자료 제공 페이지 : Rasino.tistory.com/341
### 아래 주석은 PWM/서보 동작 원리 설명

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수
servo_pin = 18           # 서보 모터가 연결된 GPIO 번호
GPIO.setmode(GPIO.BCM)           # BCM 모드: GPIO 번호 기준
GPIO.setup(servo_pin, GPIO.OUT)  # 서보 핀 출력 설정
pwm = GPIO.PWM(servo_pin, 50)    # PWM 객체 생성, 주파수 50Hz (서보 표준)
pwm.start(3.0)                   # 0도 위치 duty cycle로 시작 (3%)

for cnt in range(0, 3):          # 3회 반복
    pwm.ChangeDutyCycle(3.0)     # 서보모터를 0도로 이동
    time.sleep(1.0)              # 이동 시간 대기
    pwm.ChangeDutyCycle(7.5)     # 서보모터를 90도로 이동
    time.sleep(1.0)              # 이동 시간 대기
    pwm.ChangeDutyCycle(12.5)    # 서보모터를 180도로 이동
    time.sleep(1.0)              # 이동 시간 대기

pwm.ChangeDutyCycle(0.0)         # 모터 정지

pwm.stop()                       # PWM 종료
GPIO.cleanup()                   # GPIO 리소스 정리
