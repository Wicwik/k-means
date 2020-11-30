import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm


df = pd.read_csv('input.csv', header=None)
df.columns = ['X', 'Y', 'Z']

fig1 = plt.figure()
ax = fig1.add_subplot(111, projection = '3d')


x = df['X']
y = df['Y']
z = df['Z']

ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

ax.scatter(x, y, z)

fig = plt.figure()
ax = fig.add_subplot(111, projection = '3d')

df = pd.read_csv('output.csv')

colors = {0:'red', 1:'green', 2:'blue', 3:'orange', 4:'yellow', 5:'black', 6:'brown', 7:'purple', 8:'pink', 9:'#1ecbe1'}

x = df['0']
y = df['1']
z = df['2']
c = df.c

ax.scatter(x, y, z, c=c.map(colors))
plt.show()