import copy

state = [
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0]
]

next_state = [
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0]
]

m = len(state)
n = len(state[0])

n_iter = 10

for _iter in range(n_iter):

    for r in range(m):
        for c in range(n):
            alive_count = 0
            # count alive neighbors
            for i in range(r - 1, r + 1 + 1):
                for j in range(c - 1, c + 1 + 1):
                    if i < 0 or j < 0 or i >= m or j >= n: 
                        continue
                    elif i == r and j == c:
                        continue
                    elif state[i][j] == 1:
                        alive_count += 1
            
            next_state[r][c] = state[r][c] 
            # print(alive_count)
            if state[r][c] == 1:
                if alive_count < 2: next_state[r][c] = 0
                elif alive_count >= 2 and alive_count <= 3: next_state[r][c] = 1
                elif alive_count > 3: next_state[r][c] = 0
            else:
                if alive_count == 3: next_state[r][c] = 1
    
    print('Iter: {}'.format(_iter))

    state = copy.deepcopy(next_state)
    for r in range(m):
        print(state[r])

