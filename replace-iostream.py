import glob
import os

files = glob.glob(os.getcwd() + '/**/*.cpp', recursive=True)
for file in files:
    r = open(file, 'r')
    contents = r.read()
    contents = contents.replace('<iostream>', '"asm-lib/asmstream.h"')
    contents = contents.replace('<iostream.h>', '"asm-lib/asmstream.h"')
    r.close()

    w = open(file, 'w')
    w.write(contents)
    w.close()
