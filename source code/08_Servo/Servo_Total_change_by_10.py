"""
Servo_Total_change_by_10.py: 서보 모터 각도 제어 (0~180도, 10도 단위)
Raspberry Pi + SG90 Servo Motor control example
환경: Raspberry Pi, RPi.GPIO 필요
- PWM 신호: 50Hz (20ms 주기)
- 듀티 사이클: 각도에 따라 (angle/18)+2.5 계산
- 0도부터 180도까지 10도씩 이동
"""

import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time  # 시간 지연을 위한 라이브러리

servo_pin = 14  # 서보모터 신호선 연결 핀 번호

GPIO.setmode(GPIO.BCM)  # BCM 핀 넘버링 방식 사용
GPIO.setup(servo_pin, GPIO.OUT)  # 해당 핀을 출력으로 설정

pwm = GPIO.PWM(servo_pin, 50)  # 50Hz(20ms) PWM 객체 생성
pwm.start(0)  # 초기 duty cycle 0%로 시작 (모터 정지)

try:
    # 0도부터 180도까지 10도씩 이동
    for angle in range(0, 181, 10):  # 각도 범위: 0~180도, 10도 단위
        duty = (angle / 18) + 2.5  # 각도→duty cycle 변환 공식
        pwm.ChangeDutyCycle(duty)  # 계산된 duty cycle로 서보 위치 변경
        time.sleep(1)  # 1초 대기 (각 위치에서 멈춤)

    print("서보 모터를 0도부터 180도까지 이동시켰습니다.")

except KeyboardInterrupt:
    print("사용자에 의해 프로그램이 종료되었습니다.")

finally:
    pwm.stop()  # PWM 종료
    GPIO.cleanup()  # GPIO 리소스 해제
    print("GPIO 리소스를 정리했습니다.")
