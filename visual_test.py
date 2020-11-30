import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm


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

colors = {0:'red', 1:'green', 2:'blue', 3:'orange', 4:'yellow', 5:'black', 6:'brown', 7:'purple', 8:'pink', 9:'#1ecbe1'}

x = df.a
y = df.b
z = df.c
c = df.z

ax.scatter(x, y, z, c=c.map(colors))
ax.plot([7.8806, 21.475, 1.41195, 0.451235], [7.83461, 13.4123, 17.4721, -0.237967], [7.68792, 11.1143, 16.5204, 0.488335], c='blue');


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