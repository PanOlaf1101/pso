import sys, random

name, width, height = sys.argv[1:4]

arr = random.choice([-10.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10], p=[0.25, 0.1, 0.1, 0.1, 0.1, 0.05, 0.05, 0.05, 0.05, 0.05, 0.5], size=(height, width))

with open(name, 'w+') as f:
    print(width, height, file=f)
    for i in range(height):
        print(' '.join(map(str, arr[i])))
