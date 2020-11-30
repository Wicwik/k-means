import numpy as np

f = open('input', 'w+')

samples1 = np.random.randn(100, 3)
samples2 = 10 + 10 * np.random.randn(100, 3)
samples3 = 11 + 11 * np.random.randn(100, 3)
samples4 = 12 + 12 * np.random.randn(100, 3)

samples = np.concatenate((samples1, samples2, samples3, samples4), axis=0)

print(len(samples))

for sample in samples:
	for i, number in enumerate(sample):
		if i != (len(sample)-1):
			f.write(str(number) + ',')
		else:
			f.write(str(number) + '\n')

# f.write(str(np.random.randint(-100000000,100000000)) + ',' + str(np.random.randint(-100000000,100000000)) + '\n')
