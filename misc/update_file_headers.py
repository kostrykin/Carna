#!/usr/bin/env python3

import argparse
import pathlib

parser = argparse.ArgumentParser()
parser.add_argument('filepaths', type=str, nargs='+')
args = parser.parse_args()

with open(pathlib.Path(__file__).parent / 'file_header.txt', 'r') as file_header_file:
    file_header = file_header_file.read()

for filepath in args.filepaths:
    with open(filepath, 'r+') as file:
        content = file.read()

        # Remove existing header if present
        idx0 = content.find('/*')
        if 0 <= idx0 <= 3:
            idx1 = content.find('*/') + 2
            prefix = content[:idx0]
            content = content[idx1:]
        else:
            prefix = ''
        
        # Add new header
        content = prefix + file_header + content.strip('\n') + '\n'
        file.seek(0)
        file.write(content)
