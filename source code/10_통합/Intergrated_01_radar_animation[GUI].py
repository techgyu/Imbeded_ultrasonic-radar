import matplotlib as plt
import numpy as np
import RPi.GPIO as GPIO
import time
angle = 0
#루프를 계속 돌린다.
while(True):

    #레이더 그리기
    # 큰 반원의 각도를 설정합니다 (0도에서 180도)
    theta = np.linspace(0, np.pi, 100)

    # 큰 반원의 x, y 좌표를 계산합니다
    x = np.cos(theta)
    y = np.sin(theta)

    # 작은 반원의 각도를 설정합니다 (0도에서 180도)
    theta_small_1 = np.linspace(0, np.pi, 100)
    theta_small_2 = np.linspace(0, np.pi, 100)
    theta_small_3 = np.linspace(0, np.pi, 100)

    # 작은 반원의 크기와 위치 설정
    scale_1 = 0.5  # 작은 반원의 크기 (큰 반원의 절반 크기)
    x_offset = 0  # x 축의 중심 오프셋
    y_offset = 0  # y 축의 중심 오프셋 (큰 반원의 기준선에 맞추기 위해 0으로 설정)

    scale_2 = 0.25  # 작은 반원의 크기 (큰 반원의 절반 크기)

    scale_3 = 0.75  # 작은 반원의 크기 (큰 반원의 절반 크기)

    # 작은 반원의 x, y 좌표를 계산합니다
    x_small_1 = scale_1 * np.cos(theta_small_1) + x_offset
    y_small_1 = scale_1 * np.sin(theta_small_1) + y_offset

    x_small_2 = scale_2 * np.cos(theta_small_2) + x_offset
    y_small_2 = scale_2 * np.sin(theta_small_2) + y_offset

    x_small_3 = scale_3 * np.cos(theta_small_3) + x_offset
    y_small_3 = scale_3 * np.sin(theta_small_3) + y_offset

    # 플롯을 생성합니다
    fig, ax = plt.subplots()
    fig.patch.set_facecolor('black')  # FIGURE의 배경색 설정

    # 화면에 가득 차게 설정합니다
    ax.set_aspect('equal')
    ax.axis('off')  # 축을 숨깁니다
    # 화면에 가득 차게 조정합니다
    plt.subplots_adjust(left=0, right=1, top=1, bottom=0)

    # 화면을 그리는 함수 정의
    def draw_line(list):
        ax.clear()  # 이전에 그린 직선들을 지웁니다.

        ax.set_facecolor('black')        # 배경색 설정
        # 큰 반원을 그립니다
        ax.plot(x, y, color='lime')

        # 큰 반원의 반대편 (기준선)도 그립니다
        ax.plot([1, -1], [0, 0], color='lime')

        # 작은 반원을 그립니다
        ax.plot(x_small_1, y_small_1, color='lime')
        ax.plot(x_small_2, y_small_2, color='lime')
        ax.plot(x_small_3, y_small_3, color='lime')

        # 원의 중심 (0, 0)을 가리키는 점을 그립니다
        ax.plot(0, 0, marker='o', markersize=5, color='lime') 

        # 원의 중심을 가로지르는 세로 선을 그립니다
        ax.plot([0, 0], [0, 1], color='lime', linestyle='--')

        # 대각선 45도 방향의 직선을 그립니다
        ax.plot([0, 0.71], [0, 0.71], color='lime', linestyle='--')

        # 대각선 135도 방향의 직선을 그립니다
        ax.plot([-0.71, 0], [0.71, 0], color='lime', linestyle='--')

        # 감지된 곳에 x, y 좌표를 받아서 빨간색 *를 표시합니다.
        for x, y in list:
            ax.plot(x, y, marker='*', markersize=10, color='red')

    ##############################서보모터 회전시키기####################
    if(angle == 180): #각도 증가
        angle += 10
    # GPIO 핀 번호 설정
    SERVO_PIN = 14

    # PWM 주파수 설정
    PWM_FREQ = 50  # Hz

    # 서보 모터의 PWM 신호의 duty cycle 범위 설정
    PWM_DUTY_MIN = 2.5  # 최소 duty cycle (0도)
    PWM_DUTY_MAX = 12.5  # 최대 duty cycle (180도)

    # GPIO 핀 번호 설정 모드 설정
    GPIO.setmode(GPIO.BCM)

    # GPIO 핀 설정
    GPIO.setup(SERVO_PIN, GPIO.OUT)

    # PWM 객체 생성
    pwm = GPIO.PWM(SERVO_PIN, PWM_FREQ)

    # 서보 모터를 각도에 맞게 회전하는 함수
    def set_angle(angle):
        # 각도를 duty cycle 값으로 변환
        duty = PWM_DUTY_MIN + (PWM_DUTY_MAX - PWM_DUTY_MIN) * angle / 180.0
        # duty cycle을 설정하여 서보 모터를 회전
        pwm.start(duty)
        time.sleep(0.5)  # 회전이 완료될 때까지 잠시 대기
        pwm.stop()
 
        i = 0 
        while i < 1:
            #회전 시킨 값 + 거리 받아서 수정하기
            set_angle(angle)
            time.sleep(0.1)
            i += 0.1

    ##################################USS 인식####################################
    # GPIO 핀 번호 설정
    TRIG_PIN = 23
    ECHO_PIN = 24

    # GPIO 핀 번호 설정 모드 설정
    GPIO.setmode(GPIO.BCM)

    # GPIO 핀 설정
    GPIO.setup(TRIG_PIN, GPIO.OUT)
    GPIO.setup(ECHO_PIN, GPIO.IN)

    def get_distance():
        # TRIG 핀에 10us 동안 펄스를 출력하여 초음파 발사
        GPIO.output(TRIG_PIN, GPIO.HIGH)
        time.sleep(0.00001)
        GPIO.output(TRIG_PIN, GPIO.LOW)

        # 초음파가 되돌아오는 시간을 측정
        while GPIO.input(ECHO_PIN) == 0:
            pulse_start = time.time()

        while GPIO.input(ECHO_PIN) == 1:
            pulse_end = time.time()

        # 초음파가 되돌아오는 시간을 이용하여 거리 계산
        pulse_duration = pulse_end - pulse_start
        distance = pulse_duration * 17150  # 음속(343m/s) * 시간 / 2 (왕복 거리이므로)

        # 소수점 둘째 자리까지 반올림하여 거리 반환
        distance = round(distance, 2)
        return distance

    try:
        while True:
            distance = get_distance()
            print("Distance:", distance, "cm")
            time.sleep(1)

    except KeyboardInterrupt:
         # Ctrl+C 입력 시 GPIO 리소스 해제
        GPIO.cleanup()

##########################angle과 distance를 이용해서 좌표 값 계산##################




plt.pause(3) #3초 대기