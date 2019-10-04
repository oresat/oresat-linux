#!/usr/bin/env python3
'''
Depenedenies
pyton3, regex, python-image
'''

import subprocess
import re
import sys
import time
import binascii

def checkForImage():
    ret = False  

    output = callCANopen("./canopencomm 0x12 r 0x3002 3 u8")
    temp = output.decode('ascii')
    temp = temp.rstrip() # remove any trailing \n\r etc
    reg = re.match(r'\[\d\] (.*)', temp)
    images_available = int(reg.group(1), 16) # data. base
    if(images_available > 0):
        print("Image Available")
        ret = True

    return ret


def readFileName():
    output = callCANopen("./canopencomm 0x12 r 0x3002 1 d")

    temp = output.decode('ascii')
    temp = temp.rstrip() # remove any trailing \n\r etc

    reg = re.match(r'\[\d\] (.*)', temp)

    fileNameRaw = reg.group(1)
    fileNameRaw = fileNameRaw[:-2] # remove trailing 00 from '\0'
    fileNameRaw = fileNameRaw.replace(" ","") # remove all spaces
    fileNameRaw = binascii.unhexlify(fileNameRaw) #make binary char string

    fileName = fileNameRaw.decode('ascii') # make string

    return fileName


def readFileData():
    output = callCANopen("./canopencomm 0x12 r 0x3002 2 d")
    temp = output.decode('ascii')
    reg = re.match(r'\[\d\] (.*)', temp)
    fileData = reg.group(1)
    fileData = fileData.replace(" ","") # remove all spaces

    return fileData


def loadFile():
    callCANopen("./canopencomm 0x12 r 0x3002 4 u8")


def callCANopen(command):
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
    output, error = process.communicate()

    if(error is not None):
        print("Error: {}".format(error))
        sys.exit();

    return output

def saveImage(fileName, fileData):
    with open(fileName, 'wb') as f:
        fileData = fileData.strip() # make sure there is no whitespace
        temp = binascii.unhexlify(fileData)
        f.write(temp)

if __name__ == "__main__":
    while True:
        if(checkForImage() is True):
            loadFile()
            fileName = readFileName()
            fileData = readFileData()
            print("file name: {}".format(fileName))
            print("file data: {}".format(fileData))
            saveImage(fileName, fileData)
        time.sleep(60)
