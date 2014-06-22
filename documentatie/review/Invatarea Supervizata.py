# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

from sklearn.datasets import make_regression
import matplotlib.pyplot as plt
X, y, c = make_regression(n_samples=30, n_features=1, n_informative=1,coef=True, noise=6.0, bias=-3)
ax = plt.gca()
plt.title('Regresie')
pts = ax.scatter(X,y, c='red', label=r'${ \mathcal{D} = \{(x_i,y_i)|x_i \in X, y_i \in Y\}|_{i=1}^n }$')
line, = ax.plot(X, X*c, c='green', label=r'${f(x)\approx y}$')
plt.xlabel('x')
plt.ylabel('y')
ax.legend(loc='upper left')
plt.show()

# <codecell>

from sklearn.datasets import make_classification, make_blobs
from sklearn.linear_model import Perceptron
import numpy as np
import matplotlib.pyplot as plt
X, y = make_blobs(n_samples=50, centers=2, random_state=0, cluster_std=0.60)

plt.title('Clasificare')

classifier = Perceptron()
classifier.fit(X,y)
score = classifier.score(X,y)
print 'score=', score

xx = np.linspace(0,+4)
yy = np.linspace(0,+4)
X1, X2 = np.meshgrid(xx,yy)
Z = np.empty(X1.shape)
for (i,j), val in np.ndenumerate(X1):
    x1 = val
    x2 = X2[i,j]
    p = classifier.decision_function([x1,x2])
    Z[i,j] = p[0]

levels=[-1.0, 0.0, 1.0]
linestyles = ['dashed','solid','dashed']
linewidths = [1.0, 2.5, 1.0]
colors=['black', 'green', 'black']
c = plt.contour(X1, X2, Z, levels, colors=colors, 
                linewidths=linewidths, linestyles=linestyles)
c.collections[1].set_label('classifier')

pos_idx = y == 1
neg_idx = y == 0
plt.scatter(X[pos_idx,0], X[pos_idx,1], c='blue', s=100, label='pos')
plt.scatter(X[neg_idx,0], X[neg_idx,1], c='red', s=100, label='neg')

plt.xlabel('x0')
plt.ylabel('x1')
ax = plt.gca()
ax.legend()
plt.show()

# <codecell>


# <codecell>


