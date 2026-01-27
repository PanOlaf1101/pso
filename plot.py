import sys
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

if len(sys.argv) < 2:
	sys.exit(1)

map_file = sys.argv[1]

with open(map_file, 'r') as f:
	W, H = map(int, f.readline().split())
	map_data = []
	for _ in range(H):
		row = list(map(float, f.readline().split()))
		map_data.append(row)
	map_data = np.array(map_data)

particle_data = []
with open('particle_log.csv', 'r') as f:
    f.readline()
    for line in f:
        values = list(map(float, line.strip().split(',')))
        particle_data.append(values)

particle_data = np.array(particle_data)
n_frames = len(particle_data)
n_particles = (particle_data.shape[1] - 1) // 2

fig, ax = plt.subplots(figsize=(10, 8))
plt.subplots_adjust(bottom=0.15)

im = ax.imshow(map_data, cmap='plasma', origin='lower', extent=[0, W, 0, H], alpha=0.8)
plt.colorbar(im, ax=ax)

triangles = []
for i in range(n_particles):
	triangle = ax.plot([], [], marker='^', markersize=10, linestyle='none', color='white', markeredgecolor='black')[0]
	triangles.append(triangle)

ax.set_xlim(0, W)
ax.set_ylim(0, H)

ax_slider = plt.axes([0.15, 0.05, 0.7, 0.03])
slider = Slider(ax_slider, '', 0, n_frames - 1, valinit=0, valstep=1)

def update(val):
	idx = int(slider.val)
	frame = particle_data[idx]
	
	for i in range(n_particles):
		x = frame[1 + 2*i]
		y = frame[1 + 2*i + 1]
		triangles[i].set_data([x], [y])
	
	fig.canvas.draw_idle()

slider.on_changed(update)

update(0)
plt.show()
