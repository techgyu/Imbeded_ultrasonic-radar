
# 파일명: Servo_180_angle.py
# ------
# 서보 모터를 180도 위치로 제어하는 라즈베리파이 예제 코드
# - RPi.GPIO 라이브러리 사용
# - 주요 코드와 연산에 상세 설명 주석 추가

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time              # 시간 관련 함수


servo_pin = 14           # 서보 모터가 연결된 GPIO 번호


GPIO.setmode(GPIO.BCM)           # BCM 모드: GPIO 번호 기준
GPIO.setup(servo_pin, GPIO.OUT)  # 서보 핀 출력 설정


pwm = GPIO.PWM(servo_pin, 50)  # PWM 객체 생성, 주파수 50Hz (서보 표준)
pwm.start(0)                   # 초기 duty cycle 0으로 모터 정지


# 180도 위치로 서보 모터 제어 및 예외 처리
try:
    duty = 12.5  # 180도에 해당하는 duty cycle (서보마다 다를 수 있음)
    pwm.ChangeDutyCycle(duty)  # duty cycle 변경 (180도 위치)
    time.sleep(3)              # 3초 대기
    pwm.stop()                 # PWM 종료
    GPIO.cleanup()             # GPIO 리소스 정리
    print("GPIO 리소스를 정리했습니다.")

except KeyboardInterrupt:
    # Ctrl+C 입력 시 GPIO 리소스 해제
    pwm.stop()
    GPIO.cleanup()
    print("GPIO 리소스를 정리했습니다.")
