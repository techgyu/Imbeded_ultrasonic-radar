
# 파일명: LED_turn_on_off_loof.py
# -----------------
# Raspberry Pi의 GPIO를 사용하여 LED를 깜빡이는 예제 코드
# - GPIO 21번 핀을 출력으로 설정하여 0.5초 간격으로 LED ON/OFF 반복
# - Ctrl+C(KeyboardInterrupt) 시 GPIO 리셋 및 종료 메시지 출력

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 지연 함수


# GPIO 핀 번호 모드 설정 (BCM: Broadcom SoC 핀 번호)
GPIO.setmode(GPIO.BCM)
# GPIO 21번 핀을 출력으로, 20번 핀을 입력으로 설정
GPIO.setup(21, GPIO.OUT)
GPIO.setup(20, GPIO.IN)


print("Hello LED")  # 시작 안내 메시지 출력


try:
        # 무한 루프: LED를 0.5초 간격으로 ON/OFF 반복
        while True:
                GPIO.output(21, True)   # LED ON
                time.sleep(0.5)         # 0.5초 대기
                GPIO.output(21, False)  # LED OFF
                time.sleep(0.5)         # 0.5초 대기
except KeyboardInterrupt:
        # Ctrl+C 입력 시 GPIO 리셋 및 종료 안내
        GPIO.cleanup()
        print ("Bye Bye !!!")
