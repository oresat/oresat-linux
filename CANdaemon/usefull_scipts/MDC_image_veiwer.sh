#! /bin/bash

firstImage=0

a=1
while [[ $a -eq 1 ]]; do
    fileAvalible=`./../src/app_star_tracker/canopencomm 0x12 r 0x3002 3 u8`
    fileAvalible="${fileAvalible:4}"
    
    b=`echo $fileAvalible | grep "ERROR"`
    
    if [[ -z $b ]]; then
        fileName=`./../src/app_star_tracker/canopencomm 0x12 r 0x3002 1 d`
        fileName="${fileName:3}" # remove starting 3 chars
        fileName="${fileName::-2}" # remove last 2 chars
        fileName=`echo $fileName | xxd -r -p` # conv2ert hex string into ascii string
        
        # check for image file types
        if [[ *".bmp" == $fileName ]] || [[ *".jpg" == $fileName ]] || [[ *".png" == $fileName ]]; then 

            fileData=`./../src/app_star_tracker/canopencomm 0x12 r 0x3002 2 d`
            fileData="${fileData:3}" # remove starting 3 chars
            fileData="\x"`echo $fileData | sed 's/ /\\\x/g'` # remove all spaces

            echo -e $fileData > lastest_image # save to file

            # start feh on the first image
            if [[ $firstImage -ne 1 ]]; then
                feh latest_image -R 1 &
                firstImage=1
            fi
        fi
    fi
    
    sleep 60 
done
