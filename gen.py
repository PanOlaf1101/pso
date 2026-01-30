import sys
import numpy as np

name, width, height = sys.argv[1:4]
width, height = map(int, (width, height))

arr = np.random.uniform(low = -10.0, high = 10.0, size=(height, width))

with open(name, 'w+') as f:
    print(width, height, file=f)
    for i in range(height):
        print(' '.join(map(str, arr[i])), file=f)
