#!/opt/anacoda/bin/python

import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from scipy import ndimage, misc
from skimage import transform
from skimage import io
import argparse


parser = argparse.ArgumentParser(description='Convert RGB images to grayscale of fixed size and create reference output.')
parser.add_argument('-s','--src-dir',dest='src_dir', required=True,
                    help='directory containing images')
parser.add_argument('-g','--gs-dir',dest='grayscale_dir', required=True,
                    help='destination for grayscale images')
parser.add_argument('-e','--edge-dir',dest='edge_dir', required=True,
                    help='destination for filtered imgages')
parser.add_argument('-d','--dims',dest='src_dir', type=int, nargs='2',
                    help='dimensions of the result images')
def main():
    args = parser.parse_args()


    gs = []
    eg = []
    fmt = 'B'*gs[0].size
    for im in glob(srcdir):
        im = transform.resize(im, (128,128))
        im *= (255/im.max())
        gsim = struct.pack(fmt, im.astype(np.uint8).reshape(-1))
        gs.append(gsim)
        with open(filename, 'w+b') as f:
            f.write(gsim)
        im = np.hypot(ndimage.sobel(im,0), ndimage.sobel(im,1))
        egim = struct.pack(fmt, im.astype(np.uint8).reshape(-1))
        eg.append(egim)
        with open(filename, 'w+b') as f:
            f.write(egim)

    #matplotlib.imamge.imsave(args.grayscaledir+name, image)

if __name__ == "__main__":
    main()
