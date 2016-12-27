""" 
    PPM file reader     (for CS410 Assignment grading)
    - read ppm files
    - compare two ppm files and count the number of pixels that do not match

            by lemin (Jake Lee, lemin@cs.colostate.edu)

    Usage:
    ======
        ppm1 = PPM("file1.ppm")
        ppm2 = PPM("file2.ppm")
        print (ppm1.compare(ppm2))

        ### this will return the number of different pixels

"""
from __future__ import print_function
import sys

if sys.version_info < (3,0):
    try:
        # python 2/3 compat
        from itertools import imap as map
        from itertools import izip as zip
    except ImportError:
        pass

class PPM():
    
    def __init__(self, filen=None):
        self._verbose = False #True
        self._width = 0
        self._height = 0
        self._maxcol = 0
        self._img = {}
        if filen is not None:
            self.read(filen)

    def read(self, filen):
        with open(filen) as f:
            line = f.readline()
            if not 'P3' in line.strip().upper():
                raise Exception("wrong file type: not a ppm file!")

            nums = f.readline().strip().split()

            while len(nums) < 3:
                lstline = f.readline().strip().split()
                if len(lstline) == 0:
                    break
                nums.extend(lstline)

            if len(nums) != 3:
                raise Exception("wrong file format!")

            self._width = int(nums[0])
            self._height = int(nums[1])
            self._maxcol = int(nums[2])

            # indices for matrix
            i = 0
            j = 0
            for line in f:
                it = map(int, line.strip().split())
                for v in zip(it, it, it):
                    if v != (239, 239, 239):
                        #print(i, j, v)
                        self._img[(i,j)] = v
                    j += 1
                    if j == self.width:
                        i += 1
                        j = 0

            if self._verbose:
                print("================================================")
                print("PPM file: ", filen)
                print("  width:  ", self._width, " (", i, " rows are read)")
                print("  height: ", self._height)
                print("  maxcols:", self._maxcol)
                print("================================================")



            if i > self._height:
                raise Exception("not matching image pixels!")
    
    """
        compare two ppm objects
      
        Parameters
        ===========
        other   PPM object
                other object to compare
     
        return
        ======
        int     the number of different pixels
     
    """
    def compare(self, other, precision=1):
        if self._width != other.width:
            return self._width * self._height
        if self._height != other.height:
            return self._width * self._height
        if len(self._img) != len(other.image):
            return self._width * self._height

        count = 0
        first = True
        for this, othr in zip(sorted(self._img.iteritems()), sorted(other.image.iteritems())):
            thisKey, thisVal = this
            othrKey, othrVal = othr

            if first:
                first = False
                #print (thisKey, othrKey)
                # compute offset
                if thisKey != othrKey:
                    print("offset: ", thisKey[0]-othrKey[0], thisKey[1]-othrKey[1])

            if thisVal != othrVal:
                #print("different")
                if abs(thisVal[0] - othrVal[0]) > precision or\
                    abs(thisVal[1] - othrVal[1]) > precision or\
                    abs(thisVal[2] - othrVal[2]) > precision: 
                    count += 1

        return count

    @property
    def image(self):
        return self._img

    @property
    def height(self):
        return self._height

    @property
    def width(self):
        return self._width

if __name__ == '__main__':
    ppm = PPM("ellelltri01.ppm")
    ppm1 = PPM("out.ppm")

    print(ppm.compare(ppm1))
