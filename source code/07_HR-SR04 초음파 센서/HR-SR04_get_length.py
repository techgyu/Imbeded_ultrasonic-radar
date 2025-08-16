
# 파일명: HR-SR04_get_length.py
# ------
# 초음파 센서(HR-SR04)로 거리 측정하는 라즈베리파이 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 주요 함수 및 코드에 상세 설명 주석 추가

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수


# GPIO 핀 번호 설정
TRIG_PIN = 21  # 초음파 트리거 핀 (출력)
ECHO_PIN = 20  # 초음파 에코 핀 (입력)


# GPIO 설정 (BCM 모드: GPIO 번호 기준)
GPIO.setmode(GPIO.BCM)
GPIO.setup(TRIG_PIN, GPIO.OUT)  # 트리거 핀 출력 설정
GPIO.setup(ECHO_PIN, GPIO.IN)   # 에코 핀 입력 설정


# 초음파 센서로 거리 측정하는 함수
def measure_distance():
    # 트리거 핀을 LOW로 설정 (센서 안정화)
    GPIO.output(TRIG_PIN, False)
    time.sleep(2)

    # 트리거 핀을 HIGH로 10μs 동안 설정 (초음파 발사)
    GPIO.output(TRIG_PIN, True)
    time.sleep(0.00001)
    GPIO.output(TRIG_PIN, False)

    # 에코 핀에서 신호가 돌아올 때까지 대기 (초음파 수신 시작)
    while GPIO.input(ECHO_PIN) == 0:
        pulse_start = time.time()

    # 에코 핀에서 신호가 끝날 때까지 대기 (초음파 수신 종료)
    while GPIO.input(ECHO_PIN) == 1:
        pulse_end = time.time()

    # 신호의 시간 차이 계산 (왕복 시간)
    pulse_duration = pulse_end - pulse_start

    # 거리 계산 (소리 속도: 34300cm/s, 왕복이므로 17150)
    distance = pulse_duration * 17150
    distance = round(distance, 2)  # 소수점 둘째 자리까지 반올림

    return distance


# 메인 루프: 거리 측정 및 출력
try:
    while True:
        dist = measure_distance()  # 거리 측정
        print(f"Measured Distance = {dist} cm")  # 결과 출력
        time.sleep(1)  # 1초 대기

except KeyboardInterrupt:
    print("Measurement stopped by User")  # 사용자 중지 시 메시지 출력

finally:
    GPIO.cleanup()  # GPIO 리소스 정리
