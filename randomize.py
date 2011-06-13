#!/usr/bin/python -tt

import sys
import random
import os

def main():
  
  # read the directory list into a list
  path = sys.argv[1]
  dirList = os.listdir(path)
  lenoflist = len(dirList)
  
  
  i = 0
  while i < lenoflist:
    
    dirList = []
    dirList = os.listdir(path)
    
    # choose a random file
    file = random.choice(dirList)
    oldpath = path + file
    print 'old path:' + oldpath
    
    # save this file with new file name    
    newpath = path + "renames/" + str(i) + '.png'
    print 'new path:' + newpath
    os.rename(oldpath, newpath)  
    
    
    i = i + 1  
  
  
# This is the standard boilerplate that calls the main() function.
if __name__ == '__main__':
  main()
