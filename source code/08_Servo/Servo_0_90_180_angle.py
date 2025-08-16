
# 파일명: Servo_0_90_180_angle.py
# ------
# 서보 모터를 0도, 90도, 180도 위치로 반복 이동시키는 라즈베리파이 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 주요 함수 및 코드에 상세 설명 주석 추가

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수

# 서보 모터가 연결된 GPIO 핀 번호
servo_pin = 14

# GPIO 모드 및 핀 설정 (BCM 모드: GPIO 번호 기준)
GPIO.setmode(GPIO.BCM)
GPIO.setup(servo_pin, GPIO.OUT)

# PWM 객체 생성, 주파수 50Hz (서보 표준)
pwm = GPIO.PWM(servo_pin, 50)
pwm.start(0)  # PWM 시작, duty cycle 0으로 초기화

# 서보 모터 각도 제어 함수
def set_angle(angle):
    duty = angle / 18 + 2  # 각도에 따른 duty cycle 계산 (0~180도)
    GPIO.output(servo_pin, True)      # PWM 신호 시작
    pwm.ChangeDutyCycle(duty)         # 듀티 사이클 변경
    time.sleep(1)                     # 모터가 움직일 시간 대기
    GPIO.output(servo_pin, False)     # PWM 신호 종료
    pwm.ChangeDutyCycle(0)            # 듀티 사이클 0으로 복귀

# 메인 루프: 0도, 90도, 180도 위치로 반복 이동
try:
    while True:
        set_angle(0)    # 0도 위치 이동
        time.sleep(2)
        set_angle(90)   # 90도 위치 이동
        time.sleep(2)
        set_angle(180)  # 180도 위치 이동
        time.sleep(2)

except KeyboardInterrupt:
    pwm.stop()        # PWM 종료
    GPIO.cleanup()    # GPIO 리소스 정리