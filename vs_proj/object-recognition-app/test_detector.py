from __future__ import division, print_function

import sys
import os
import numpy as np
from math import floor
import pickle

import cv2

os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')

pid = os.getpid()
print('PID={}'.format(pid))

from object_recognition_toolkit import *
from my_hog import *
from my_svm import *


def main():

    raw_input('Press any key to begin...')


    pyramid_builder = FloatPyramidBuilder(scale_factor=1.2, min_size=Size(), max_size=Size())
    image_scanner = DenseImageScanner(win_size=Size(64,128), win_step=Size(8,8), padding=Size())
    feature_extractor = HogExtractor()
    nms = PassThroughNms()
    classifier = None
    
    with open('./training_0/classifier_2.pkl', 'r') as f:
        classifier = pickle.load(f)

    w = VecF32()
    w[:] = classifier.GetCoefs()[0, :]
    b = np.asscalar(classifier.GetIntercept())

    classifier = LinearClassifier(b, w)
    

    print(classifier)

    detector = DetectorBase()
    detector.pyramid_builder = pyramid_builder
    detector.image_scanner = image_scanner
    detector.feature_extractor = feature_extractor
    detector.non_maxima_suppressor = nms
    detector.classifier = classifier

    im = cv2.imread(r'd:\DEV.0\object-recognition-toolkit\vs_proj\datasets\INRIAPerson\Test\pos\crop001574.png', cv2.IMREAD_GRAYSCALE)
    disp = cv2.cvtColor(im, cv2.COLOR_GRAY2BGR)


    dets = BoxVector()
    confs = VecF64()
    detector.Detect(im, dets, confs, 0.0)


    for det in dets:
        x = det.x
        y = det.y
        w = det.width
        h = det.height
        cv2.rectangle(disp, (x,y), (x+w,y+h), (255,0,0))

    cv2.imshow('out', disp)
    cv2.waitKey()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()