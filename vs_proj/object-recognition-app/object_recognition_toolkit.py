import sys
import os

#add path to binaries and 3rd party
__OBJECT_RECOGNITION_TOOLKIT_BIN_DIR__ = '../bin'
__OPENCV_BIN_DIR__ = '../3rd-party/opencv/x64/vc12/bin'
__BOOST_SERIALIZATION_BIN_DIR__ = '../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib'
sys.path.append(__OBJECT_RECOGNITION_TOOLKIT_BIN_DIR__)
os.environ['PATH'] += (';' + __OPENCV_BIN_DIR__)
os.environ['PATH'] += (';' + __BOOST_SERIALIZATION_BIN_DIR__)


__OBJECT_RECOGNITION_TOOLKIT_DEBUG__ = True
if __OBJECT_RECOGNITION_TOOLKIT_DEBUG__ == True:
    from object_recognition_toolkitd_ext import *
else:
    from object_recognition_toolkit_ext import *

from my_svm import LinearSVM_Trainer, LinearSVM_Classifier
from my_hog import HogExtractor
from my_detector import SlidingWindowDetector, SlidingWindowDetector2


