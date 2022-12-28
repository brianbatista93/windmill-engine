#!/usr/bin/env python

import os
import re
import glob
import subprocess

header_filter = r'(?:Engine(\/|\\)Source(\/|\\)(Modules|Tools)).*(hpp|cpp)$'

def main():
    headers = [f for f in glob.iglob('./**/*', recursive=True) if re.search(header_filter, f)]
    source_files = []
    for filename in headers:
        source_files.append(os.path.abspath(filename).replace('\\', '/'))

    subprocess.call(['clang-tidy', '--export-fixes=clang-fixes.yml', '--fix-errors'] + source_files)        
    
if __name__ == '__main__':
    main()
