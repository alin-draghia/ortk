from __future__ import division, print_function

import sys
import os
import inspect
import cv2
import pickle
import numpy as np

from threading import Thread

import matplotlib
matplotlib.use('Qt4Agg')

import pylab
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg  as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.patches import Rectangle


from PySide import QtUiTools, QtCore, QtGui

import object_recognition_toolkit as objrec_tk


def get_property_list(t):
    return [p for p in dir(t) if isinstance(getattr(t,p),property)]

class toolkit_object(object):
    def __init__(self, cls, name):
        self.cls = cls
        self.name = name


def load_detector():
    pyramid_builder = objrec_tk.FloatPyramidBuilder(scale_factor=1.05, min_size=objrec_tk.Size(63,127), max_size=objrec_tk.Size())
    image_scanner = objrec_tk.DenseImageScanner(win_size=objrec_tk.Size(64,128), win_step=objrec_tk.Size(8,8), padding=objrec_tk.Size())
    #nms = objrec_tk.PassThroughNms()
    nms = objrec_tk.GroupRectanglesNms()
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

    #detector = objrec_tk.SlidingWindowDetector()
    detector = objrec_tk.SlidingWindowDetector2()

    detector.pyramid_builder = pyramid_builder
    detector.image_scanner = image_scanner
    detector.feature_extractor = feature_extractor
    detector.non_maxima_suppressor = nms
    detector.classifier = classifier

    return detector


class TheApp(object):

    def __init__(self, main_window):
        self.detector = load_detector()
        self.main_window = main_window
        self.main_window.actionImage.triggered.connect(self.open_image)
        self.main_window.actionOpen_Detector.triggered.connect(self.open_detector)
        self.main_window.actionConfigure_Detector.triggered.connect(self.configure_detector)
        self.main_window.actionRun_Detector.triggered.connect(self.run_detector)
        self.main_window.actionSave_Detector.triggered.connect(self.save_detector)
        self.configure_detector_dialog = None
        self.image = None
        self.classification_threshold = 0.0

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

        self.fig = Figure(figsize=(700,700), dpi=72, frameon=False)
        #ax = self.fig.add_subplot(111)
        ax = self.fig.gca(label="image")
        ax.imshow(self.color_image)
 
        self.canvas = FigureCanvas(self.fig)
        self.main_window.setCentralWidget(self.canvas)



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

        scale_factor = self.detector.pyramid_builder.scale_factor
        self.configure_detector_dialog.scale_factor.setValue(scale_factor)

        win_step_x = self.detector.image_scanner.win_step.width
        win_step_y = self.detector.image_scanner.win_step.height
        self.configure_detector_dialog.scanner_win_step_x.setValue(win_step_x)
        self.configure_detector_dialog.scanner_win_step_y.setValue(win_step_y)

        pad_x = self.detector.image_scanner.padding.width
        pad_y = self.detector.image_scanner.padding.height

        self.configure_detector_dialog.scanner_pad_x.setValue(pad_x)
        self.configure_detector_dialog.scanner_pad_y.setValue(pad_y)

        nms_group = self.detector.non_maxima_suppressor.GroupThreshold
        nms_eps   = self.detector.non_maxima_suppressor.eps

        self.configure_detector_dialog.nms_group_threshold.setValue(nms_group)
        self.configure_detector_dialog.nms_eps.setValue(nms_eps)

        self.configure_detector_dialog.classification_threshold.setValue(self.classification_threshold)
        
        r = self.configure_detector_dialog.exec_()
        if r == QtGui.QDialog.DialogCode.Accepted:
            print('ok')

            self.detector.pyramid_builder.scale_factor = self.configure_detector_dialog.scale_factor.value()
            
            self.detector.image_scanner.win_step.width  = self.configure_detector_dialog.scanner_win_step_x.value()
            self.detector.image_scanner.win_step.height = self.configure_detector_dialog.scanner_win_step_y.value()

            self.detector.image_scanner.padding.width  = self.configure_detector_dialog.scanner_pad_x.value()
            self.detector.image_scanner.padding.height = self.configure_detector_dialog.scanner_pad_y.value()
            
            self.detector.non_maxima_suppressor.GroupThreshold  = self.configure_detector_dialog.nms_group_threshold.value()
            self.detector.non_maxima_suppressor.eps             = self.configure_detector_dialog.nms_eps.value()

            self.classification_threshold = self.configure_detector_dialog.classification_threshold.value()

        elif r == QtGui.QDialog.DialogCode.Rejected:
            print('reject')
        else:
            print('unk')


        return

    
    def detect_proc(self):
        self.detection_boxex = objrec_tk.BoxVector()
        self.detection_scores = objrec_tk.VecF64()
        self.detector.callback = self.on_detector_window
        self.prev_patch = None
        self.detector.Detect(self.gray_image, self.detection_boxex, self.detection_scores, self.classification_threshold)

        diplay_image = np.copy(self.color_image)
        #for det in self.detection_boxex:
        #    x = det.x
        #    y = det.y
        #    w = det.width
        #    h = det.height
        #    cv2.rectangle(diplay_image, (x,y), (x+w,y+h), (255,0,0))

        

        self.fig.patches = []
        #ax = self.fig.add_subplot(111)
        #ax.imshow(diplay_image)
        ax = self.fig.gca()
        self.fig.delaxes(ax)
        ax = self.fig.gca()
        ax.imshow(diplay_image)
        for det in self.detection_boxex:
            x = det.x
            y = det.y
            w = det.width
            h = det.height

            r = Rectangle((x,y), w, h, fill=False)
            r.set_edgecolor('#1AFF00')
            ax.add_patch(r)

        self.canvas.draw()
        
        #canvas = FigureCanvas(self.fig)
        #self.main_window.setCentralWidget(canvas)

    def run_detector(self):
        print('run detector...')

        if(self.gray_image is None):
            return


        class DetectionThread(Thread):

            def __init__(self, parent):
                Thread.__init__(self)
                self.patent = parent
                return

            def run(self):
                self.patent.detect_proc()

            pass
            


        self.detection_thread = DetectionThread(self)
        self.detection_thread.start()

        

        return

    def on_detector_window(self, msg):
        if self.prev_patch is not None:
            self.prev_patch.remove()
            self.prev_patch = None
        box, score = msg
        ax = self.fig.gca()
        x = box.x
        y = box.y
        w = box.width
        h = box.height
        r = Rectangle((x,y), w, h, label='detX', fill=False)

        if score > self.classification_threshold:
            r.set_edgecolor('#1AFF00')
        else:
            r.set_edgecolor('#E500FF')

        ax.add_patch(r)
        self.canvas.draw()
        
        if score > 0.0:
            self.prev_patch = None
        else:
            self.prev_patch = r
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
