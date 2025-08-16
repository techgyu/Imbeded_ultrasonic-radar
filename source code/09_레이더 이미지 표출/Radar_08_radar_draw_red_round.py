import matplotlib.pyplot as plt
import numpy as np

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

    # 이전에 그렸던 작은 원들을 그립니다
    for circle in small_circles:
        ax.plot(circle[0], circle[1], marker='o', markersize=5, color='red')

    # 작은 원을 그립니다
    draw_small_circle(angle, 0.4, 1)  # 각도와 작은 원이 표시되는 위치를 인수로 전달

    # 핵심 계산 부분
    x_line = [0, np.cos(angle)]  # x 좌표를 계산합니다.
    y_line = [0, np.sin(angle)]  # y 좌표를 계산합니다.
    ax.plot(x_line, y_line, color='red')  # 직선을 그립니다.

# 작은 원을 그리는 함수
def draw_small_circle(angle, radius, plag):
    if plag != 0:
        x_circle = radius * np.cos(angle) + x_offset
        y_circle = radius * np.sin(angle) + y_offset
        small_circles.append((x_circle, y_circle))

while True:
    # 각도를 증가시키며 애니메이션을 만듭니다
    for angle in np.linspace(0, np.pi, 100):
        draw_line(angle)
        plt.pause(0.05)  # 잠시 멈춥니다.
    
    # 각도를 감소시키며 애니메이션을 만듭니다
    for angle in np.linspace(np.pi, 0, 100):
        draw_line(angle)
        plt.pause(0.05)  # 잠시 멈춥니다.

plt.show()
