from __future__ import division, print_function

from multiprocessing import Pool
from functools import partial

import sys
import os
import numpy as np
import skimage.feature

from math import floor

try:
    import cPickle as pickle
except:
    import pickle

os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')


import object_recognition_toolkit as objrec_tk


def _compute_multi(extractor, image):
    return extractor.Compute(image)

class HogExtractor(objrec_tk.FeatureExtractor):

    def __init__(self, win_size=(64,128), num_orientations=9, pixels_per_cell=(8,8), cells_per_block=(2,2), normalise=False):
        objrec_tk.FeatureExtractor.__init__(self)

        self.win_size_ = win_size
        self.num_orientations_ = num_orientations
        self.pixels_per_cell_ = pixels_per_cell
        self.cells_per_block_ = cells_per_block
        self.normalise_ = normalise

        sx, sy = self.win_size_
        cx, cy = self.pixels_per_cell_
        bx, by = self.cells_per_block_
        num_cells_x = int(floor(sx//cy))
        num_cells_y = int(floor(sy//cy))
        num_blocks_x = (num_cells_x - bx) + 1
        num_blocks_y = (num_cells_y - by) + 1

        self.lenght_ = num_blocks_x * num_blocks_y * bx * by * self.num_orientations_
        self.visualisation_ = None

        return

    def Clone(self):
        return pickle.loads(pickle.dumps(self))

    def Lenght(self):
        return self.lenght_

    def Compute(self, image):
        feats = skimage.feature.hog(image, self.num_orientations_, self.pixels_per_cell_, self.cells_per_block_)
        x = np.ascontiguousarray(feats, dtype=np.float32)
        return x

    def ComputeMulti(self, images, features):
        

        func = partial(_compute_multi, self)
        #p = Pool()
        #features[:] = p.map(func, images)             
        #p.close()
        #p.join()

        features[:] = map(func, images)     
        return 

    def visualisation(self):
        return self.visualisation_

    #implement pickle support
    def __reduce__(self):
        initargs = (self.win_size_, self.num_orientations_, self.pixels_per_cell_, self.cells_per_block_, self.normalise_, )
        state = {'lenght_': self.lenght_, 'visualisation_': self.visualisation_}
        return (HogExtractor, initargs, state, )

    def __setstate__(self, state):
        self.lenght_ = state['lenght_']
        self.visualisation_ = state['visualisation_']
        return

__all__ = ['HogExtractor',]

