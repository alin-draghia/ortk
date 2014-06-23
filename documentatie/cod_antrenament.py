from __future__ import division, print_function

import sys
import os
import numpy as np
from math import floor
import pickle
from object_recognition_toolkit import *
from my_hog import *
from my_svm import *


class MyTrainingCallback(BootstrappingDetectorTrainerCallback):
    def __init__(self, num_iter, num_pos, num_neg, data_dir):
        BootstrappingDetectorTrainerCallback.__init__(self)
        self.num_iterations = num_iter
        self.num_positives = num_pos
        self.num_negatives = num_neg
        self.data_dir = data_dir
        self.current_iteration = 0;
        return

    def OnBeginIteration(self, iteration):
        self.current_iteration = iteration + 1;
        print()
        print('Begin iteration {}/{}'.format(
            self.current_iteration,
            self.num_iterations))
        print()
        return

    def OnClassifier(self, classifier):
        print()
        cls_fn = os.path.join(self.data_dir,
            'classifier_{}.pkl'.format(self.current_iteration))
        with open(cls_fn, mode='w') as f:
            pickle.dump(classifier, f)

        del classifier

        print('Got classifier -> {}'.format(cls_fn))
        print()
        return

    def OnEndIteration(self, iteration):
        print()
        print('End iteration {}/{}'.format(
            self.current_iteration,
            self.num_iterations))
        print()
        return 

    def OnPositiveSample(self, count, image, features_vector):      
        msg = 'collecting positive samples {}/{}'.format(
            count, 
            self.num_positives)
        sys.stdout.write('\r' + msg)
        sys.stdout.flush()
        return

    def OnDoneCollectiongPositiveSamples(self, features):
        print()
        msg = 'done collecting positive samples {}/{}'.format(
            features.shape[0], 
            self.num_positives)
        print(msg)
        fn = os.path.join(self.data_dir, 'positive_samples_{}.npy'.format(
            self.current_iteration))
        np.save(fn, features);
        msg = 'positive samples saved to: '.format(fn)
        print(msg)
        print()
        return

    def OnNegativeSample(self, count, image, features_vector, score):        
        msg = 'collecting negatve samples {}/{}'.format(
            count, 
            self.num_negatives)
        sys.stdout.write('\r' + msg)
        sys.stdout.flush()
        return 

    def OnDoneCollectiongNegativeSamples(self, features):
        print()
        msg = 'done collecting negative samples {}/{}'.format(
            features.shape[0], 
            self.num_negatives)
        print(msg)
        fn = os.path.join(self.data_dir, 'negative_samples_{}.npy'.format(
            self.current_iteration))
        np.save(fn, features);
        msg = 'negative samples saved to: '.format(fn)
        print(msg)
        print()
        return

def main():

    pid = os.getpid()
    print('PID={}'.format(pid))
    
    raw_input('Press any key to begin training...')

    num_iterations = 10
    num_positives = 2500
    num_negatives = 1000
    data_dir = './training_10'

    if os.path.exists(data_dir) == False:
        os.makedirs(data_dir)

    scale_factor = 1.2
    win_size = Size(64,128)
    win_step = Size(8,8)
    block_size = Size(16,16)
    block_stride = win_step
    cell_size = Size(8,8)
    n_bins = 9

    pyramid_builder = FloatPyramidBuilder(
        scale_factor=scale_factor, 
        min_size=win_size, 
        max_size=Size())
    image_scanner = DenseImageScanner(
        win_size=win_size, 
        win_step=win_step, 
        padding=Size())
    feature_extractor = HogFeatureExtractor(
        win_size, 
        block_size, 
        block_stride,
        cell_size,
        n_bins)
    trainer = LinearSVM_Trainer(C=0.1)
    nms = PassThroughNms()
    callback = MyTrainingCallback(num_iterations, 
        num_positives,
         num_negatives, 
         data_dir)

    feature_extracotr_path = os.path.join(data_dir, 'feature_extracotr.pkl')
    with open(feature_extracotr_path, 'w') as f:
        pickle.dump(feature_extractor, f)

    detector_trainer = BootstrappingDetectorTrainer()

    detector_trainer.num_iterations=num_iterations
    detector_trainer.num_positives=num_positives
    detector_trainer.num_negatives=num_negatives
    detector_trainer.detector_size=win_size
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


    detector = detector_trainer.TrainWithDataset(positive_dataset,
        negative_dataset)

if __name__ == '__main__':
    main()