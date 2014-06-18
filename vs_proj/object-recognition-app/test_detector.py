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
from my_hog import HogExtractor
from my_svm import LinearSVM_Trainer
from my_detector import SlidingWindowDetector

def main():

    raw_input('Press any key to begin...')


    pyramid_builder = FloatPyramidBuilder(scale_factor=1.05, min_size=Size(63,127), max_size=Size())
    image_scanner = DenseImageScanner(win_size=Size(64,128), win_step=Size(8,8), padding=Size())
    nms = PassThroughNms()
    nms0 = GroupRectanglesNms()
    feature_extractor = None
    classifier = None
    
    with open(r'training_10\classifier_10.pkl', 'r') as f:
        classifier = pickle.load(f)

    with open(r'training_10\feature_extracotr_dbg.pkl', 'r') as f:
        feature_extractor = pickle.load(f)

    w = VecF32()
    w[:] = classifier.GetCoefs()[0, :]
    b = np.asscalar(classifier.GetIntercept())

    classifier_ = classifier
    classifier = LinearClassifier(b, w)
    

    print(classifier)

    #detector = DetectorBase()
    detector = SlidingWindowDetector()
    detector.pyramid_builder = pyramid_builder
    detector.image_scanner = image_scanner
    detector.feature_extractor = feature_extractor
    detector.non_maxima_suppressor = nms
    detector.classifier = classifier

    #im = cv2.imread(r'..\datasets\INRIAPerson\Test\pos\crop001501.png', cv2.IMREAD_GRAYSCALE)
    im = cv2.imread(r'..\datasets\INRIAPerson\Train\pos\person_and_bike_129.png', cv2.IMREAD_GRAYSCALE)
    #im = cv2.imread(r'..\datasets\INRIAPerson\train_64x128_H96\pos\crop001001a.png', cv2.IMREAD_GRAYSCALE)
    disp = cv2.cvtColor(im, cv2.COLOR_GRAY2BGR)
    disp2 = np.copy(disp)

    dets = BoxVector()
    confs = VecF64()

    detector_dump = pickle.dumps(detector)
    detector_ = pickle.loads(detector_dump)

    detector_.Detect(im, dets, confs, 0.0)


    for det in dets:
        x = det.x
        y = det.y
        w = det.width
        h = det.height
        cv2.rectangle(disp, (x,y), (x+w,y+h), (0,0,255))

    nms0.suppress(dets, confs)
    for det in dets:
        x = det.x
        y = det.y
        w = det.width
        h = det.height
        cv2.rectangle(disp2, (x,y), (x+w,y+h), (0,0,255))

    cv2.imshow('out', disp)
    cv2.imshow('out2', disp2)

    cv2.waitKey()
    cv2.destroyAllWindows()


if __name__ == '__main__':
    main()