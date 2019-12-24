import random
import sys

n = int(sys.argv[1])
m = int(sys.argv[2])

with open(sys.argv[3], 'w') as f:
    f.write('{} {}\n'.format(n, m))
    for i in range(n):
        s = ''
        for j in range(m): 
            digit = random.randint(0, 1)
            s += str(digit)
        f.write(s + '\n')