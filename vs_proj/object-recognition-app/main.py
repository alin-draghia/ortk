from __future__ import division, print_function

import sys
import os
import inspect
import cv2

from PySide import QtUiTools, QtGui

import object_recognition_toolkit as my_toolkit


def get_property_list(t):
    return [p for p in dir(t) if isinstance(getattr(t,p),property)]

class toolkit_object(object):
    def __init__(self, cls, name):
        self.cls = cls
        self.name = name


class TheApp(object):

    def __init__(self, main_window):
        self.main_window = main_window
        self.main_window.actionImage.triggered.connect(self.open_image)
        self.main_window.actionOpen_Detector.triggered.connect(self.open_detector)
        self.main_window.actionConfigure_Detector.triggered.connect(self.configure_detector)
        self.main_window.actionRun_Detector.triggered.connect(self.run_detector)
        self.main_window.actionSave_Detector.triggered.connect(self.save_detector)
        self.configure_detector_dialog = None
        self.image = None
        return

    def open_image(self):
        print('open image...')
        filename, selected_filter = QtGui.QFileDialog.getOpenFileName(self.main_window, "Open Image...")
        if not filename:
            return


        return

    def open_detector(self):
        print('open detector...')
        filename, selected_filter = QtGui.QFileDialog.getOpenFileName(self.main_window, "Open Detector...")
        if not filename:
            return

        return

    def save_detector(self):
        print('save detector...')
        filename, selected_filter = QtGui.QFileDialog.getSaveFileName(self.main_window, "Save Detector...")
        if not filename:
            return

        return

    def configure_detector(self):
        print('configure detector...')

        r = self.configure_detector_dialog.exec_()
        if r == QtGui.QDialog.DialogCode.Accepted:
            print('ok')
        elif r == QtGui.QDialog.DialogCode.Rejected:
            print('reject')
        else:
            print('unk')


        return

    def run_detector(self):
        print('run detector...')
        return

def main():


    pyramid_builders = []
    pyramid_builders.append(toolkit_object(my_toolkit.FloatPyramidBuilder, "FloatPyramidBuilder"))

    image_scanners = []
    image_scanners.append(toolkit_object(my_toolkit.DenseImageScanner, "DenseImageScanner"))

    feature_extractors = []
    feature_extractors.append(toolkit_object(my_toolkit.HogExtractor, "Python HogExtractor"))
    feature_extractors.append(toolkit_object(my_toolkit.HogFeatureExtractor, "C++ HogExtractor"))

    cls_subs = my_toolkit.Classifier.__subclasses__()
    classifiers = []
    classifiers.append(toolkit_object(my_toolkit.MockPersonClassifier, "MockPersonClassifier"))
    classifiers.append(toolkit_object(my_toolkit.LinearClassifier, "LinearClassifier"))
    classifiers.append(toolkit_object(my_toolkit.LinearSVM_Classifier, "LinearSVM_Classifier"))

    nms_subs = my_toolkit.NonMaximaSuppressor.__subclasses__()
    nmss = []
    nmss.append(toolkit_object(my_toolkit.PassThroughNms, "PassThroughNms"))
    nmss.append(toolkit_object(my_toolkit.GroupRectanglesNms, "GroupRectanglesNms"))
    

    #main application
    app = QtGui.QApplication(sys.argv)

    loader = QtUiTools.QUiLoader()

    #load the main window file
    main_window = loader.load('main_window.ui')
    the_ = TheApp(main_window)

    the_.configure_detector_dialog = loader.load('configure_detector_dialog.ui')

    main_window.showNormal()
    app.exec_()
    #sys.exit(app.exec_())

    return

if __name__ == '__main__':
    main()
