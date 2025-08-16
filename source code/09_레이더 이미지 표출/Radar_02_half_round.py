import matplotlib.pyplot as plt
import numpy as np

# 반원의 각도를 설정합니다 (0도에서 180도)
theta = np.linspace(0, np.pi, 100)

# 반원의 x, y 좌표를 계산합니다
x = np.cos(theta)
y = np.sin(theta)

# 플롯을 생성합니다
fig, ax = plt.subplots()

# 반원을 그립니다
ax.plot(x, y, color='b')

# 반원의 반대편 (기준선)도 그립니다
ax.plot([1, -1], [0, 0], color='b')

# 화면에 가득 차게 설정합니다
ax.set_aspect('equal')
ax.axis('off')  # 축을 숨깁니다

# 화면에 가득 차게 조정합니다
plt.subplots_adjust(left=0, right=1, top=1, bottom=0)

plt.show()
