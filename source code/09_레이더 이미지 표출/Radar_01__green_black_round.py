import numpy as np
import matplotlib.pyplot as plt

# 데이터 설정
num_bins = 180
angles = np.linspace(0, np.pi, num_bins)
radii = np.random.rand(num_bins)

# 차트 설정
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
fig.patch.set_facecolor('black')  # FIGURE의 배경색 설정

# 반원 레이더 설정
ax.set_theta_zero_location('N')  # 0도 위치를 북쪽으로 설정
ax.set_theta_direction(-1)       # 시계 반대 방향으로 각도 설정
ax.set_ylim(0, 1)                # 반지름 범위 설정
ax.set_thetagrids(np.degrees(angles), labels=[])  # 각도 레이블 제거
ax.set_facecolor('black')        # 배경색 설정

# 데이터 플로팅
bars = ax.bar(angles, radii, width=np.pi/num_bins, bottom=0, color=plt.cm.viridis(radii / max(radii)))

# 스타일 설정
ax.spines['polar'].set_color('lime')
ax.spines['polar'].set_linewidth(2)
ax.grid(color='lime', linestyle='-', linewidth=1.5)

# 그래프 보여주기
plt.show()
