"""
실습 3-3: 서보모터 각도 조절 (0↔180도 왕복, 1% 단위 x 3회)
Raspberry Pi + SG90 Servo Motor control example
환경: Raspberry Pi, RPi.GPIO 필요
- PWM 신호: 50Hz (20ms 주기)
- 듀티 사이클: 3% (0도) ~ 12% (180도)
- 이 스크립트는 서보를 0~180도로 왕복(1% 단위) 3회 이동시킵니다.
- 끝에서 떨림이 있으면 듀티 범위를 미세 조정하세요.
"""

### 【 라즈이노 IoT 】
### (실습 3-3) 서보모터 각도 조절해 보기 (0↔180도 1% 단위로 왕복 움직이기 x 3회)
### 학습 자료 제공 페이지 : Rasino.tistory.com/341
# 서보는 PWM을 20ms(50hz) 주기로 동작시키고 펄스 폭(duty)만 조절해 위치(각도)를 조절함.
# 여기서 duty는 1주기 동안 High를 유지하는 기간이며, 
# 20ms 주기에 100% 면 20ms가 되는 것이고 10% 면 2ms 5% 면 1ms가 됨.
# 펄스 폭은 1ms 일때 0도 2ms 일때 180도, 중간값 1.5ms 일때 90도로 이동함.
# 단, 저렴한 서보는 3%~12%(0도~180도)의 동작 특성을 가짐.
# 만약 모터가 떨린다면, 이 범위를 벗어났기 때문이며, 0도에서 떨린다면
# 3%값에서 조금 올려주고, 180도에서 떨린다면 12%에서 값을 조금 내려주면 됨.
import RPi.GPIO as GPIO  # 라즈베리파이 GPIO 제어 라이브러리
import time  # 시간 지연을 위한 라이브러리

servo_pin = 18  # 서보모터 신호선 연결 핀 번호
GPIO.setmode(GPIO.BCM)  # BCM 핀 넘버링 방식 사용
GPIO.setup(servo_pin,GPIO.OUT)  # 해당 핀을 출력으로 설정
pwm = GPIO.PWM(servo_pin, 50)  # 50Hz(20ms) PWM 객체 생성
pwm.start(3.0)  # 3% 듀티(약 0도)로 PWM 시작

# 3회 반복하여 서보를 0~180도로 왕복 이동
for i in range (0,3) :    
    # 3%~12.4%까지 0.1%씩 증가하며 0→180도 이동
    for high_time in range (30, 125):
        pwm.ChangeDutyCycle(high_time/10.0) # 정수→실수 변환하여 듀티 설정
        time.sleep(0.02)  # 부드러운 이동을 위한 짧은 대기
    
    # 12.4%~3%까지 0.1%씩 감소하며 180→0도 이동
    for high_time in range (124, 30,-1):
        pwm.ChangeDutyCycle(high_time/10.0) # 정수→실수 변환하여 듀티 설정
        time.sleep(0.02)  # 부드러운 이동을 위한 짧은 대기
        
# 서보 신호를 끄고 GPIO 리소스 해제
pwm.ChangeDutyCycle(0.0)  # PWM 신호 끄기(서보 off)
pwm.stop()  # PWM 종료
GPIO.cleanup()  # GPIO 리소스 해제
