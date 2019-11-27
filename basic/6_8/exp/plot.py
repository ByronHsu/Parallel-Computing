import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.read_csv('./pingpong.o5072629', header=None)
data = np.array(df)

# Data for plotting
n = data[:, 0]
time = data[:, 1]

z = np.polyfit(n, time, 1)
p = np.poly1d(z)
func = p(n)
print(z)
print(p(1))


fig, ax = plt.subplots()
ax.plot(n, time, label='raw data')
ax.plot(n, func, label='fitted line: y = 4.95e-10 * x  + 3.63e-06')

ax.set(xlabel='size(byte)', ylabel='time(s)',
       title='PingPong')
ax.legend()
ax.grid()

fig.savefig("pingpong.png")
# plt.show()

