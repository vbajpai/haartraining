#!/usr/bin/python -tt

import sys
import os

def main():
  # Get the name from the command line, using 'World' as a fallback.
  path = sys.argv[1]
  f = open('negativeImage.txt', 'w')
  
  dirList = os.listdir(path)
  for filename in dirList:
    fullpath = path + '/' + filename + '\n'
    f.write(fullpath)
  f.close()
  
    
# This is the standard boilerplate that calls the main() function.
if __name__ == '__main__':
  main()
