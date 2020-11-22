import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('input', header=None)
df.columns = ['X', 'Y']
sns.scatterplot(x=df['Y'], 
                y=df['X'])
plt.title('Scatterplot')

plt.figure()
df = pd.read_csv('output')
sns.scatterplot(x=df.x, y=df.y, 
                hue=df.c, 
                palette=sns.color_palette('hls', n_colors=10))
plt.xlabel('X')
plt.ylabel('Y')
plt.title('Clustered')


plt.show()