"""
The updater file contains functions for installing updates for OreSat from
a .tar file. The primary function can be called using the following format. 

python3 updater.py updatefile.tar

The name of the updatefile.tar does not matter as long as it is a .tar file.
The updatefile.tar should contain one or more .deb files. This is the only
file type that will be installed by this function.

The function will have an associated, permament directory labled: old_updates. 
Once a .deb file has been installed, it will be moved to this directory to be 
saved. The .tar file and the tar directory will also be moved to this directory
once the process has been completed. 
"""



import os, sys, subprocess
ARGS = 3 #how many args are expected

def file_read(txt_file, tar_file, computer):
    
    #this is just for testing, delete this condition and only use the else
    if(tar_file == "0"):
        file_object = open(txt_file, "r")
    else:
        tar_dir = untar(tar_file)
        #text file passed in with the instructions
        file_object = open(tar_file + "/" + txt_file, "r")
        #file_contents is a list of strings, each a line from txt file
    file_contents = file_object.readlines()
    computer_name = computer 
    instruction = "-remove-pkgs"
    read_list(computer_name, tar_file, file_contents, instruction)
    computer_name = computer 
    instruction = "-install-pkgs"
    read_list(computer_name, tar_file, file_contents, instruction)
    
    prepare_tar(tar_file, computer)
    

#function takes a .tar file as an argument
def untar(file_name):
    if (file_name.endswith(".tar")): #verfies file is a .tar file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xvf " + file_name
        output = subprocess.check_call(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        return file_name[0:len(file_name)-4]

    elif (file_name.endswith(".tar.gz")): #verfies file is a .tar.gz file
        #contents of .tar file are extracted to a directory with the same
        #name as the tar file file without the .tar extension
        bashCommand = "tar -xzf " + file_name
        output = subprocess.check_call(['bash','-c', bashCommand])
        #create a str with the directory name by slicing off the .tar extension
        return file_name[0:len(file_name)-7]

    else: #if the files passed is not a .tar file error
        print("Error: " + file_name + " is not a tar file")
        return 0

#maybe if this is quite enough I can just stuff it in the result file
#also try the python apt module. it might solve this, 
#https://stackoverflow.com/questions/17537390/how-to-install-a-package-using-the-python-apt-api
def install(file_path):
    if(file_path.endswith("deb")): #TODO test this stuff below
       bashCommand = "sudo apt-get -qq install ./" + file_path
       output = subprocess.check_call(['bash','-c', bashCommand])
       #cli output for each install commans is in output
       #TODO add error checking
       return output
    else:
       return 1 #output will be 0 if it completes install, anything else fails
	

def remove(file_path): #TODO test the -q, test -qq
    #make sure the file contains the package name, not a deb
    bashCommand = "sudo apt-get -qq remove ./"+ file_path
    output = subprocess.check_call(['bash','-c', bashCommand])
    print("remove")
    return output

def read_list(computer_name, file_path, file_contents, instruction):
    list_len = len(file_contents)
    section_start = 0
    i = 0
    for i in range(list_len):
        if(file_contents[i].startswith("["+computer_name+instruction+"]")):
            section_start = i
            break
    i = section_start + 1
    while file_contents[i].startswith("[") != True: #TODO test this shit
        if(instruction == "-remove-pkgs"):
            if(remove(file_path + "/" + file_contents[i]) == 0):
                file_contents += "     completed"
            else:
                file_contents += "     failed"
        else:
            if(install(file_path + "/" + file_contents[i]) == 0):
                file_contents += "     completed"
            else:
                file_contents += "     failed"
        i+=1
    if(i == section_start + 1):
        return 0
    else:
        return 1
        


def prepare_tar(file_path, computer):
    bashCommand = "tar -zcvf " + computer + "-completed.tar.gz ./" + file_path
    output = subprocess.check_call(['bash','-c', bashCommand])


def main():
    if(len(sys.argv) == ARGS+1):
        file_read(sys.argv[1], sys.argv[2], sys.argv[3])
    else:
        print("Error: updater.py function called with incorrect # of args")


if __name__=="__main__":
    main()

