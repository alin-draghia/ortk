from __future__ import division, print_function

import sys
import os
import numpy as np
import cv2
from matplotlib import pylab as pl

try:
    import cPickle as pickle
except:
    import pickle

os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')

from object_recognition_toolkit import *
from hog import HogExtractor

def main():
    """ the application entry point """
    
    cpp_feature_extractor = objrec_tk.HogFeatureExtractor()
    py_feature_extracotr = HogExtractor(win_size=(64,128))

    file_path = r'..\datasets\INRIAPerson\train_64x128_H96\pos\person_and_bike_155a.png'
    im = cv2.imread(file_path, cv2.IMREAD_GRAYSCALE)

    im = im[15:15+128,15:15+64]
        
    cpp_feats = cpp_feature_extractor.compute(im)
    py_feats = py_feature_extracotr.compute(im)

    diff = cpp_feats - py_feats

    val = np.sum(diff)

    viz = py_feature_extracotr.visualisation()

    pl.figure()
    pl.subplot(121)
    pl.imshow(im, cmap='gray')
    pl.subplot(122)
    pl.imshow(viz)
    pl.show()
    
    return

if __name__ == '__main__':
    main()
else:
    pass