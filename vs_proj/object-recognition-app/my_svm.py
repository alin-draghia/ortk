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


import object_recognition_toolkit as objrec_tk


class LinearSVM_Classifier(objrec_tk.Classifier):

    def __init__(self, inner_classifier):
        objrec_tk.Classifier.__init__(self)
        self.inner_classifier_ = inner_classifier
        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Predict(self, feature_vector):
        try:
            x = np.reshape(feature_vector, (1, -1,))
            y = self.inner_classifier_.predict(x)
            return np.asscalar(y)
        except:
            print("Predict error:", sys.exc_info()[0])
            print("feature_vector:")
            print(feature_vector)
            return -1.0

    def PredictMulti(self, X, y):
        try:
            aiu = self.inner_classifier_.predict(X) 
            y[:,0] = aiu[:]
        except ValueError as e:
            pass
        
        return

    def GetCoefs(self):
        return self.inner_classifier_.coef_

    def GetIntercept(self):
        return self.inner_classifier_.intercept_

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
        self.cls = LinearSVC(C=self.C, verbose=9)
        self.cls.fit(X,y)

        avg_accuracy = self.cls.score(X,y)
        print('avg_accuracy=', avg_accuracy)

        cls_ = LinearSVM_Classifier(self.cls)

        self.cls = None

        return cls_

    #implement pickle support
    def __reduce__(self):
        initargs = (self.C, )
        state = {'cls', self.cls}
        return (LinearSVM_Trainer, initargs, state, )

    def __setstate__(self, state):
        self.cls = state['cls']
        return




__all__ = ['LinearSVM_Trainer','LinearSVM_Classifier']