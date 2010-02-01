#!/usr/bin/python
# File that randomly separates dataset into training and test file
import sys
def main(argv):
    try:
        opts, args = getopt.getopt(argv[1:], "hid:t:k",["help","input","data","test"])
    except: getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(2)
    output = None
    verbose = False 
    data = "training_file.dat"
    test = "testing_file.dat"
    for o, a in opts:
        if o == "-v":
            verbose=True
        elif o in ("-h", "--help"):
            usage()
            sys.exit()
        elif o in ("-d", "--data"):
            
        
