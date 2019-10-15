import os, sys, subprocess

#run this program on the command line with python3 updater.py tarfile.tar

#function takes a .tar file as an argument
def untar(file_name):
    if (file_name.endswith(".tar")): #verfies file is a .tar file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xvf " + file_name
        output = subprocess.check_output(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        target_dir = file_name[0:len(file_name)-4]
        install(target_dir)

    elif (file_name.endswith(".tar.gz")): #verfies file is a .tar.gz file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xzf " + file_name
        output = subprocess.check_output(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        target_dir = file_name[0:len(file_name)-7]
        install(target_dir)

    else: #if the files passed is not a .tar file error
        print("Error: " + file_name + " is not a tar file")


def install(target_dir):
    #creates a list of all the directory contents
    directory = os.listdir("./" + target_dir)
    i = 0
    #iterates through the directory 
    while i < len(directory):
        #if a deb file, use the bash commans to install it
        if(directory[i].endswith("deb")):
            bashCommand = "sudo apt install ./" + target_dir + "/" + directory[i]
            output = subprocess.check_output(['bash','-c', bashCommand])
            #cli output for each install commans is in output
        i += 1
    bashCommand = "mkdir ./old_updates/" + target_dir
    output = subprocess.check_output(['bash','-c', bashCommand])
    bashCommand = "mv " + target_dir + " old_updates/" + target_dir
    output = subprocess.check_output(['bash','-c', bashCommand])


#passes the function the file that was used to call the script
if(len(sys.argv) == 2):
    untar(sys.argv[1])
else:
    print("Error: updater.py function called with incorrect # of args")

#TODO: create error for if script is called with incorrect args, 
#verify install works
#add support for multiple .tar files to be used at one time
#create a directory for the completed install files
