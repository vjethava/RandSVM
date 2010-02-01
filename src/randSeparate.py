#!/usr/bin/python
# File that randomly separates dataset into training and test file
import sys
from optparse import OptionParser
from random import randrange
def main():
    parser = OptionParser()
    usage = "usage: %prog [options] arg1 arg2"
    parser = OptionParser(usage=usage)
    parser.add_option("-i", "--input_file", action="store",
    type="string", dest="input_file", metavar="FILE");
    parser.add_option("-d", "--training_file", action="store",
                      type="string", dest="training_file",
                      metavar="FILE", default="training_file.dat"
                      );
    parser.add_option("-t", "--test_file", action="store",
                      type="string", dest="test_file", metavar="FILE",
                      default="test_file.dat");
    parser.add_option("-k", "--test_percent", action="store",
                      type="int", dest="k", metavar="NUM(0..100)", default=10);
    parser.add_option("-v", action="store_true", dest="verbose", default=False)
    (options, args) = parser.parse_args()
    if(options.verbose == True):
        print options
    if(options.input_file == None):
        print "Please give input file"
        sys.exit()
    fin = open(options.input_file, "r")
    fdata = open(str(options.training_file), "w")
    ftest = open(str(options.test_file), "w")
    counter = 0
    select = 0
    factor = 100/options.k
    for line in fin:
        if(counter == 0):
            select = randrange(0, factor)
        if (counter == select):
            ftest.write(line)
        else:
            fdata.write(line)
        counter = (counter + 1)%factor
    fin.close()
    fdata.close()
    ftest.close()
            
if __name__== "__main__":
    main() 





