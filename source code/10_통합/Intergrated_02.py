import matplotlib.pyplot as plt
import numpy as np

import RPi.GPIO as GPIO
import time

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

#원 객체의 정보를 저장하는 연결 리스트
class Node:
    def __init__(self, x, y):
        self.x = x  # x 좌표
        self.y = y  # y 좌표
        self.Transparency = 100  # 투명도
        self.next = None

class LinkedList:
    def __init__(self):
        self.head = None

    # 연결 리스트의 끝에 노드를 추가하는 메서드
    def append(self, x, y):
        new_node = Node(x, y)
        if not self.head:
            self.head = new_node
            return
        last_node = self.head
        while last_node.next:
            last_node = last_node.next
        last_node.next = new_node

    # 연결 리스트에서 특정 값을 제거하고 다시 이어주는 메서드
    def remove(self, value):
        current = self.head
        if current is None:
            return

        # 헤드 노드를 제거해야 할 경우
        if current.x == value.x and current.y == value.y:
            self.head = current.next
            current = None
            return

        prev = None
        while current:
            if current.x == value.x and current.y == value.y:
                break
            prev = current
            current = current.next

        if current is None:
            print("Node not found")
            return

        prev.next = current.next
        current = None

    # 연결 리스트를 출력하는 메서드
    def display(self):
        current = self.head
        while current:
            print(f"({current.x}, {current.y}) ", end="-> ")
            current = current.next
        print("None")

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

# 작은 원들을 저장할 리스트
small_circles = []

# 화면을 그리는 함수 정의
def draw_line(angle):
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

    # 작은 원을 그립니다
    linked_list.

    # 핵심 계산 부분
    x_line = [0, np.cos(angle)]  # x 좌표를 계산합니다.
    y_line = [0, np.sin(angle)]  # y 좌표를 계산합니다.
    ax.plot(x_line, y_line, color='yellow')  # 직선을 그립니다.

# 작은 원을 그리는 함수
def calculate_circle(angle, distance):
    x = distance * np.cos(angle) + x_offset
    y = distance * np.sin(angle) + y_offset
    linked_list.append(x, y)

# 연결 리스트 인스턴스 생성
linked_list = LinkedList()

##############1. 계속 지우고 다시 그림###############
while True:
    distance = get_distance() * 0.00125 # 1 / 800로 줄임
    angle = get_angle # servo motor 돌면서, get_distance할 때 같이 값 수집
    calculate_circle(angle, distance) # 원에 대한 정보 입력

    # 각도를 증가시키며 애니메이션을 만듭니다
    for angle in np.linspace(0, np.pi, 100):
        draw_line(angle)
        plt.pause(0.05)  # 잠시 멈춥니다.
    
    # 각도를 감소시키며 애니메이션을 만듭니다
    for angle in np.linspace(np.pi, 0, 100):
        draw_line(angle)
        plt.pause(0.05)  # 잠시 멈춥니다.

plt.show()
