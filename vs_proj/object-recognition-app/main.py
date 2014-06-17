from __future__ import division, print_function

import sys
import os

from PySide import QtUiTools, QtGui

from object_recognition_toolkit import *

def main():

    #main application
    app = QtGui.QApplication(sys.argv)

    loader = QtUiTools.QUiLoader()

    #load the main window file
    main_window = loader.load('main_window.ui')
    main_window.show()
    sys.exit(app.exec_())
    return

if __name__ == '__main__':
    main()
