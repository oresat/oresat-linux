/*
 *
 */


#include "CANopen.h"
#include "CO_driver.h"
#include "app_OD_functions.h"
#include "app_OD_helpers.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


pthread_mutex_t APP_ODF_mtx;
#define APP_LOCK_ODF()            {if(pthread_mutex_lock(&APP_ODF_mtx) != 0) CO_errExit("Mutex lock CO_OD_mtx failed");}
#define APP_UNLOCK_ODF()          {if(pthread_mutex_unlock(&APP_ODF_mtx) != 0) CO_errExit("Mutex unlock CO_OD_mtx failed");}

#ifndef FILE_RECEIVE_FOLDER 
    #define FILE_RECEIVE_FOLDER "/tmp/received_files/"
#endif
#ifndef FILE_SEND_FOLDER
    #define FILE_SEND_FOLDER "/tmp/send_files/"
#endif
#ifndef FILE_TRANSFER_MAX_SIZE
    #define FILE_TRANSFER_MAX_SIZE 6000
#endif
#ifndef FILE_PATH_MAX_LENGTH 
    #define FILE_PATH_MAX_LENGTH 100
#endif


/******************************************************************************/
/* data structures */


/**
 * Object Dictionary storage object for receiving files.
 *
 * Object is used with CANopen OD objects that are for file transfer.
 */
typedef struct {
    char        fileName[FILE_PATH_MAX_LENGTH];
    uint32_t    fileSize;
    int8_t      fileData[FILE_TRANSFER_MAX_SIZE];
    uint8_t     filesAvalible;
} file_buffer_t;


/******************************************************************************/
/* Static Variables */


static file_buffer_t odReceiveFileBuffer;     /* for OD entry 0x3001 */
static file_buffer_t odSendFileBuffer;        /* for OD entry 0x3002 */


/******************************************************************************/


void app_ODF_configure(void){
    /* Add data structs to SDO Object Dictionary */
    CO_OD_configure(CO->SDO[0], 0x3001, CO_ODF_3001, (void*)&odReceiveFileBuffer, 0, 0U);
    CO_OD_configure(CO->SDO[0], 0x3002, CO_ODF_3002, (void*)&odSendFileBuffer, 0, 0U);

    return;
}


/******************************************************************************/
/* Receiveing incoming file, OD entry 3001 */


/**
 * Wrapper function used by CP_ODF_3001 to save file data into SDO buffer from struct. 
 * It can handle spilting large data files into multiple segments.
 * */
static CO_SDO_abortCode_t save_file_data(CO_ODF_arg_t *ODF_arg, file_buffer_t *odFileBuffer) {
    if(ODF_arg == NULL || odFileBuffer == NULL || ODF_arg->data == NULL)
        return CO_SDO_AB_NO_DATA;

    if(ODF_arg->firstSegment == 1) { /* 1st segment only */
        if(ODF_arg->dataLengthTotal > FILE_TRANSFER_MAX_SIZE) {
            return CO_SDO_AB_OUT_OF_MEM; /* file is larger than domain buffer */
        }
        
        odFileBuffer->fileSize = 0; 
        ODF_arg->offset = 0;
    }

    /** 
     * check if there are more segements comming. This will also check if 
     * dataLengthTotal in not set (is 0).
     * */
    if((odFileBuffer->fileSize + ODF_arg->dataLength) >= ODF_arg->dataLengthTotal) {
        ODF_arg->lastSegment = true;
    }

    /* update fileSize */
    odFileBuffer->fileSize += ODF_arg->dataLength;

    /* copy data */
    memcpy(&odFileBuffer->fileData[ODF_arg->offset], ODF_arg->data, ODF_arg->dataLength);

    /* update offset for next call */
    ODF_arg->offset += ODF_arg->dataLength; 

    return CO_SDO_AB_NONE;
}


CO_SDO_abortCode_t CO_ODF_3001(CO_ODF_arg_t *ODF_arg) {
    file_buffer_t *odFileBuffer;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;

    odFileBuffer = (file_buffer_t*) ODF_arg->object;

    /* error, no data to write */
    if(ODF_arg->dataLength == 0)
        return CO_SDO_AB_NO_DATA; 

    APP_LOCK_ODF();

    /* write parameters */
    if(ODF_arg->reading == false) { 
        if(ODF_arg->subIndex == 1) { /* file name */
            /* check if new data will fit in struct */
            if(ODF_arg->dataLength > FILE_PATH_MAX_LENGTH)
                ret = CO_SDO_AB_OUT_OF_MEM; 
            else {
                /* write data */
                memcpy(odFileBuffer->fileName, ODF_arg->data, ODF_arg->dataLength);

                /* no null char */
                if(ODF_arg->data[ODF_arg->dataLength-1] != '\0')
                    ODF_arg->data[ODF_arg->dataLength] = '\0';
            }

            ODF_arg->lastSegment = true;
        }
        else if(ODF_arg->subIndex == 2) { /* file data */
            ret = save_file_data(ODF_arg, odFileBuffer);
        }
        else if(ODF_arg->subIndex == 3) { /* save file */
            /* error, no data to save */
            if(odFileBuffer->fileSize == 0 || odFileBuffer->fileName[0] == '\0') {
                APP_UNLOCK_ODF();
                return CO_SDO_AB_NO_DATA;
            }

            /* save file to recieve folder */
            char filePath[] = FILE_RECEIVE_FOLDER;
            strcat(filePath, odFileBuffer->fileName);
            FILE *f = fopen(filePath, "wb");
            if(f != NULL) {
                fwrite(odFileBuffer->fileData, sizeof(char), odFileBuffer->fileSize, f);
                fclose(f);

                /* clear buffer */
                odFileBuffer->fileSize = 0;
                odFileBuffer->fileName[0] = '\0';
            }
            else
                ret = CO_SDO_AB_GENERAL; 

            ODF_arg->lastSegment = true;
        }
        else 
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }
    else 
        ret = CO_SDO_AB_READONLY; 

    APP_UNLOCK_ODF();

    return ret;
}


/******************************************************************************/
/* Send file, OD entry 3002 */


/**
* Wrapper funciton for CO_ODF_3002. Checks the directory for any files.
*
* @return # of file availibe on sucess, and sets filePath if more than 0
* files are available.
*/
static int32_t find_file(char *directory, char *filePath){
    int32_t totalFiles = 0;
    DIR *d;
    struct dirent *dir;
    int a, b;
    bool fileFound = 0;

    d = opendir(directory);
    if(d != NULL) { /* directory found */
        while((dir = readdir(d)) != NULL) {
            a = strncmp(dir->d_name, ".", sizeof(dir->d_name));
            b = strncmp(dir->d_name, "..", sizeof(dir->d_name));

            if(a != 0 && b != 0) {
                if(fileFound != 1) {
                    /* file found, make path */
                    strncpy(filePath, directory, strlen(directory) + 1);
                    strncat(filePath, dir->d_name, strlen(dir->d_name) + 1);
                    fileFound = 1;
                }
                ++totalFiles;
            }
        }
        
        closedir(d);
    }
    else { /* directory not found */
        totalFiles = 0;
    }

    return totalFiles;
}


/**
* Wrapper funciton for CO_ODF_3002. Remove the path from the file path name.
*
* @return the length of the file name without '\0' or 0 on failure.
*/
static uint32_t get_file_name(const char *filePath, char *fileName) {
    uint32_t pathNameSize, start, fileNameSize = 0;

    if(filePath == NULL || filePath[0] == '\0') {
        return 0; /* error, input(s) invaild */
    }

    start = strlen(filePath);
    pathNameSize = start + 1;
    
    /* find right most '/' */
    while(start > 0) {
        if(filePath[start] == '/') {
            ++start;
            fileNameSize = pathNameSize - start;
            break;
        }
        --start;
    }

    if(fileNameSize <= FILE_PATH_MAX_LENGTH) {
        /* copy only file name */
        strncpy(fileName, &filePath[start], fileNameSize);
        fileName[fileNameSize] = '\0';
    }
    else {
        fileNameSize = 0; /* error, data too big for buffer */
    }

    return fileNameSize;
}


/**
* Wrapper funciton for CO_ODF_3002. Read in file data.
*
* @return file size or 0 on failure.
*/
static uint32_t get_file_data(const char *filePath, int8_t *fileData) {
    uint32_t fileSize = 0;
    FILE *f;

    if(filePath == NULL || filePath[0] == '\0') {
        return 1;
    }

    f = fopen(filePath, "r");
    if(f != NULL) {
        fseek(f, 0, SEEK_END);
        fileSize = ftell(f);

        if(fileSize != 0) {
            fseek(f, 0, 0);

            // read into buffer
            fread(fileData, fileSize, 1, f);
        }

        fclose(f);
    }

    return fileSize;
}


/**
 * Wrapper function used by CP_ODF_3002 to read file data into SDO buffer from struct. 
 * It can handle spilting large data files into multiple segments.
 * */
static CO_SDO_abortCode_t read_file_data(CO_ODF_arg_t *ODF_arg, file_buffer_t *odFileBuffer) {
    if( ODF_arg == NULL || odFileBuffer == NULL)
        return CO_SDO_AB_NO_DATA;

    if(ODF_arg->firstSegment == 1) { /* 1st segment */
        if(odFileBuffer->fileSize > FILE_TRANSFER_MAX_SIZE) {
            return CO_SDO_AB_OUT_OF_MEM; /* file is larger than domain buffer */
        }
        
        ODF_arg->dataLengthTotal = odFileBuffer->fileSize;
        ODF_arg->offset = 0;

        if(odFileBuffer->fileSize <= CO_SDO_BUFFER_SIZE) {
            /* only need 1 segment */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = odFileBuffer->fileSize;
        }
        else { 
            /* multiple segments needed */
            ODF_arg->dataLength = CO_SDO_BUFFER_SIZE;
        }
    }
    else { /* not 1st segment */
        uint32_t next_seg_len = ODF_arg->dataLengthTotal - ODF_arg->offset;
        if(next_seg_len > CO_SDO_BUFFER_SIZE) {
            /* more segments needed */
            ODF_arg->dataLength = CO_SDO_BUFFER_SIZE;
        }
        else {
            /* last segement */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = next_seg_len;
        }
    }

    /* copy data */
    memcpy(ODF_arg->data, &odFileBuffer->fileData[ODF_arg->offset], ODF_arg->dataLength);

    /* update offset for next call */
    ODF_arg->offset += ODF_arg->dataLength; 

    return CO_SDO_AB_NONE;
}


CO_SDO_abortCode_t CO_ODF_3002(CO_ODF_arg_t *ODF_arg) {
    file_buffer_t *odFileBuffer;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    char filePath[FILE_PATH_MAX_LENGTH];

    odFileBuffer = (file_buffer_t*) ODF_arg->object;

    APP_LOCK_ODF();

    if(ODF_arg->reading) { /* read parameters */
        if(ODF_arg->subIndex == 1) { /* filename */
            if(strlen(odFileBuffer->fileName) == 0) { /* error, no data to read */
                APP_UNLOCK_ODF();
                return CO_SDO_AB_NO_DATA; 
            }

            /* check if new data will fit in struct */
            if(strlen(odFileBuffer->fileName) > FILE_TRANSFER_MAX_SIZE)
                ret = CO_SDO_AB_OUT_OF_MEM; 
            else {
                ODF_arg->dataLength = strlen(odFileBuffer->fileName) + 1;
                memcpy(ODF_arg->data, odFileBuffer->fileName, ODF_arg->dataLength);
            }
            ODF_arg->lastSegment = true;
        }
        else if(ODF_arg->subIndex == 2) { /* file data */

            if(odFileBuffer->fileSize == 0) { /* error, no data to read */
                APP_UNLOCK_ODF();
                return CO_SDO_AB_NO_DATA; 
            }
            
            ret = read_file_data(ODF_arg, odFileBuffer);
        }
        else if(ODF_arg->subIndex == 3) { /* load file from folder */
            /* get file path if a file is in the send folder */
            odFileBuffer->filesAvalible = find_file(FILE_SEND_FOLDER, filePath);
            if(odFileBuffer->filesAvalible != 0) { /* file(s) found */
                /* get the file name */
                if(get_file_name(filePath, odFileBuffer->fileName) == 0) {
                    ret = CO_SDO_AB_GENERAL; /* error with file */
                }

                /* get the file data */
                odFileBuffer->fileSize = get_file_data(filePath, odFileBuffer->fileData);
                if(odFileBuffer->fileSize == 0) {
                    ret = CO_SDO_AB_GENERAL; /* error with file */
                }

                /* remove file after file is sent */
                // if(remove(filePath) != 0)
                //    ret = CO_SDO_AB_GENERAL; /* delete failed */
            }
            else /* no files */
                ret = CO_SDO_AB_NO_DATA;

            ODF_arg->dataLength = sizeof(odFileBuffer->filesAvalible);
            memcpy(ODF_arg->data, &odFileBuffer->filesAvalible, ODF_arg->dataLength);
            ODF_arg->lastSegment = true;
        }
        else if(ODF_arg->subIndex == 4) { /* # of files avalible */
            ODF_arg->dataLength = sizeof(odFileBuffer->filesAvalible);
            memcpy(ODF_arg->data, &odFileBuffer->filesAvalible, ODF_arg->dataLength);
            ODF_arg->lastSegment = true;
        }
        else
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }
    else 
        ret = CO_SDO_AB_WRITEONLY; 

    APP_UNLOCK_ODF();

    return ret;
}


int32_t APP_ODF_3002(const char *filePath) {
    int32_t ret = 0;
    char fileName[FILE_PATH_MAX_LENGTH];
    char newFilePath[FILE_PATH_MAX_LENGTH] = FILE_SEND_FOLDER;
    int source, dest;
    struct stat stat_source;

    get_file_name(filePath, fileName);
    strcat(newFilePath, fileName);

    if(filePath == NULL || filePath[0] == '\0')
        ret = 1;
    else {
        /* copy file into send folder */

        source = open(filePath, O_RDONLY, 0);
        dest = open(newFilePath, O_WRONLY | O_CREAT, 0644);

        fstat(source, &stat_source);
        sendfile(dest, source, 0, stat_source.st_size);

        close(source);
        close(dest);
    }

    return ret; 
}


/******************************************************************************/
/* TBD */
