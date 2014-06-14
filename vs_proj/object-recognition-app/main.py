from __future__ import division, print_function

import sys
import os
import numpy as np
import cv2

try:
    import cPickle as pickle
except:
    import pickle

os.environ['PATH'] += (';../3rd-party/opencv/x64/vc12/bin')
os.environ['PATH'] += (';../packages/boost_serialization-vc120.1.55.0.16/lib/native/address-model-64/lib')

debug_toolkit = True

if debug_toolkit:
    import object_recognition_toolkitd as objrec_tk
else:
    import object_recognition_toolkit as objrec_tk

def main():
    """ the application entry point """
    return

if __name__ == '__main__':
    main()
else:
    pass