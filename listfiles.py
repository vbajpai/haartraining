#!/usr/bin/env python -tt
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
import os
import getopt

def write_dirlist_tosock(dir, sock, verbose=False):
  dirList = os.listdir(dir)
  for filename in dirList:
    path = os.path.join(dir, filename)
    abspath = os.path.abspath(path)
    if verbose: print '%s ... done' %(abspath)
    sock.write(abspath+'\n')

def usage():
  print __doc__

def main(args):
  verbose = False
  srcdir = filename = None
  try:
    opts, args = getopt.getopt(args,
                               'hvd:f:', ['help', 'verbose', 'dir=', 'file='])
  except getopt.GetoptError:
    usage()
    sys.exit(2)
  for opt, arg in opts:
    if opt in ('-h', '--help'):
      usage()
      sys.exit(0)
    if opt in ('-v', '--verbose'):
      verbose = True
    elif opt in ('-d', '--dir'):
      srcdir = arg
    elif opt in ('-f', '--file'):
      filename = arg
  if srcdir and filename:
    fsock = open(filename, 'w')
    write_dirlist_tosock(srcdir, fsock, verbose)
    fsock.close()
  else:
    usage()
    sys.exit(1)

if __name__ == '__main__':
  main(sys.argv[1:])
