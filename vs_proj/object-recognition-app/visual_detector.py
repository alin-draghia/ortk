__all__ = ["VisualDetector"]

import pickle
import time
import multiprocessing
import numpy as np

from object_recognition_toolkit import *



def _detection_proc(me, image, sliding_window_queue):
    

    print('DETECTION PROCESS:', 'Hello')

    t0 = time.time()

    boxes = BoxVector()
    scores = VecF64()

    pyramid = me.pyramid_builder.Build(image)

    num_pyramid_leveles = pyramid.GetNumLevels();

    for pyramid_level_index in range(num_pyramid_leveles):

        pyramid_level = pyramid.GetLevel(pyramid_level_index)
        pyramid_level_image = pyramid_level.GetImage()
        
        rows, cols = pyramid_level_image.shape
        
        num_slides_x = cols / me.image_scanner.win_step.width
        slides_to_skip = int(num_slides_x/10)

        windows = me.image_scanner.compute(pyramid_level_image)
            
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

        X = me.feature_extractor.ComputeMulti(windows_images)
        y = me.classifier.PredictMulti(X)          
            
        num_ = len(y)

        for i in range(0,num_,1):
            confidence = np.asscalar(y[i])
            if confidence > me.classification_treshold:
                boxes.append(windows_boxes[i])
                scores.append(confidence)   
                sliding_window_queue.put((windows_boxes[i], np.asscalar(y[i]),), block=False)

            if i%slides_to_skip==0:
                sliding_window_queue.put((windows_boxes[i], np.asscalar(y[i]),), block=False)
           

    me.post_processor.suppress(boxes, scores)        

    t1 = time.time()

    duration = t1 - t0;

    #send santinel object
    sliding_window_queue.put((None, None))
    print('DETECTION PROCESS:', 'processing time = %0.3f seconds'%(duration,))

    return


class VisualDetector(object):
    def __init__(self):
        self._pyramid_builder               = None
        self._image_scanner                 = None
        self._feature_extractor             = None
        self._classifier                    = None
        self._post_processor                = None
        self._classification_treshold       = None
        return


    def run_detection(self, image):
        
        #VisualDetector._detection_proc(self, image, boxes, scores, None)

        self._sliding_window_queue = multiprocessing.Queue()        
        
        self._proc = multiprocessing.Process(target=_detection_proc, args = (self, image, self._sliding_window_queue,))
        self._proc.daemon = True
        self._proc.start()
        return

    def load(self, filename):
        with open(filename, 'r') as fid:
            di = pickle.load(fid)
            self._pyramid_builder            = di['pyramid_builder']         
            self._image_scanner              = di['image_scanner']           
            self._feature_extractor          = di['feature_extractor']       
            self._classifier                 = di['classifier']              
            self._post_processor             = di['post_proc']               
            self._classification_treshold    = di['classification_treshold'] 
        return

    def save(self, filename):
        with open(filename, 'w') as fid:
            di = dict()
            di['pyramid_builder']           = self.pyramid_builder
            di['image_scanner']             = self.image_scanner
            di['feature_extractor']         = self.feature_extractor
            di['classifier']                = self.classifier
            di['post_proc']                 = self.post_processor
            di['classification_treshold']   = self.classification_treshold
            di['author']                    = r"alin.draghia@gmail.com"
            di['comment']                   = r"N/A"
            pickle.dump(di, fid)
        return



    @property
    def pyramid_builder(self):
        return self._pyramid_builder

    @property
    def image_scanner(self):
        return self._image_scanner

    @property
    def feature_extractor(self):
        return self._feature_extractor

    @property
    def classifier(self):
        return self._classifier

    @property
    def post_processor(self):
        return self._post_processor

    @property
    def classification_treshold(self):
        return self._classification_treshold

    @classification_treshold.setter
    def classification_treshold(self, val):
        self._classification_treshold = val

    @property
    def scale_factor(self):
        return self.pyramid_builder.scale_factor

    @scale_factor.setter
    def scale_factor(self, val):
        self.pyramid_builder.scale_factor = val 

    @property
    def win_step(self):
        return self.image_scanner.win_step

    @win_step.setter
    def win_step(self, val):
        self.image_scanner.win_step = val

    @property
    def padding(self):
        return self.image_scanner.padding

    @padding.setter
    def padding(self, val):
        self.image_scanner.padding = val

    @property
    def group_treshold(self):
        return self.post_processor.GroupThreshold

    @group_treshold.setter
    def group_treshold(self, val):
        self.post_processor.GroupThreshold = val

    @property
    def group_eps(self):
        return self.post_processor.eps

    @group_eps.setter
    def group_eps(self, val):
        self.post_processor.eps = val


