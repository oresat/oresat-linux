import os, sys, subprocess
ARGS = 3 #how many args are expected

def file_read(txt_file, tar_file, computer):
    
    #this is just for testing, delete this condition and only use the else
    if(tar_file == "0"):
        file_object = open(txt_file, "r")
    else:
        tar_dir = untar(tar_file)
        #file_name is the text file passed in with the instructions
        txt_file_path = "./tar_dir/"
        file_object = open(txt_file_path + txt_file, "r")
        #file_contents is a list of strings, each a line from txt file
    file_contents = file_object.readlines()
    
     
    

#function takes a .tar file as an argument
def untar(file_name):
    if (file_name.endswith(".tar")): #verfies file is a .tar file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xvf " + file_name
        output = subprocess.check_output(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        return file_name[0:len(file_name)-4]

    elif (file_name.endswith(".tar.gz")): #verfies file is a .tar.gz file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xzf " + file_name
        output = subprocess.check_output(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        return file_name[0:len(file_name)-7]

    else: #if the files passed is not a .tar file error
        print("Error: " + file_name + " is not a tar file")
        return 0


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



if(len(sys.argv) == ARGS+1):
    file_read(sys.argv[1], sys.argv[2], sys.argv[3])
else:
    print("Error: updater.py function called with incorrect # of args")

