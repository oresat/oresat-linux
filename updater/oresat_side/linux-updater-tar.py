#!/usr/bin/env python3


import subprocess


def prepare_tar(output_file, files_to_tar):
    bashCommand = "tar -zcvf " + WORKING_DIR + self.computer_name + "-completed.tar.gz " + WORKING_DIR + file_name
    output = subprocess.check_call(['bash','-c', bashCommand])
    return output


def untar(self, file_name):
    """
    contents of .tar.gz file are extracted to a directory with the same
    name as the tar file file without the .tar.gz extension
    """
    
    bashCommand = "tar -xzf " + file_name  " -C " + WORKING_DIR
    output = subprocess.check_call(['bash','-c', bashCommand])

    #create a str with the directory name by slicing off the .tar.gz extension
    return file_name[0:len(file_name)-7]

