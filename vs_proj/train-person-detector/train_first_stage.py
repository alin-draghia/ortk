import numpy as np
from sklearn.svm import LinearSVC
import cPickle

print('Loading dataset...');

#dataset = np.loadtxt('first.stage.in', delimiter=',');

#np.savez('first.stage.in.npz', dataset)
dataset = np.load('first.stage.in.npz')['arr_0']
X = dataset[:, 1:]
y = dataset[:, 0]

num_samples = dataset.shape[0]

print('num_samples=', num_samples)

print()
print('Begin trainig...')

cls = LinearSVC(C=0.01, verbose=1)

cls.fit(X, y);

with open('first.stage.LinearSVC.pkl', 'wb') as fid:
    cPickle.dump(cls, fid)


np.savetxt('first.stage.coef_.out', cls.coef_)
np.savetxt('first.stage.intercept_.out', cls.intercept_)



print('Training done')




