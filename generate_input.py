import numpy as np

f = open('input', 'w+')

samples = np.random.randn(10000, 3)

for sample in samples:
	for i, number in enumerate(sample):
		if i != (len(sample)-1):
			f.write(str(number) + ',')
		else:
			f.write(str(number) + '\n')

# f.write(str(np.random.randint(-100000000,100000000)) + ',' + str(np.random.randint(-100000000,100000000)) + '\n')
