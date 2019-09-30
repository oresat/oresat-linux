/*
 *
 *
 */
#include "CANopen.h"
#include "CO_SDO.h" /* only for return codes*/
#include "app_OD_helpers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

extern CO_OD_entry_t CO_OD[CO_OD_NoOfElements];

/******************************************************************************/
uint16_t app_OD_find(uint16_t index){
    /* Fast search in ordered Object Dictionary. If indexes are mixed, this won't work. */
    /* If Object Dictionary has up to 2^N entries, then N is max number of loop passes. */
    uint16_t cur, min, max;
    const CO_OD_entry_t* object;

    min = 0U;
    max = CO_OD_NoOfElements - 1U;
    while(min < max){
        cur = (min + max) / 2;
        object = &CO_OD[cur];
        /* Is object matched */
        if(index == object->index){
            return cur;
        }
        if(index < object->index){
            max = cur;
            if(max) max--;
        }
        else
            min = cur + 1U;
    }

    if(min == max){
        object = &CO_OD[min];
        /* Is object matched */
        if(index == object->index){
            return min;
        }
    }

    return 0xFFFFU;  /* object does not exist in OD */
}


/******************************************************************************/
uint32_t app_readOD(uint16_t index, uint16_t subIndex, void *data, uint16_t *length){
    CO_OD_entry_t* object = NULL;
    int8_t *ODdata = NULL;
    uint16_t ODlength;
    uint16_t ODattribute;

    if(data == NULL) {
        return CO_SDO_AB_NO_DATA; 
    }

    /* get object location */
    uint16_t entryNo = app_OD_find(index);
    if(entryNo == 0xFFFE) {
        return CO_SDO_AB_NOT_EXIST;  /* index not found */
    }
    
    /* get object */
    object = &CO_OD[entryNo];

    /* check if sub index is valid */
    if(subIndex >= object->maxSubIndex) {
        return CO_SDO_AB_SUB_UNKNOWN;     /* Sub index does not exist  */
    }

    /* figure out OD entry type */
    if(object->maxSubIndex == 0U){    /* Object type is Var */
        ODattribute = object->attribute;
        ODlength = object->length;
        ODdata = object->pData;
    }
    else if(object->attribute != 0U){ /* Object type is Array */
        ODattribute = object->attribute;
        if(subIndex == 0) {
            /* First subIndex is readonly, it holds the length of Array */
            ODattribute &= ~(CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE);
            ODattribute |= CO_ODA_READABLE;
            ODlength = sizeof(object->maxSubIndex);
            ODdata = &object->maxSubIndex;
        }
        else { /* array data */
            ODlength = object->length;
            ODdata = ((int8_t*)object->pData) + ((subIndex-1) * object->length);
        }
    }
    else{                            /* Object type is Record */
        ODattribute = ((const CO_OD_entryRecord_t*)(object->pData))[subIndex].attribute;
        ODlength = ((const CO_OD_entryRecord_t*)(object->pData))[subIndex].length;
        ODdata = ((const CO_OD_entryRecord_t*)(object->pData))[subIndex].pData;
    }

    /* is object readable? */
    if((ODattribute & CO_ODA_READABLE) == 0){
        return CO_SDO_AB_WRITEONLY;     /* attempt to read a write-only object */
    }

    /* check if domain type */
    if(ODdata != NULL){ /* not a domain type*/
        CO_LOCK_OD();

        /* copy data */
        *length = ODlength;
        memcpy(data, ODdata, ODlength);

        CO_UNLOCK_OD();
    }
    else { /* domain type */
        //TODO call domain functions
    }

    return 0;
}


/******************************************************************************/
uint32_t app_writeOD(uint16_t index, uint16_t subIndex, void *data, uint16_t length){
    CO_OD_entry_t* object = NULL;
    int8_t *ODdata = NULL;
    uint16_t ODlength;
    uint16_t ODattribute;

    if(data == NULL) {
        return CO_SDO_AB_NO_DATA; 
    }

    uint16_t entryNo = app_OD_find(index);
    if(entryNo == 0xFFFE) {
        return CO_SDO_AB_NOT_EXIST;  /* index not found */
    }

    /* get object */
    object = &CO_OD[entryNo];

    /* check if sub index is valid */
    if(subIndex >= object->maxSubIndex) {
        return CO_SDO_AB_SUB_UNKNOWN;     /* Sub index does not exist  */
    }

    /* figure out OD entry type */
    if(object->maxSubIndex == 0U){    /* Object type is Var */
        ODattribute = object->attribute;
        ODlength = object->length;
        ODdata = object->pData;
    }
    else if(object->attribute != 0U){ /* Object type is Array */
        ODattribute = object->attribute;
        if(subIndex == 0) {
            /* First subIndex is readonly, it holds the length of Array */
            ODattribute &= ~(CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE);
            ODattribute |= CO_ODA_READABLE;
            ODlength = sizeof(object->maxSubIndex);
            ODdata = &object->maxSubIndex;
        }
        else { /* array data */
            ODlength = object->length;
            ODdata = ((int8_t*)object->pData) + ((subIndex-1) * object->length);
        }
    }
    else{                            /* Object type is Record */
        CO_OD_entryRecord_t *temp = (CO_OD_entryRecord_t *)object->pData;
        ODattribute = temp[subIndex].attribute;
        ODlength = temp[subIndex].length;
        ODdata = temp[subIndex].pData;
    }

    /* is object writeable? */
    if((ODattribute & CO_ODA_WRITEABLE) == 0){
        return CO_SDO_AB_READONLY;     /* attempt to write a read-only object */
    }

    /* check if domain type */
    if(ODdata != NULL){ /* not a domain type*/
        if(length > ODlength) {
            return CO_SDO_AB_TYPE_MISMATCH;     /* Length of service parameter does not match */
        }

        CO_LOCK_OD();

        /* copy data  */
        ODlength = length;
        memcpy(ODdata, data, length);

        CO_UNLOCK_OD();
    }
    else { /* domain type */
        //TODO call domain functions
    }

    return 0;
}

