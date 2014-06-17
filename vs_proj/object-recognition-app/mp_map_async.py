import os
import time
from multiprocessing import Pool


def work(i):
    time.sleep(0.1)
    return (i, i*2, os.getpid())

def cb(r):
    print 'cb pid:', os.getpid(), r
    return

def main():

    li = [1, 2, 3, 4, 5, 6, 7, 8]

    print 'main pid:', os.getpid()
    
    p = Pool(processes=3)

    for i in li:
        p.apply_async(work, (i,), callback=cb)
    p.close()
    p.join()

    return

if __name__ == '__main__':
    main()