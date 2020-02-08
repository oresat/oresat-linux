#include "CANopen.h"
#include "CO_driver.h"
#include "OD_helpers.h"
#include "log_message.h"
#include "file_transfer_ODF.h"
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>


#define FILE_RECEIVE_FOLDER     "/tmp/received_files/"
#define FILE_SEND_FOLDER        "/tmp/send_files/"
#ifndef CO_SDO_BUFFER_SIZE
#define CO_SDO_BUFFER_SIZE      889 // CO_driver.h should define this
#endif


// ***************************************************************************
// Static Variables


static pthread_mutex_t          FT_ODF_mtx;
static received_file_data_t     recvFileBuffer;
static send_file_data_t         sendFileBuffer;


// ***************************************************************************
// Static functions headers


static int ft_lock_mtx(void);
static int ft_unlock_mtx(void);
static CO_SDO_abortCode_t save_file_data(CO_ODF_arg_t *ODF_arg, received_file_data_t *recvFileBuffer);
static int initFileList(send_file_data_t *sendFileBuffer);
static uint32_t get_file_name(const char *filePath, char *fileName);
static uint32_t get_file_data(const char *filePath, int8_t *fileData);
static CO_SDO_abortCode_t read_file_data(CO_ODF_arg_t *ODF_arg);


// ***************************************************************************
// Mutex functions


static int ft_lock_mtx(void) {
    if(pthread_mutex_lock(&FT_ODF_mtx) != 0) 
        log_message(LOG_ERR, "File transfer mutex lock failed");
    return 0;
}

static int ft_unlock_mtx(void) {
    if(pthread_mutex_unlock(&FT_ODF_mtx) != 0) 
        log_message(LOG_ERR, "File transfer mutex unlock failed");
    return 0;
}


// ***************************************************************************
// Add static data to SDO struct


int file_transfer_ODF_setup(void) {

    // make sure /tmp dir exist
    struct stat st = {0};
    if(stat(FILE_RECEIVE_FOLDER, &st) == -1)
        mkdir(FILE_RECEIVE_FOLDER, 0700);
    if(stat(FILE_SEND_FOLDER, &st) == -1)
        mkdir(FILE_SEND_FOLDER, 0700);

    initFileList(&sendFileBuffer);

    CO_OD_configure(CO->SDO[0], 0x3001, recv_file_ODF, (void*)&recvFileBuffer, 0, 0U);
    CO_OD_configure(CO->SDO[0], 0x3002, send_file_array_ODF, (void*)&sendFileBuffer, 0, 0U);
    CO_OD_configure(CO->SDO[0], 0x3003, send_file_ODF, (void*)&sendFileBuffer, 0, 0U);

    return 0;
}


// ***************************************************************************
// Receiveing incoming file ODF


/**
 * Wrapper function used by recv_file_ODF to save file data into SDO buffer from struct. 
 * It can handle spilting large data files into multiple segments.
 * */
static CO_SDO_abortCode_t save_file_data(CO_ODF_arg_t *ODF_arg, received_file_data_t *recvFileBuffer) {
    if(ODF_arg == NULL || recvFileBuffer == NULL || ODF_arg->data == NULL)
        return CO_SDO_AB_NO_DATA;

    if(ODF_arg->firstSegment == 1) { /* 1st segment only */
        if(ODF_arg->dataLengthTotal > FILE_TRANSFER_MAX_SIZE) {
            return CO_SDO_AB_OUT_OF_MEM; /* file is larger than domain buffer */
        }
        
        recvFileBuffer->fileSize = 0; 
        ODF_arg->offset = 0;
    }

    /** 
     * check if there are more segements comming. This will also check if 
     * dataLengthTotal in not set (is 0).
     * */
    if((recvFileBuffer->fileSize + ODF_arg->dataLength) >= ODF_arg->dataLengthTotal)
        ODF_arg->lastSegment = true;
    else
        ODF_arg->lastSegment = false;

    /* update fileSize */
    recvFileBuffer->fileSize += ODF_arg->dataLength;

    /* copy data */
    memcpy(&recvFileBuffer->fileData[ODF_arg->offset], ODF_arg->data, ODF_arg->dataLength);

    return CO_SDO_AB_NONE;
}


CO_SDO_abortCode_t recv_file_ODF(CO_ODF_arg_t *ODF_arg) {
    received_file_data_t *recvFileBuffer;
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    char filePath[] = FILE_RECEIVE_FOLDER;
    FILE *f;

    recvFileBuffer = (received_file_data_t*) ODF_arg->object;

    /* error, no data to write */
    if(ODF_arg->dataLength == 0)
        return CO_SDO_AB_NO_DATA; 

    if(ODF_arg->reading == false) 
        return CO_SDO_AB_READONLY; /* can't write parameters, read only */

    ft_lock_mtx();

    switch(ODF_arg->subIndex) {
        case 1 : /* file name */
            
            /* check if new data will fit in struct */
            if(ODF_arg->dataLength > FILE_PATH_MAX_LENGTH)
                ret = CO_SDO_AB_OUT_OF_MEM; 
            else {
                /* write data */
                memcpy(recvFileBuffer->fileName, ODF_arg->data, ODF_arg->dataLength);

                /* no null char */
                if(ODF_arg->data[ODF_arg->dataLength-1] != '\0')
                    ODF_arg->data[ODF_arg->dataLength] = '\0';

                ODF_arg->lastSegment = true;
            }
            break;

        case 2 : /* file data */
            
            ret = save_file_data(ODF_arg, recvFileBuffer);
            break;

        case 3 : /* save file */
            
            if(recvFileBuffer->fileSize == 0 || recvFileBuffer->fileName[0] == '\0')
                ret = CO_SDO_AB_NO_DATA; /* error, no data to save */
            else { /* save file to recieve folder */
                strcat(filePath, recvFileBuffer->fileName);
                
                f = fopen(filePath, "wb");
                if(f == NULL)
                    ret = CO_SDO_AB_GENERAL; 
                else {
                    fwrite(recvFileBuffer->fileData, sizeof(char), recvFileBuffer->fileSize, f);
                    fclose(f);

                    /* clear buffer */
                    recvFileBuffer->fileSize = 0;
                    recvFileBuffer->fileName[0] = '\0';

                    ODF_arg->lastSegment = true;
                }
            }
            break;

        default :
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }

    ft_unlock_mtx();
    return ret;
}


// ***************************************************************************
// Send file array ODF


CO_SDO_abortCode_t send_file_array_ODF(CO_ODF_arg_t *ODF_arg) {
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    send_file_data_t *sendFileBuffer;

    sendFileBuffer = (send_file_data_t*) ODF_arg->object;

    if(ODF_arg->reading == false) 
        return CO_SDO_AB_READONLY; /* can't write parameters, read only */

    ft_lock_mtx();

    if(ODF_arg->subIndex == 0) {
        uint8_t temp = SEND_FILE_LIST_SIZE;
        ODF_arg->dataLength = sizeof(temp);
        memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
    }
    else if(ODF_arg->subIndex > 0 && ODF_arg->subIndex <= SEND_FILE_LIST_SIZE) {
        ODF_arg->dataLength = strlen(sendFileBuffer->fileList[ODF_arg->subIndex-1]) + 1;
        memcpy(ODF_arg->data, sendFileBuffer->fileList[ODF_arg->subIndex-1], ODF_arg->dataLength);
    }
    else
        ret = CO_SDO_AB_SUB_UNKNOWN;

    ft_unlock_mtx();
    return ret;
}


int app_send_file(const char *filePath) {
    int ret = 0;
    char fileName[FILE_PATH_MAX_LENGTH];
    char newFilePath[FILE_PATH_MAX_LENGTH] = FILE_SEND_FOLDER;
    int source, dest;
    struct stat stat_source;

    get_file_name(filePath, fileName);
    strcat(newFilePath, fileName);

    if(filePath == NULL || filePath[0] == '\0')
        ret = 1; /* not a valid file path */
    else if(filePath[0] != '/')
        ret = 2; /* not a a absolute path */
    else {
        /* move file into send folder */

        source = open(filePath, O_RDONLY, 0);
        dest = open(newFilePath, O_WRONLY | O_CREAT, 0644);

        fstat(source, &stat_source);
        sendfile(dest, source, 0, stat_source.st_size);

        close(source);
        close(dest);

        /* remove old file */
        remove(filePath);

        ft_lock_mtx();

        /* there is a empty spot in file list, add filename to the list */
        if(sendFileBuffer.filesAvailable < SEND_FILE_LIST_SIZE) { 
            unsigned int i=0;

            /* Since the file list array will be 127 at maxiumum, 
             * a iterative search through the list is fine, 
             * it will have a O(1).
             */

            /* spin until an empty spot in the file list is found or at end */
            while(sendFileBuffer.fileList[0][i] != '\0' && i < SEND_FILE_LIST_SIZE) 
                ++i;

            /* add file name to file list if there is valid empty spot */
            if(i < SEND_FILE_LIST_SIZE)
                strncpy(sendFileBuffer.fileList[i], fileName, strlen(fileName) + 1);
            else
                ++sendFileBuffer.overflow;

            ++sendFileBuffer.filesAvailable;
        }

        ft_unlock_mtx();
    }

    return ret; 
}


/******************************************************************************/
/* Send file, OD entry 3003 */


/**
* Wrapper funciton for send_file_ODF. Initalizes the file list array to empty 
* and then adds any files found in the send folder to the file list. 
*
* @return 1 on sucess and -1 on error
*/
static int initFileList(send_file_data_t *sendFileBuffer) {
    DIR *d;
    struct dirent *dir;
    int a, b;

    /* mark every entry in file array as empty */
    for(unsigned int i=0; i<SEND_FILE_LIST_SIZE; ++i) 
        sendFileBuffer->fileList[0][i] = '\0';

    /* defualt values */
    sendFileBuffer->filesAvailable = 0;
    sendFileBuffer->overflow = 0;
    sendFileBuffer->fileName = sendFileBuffer->fileList[0];
    sendFileBuffer->filePath[0] = '\0';

    if((d = opendir(FILE_SEND_FOLDER)) != NULL) { 
        /* directory found */
        while((dir = readdir(d)) != NULL) {
            /* file found */
            a = strncmp(dir->d_name, ".", sizeof(dir->d_name));
            b = strncmp(dir->d_name, "..", sizeof(dir->d_name));

            if(a != 0 && b != 0) { /* file found */
                if(sendFileBuffer->filesAvailable < SEND_FILE_LIST_SIZE) /* space in file array  */
                    strncpy(sendFileBuffer->fileList[sendFileBuffer->filesAvailable], dir->d_name, strlen(dir->d_name) + 1);
                else /* file array is full */
                    ++sendFileBuffer->overflow;

                ++sendFileBuffer->filesAvailable;
            }
        }
        
        closedir(d);

        /* load 1st file, if there is one */
        if(sendFileBuffer->fileList[0][0] != '\0') {
            /* update file path */
            strncpy(sendFileBuffer->filePath, FILE_SEND_FOLDER, strlen(FILE_SEND_FOLDER) + 1);
            strncat(sendFileBuffer->filePath, sendFileBuffer->fileName, strlen(sendFileBuffer->fileName) + 1);

            /* load file into buffer and get size */
            sendFileBuffer->fileSize = get_file_data(sendFileBuffer->filePath, sendFileBuffer->fileData);
        }
    }
    else
        return -1;

    return 1;
}


/**
* Wrapper funciton for send_file_ODF. Remove the path from the file path name.
*
* @return the length of the file name without '\0' or 0 on failure.
*/
static uint32_t get_file_name(const char *filePath, char *fileName) {
    uint32_t pathNameSize, start, fileNameSize = 0;

    if(filePath == NULL || filePath[0] == '\0')
        return 0; /* error, input(s) invaild */

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
    else
        fileNameSize = 0; /* error, data too big for buffer */

    return fileNameSize;
}


/**
* Wrapper funciton for send_file_ODF. Read in file data.
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
 * Wrapper function used by send_file_ODF to read file data into SDO buffer from struct. 
 * It can handle spilting large data files into multiple segments.
 * */
static CO_SDO_abortCode_t read_file_data(CO_ODF_arg_t *ODF_arg) {
    send_file_data_t *sendFileBuffer;

    sendFileBuffer = (send_file_data_t*) ODF_arg->object;

    if(ODF_arg->reading == false) 
        return CO_SDO_AB_READONLY; /* can't write parameters, read only */

    /* if the file path is empty, don't use the data buffer */
    if(sendFileBuffer->filePath[0] == '\0') {
        ODF_arg->dataLength = 1;
        memcpy(ODF_arg->data, sendFileBuffer->filePath, ODF_arg->dataLength);
        return CO_SDO_AB_NONE;
    }

    if(ODF_arg->firstSegment == true) { /* 1st segment */
        if(sendFileBuffer->fileSize > FILE_TRANSFER_MAX_SIZE)
            return CO_SDO_AB_OUT_OF_MEM; /* file is larger than domain buffer */
        
        ODF_arg->dataLengthTotal = sendFileBuffer->fileSize;
        ODF_arg->offset = 0;

        if(sendFileBuffer->fileSize <= CO_SDO_BUFFER_SIZE) {
            /* only need 1 segment */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = sendFileBuffer->fileSize;
        }
        else { 
            /* multiple segments needed */
            ODF_arg->lastSegment = false;
            ODF_arg->dataLength = CO_SDO_BUFFER_SIZE;
        }
    }
    else { /* not 1st segment */
        uint32_t next_seg_len = ODF_arg->dataLengthTotal - ODF_arg->offset;
        if(next_seg_len > CO_SDO_BUFFER_SIZE) {
            /* more segments needed */
            ODF_arg->lastSegment = false;
            ODF_arg->dataLength = CO_SDO_BUFFER_SIZE;
        }
        else {
            /* last segement */
            ODF_arg->lastSegment = true;
            ODF_arg->dataLength = next_seg_len;
        }
    }

    /* copy data */
    memcpy(ODF_arg->data, &sendFileBuffer->fileData[ODF_arg->offset], ODF_arg->dataLength);

    return CO_SDO_AB_NONE;
}


CO_SDO_abortCode_t send_file_ODF(CO_ODF_arg_t *ODF_arg) {
    CO_SDO_abortCode_t ret = CO_SDO_AB_NONE;
    send_file_data_t *sendFileBuffer;

    sendFileBuffer = (send_file_data_t*) ODF_arg->object;

    ft_lock_mtx();

    switch(ODF_arg->subIndex) {
        case 1 : /* load file from send folder (read only) */

            if(ODF_arg->reading == true) { /* reading, get file pointer index */
                uint8_t temp = sendFileBuffer->filePointer+1;
                ODF_arg->dataLength = sizeof(temp);
                memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
            }
            else { /*writing, load file data */
                if(ODF_arg->dataLength != sizeof(sendFileBuffer->filePointer)) {
                    ret = CO_SDO_AB_GENERAL; /* error with file */
                    break;
                }

                memcpy(&sendFileBuffer->filePointer, ODF_arg->data, ODF_arg->dataLength);

                if(sendFileBuffer->filePointer == 0 || sendFileBuffer->filePointer > SEND_FILE_LIST_SIZE) {
                    ret = CO_SDO_AB_GENERAL; /* error, non valid index in array */
                    break;
                }

                --sendFileBuffer->filePointer; /* fix offset value */

                sendFileBuffer->fileName = sendFileBuffer->fileList[sendFileBuffer->filePointer];
                
                /* if there is a valid file name in the file list, make the file path */
                if(sendFileBuffer->fileName[0] != '\0') {

                    /* update file path */
                    strncpy(sendFileBuffer->filePath, FILE_SEND_FOLDER, strlen(FILE_SEND_FOLDER)+1);
                    strncat(sendFileBuffer->filePath, sendFileBuffer->fileName, strlen(sendFileBuffer->fileName) + 1);

                    /* load file into buffer and get size */
                    sendFileBuffer->fileSize = get_file_data(sendFileBuffer->filePath, sendFileBuffer->fileData);
                    if(sendFileBuffer->fileSize > FILE_TRANSFER_MAX_SIZE) {
                        ret = CO_SDO_AB_OUT_OF_MEM; /* error file to large */
                    }
                }
            }

            break;

        case 2 : /* file name (read only) */

            if(ODF_arg->reading == false) 
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            else {
                uint32_t temp = strlen(sendFileBuffer->fileName);
                if(temp >= FILE_TRANSFER_MAX_SIZE)
                    ret = CO_SDO_AB_OUT_OF_MEM; /* error, new data will not fit in buffer */
                else {
                    /* send file path, this can handle empty file name */
                    ODF_arg->dataLength = temp + 1;
                    memcpy(ODF_arg->data, sendFileBuffer->fileName, ODF_arg->dataLength);
                }
            }

            break;

        case 3 : /* file data (read only) */

            ret = read_file_data(ODF_arg); /* wrapper function */

            break;

        case 4 : /* file size (read only) */

            if(ODF_arg->reading == false) 
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            else {
                ODF_arg->dataLength = sizeof(sendFileBuffer->fileSize);
                memcpy(ODF_arg->data, &sendFileBuffer->fileSize, ODF_arg->dataLength);
            }

            break;

        case 5 : /* delete current file (read or write to delete file) */

            if(ODF_arg->reading == true) { /* reading */
                bool_t temp = true;
                ODF_arg->dataLength = sizeof(temp);
                memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
            }

            /* delete file if there is a valid file path */
            if(sendFileBuffer->filePath[0] != '\0') {
                remove(sendFileBuffer->filePath); /* delete file */
                sendFileBuffer->fileName[0] = '\0';
                sendFileBuffer->filePath[0] = '\0';
                sendFileBuffer->fileSize = 0;
                --sendFileBuffer->filesAvailable;
            }

            break;

        case 6 : /* total # of files available (read only) */

            if(ODF_arg->reading == false) 
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            else {
                ODF_arg->dataLength = sizeof(sendFileBuffer->filesAvailable);
                memcpy(ODF_arg->data, &sendFileBuffer->filesAvailable, ODF_arg->dataLength);
            }

            break;

        case 7 : /* # of files that overflowed (not in file array) (read only) */
            
            if(ODF_arg->reading == false) 
                ret = CO_SDO_AB_READONLY; /* can't write parameters, read only */
            else {
                ODF_arg->dataLength = sizeof(sendFileBuffer->overflow);
                memcpy(ODF_arg->data, &sendFileBuffer->overflow, ODF_arg->dataLength);
            }

            break;

        case 8 : /* refresh file array (read or write to refresh )*/

            if(ODF_arg->reading == true) { /* reading */
                bool_t temp = true;
                ODF_arg->dataLength = sizeof(temp);
                memcpy(ODF_arg->data, &temp, ODF_arg->dataLength);
            }

            initFileList(sendFileBuffer);

            break;

        default :
            ret = CO_SDO_AB_SUB_UNKNOWN; 
    }

    ft_unlock_mtx();
    return ret;
}

