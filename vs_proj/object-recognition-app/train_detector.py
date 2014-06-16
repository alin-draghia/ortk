from __future__ import division, print_function

import sys
import os
import numpy as np
from math import floor
import pickle


os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')



from object_recognition_toolkit import *
from my_hog import *
from my_svm import *


class MyTrainingCallback(BootstrappingDetectorTrainerCallback):
    def __init__(self, num_iterations, num_positives, num_negatives, data_dir):
        BootstrappingDetectorTrainerCallback.__init__(self)
        self.num_iterations = num_iterations
        self.num_positives = num_positives
        self.num_negatives = num_negatives
        self.data_dir = data_dir
        self.current_iteration = 0;
        return

    def OnBeginIteration(self, iteration):
        self.current_iteration = iteration + 1;
        print('Begin iteration {}/{}'.format(self.current_iteration, self.num_iterations))
        return

    def OnClassifier(self, classifier):
        cls_fn = os.path.join(self.data_dir, 'classifier_{}.pkl'.format(self.current_iteration))
        with open(cls_fn, mode='w') as f:
            pickle.dump(classifier, f)
        print('Got classifier -> {}'.format(cls_fn))
        return

    def OnEndIteration(self, iteration):
        print('End iteration {}/{}'.format(self.current_iteration, self.num_iterations))
        return 

    def OnPositiveSample(self, count, image, features_vector):      
        msg = 'collecting positive samples {}/{}'.format(count, self.num_positives)
        sys.stdout.write('\r' + msg)
        sys.stdout.flush()
        return

    def OnNegativeSample(self, count, image, features_vector, score):        
        msg = 'collecting negatve samples {}/{}'.format(count, self.num_negatives)
        sys.stdout.write('\r' + msg)
        sys.stdout.flush()
        return 

def main():

    pid = os.getpid()
    print('PID={}'.format(pid))
    
    raw_input('Press any key to begin training...')

    num_iterations = 5
    num_positives = 2500
    num_negatives = 2500
    data_dir = './training_1'

    if os.path.exists(data_dir) == False:
        os.makedirs(data_dir)

    pyramid_builder = FloatPyramidBuilder(scale_factor=1.2, min_size=Size(), max_size=Size())
    image_scanner = DenseImageScanner(win_size=Size(64,128), win_step=Size(8,8), padding=Size())
    feature_extractor = HogFeatureExtractor()
    trainer = LinearSVM_Trainer(C=1.0)
    nms = GroupRectanglesNms()
    callback = MyTrainingCallback(num_iterations, num_positives, num_negatives, data_dir)

    with open(os.path.join(data_dir, 'feature_extracotr.pkl'), 'w') as f:
        pickle.dump(feature_extractor, f)

    detector_trainer = BootstrappingDetectorTrainer()

    detector_trainer.num_iterations=num_iterations
    detector_trainer.num_positives=num_positives
    detector_trainer.num_negatives=num_negatives
    detector_trainer.detector_size=Size(64,128)
    detector_trainer.data_directory=data_dir
    detector_trainer.pyramid_builder=pyramid_builder
    detector_trainer.image_scanner=image_scanner
    detector_trainer.feature_extractor=feature_extractor
    detector_trainer.trainer=trainer
    detector_trainer.non_max_supperssor=nms
    detector_trainer.callback = callback


    positive_dataset = Dataset()
    negative_dataset = Dataset()

    LoadDatasetDlib("positive_train_dataset_crop.xml", positive_dataset)
    LoadDatasetDlib("negative_train_dataset.xml", negative_dataset)


    detector = detector_trainer.TrainWithDataset(positive_dataset, negative_dataset)

if __name__ == '__main__':
    main()



