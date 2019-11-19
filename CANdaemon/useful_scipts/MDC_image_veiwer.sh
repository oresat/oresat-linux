#! /bin/bash

# NOTE: the temp variable is only for removing stdout

firstImage=0
outputImageName="latest_image"

a=1
while [[ $a -eq 1 ]]; do
    temp=`./../CANopenComm/canopencomm 0x10 r 0x3100 0 u8` # take an image
    
    sleep 5
    
    temp=`./../CANopenComm/canopencomm 0x10 r 0x3003 8 u8` # refresh file list

    fileAvalible=`./../CANopenComm/canopencomm 0x10 r 0x3003 6 u32`
    fileAvalible="${fileAvalible:6}"  #remove data length and 0x from output
    echo $fileAvalible
    fileAvalible=`echo $fileAvalible | tr [a-z] [A-Z]` # captize hex values
    echo $fileAvalible

    fileAvalible=`echo -e $fileAvalible | tr -d '[:space:]'`
    echo $fileAvalible

    fileAvalible=`echo "obase=10; ibase=16; $fileAvalible" | bc` # hex to dec
    echo $fileAvalible

    while [ $fileAvalible -gt 00000000 ]; do
        fileName=`./../CANopenComm/canopencomm 0x10 r 0x3003 2 d`
        fileName="${fileName:3}" # remove starting 3 chars
        fileName="${fileName::-2}" # remove last 2 chars
        fileName=`echo $fileName | xxd -r -p` # convert hex string into ascii string
        
        # check for image file types
        if [[ $fileName =~ .*".bmp" ]] || [[ $fileName =~ .*".jpg" ]] || [[ $fileName =~ .*".jpeg" ]]; then 

            fileData=`./../CANopenComm/canopencomm 0x10 r 0x3003 3 d`
            
            outputImageName="/tmp/output.bin"
            
            eval `convert $outputImageName -fill white -pointsize 50 -annotate +50+50 "$fileName $(date)" $outputImageName`

            # start feh on the first image
            if [[ $firstImage -ne 1 ]]; then
                feh $outputImageName --scale-down --auto-zoom -R 1 &
                firstImage=1
            fi
        fi
        
        temp=`./../CANopenComm/canopencomm 0x10 r 0x3003 5 u8` # delete image on linux board

        fileAvalible=`./../CANopenComm/canopencomm 0x10 r 0x3003 6 u32`
        fileAvalible="${fileAvalible:4}"
    done 

    sleep 300 
done
