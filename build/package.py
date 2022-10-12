#! /usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
import os
import sys
import string
import _winreg
import copy
import tarfile

def _tar_gz(source_dir, tar_file_path, arcname):
    tar_file = tarfile.open(tar_file_path, "w:gz")
    tar_file.add(source_dir, arcname=arcname)
    tar_file.close()

def main():	
	if (len(sys.argv) <= 3):
		print('no vcproject path input')
		return;
   
	source_dir = sys.argv[1]
	print('source_dir=%s' % source_dir)

	tar_file_path = sys.argv[2]
	print('tar_file_path=%s' % tar_file_path)

	arcname = sys.argv[3]
	print('arcname=%s' % arcname)

	_tar_gz(source_dir, tar_file_path, arcname)

if __name__ == '__main__':
    main() 