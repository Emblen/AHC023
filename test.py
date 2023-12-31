import matplotlib.pyplot as plt
import numpy as np
f = open('mapdata.txt', 'r')
s = f.readline()
print(s)
t, h, w, i0 = map(int, s.split())


# 図を描く大きさと、図の変数名を宣言
fig = plt.figure(figsize=(8,8))
ax = plt.gca()

# 描画範囲の設定と目盛りを消す設定
ax.set_xlim(0, 20)
ax.set_ylim(0, 20)
plt.tick_params(axis='both', which='both', bottom='off', top='off',
                labelbottom='off', right='off', left='off', labelleft='off')
ax.invert_yaxis()
ax.set_yticks(np.linspace(0, 20, 21))
ax.xaxis.tick_top()
ax.set_xticks(np.linspace(0, 20, 21))
plt.grid(linestyle='dotted')

# 柵と出入り口の描写
plt.plot([0, 0], [0, i0], color='black', linewidth=3)
plt.plot([0, 0], [i0, i0+1], color='red', linewidth=5)
plt.plot([0, 0], [i0+1, 20], color='black', linewidth=3)
plt.plot([20, 20], [0, 20], color='black', linewidth=3)
plt.plot([0, 20], [0, 0], color='black', linewidth=3)
plt.plot([0, 20], [20, 20], color='black', linewidth=3)

# 水路の描写
for i in range(1,h):
    s = f.readline()
    for j in range(len(s)):
        if s[j]=="1": 
            plt.plot([j, j+1], [i, i], color='black', linewidth=2)
            
for i in range(h):
    s = f.readline()
    for j in range(len(s)):
        if s[j]=="1": 
            plt.plot([j+1, j+1], [i, i+1], color='black', linewidth=2)    

#距離用
f2 = open('dist.txt', 'r')
dist = f2.read().split()
for i in range(h):
    for j in range(w):
        c = dist[i*w+j]
        if c=='1':
            print(c)
            ax.axhspan(i, i+1, j*0.05, (j+1)*0.05, color="coral")
        ax.text(j+0.2, i+0.8, c, size=8)
        
#パス用
# f2 = open('dist.txt', 'r')
# dist = f2.read().split()
# dist_int = [int(i) for i in dist]
# for i in range(int(len(dist)/2)):
#     y = dist_int[i*2]
#     x = dist_int[i*2+1]
#     ax.text(x+0.2, y+0.8, 0, size=8)
    

#栽培期間のヒストグラム
fig2 = plt.figure()
f3 = open('hist.txt', 'r')
pdata = f3.read().split()
pdata_int = [int(i) for i in pdata]
plt.grid(linestyle='dotted')
plt.hist(pdata_int, color='green', rwidth=0.9, bins=25)

plt.show()