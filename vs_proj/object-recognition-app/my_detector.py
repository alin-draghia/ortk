from __future__ import division, print_function

from object_recognition_toolkit import *
import numpy as np

try:
    import cPickle as pickle
except:
    import pickle

class SlidingWindowDetector(Detector):

    def __init__(self):
        Detector.__init__(self)
        self.pyramid_builder = None
        self.image_scanner = None
        self.feature_extractor = None
        self.classifier = None
        self.non_maxima_suppressor = None
        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Detect(self, image, detections, confidences, treshold):
        
        pyramid = self.pyramid_builder.Build(image)

        num_pyramid_leveles = pyramid.GetNumLevels();

        for pyramid_level_index in range(num_pyramid_leveles):
            pyramid_level = pyramid.GetLevel(pyramid_level_index)
            pyramid_level_image = pyramid_level.GetImage()
            
            windows = self.image_scanner.compute(pyramid_level_image)
            

            windows_images_li = []
            
            windows_images = VecMat()
            windows_boxes = BoxVector()

            for window in windows:
                window_image = window.Image
                window_box = window.Box
                windows_images_li.append(window_image)
                windows_images.append(window_image)
                windows_boxes.append(pyramid_level.Invert(window_box))
                pass


            X = self.feature_extractor.ComputeMulti(windows_images)
            y = self.classifier.PredictMulti(X)          
            
            num_ = len(y)

            for i in range(num_):
                confidence = np.asscalar(y[i])
                if confidence > treshold:
                    detections.append(windows_boxes[i])
                    confidences.append(confidence)                        

        return
