import random
import subprocess
import numpy as np
from sklearn.datasets.samples_generator import make_blobs

print('Generating input to file input.csv')
f = open('input.csv', 'w+')

centers = [(-5, -5, -5), (5, 5, 5), (10, 10, 10), (-10, -10, -10), (0, 0, 0)]
cluster_std = [0.8, 1, 0.5, 0.75, 0.2]

X, y = make_blobs(n_samples=600, cluster_std=cluster_std, centers=centers, n_features=5, random_state=1)

random.shuffle(X);

for sample in X:
	for i, number in enumerate(sample):
		if i != (len(sample)-1):
			f.write(str(number) + ',')
		else:
			f.write(str(number) + '\n')