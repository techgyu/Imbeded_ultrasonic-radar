
# 파일명: Servor_change_by_10.py
# ------
# 서보 모터를 0~180도 범위로 회전시키는 라즈베리파이 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 주요 함수 및 코드에 상세 설명 주석 추가

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수


# GPIO 핀 번호 설정
SERVO_PIN = 14           # 서보 모터가 연결된 GPIO 번호


# PWM 주파수 설정 (서보 표준: 50Hz)
PWM_FREQ = 50  # Hz


# 서보 모터의 PWM 신호 duty cycle 범위 설정
PWM_DUTY_MIN = 2.5   # 최소 duty cycle (0도)
PWM_DUTY_MAX = 12.5  # 최대 duty cycle (180도)


# GPIO 모드 설정 (BCM: GPIO 번호 기준)
GPIO.setmode(GPIO.BCM)


# GPIO 핀 출력 설정
GPIO.setup(SERVO_PIN, GPIO.OUT)


# PWM 객체 생성
pwm = GPIO.PWM(SERVO_PIN, PWM_FREQ)


# 서보 모터를 각도에 맞게 회전시키는 함수
def set_angle(angle):
    duty = PWM_DUTY_MIN + (PWM_DUTY_MAX - PWM_DUTY_MIN) * angle / 180.0  # 각도→duty 변환
    pwm.start(duty)      # PWM 시작 및 duty cycle 설정
    time.sleep(0.5)      # 회전 완료까지 대기
    pwm.stop()           # PWM 종료


# 메인 루프: 각도 변경 및 예외 처리
try:
    while True:
        # 0도부터 180도까지 10도씩 회전
        for angle in range(0, 181, 10):
            set_angle(angle)
            time.sleep(1)  # 각도 변경 후 대기

        # 180도부터 0도까지 10도씩 회전
        for angle in range(180, -1, -10):
            set_angle(angle)
            time.sleep(1)  # 각도 변경 후 대기

except KeyboardInterrupt:
    # Ctrl+C 입력 시 GPIO 리소스 해제
    pwm.stop()
    GPIO.cleanup()
