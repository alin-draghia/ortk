from __future__ import division, print_function

import sys
import os
import inspect

from PySide import QtUiTools, QtGui

from object_recognition_toolkit import *
import my_detector
import my_hog
import my_svm


def main():

    
    detector_subs = Detector.__subclasses__()

    feature_extractor_subs = FeatureExtractor.__subclasses__()

    nms_subs = NonMaximaSuppressor.__subclasses__()

    pyramid_builder_subs = PyramidBuilder.__subclasses__()

    image_scanner_subs = ImageScanner.__subclasses__()

    classifier_subs = Classifier.__subclasses__()

    classifier_trainer_subs = Trainer.__subclasses__()

    detector_trainer_subs = DetectorTrainer.__subclasses__()

    #main application
    app = QtGui.QApplication(sys.argv)

    loader = QtUiTools.QUiLoader()

    #load the main window file
    main_window = loader.load('main_window.ui')
    main_window.show()
    sys.exit(app.exec_())
    return

if __name__ == '__main__':
    main()
