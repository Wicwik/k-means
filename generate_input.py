import numpy as np

f = open('input', 'w+')

for i in range(100000):
	f.write(str(np.random.randint(10000)) + ',' + str(np.random.randint(10000)) + '\n')