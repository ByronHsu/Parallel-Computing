import subprocess
def parse(out):
    for line in out.split('\n'):
        if line.startswith('Execution Time:'):
            time = float(line.split(' ')[2])
            return time

if __name__ == '__main__':

    for i in range(1, 8 + 1):
        arr = []
        for j in range(20):
            p = subprocess.Popen(args = 'mpirun -np {} ./6_13.o ./1000_1000.txt 100 1'.format(i), shell = True, cwd = '.', stdout = subprocess.PIPE, stderr = subprocess.PIPE)
            out, err = p.communicate()
            out = out.decode()
            time = parse(out)
            arr.append(time)
        print('# Processers = {}, time = {:.8f}'.format(i, float(sum(arr) / len(arr))))
