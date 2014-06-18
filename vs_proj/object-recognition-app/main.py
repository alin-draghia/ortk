from __future__ import division, print_function

import sys
import os
import inspect
import cv2
import pickle
import numpy as np

import matplotlib
matplotlib.use('Qt4Agg')

import pylab
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg  as FigureCanvas
from matplotlib.figure import Figure



from PySide import QtUiTools, QtCore, QtGui

import object_recognition_toolkit as objrec_tk


def get_property_list(t):
    return [p for p in dir(t) if isinstance(getattr(t,p),property)]

class toolkit_object(object):
    def __init__(self, cls, name):
        self.cls = cls
        self.name = name


def make_detector():
    pyramid_builder = objrec_tk.FloatPyramidBuilder(scale_factor=1.05, min_size=objrec_tk.Size(63,127), max_size=objrec_tk.Size())
    image_scanner = objrec_tk.DenseImageScanner(win_size=objrec_tk.Size(64,128), win_step=objrec_tk.Size(8,8), padding=objrec_tk.Size())
    nms = objrec_tk.PassThroughNms()
    nms0 = objrec_tk.GroupRectanglesNms()
    feature_extractor = None
    classifier = None
    with open(r'training_10\classifier_10.pkl', 'r') as f:
        classifier = pickle.load(f)

    with open(r'training_10\feature_extracotr_dbg.pkl', 'r') as f:
        feature_extractor = pickle.load(f)

    w = objrec_tk.VecF32()
    w[:] = classifier.GetCoefs()[0, :]
    b = np.asscalar(classifier.GetIntercept())

    classifier_ = classifier
    classifier = objrec_tk.LinearClassifier(b, w)

    detector = objrec_tk.SlidingWindowDetector()
    detector.pyramid_builder = pyramid_builder
    detector.image_scanner = image_scanner
    detector.feature_extractor = feature_extractor
    detector.non_maxima_suppressor = nms
    detector.classifier = classifier

    return detector


class TheApp(object):

    def __init__(self, main_window):
        self.detector = make_detector()
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


        im = cv2.imread(filename, cv2.IMREAD_COLOR)
        if im is None:
            QtGui.QMessageBox.critical(self.main_window, "Open Image...", "Could not open the requested image!")
            return

        self.color_image = cv2.cvtColor(im, cv2.COLOR_BGR2RGB)
        self.gray_image = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
        
  
        #self.qt_image = QtGui.QImage()
        #self.qt_image.load(filename);
        #self.main_window.image_label.setPixmap(QtGui.QPixmap.fromImage(self.qt_image))

        self.fig = Figure()
        ax = self.fig.add_subplot(111)
        ax.imshow(self.color_image)
        
        canvas = FigureCanvas(self.fig)
        self.main_window.setCentralWidget(canvas)

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

        if(self.gray_image is None):
            return

        self.detection_boxex = objrec_tk.BoxVector()
        self.detection_scores = objrec_tk.VecF64()
        self.detector.Detect(self.gray_image, self.detection_boxex, self.detection_scores, 0.0)

        diplay_image = np.copy(self.color_image)
        for det in self.detection_boxex:
            x = det.x
            y = det.y
            w = det.width
            h = det.height
            cv2.rectangle(diplay_image, (x,y), (x+w,y+h), (255,0,0))

        self.fig = Figure()
        ax = self.fig.add_subplot(111)
        ax.imshow(diplay_image)
        
        canvas = FigureCanvas(self.fig)
        self.main_window.setCentralWidget(canvas)

        return

def main():


    pyramid_builders = []
    pyramid_builders.append(toolkit_object(objrec_tk.FloatPyramidBuilder, "FloatPyramidBuilder"))

    image_scanners = []
    image_scanners.append(toolkit_object(objrec_tk.DenseImageScanner, "DenseImageScanner"))

    feature_extractors = []
    feature_extractors.append(toolkit_object(objrec_tk.HogExtractor, "Python HogExtractor"))
    feature_extractors.append(toolkit_object(objrec_tk.HogFeatureExtractor, "C++ HogExtractor"))

    cls_subs = objrec_tk.Classifier.__subclasses__()
    classifiers = []
    classifiers.append(toolkit_object(objrec_tk.MockPersonClassifier, "MockPersonClassifier"))
    classifiers.append(toolkit_object(objrec_tk.LinearClassifier, "LinearClassifier"))
    classifiers.append(toolkit_object(objrec_tk.LinearSVM_Classifier, "LinearSVM_Classifier"))

    nms_subs = objrec_tk.NonMaximaSuppressor.__subclasses__()
    nmss = []
    nmss.append(toolkit_object(objrec_tk.PassThroughNms, "PassThroughNms"))
    nmss.append(toolkit_object(objrec_tk.GroupRectanglesNms, "GroupRectanglesNms"))
    

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
