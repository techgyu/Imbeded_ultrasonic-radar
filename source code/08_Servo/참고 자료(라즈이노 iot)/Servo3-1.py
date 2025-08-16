'''
파일명: Servo3-1.py
------
서보 모터를 0~180도까지 1% 단위로 부드럽게 이동시키는 라즈베리파이 예제 코드
- RPi.GPIO 라이브러리 사용
- 주요 코드와 연산에 상세 설명 주석 추가

※ 라즈베리파이 환경에서만 동작 (RPi.GPIO 필요)
※ 서보 특성에 따라 duty cycle 범위(3~12%) 조정 필요
'''

### 【 라즈이노 IoT 】
### (실습 3-1) 서보모터 각도 조절해 보기 (0~180도까지, 1%단위로 조절하기)
### 학습 자료 제공 페이지 : Rasino.tistory.com/341
### 아래 주석은 PWM/서보 동작 원리 설명

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수

servo_pin = 18           # 서보 모터가 연결된 GPIO 번호
GPIO.setmode(GPIO.BCM)           # BCM 모드: GPIO 번호 기준
GPIO.setup(servo_pin,GPIO.OUT)   # 서보 핀 출력 설정
pwm = GPIO.PWM(servo_pin, 50)    # PWM 객체 생성, 주파수 50Hz (서보 표준)
pwm.start(3.0)                   # 0도 위치 duty cycle로 시작 (3%)

# 0~180도까지 1% 단위로 부드럽게 이동
for high_time in range(30, 125):
    pwm.ChangeDutyCycle(high_time/10.0) # duty cycle을 3.0~12.4까지 0.1씩 증가
    time.sleep(0.02)                    # 각도 변경 후 짧게 대기

pwm.ChangeDutyCycle(3.0)                # 0도 위치로 복귀
time.sleep(1.0)                         # 1초 대기
pwm.ChangeDutyCycle(0.0)                # 모터 정지

pwm.stop()                              # PWM 종료
GPIO.cleanup()                          # GPIO 리소스 정리
