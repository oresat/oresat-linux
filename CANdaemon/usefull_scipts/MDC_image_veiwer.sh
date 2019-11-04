#! /bin/bash

firstImage=0
outputImageName="latest_image"

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
        if [[ $fileName =~ .*".bmp" ]] || [[ $fileName =~ .*".jpg" ]] || [[ $fileName =~ .*".jpeg" ]]; then 

            fileData=`./../src/app_star_tracker/canopencomm 0x12 r 0x3002 2 d`
            fileData="${fileData:3}" # remove starting 3 chars
            fileData="\x"`echo $fileData | sed 's/ /\\\x/g'` # remove all spaces

            echo -e $fileData > $outputImageName # save to file
            
            eval `convert $outputImageName -font Noto-Sans-Black  -fill blue -pointsize 10 -annotate +10+10 "$fileName $(date)" $outputImageName`

            # start feh on the first image
            if [[ $firstImage -ne 1 ]]; then
                feh $outputImageName -R 1 &
                firstImage=1
            fi
        fi
    fi
    
    sleep 60 
done
