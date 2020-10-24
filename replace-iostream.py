import glob
import os

files = glob.glob(os.getcwd() + '/**/*.cpp', recursive=True)
for file in files:
    r = open(file, 'r')
    contents = r.read()
    if file.endswith('.h'):
        continue
    contents = contents.replace('<iostream>', '"asm-lib/asmstream.h"')
    contents = contents.replace('<iostream.h>', '"asm-lib/asmstream.h"')
    contents = contents.replace('cout', 'asmcout');
    contents = contents.replace('cin', 'asmcin');
    contents = contents.replace('cerr', 'asmcerr');
    r.close()

    w = open(file, 'w')
    w.write(contents)
    w.close()
