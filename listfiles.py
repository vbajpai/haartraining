#!/usr/bin/python -tt

import sys
import os

def main():
  dir = sys.argv[1]
  f = open('negativeImage.txt', 'w')
  
  dirList = os.listdir(dir)
  for filename in dirList:
    path = os.path.join(dir, filename)
    abspath = os.path.abspath(path)
    f.write(abspath+'\n')
  f.close()
  
    
# This is the standard boilerplate that calls the main() function.
if __name__ == '__main__':
  main()
