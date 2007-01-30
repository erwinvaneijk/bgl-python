#!/usr/bin/env python
import sys
import re
import os
import os.path as path
import shutil
import glob

def usage():
    print """Usage: setup.py [options] install

Options can contain:

  --home=DIR          This option will install the BGL-Python module
                      into the home directory DIR, with the Python
                      module stored in DIR/lib/python and supporting
                      shared libraries stored in DIR/lib. Be sure that
                      DIR/lib/python is in your PYTHONPATH and DIR/lib
                      is in your dynamic linker's path (e.g.,
                      LD_LIBRARY_PATH or DYLD_LIBRARY_PATH).

  --exec-prefix=DIR   This option will change the executable prefix used
                      to install BGL-Python as a site-wide
                      package. The BGL-Python module will be placed in
                      DIR/lib/pythonVERSION/site-packages and the
                      supporting libraries will be placed in DIR/lib.

  --pythonlibdir=DIR  This option will force the installer to place
                      the BGL-Python module in DIR.

  --libdir=DIR        This option will force the installer to place
                      the BGL-Python supporting libraries in DIR.

  --help              Display this message
"""
    pass

def maybe_create_directory(dir):
    if not path.isdir(dir):
        try:
            print "Creating directory " + dir + "...",
            os.makedirs(dir)
            print " done."
        except OSError, (errno, strerror):
            print strerror
            
        
home = None
exec_prefix = None
libdir = None
pythonlibdir = None
found_install = False
key_value_regex = re.compile("--([^=]*)=(.*)")
for arg in sys.argv[1:]:
    if arg == 'install':
        found_install = True
    elif arg == "--help":
        usage()
        sys.exit(0)
    else:
        m = key_value_regex.match(arg);
        if m:
            key = m.group(1)
            value = m.group(2)

            if key == 'home': home = value
            elif key == 'exec-prefix': exec_prefix = value
            elif key == 'libdir': libdir = value
            elif key == 'pythonlibdir': pythonlibdir = value
            else:
                print "command line error: unknown option --" + key + "=" + value
                usage()
                sys.exit(-1)
        else:
            print "command line error: unknown option " + arg
            usage()
            sys.exit(-1)

if home:
    if exec_prefix:
        print """error: cannot specify both --home and --exec-prefix
  --home is for home-directory installations
  --exec-prefix is for system-wide installations
  """
        sys.exit(-1)

    if not libdir: libdir = home + '/lib'
    if not pythonlibdir: pythonlibdir = home + '/lib/python'
else:
    if not exec_prefix: exec_prefix = sys.exec_prefix
    if not pythonlibdir:
        version = str(sys.version_info[0]) + '.' + str(sys.version_info[1])
        pythonlibdir = exec_prefix + '/lib/python' + version + '/site-packages'
    if not libdir:
        if os.uname()[0] == 'Darwin': libdir = '/usr/local/lib'
        else: libdir = exec_prefix + '/lib'

if os.environ.has_key('HOME'):
    libdir = libdir.replace('~', os.environ['HOME'])
    pythonlibdir = pythonlibdir.replace('~', os.environ['HOME'])
    
print "Installing BGL Python support libraries in " + libdir + "..."
print "Installing BGL Python module in " + pythonlibdir + "..."

maybe_create_directory(libdir)
maybe_create_directory(pythonlibdir)
maybe_create_directory(pythonlibdir + '/boost')
maybe_create_directory(pythonlibdir + '/boost/graph')

files = glob.glob('bin/*')
for myfile in files:
    if path.isfile(myfile):
        myfile = myfile.replace('bin/', '')
        print "Installing " + myfile + " in " + libdir + "...",
        try:
            shutil.copyfile('bin/' + myfile, libdir + '/' + myfile)
            print " done."
        except IOError, (errno, strerror):
            print strerror
            
files = glob.glob('boost/graph/*') + glob.glob('boost/*')
for myfile in files:
    if path.isfile(myfile):
        print "Installing " + myfile + " in " + pythonlibdir + "...",
        try:
            shutil.copyfile(myfile, pythonlibdir + '/' + myfile)
            print " done."
        except IOError, (errno, strerror):
            print strerror
        
