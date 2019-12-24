def isPrime(x):
    for i in range(2, int(x ** (1 / 2)) + 1):
        if x % i == 0: return False
    return True

if __name__ == '__main__':
    # for i in range(20):
    #     if isPrime(i):
    #         print(i)
    for n in range(2, 100):
        if isPrime(2 ** n - 1):
            print((2 ** n - 1) * (2 ** (n - 1))) 