from __future__ import division, print_function
import sys
import Queue
import itertools

import matplotlib
matplotlib.use('Qt4Agg')

import matplotlib.animation
from matplotlib.backends.backend_qt4agg import FigureCanvasQTAgg  as FigureCanvas
from matplotlib.figure import Figure
from matplotlib.patches import Rectangle
from matplotlib.collections import PatchCollection

from PySide import QtUiTools, QtCore, QtGui

import cv2
from visual_detector import VisualDetector
from object_recognition_toolkit import BoxVector, VecF64

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
        
        self._detector = VisualDetector()

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

        #self.fig = Figure(figsize=(700,700), dpi=72, frameon=False)
        self.fig = Figure(tight_layout=True)
        #ax = self.fig.add_subplot(111)
        ax = self.fig.gca(label="image")
        #ax.set_animated(True)
        ax.imshow(self.color_image)
 
        self.canvas = FigureCanvas(self.fig)
        self.main_window.setCentralWidget(self.canvas)

        return

    def open_detector(self):
        print('open detector...')
        filename, selected_filter = QtGui.QFileDialog.getOpenFileName(self.main_window, "Open Detector...")
        if not filename:
            return

        self._detector.load(filename)

        return

    def save_detector(self):
        print('save detector...')
        filename, selected_filter = QtGui.QFileDialog.getSaveFileName(self.main_window, "Save Detector...")
        if not filename:
            return

        self._detector.save(filename)
        return

    def configure_detector(self):
        print('configure detector...')

        if self._detector is None:
            return 

        cfg = self.configure_detector_dialog

        cfg.scale_factor.setValue(self._detector.scale_factor)
        cfg.scanner_win_step_x.setValue(self._detector.win_step.width)
        cfg.scanner_win_step_y.setValue(self._detector.win_step.height)
        cfg.scanner_pad_x.setValue(self._detector.padding.width)
        cfg.scanner_pad_y.setValue(self._detector.padding.height)
        cfg.nms_group_threshold.setValue(self._detector.group_treshold)
        cfg.nms_eps.setValue(self._detector.group_eps)
        cfg.classification_threshold.setValue(self._detector.classification_treshold)
        
        r = self.configure_detector_dialog.exec_()
        if r == QtGui.QDialog.DialogCode.Accepted:
            print('ok')

            self._detector.scale_factor = self.configure_detector_dialog.scale_factor.value()
            
            self._detector.win_step.width  = self.configure_detector_dialog.scanner_win_step_x.value()
            self._detector.win_step.height = self.configure_detector_dialog.scanner_win_step_y.value()

            self._detector.padding.width  = self.configure_detector_dialog.scanner_pad_x.value()
            self._detector.padding.height = self.configure_detector_dialog.scanner_pad_y.value()
            
            self._detector.group_treshold = self.configure_detector_dialog.nms_group_threshold.value()
            self._detector.group_eps = self.configure_detector_dialog.nms_eps.value()

            self._detector.classification_treshold = self.configure_detector_dialog.classification_threshold.value()

        elif r == QtGui.QDialog.DialogCode.Rejected:
            print('reject')
        else:
            print('unk')


        return

    def sliding_window_anim_func(self, i):
        try:
            #box, score = self._detector._queue.get(timeout=0.02)
            box, score = self._detector._sliding_window_queue.get(block=False)

            if box == None:
                self.sliding_window_anim_._stop()                
                self.sliding_window.set_visible(False)
                self.detected_windows_collection.set_visible(False)

                self._detector.post_processor.suppress(self.boxes, self.scores)
                for b in self.boxes:
                    R = Rectangle((b.x,b.y), b.width, b.height)
                    self.boxes_patches.append(R)

                self.boxes_collection.set_paths(self.boxes_patches)
                return self.sliding_window, self.detected_windows_collection, self.boxes_collection

            self.sliding_window.set_x(box.x)
            self.sliding_window.set_y(box.y)
            self.sliding_window.set_width(box.width)
            self.sliding_window.set_height(box.height)
            self.sliding_window.set_visible(True)
            self.sliding_window.set_edgecolor('#E500FF')

            if(score>self._detector.classification_treshold):
                self.boxes.append(box)
                self.scores.append(score)
                D = Rectangle((box.x, box.y), box.width, box.height)         
                self.detected_windows.append(D)
                self.detected_windows_collection.set_paths(self.detected_windows)
                
        except Queue.Empty:
            pass

        return self.sliding_window, self.detected_windows_collection, self.boxes_collection


    def run_detector(self):
        print('run detector...')

        if(self.gray_image is None):
            return

        self._detector.run_detection(self.gray_image)


        self.sliding_window = Rectangle((5,5), 5, 5)        
        self.sliding_window.set_fill(False)
        self.sliding_window.set_edgecolor('#E500FF')
        self.sliding_window.set_visible(False)

        self.detected_windows = []
        self.detected_windows_collection = PatchCollection(patches=self.detected_windows)
        self.detected_windows_collection.set_edgecolor('#1AFF00')
        self.detected_windows_collection.set_linewidth(2.0)
        self.detected_windows_collection.set_facecolor('none')

        self.boxes = BoxVector()
        self.scores = VecF64()
        self.boxes_patches = []
        self.boxes_collection = PatchCollection(self.boxes_patches)
        self.boxes_collection.set_edgecolor('#1AFF00')
        self.boxes_collection.set_linewidth(2.0)
        self.boxes_collection.set_facecolor('none')
        ax = self.fig.gca()        

        ax.add_collection(self.boxes_collection)
        ax.add_collection(self.detected_windows_collection)

        ax.add_patch(self.sliding_window)
        self.sliding_window_anim_ = matplotlib.animation.FuncAnimation(self.fig, self.sliding_window_anim_func, interval=1, blit=True)
        
        self.detections_li = []
        return


def main():


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
