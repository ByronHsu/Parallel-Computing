import numpy as np

n = 2 ** 31
samples = np.random.uniform(-1, 1, (n, 3))
x, y, z = samples[:, 0], samples[:, 1], samples[:, 2]
distances = np.sin(np.arccos((x + y + z) / (np.sqrt(3) * np.sqrt(x ** 2 + y ** 2 + z ** 2)))) * np.sqrt(x ** 2 + y ** 2 + z ** 2)
insides = distances >= 0.15
prob = np.sum(insides) / n
base = 2 ** 3
volume =  prob * base
print(area)