from __future__ import division, print_function
from multiprocessing import Pool
from functools import partial

import sys
import os
import numpy as np
import copy

from sklearn.svm import LinearSVC


from math import floor

try:
    import cPickle as pickle
except:
    import pickle

os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')


import object_recognition_toolkitd as objrec_tk


class LinearSVM_Classifier(objrec_tk.Classifier):

    def __init__(self, inner_classifier):
        objrec_tk.Classifier.__init__(self)
        self.inner_classifier_ = inner_classifier
        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Predict(self, feature_vector):
        x = np.reshape(feature_vector, (1, -1,))
        y = self.inner_classifier_.predict(x)
        return np.asscalar(y)

    def PredictMulti(self, X, y):

        aiu = self.inner_classifier_.predict(X) 
        y[:,0] = aiu[:]
        
        return

    #implement pickle support
    def __reduce__(self):
        initargs = (self.inner_classifier_, )
        return (LinearSVM_Classifier, initargs, )

class LinearSVM_Trainer(objrec_tk.Trainer):

    def __init__(self, C):
        objrec_tk.Trainer.__init__(self)        
        self.C = C
        self.cls = None
        return
   
    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Train(self, features, labels):
        X = features
        y = labels

        self.cls = LinearSVC(C=self.C, verbose=1)
        self.cls.fit(X,y)
        return LinearSVM_Classifier(self.cls)

    #implement pickle support
    def __reduce__(self):
        initargs = (self.C, )
        state = {'cls', self.cls}
        return (LinearSVM_Trainer, initargs, state, )

    def __setstate__(self, state):
        self.cls = state['cls']
        return




__all__ = ['LinearSVM_Trainer','LinearSVM_Classifier']