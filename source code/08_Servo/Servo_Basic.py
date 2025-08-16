
# 파일명: Servo_Basic.py
# ------
# 서보 모터를 PWM으로 제어하는 라즈베리파이 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 주요 함수 및 코드에 상세 설명 주석 추가

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수


# GPIO 핀 번호 설정
servo_pin = 14  # 서보 모터가 연결된 GPIO 번호


# GPIO 모드 및 핀 설정 (BCM 모드: GPIO 번호 기준)
GPIO.setmode(GPIO.BCM)
GPIO.setup(servo_pin, GPIO.OUT)  # 서보 핀 출력 설정


# PWM 객체 생성, 주파수 50Hz (서보 모터 표준)
pwm = GPIO.PWM(servo_pin, 50)
pwm.start(0)  # PWM 시작, 듀티 사이클 0으로 초기화


# 서보 모터 각도 제어 함수
def set_angle(angle):
    duty = angle / 18 + 2  # 각도에 따른 듀티 사이클 계산 (0~180도)
    GPIO.output(servo_pin, True)      # PWM 신호 시작
    pwm.ChangeDutyCycle(duty)         # 듀티 사이클 변경
    time.sleep(0.5)                   # 모터가 움직일 시간 대기
    GPIO.output(servo_pin, False)     # PWM 신호 종료
    pwm.ChangeDutyCycle(0)            # 듀티 사이클 0으로 복귀


# 메인 루프: 각도 변경 및 예외 처리
try:
    # 0도부터 180도까지 10도씩 증가하며 서보 제어
    for angle in range(0, 181, 10):
        set_angle(angle)
        time.sleep(1)  # 각도 변경 후 대기

    # 180도부터 0도까지 10도씩 감소하며 서보 제어
    for angle in range(180, -1, -10):
        set_angle(angle)
        time.sleep(1)

except KeyboardInterrupt:
    print("프로그램 종료")  # 사용자 중지 시 메시지 출력

finally:
    pwm.stop()        # PWM 종료
    GPIO.cleanup()    # GPIO 리소스 정리
