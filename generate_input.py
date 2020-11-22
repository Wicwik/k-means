import numpy as np

f = open('input', 'w+')

for i in range(100):
	f.write(str(np.random.randint(-100000000,100000000)) + ',' + str(np.random.randint(-100000000,100000000)) + '\n')