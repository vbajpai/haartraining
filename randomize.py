#!/usr/bin/env python
"""
Saves a dir listing in a file
Usage: python listfiles.py -d dir -f filename [flags]
Arguments:
  -d, --dir               dir; ls of which will be saved in a file
  -f, --file              filename (if existing will be overwritten)
Flags:
  -h, --help              show this help 
  -v, --verbose           be verbose
"""
import sys
import random
import os

def main():
  
  # read the directory list into a list
  if len(sys.argv) != 2:
    print 'usage: randomize dirpath'
    sys.exit(1)

  path = sys.argv[1]
  dirList = os.listdir(path)
  lenoflist = len(dirList)  

  i = 0
  while lenoflist:
    
    # choose a random file from list and echo its name
    oldfile = random.choice(dirList)
    print 'old filename:' + oldfile
    
    # rename this file and echo its new name
    canonical_name, ext = os.path.splitext(oldfile)
    canonical_name+='-' + str(i)
    newfile = canonical_name + ext
    print 'new filename:' + newfile

    # get absolute paths for both files and rename the old with new name
    oldfile_abspath = os.path.abspath(os.path.join(path, oldfile))
    newfile_abspath = os.path.abspath(os.path.join(path, newfile))
    os.rename(oldfile_abspath, newfile_abspath)  
    
    # remove the renamed file from the list NOT to be randomly chosen again
    del dirList[dirList.index(oldfile)]
    lenoflist -= 1
    i += 1

if __name__ == '__main__':
  main()