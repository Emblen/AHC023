import matplotlib.pyplot as plt
# %matplotlib inline
# 初期位置での迷路の様子
 
# 図を描く大きさと、図の変数名を宣言
fig = plt.figure(figsize=(5, 5))
ax = plt.gca()
 
# 壁を描く
plt.plot([0, 0], [0, 1], color='black', linewidth=2)


 
# 描画範囲の設定と目盛りを消す設定
ax.set_xlim(0, 20)
ax.set_ylim(0, 20)
plt.tick_params(axis='both', which='both', bottom='off', top='off',
                labelbottom='off', right='off', left='off', labelleft='off')
ax.invert_yaxis()
ax.xaxis.tick_top()
 
plt.show()