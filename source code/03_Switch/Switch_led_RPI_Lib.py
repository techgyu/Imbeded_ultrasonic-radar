
# 파일명: Switch_led_RPI_Lib.py
# ------------------
# 라즈베리파이 GPIO를 이용해 스위치 입력에 따라 LED를 제어하는 예제 코드
# - RPi.GPIO 라이브러리 사용
# - GPIO21(LED), GPIO20(스위치) 핀 사용
# - 각 줄마다 기능 설명 주석 추가

import RPi.GPIO as GPIO      # 라즈베리파이 GPIO 제어 라이브러리 임포트
import time                  # 시간 관련 함수 임포트

GPIO.setmode(GPIO.BCM)       # GPIO 핀 번호를 BCM 모드로 설정
GPIO.setup(21, GPIO.OUT)     # GPIO21을 출력(LED)으로 설정
GPIO.setup(20, GPIO.IN)      # GPIO20을 입력(스위치)으로 설정

print("Hello LED SW")        # 시작 메시지 출력

try:
	while True:              # 무한 반복
		GPIO.output(21, False)   # LED OFF
		while GPIO.input(20) == 0: # 스위치가 눌리면
			GPIO.output(21, True)  # LED ON
except KeyboardInterrupt:    # Ctrl+C 등 인터럽트 발생 시
	GPIO.cleanup()           # GPIO 설정 초기화
	print ("Bye Bye !!!")    # 종료 메시지 출력
