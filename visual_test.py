import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('input', header=None)
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

df = pd.read_csv('output')

colors = {0:'red', 1:'green', 2:'blue', 3:'orange'}

x = df.a
y = df.b
z = df.c
c = df.z

ax.scatter(x, y, z, c=c.map(colors))


plt.show()


# 
# sns.scatterplot(x=df['Y'], 
#                  y=df['X'])
# plt.title('Scatterplot')

# plt.figure()
# df = pd.read_csv('output')
# sns.scatterplot(x=df.x, y=df.y, 
#                 hue=df.c, 
#                 palette=sns.color_palette('hls', n_colors=10))
# plt.xlabel('X')
# plt.ylabel('Y')
# plt.title('Clustered')


# plt.show()