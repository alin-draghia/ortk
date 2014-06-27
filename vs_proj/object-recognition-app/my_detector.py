from __future__ import division, print_function

from object_recognition_toolkit import *
import numpy as np
import time

from skimage.feature import hog

try:
    import cPickle as pickle
except:
    import pickle

class SlidingWindowDetector(Detector):


    def __init__(self, pyramid_builder=None, image_scanner=None, feature_extractor=None, classifier=None, non_maxima_suppressor=None):
        Detector.__init__(self)
        self.pyramid_builder = pyramid_builder
        self.image_scanner = image_scanner
        self.feature_extractor = feature_extractor
        self.classifier = classifier
        self.non_maxima_suppressor = non_maxima_suppressor
        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Detect(self, image, detections, confidences, treshold):
        
        pyramid = self.pyramid_builder.Build(image)

        num_pyramid_leveles = pyramid.GetNumLevels();

        for pyramid_level_index in range(num_pyramid_leveles):

            t0 = time.time()

            pyramid_level = pyramid.GetLevel(pyramid_level_index)
            pyramid_level_image = pyramid_level.GetImage()
            
            windows = self.image_scanner.compute(pyramid_level_image)
            

            windows_images_li = []
            
            windows_images = VecMat()
            windows_boxes = BoxVector()

            k = 0;
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
                    
            t = time.time() - t0                     
            print('time=', t)

        return

    #implement pickle support
    def __getinitargs__(self):
        return (self.pyramid_builder, self.image_scanner, self.feature_extractor, self.classifier, self.non_maxima_suppressor)


class SlidingWindowDetector2(Detector):


    def __init__(self, pyramid_builder=None, image_scanner=None, feature_extractor=None, classifier=None, non_maxima_suppressor=None):
        Detector.__init__(self)
        self.pyramid_builder = pyramid_builder
        self.image_scanner = image_scanner
        self.feature_extractor = feature_extractor
        self.classifier = classifier
        self.non_maxima_suppressor = non_maxima_suppressor
        self.callback = None
        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Detect(self, image, detections, confidences, treshold):
        
        pyramid = self.pyramid_builder.Build(image)

        num_pyramid_leveles = pyramid.GetNumLevels();

        for pyramid_level_index in reversed(range(num_pyramid_leveles)):
            pyramid_level = pyramid.GetLevel(pyramid_level_index)
            pyramid_level_image = pyramid_level.GetImage()
            
            windows = self.image_scanner.compute(pyramid_level_image)
            
            for window in windows:
                window_image = window.Image
                window_box = window.Box
 

                x = self.feature_extractor.Compute(window_image)
                y = self.classifier.Predict(x)          
 
                if self.callback is not None:
                    msg = (pyramid_level.Invert(window_box), y,)
                    self.callback(msg)
                    
                if y > treshold:
                    detections.append(pyramid_level.Invert(window_box))
                    confidences.append(y)      
                 
        
        self.non_maxima_suppressor.suppress(detections, confidences);           
        return

    #implement pickle support
    def __getinitargs__(self):
        return (self.pyramid_builder, self.image_scanner, self.feature_extractor, self.classifier, self.non_maxima_suppressor)