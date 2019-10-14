/*******************************************************************************

   File - CO_OD.c/CO_OD.h
   CANopen Object Dictionary.

   Copyright (C) 2004-2008 Janez Paternoster

   License: GNU Lesser General Public License (LGPL).

   <http://canopennode.sourceforge.net>

   (For more information see <CO_SDO.h>.)

   This file is part of CANopenNode, an open source CANopen Stack.
   Project home page is <https://github.com/CANopenNode/CANopenNode>.
   For more information on CANopen see <http://www.can-cia.org/>.
 
   CANopenNode is free and open source software: you can redistribute
   it and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation, either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>.
  
   Following clarification and special exception to the GNU General Public
   License is included to the distribution terms of CANopenNode:
  
   Linking this library statically or dynamically with other modules is
   making a combined work based on this library. Thus, the terms and
   conditions of the GNU General Public License cover the whole combination.
  
   As a special exception, the copyright holders of this library give
   you permission to link this library with independent modules to
   produce an executable, regardless of the license terms of these
   independent modules, and to copy and distribute the resulting
   executable under terms of your choice, provided that you also meet,
   for each linked independent module, the terms and conditions of the
   license of that module. An independent module is a module which is
   not derived from or based on this library. If you modify this
   library, you may extend this exception to your version of the
   library, but you are not obliged to do so. If you do not wish
   to do so, delete this exception statement from your version.
 
   This file was automatically generated with libedssharp Object
   Dictionary Editor v0.7-9-g821eedb   DON'T EDIT THIS FILE MANUALLY !!!!
*******************************************************************************/


#pragma once

/*******************************************************************************
   CANopen DATA TYPES
*******************************************************************************/
   typedef bool_t       BOOLEAN;
   typedef uint8_t      UNSIGNED8;
   typedef uint16_t     UNSIGNED16;
   typedef uint32_t     UNSIGNED32;
   typedef uint64_t     UNSIGNED64;
   typedef int8_t       INTEGER8;
   typedef int16_t      INTEGER16;
   typedef int32_t      INTEGER32;
   typedef int64_t      INTEGER64;
   typedef float32_t    REAL32; 
   typedef float64_t    REAL64; 
   typedef char_t       VISIBLE_STRING;
   typedef oChar_t      OCTET_STRING;

   #ifdef DOMAIN
   #undef DOMAIN
   #endif

   typedef domain_t     DOMAIN;

#ifndef timeOfDay_t
    typedef union {
        unsigned long long ullValue;
        struct {
            unsigned long ms:28;
            unsigned reserved:4;
            unsigned days:16;
            unsigned reserved2:16;
        };
    }timeOfDay_t;
#endif

    typedef timeOfDay_t TIME_OF_DAY;
    typedef timeOfDay_t TIME_DIFFERENCE;


/*******************************************************************************
   FILE INFO:
      FileName:     StarTracker_objDict.eds
      FileVersion:  0
      CreationTime: 12:00AM
      CreationDate: 01-01-0001
      CreatedBy:    JP
******************************************************************************/


/*******************************************************************************
   DEVICE INFO:
      VendorName:     CANopenNode
      VendorNumber    0
      ProductName:    CANopenNode
      ProductNumber:  0
******************************************************************************/


/*******************************************************************************
   FEATURES
*******************************************************************************/
  #define CO_NO_SYNC                     1   //Associated objects: 1005-1007
  #define CO_NO_EMERGENCY                1   //Associated objects: 1014, 1015
  #define CO_NO_SDO_SERVER               1   //Associated objects: 1200-127F
  #define CO_NO_SDO_CLIENT               1   //Associated objects: 1280-12FF
  #define CO_NO_LSS_SERVER               0   //LSS Slave
  #define CO_NO_LSS_CLIENT               0   //LSS Master
  #define CO_NO_RPDO                     16   //Associated objects: 14xx, 16xx
  #define CO_NO_TPDO                     16   //Associated objects: 18xx, 1Axx
  #define CO_NO_NMT_MASTER               1


/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
   #define CO_OD_NoOfElements             107


/*******************************************************************************
   TYPE DEFINITIONS FOR RECORDS
*******************************************************************************/
/*1018    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     vendorID;
               UNSIGNED32     productCode;
               UNSIGNED32     revisionNumber;
               UNSIGNED32     serialNumber;
               }              OD_identity_t;
/*1200    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               }              OD_SDOServerParameter_t;
/*1280    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDClientToServer;
               UNSIGNED32     COB_IDServerToClient;
               UNSIGNED8      nodeIDOfTheSDOServer;
               }              OD_SDOClientParameter_t;
/*1400    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByRPDO;
               UNSIGNED8      transmissionType;
               }              OD_RPDOCommunicationParameter_t;
/*1600    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_RPDOMappingParameter_t;
/*1800    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               UNSIGNED32     COB_IDUsedByTPDO;
               UNSIGNED8      transmissionType;
               UNSIGNED16     inhibitTime;
               UNSIGNED8      compatibilityEntry;
               UNSIGNED16     eventTimer;
               UNSIGNED8      SYNCStartValue;
               }              OD_TPDOCommunicationParameter_t;
/*1a00    */ typedef struct {
               UNSIGNED8      numberOfMappedObjects;
               UNSIGNED32     mappedObject1;
               UNSIGNED32     mappedObject2;
               UNSIGNED32     mappedObject3;
               UNSIGNED32     mappedObject4;
               UNSIGNED32     mappedObject5;
               UNSIGNED32     mappedObject6;
               UNSIGNED32     mappedObject7;
               UNSIGNED32     mappedObject8;
               }              OD_TPDOMappingParameter_t;
/*2130    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               VISIBLE_STRING string[1];
               UNSIGNED64     epochTimeBaseMs;
               UNSIGNED32     epochTimeOffsetMs;
               }              OD_time_t;
/*3001    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               DOMAIN         fileName;
               DOMAIN         fileData;
               UNSIGNED8      saveFile;
               }              OD_receiveFile_t;
/*3002    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               DOMAIN         fileName;
               DOMAIN         fileData;
               UNSIGNED8      loadFile;
               UNSIGNED8      filesAvalible;
               }              OD_sendFile_t;
/*3100    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               INTEGER8       currentState;
               INTEGER8       getLatestImage;
               }              OD_starTrackerStatusControl_t;
/*3101    */ typedef struct {
               UNSIGNED8      maxSubIndex;
               INTEGER16      rotationY;
               INTEGER16      rotationZ;
               INTEGER16      rotationCamera;
               }              OD_orienation_t;

/*******************************************************************************
   TYPE DEFINITIONS FOR OBJECT DICTIONARY INDEXES

   some of those are redundant with CO_SDO.h CO_ObjDicId_t <Common CiA301 object 
   dictionary entries>
*******************************************************************************/
/*1000 */
        #define OD_1000_deviceType                                  0x1000

/*1001 */
        #define OD_1001_errorRegister                               0x1001

/*1002 */
        #define OD_1002_manufacturerStatusRegister                  0x1002

/*1003 */
        #define OD_1003_preDefinedErrorField                        0x1003

        #define OD_1003_0_preDefinedErrorField_maxSubIndex          0
        #define OD_1003_1_preDefinedErrorField_standardErrorField   1
        #define OD_1003_2_preDefinedErrorField_standardErrorField   2
        #define OD_1003_3_preDefinedErrorField_standardErrorField   3
        #define OD_1003_4_preDefinedErrorField_standardErrorField   4
        #define OD_1003_5_preDefinedErrorField_standardErrorField   5
        #define OD_1003_6_preDefinedErrorField_standardErrorField   6
        #define OD_1003_7_preDefinedErrorField_standardErrorField   7
        #define OD_1003_8_preDefinedErrorField_standardErrorField   8

/*1005 */
        #define OD_1005_COB_ID_SYNCMessage                          0x1005

/*1006 */
        #define OD_1006_communicationCyclePeriod                    0x1006

/*1007 */
        #define OD_1007_synchronousWindowLength                     0x1007

/*1008 */
        #define OD_1008_manufacturerDeviceName                      0x1008

/*1009 */
        #define OD_1009_manufacturerHardwareVersion                 0x1009

/*100a */
        #define OD_100a_manufacturerSoftwareVersion                 0x100a

/*1010 */
        #define OD_1010_storeParameters                             0x1010

        #define OD_1010_0_storeParameters_maxSubIndex               0
        #define OD_1010_1_storeParameters_saveAllParameters         1

/*1011 */
        #define OD_1011_restoreDefaultParameters                    0x1011

        #define OD_1011_0_restoreDefaultParameters_maxSubIndex      0
        #define OD_1011_1_restoreDefaultParameters_restoreAllDefaultParameters 1

/*1014 */
        #define OD_1014_COB_ID_EMCY                                 0x1014

/*1015 */
        #define OD_1015_inhibitTimeEMCY                             0x1015

/*1016 */
        #define OD_1016_consumerHeartbeatTime                       0x1016

        #define OD_1016_0_consumerHeartbeatTime_maxSubIndex         0
        #define OD_1016_1_consumerHeartbeatTime_consumerHeartbeatTime 1
        #define OD_1016_2_consumerHeartbeatTime_consumerHeartbeatTime 2
        #define OD_1016_3_consumerHeartbeatTime_consumerHeartbeatTime 3
        #define OD_1016_4_consumerHeartbeatTime_consumerHeartbeatTime 4

/*1017 */
        #define OD_1017_producerHeartbeatTime                       0x1017

/*1018 */
        #define OD_1018_identity                                    0x1018

        #define OD_1018_0_identity_maxSubIndex                      0
        #define OD_1018_1_identity_vendorID                         1
        #define OD_1018_2_identity_productCode                      2
        #define OD_1018_3_identity_revisionNumber                   3
        #define OD_1018_4_identity_serialNumber                     4

/*1019 */
        #define OD_1019_synchronousCounterOverflowValue             0x1019

/*1029 */
        #define OD_1029_errorBehavior                               0x1029

        #define OD_1029_0_errorBehavior_maxSubIndex                 0
        #define OD_1029_1_errorBehavior_communication               1
        #define OD_1029_2_errorBehavior_communicationOther          2
        #define OD_1029_3_errorBehavior_communicationPassive        3
        #define OD_1029_4_errorBehavior_generic                     4
        #define OD_1029_5_errorBehavior_deviceProfile               5
        #define OD_1029_6_errorBehavior_manufacturerSpecific        6

/*1200 */
        #define OD_1200_SDOServerParameter                          0x1200

        #define OD_1200_0_SDOServerParameter_maxSubIndex            0
        #define OD_1200_1_SDOServerParameter_COB_IDClientToServer   1
        #define OD_1200_2_SDOServerParameter_COB_IDServerToClient   2

/*1280 */
        #define OD_1280_SDOClientParameter                          0x1280

        #define OD_1280_0_SDOClientParameter_maxSubIndex            0
        #define OD_1280_1_SDOClientParameter_COB_IDClientToServer   1
        #define OD_1280_2_SDOClientParameter_COB_IDServerToClient   2
        #define OD_1280_3_SDOClientParameter_nodeIDOfTheSDOServer   3

/*1400 */
        #define OD_1400_RPDOCommunicationParameter                  0x1400

        #define OD_1400_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1400_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1400_2_RPDOCommunicationParameter_transmissionType 2

/*1401 */
        #define OD_1401_RPDOCommunicationParameter                  0x1401

        #define OD_1401_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1401_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1401_2_RPDOCommunicationParameter_transmissionType 2

/*1402 */
        #define OD_1402_RPDOCommunicationParameter                  0x1402

        #define OD_1402_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1402_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1402_2_RPDOCommunicationParameter_transmissionType 2

/*1403 */
        #define OD_1403_RPDOCommunicationParameter                  0x1403

        #define OD_1403_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1403_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1403_2_RPDOCommunicationParameter_transmissionType 2

/*1404 */
        #define OD_1404_RPDOCommunicationParameter                  0x1404

        #define OD_1404_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1404_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1404_2_RPDOCommunicationParameter_transmissionType 2

/*1405 */
        #define OD_1405_RPDOCommunicationParameter                  0x1405

        #define OD_1405_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1405_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1405_2_RPDOCommunicationParameter_transmissionType 2

/*1406 */
        #define OD_1406_RPDOCommunicationParameter                  0x1406

        #define OD_1406_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1406_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1406_2_RPDOCommunicationParameter_transmissionType 2

/*1407 */
        #define OD_1407_RPDOCommunicationParameter                  0x1407

        #define OD_1407_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1407_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1407_2_RPDOCommunicationParameter_transmissionType 2

/*1408 */
        #define OD_1408_RPDOCommunicationParameter                  0x1408

        #define OD_1408_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1408_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1408_2_RPDOCommunicationParameter_transmissionType 2

/*1409 */
        #define OD_1409_RPDOCommunicationParameter                  0x1409

        #define OD_1409_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_1409_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_1409_2_RPDOCommunicationParameter_transmissionType 2

/*140a */
        #define OD_140a_RPDOCommunicationParameter                  0x140a

        #define OD_140a_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140a_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140a_2_RPDOCommunicationParameter_transmissionType 2

/*140b */
        #define OD_140b_RPDOCommunicationParameter                  0x140b

        #define OD_140b_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140b_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140b_2_RPDOCommunicationParameter_transmissionType 2

/*140c */
        #define OD_140c_RPDOCommunicationParameter                  0x140c

        #define OD_140c_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140c_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140c_2_RPDOCommunicationParameter_transmissionType 2

/*140d */
        #define OD_140d_RPDOCommunicationParameter                  0x140d

        #define OD_140d_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140d_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140d_2_RPDOCommunicationParameter_transmissionType 2

/*140e */
        #define OD_140e_RPDOCommunicationParameter                  0x140e

        #define OD_140e_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140e_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140e_2_RPDOCommunicationParameter_transmissionType 2

/*140f */
        #define OD_140f_RPDOCommunicationParameter                  0x140f

        #define OD_140f_0_RPDOCommunicationParameter_maxSubIndex    0
        #define OD_140f_1_RPDOCommunicationParameter_COB_IDUsedByRPDO 1
        #define OD_140f_2_RPDOCommunicationParameter_transmissionType 2

/*1600 */
        #define OD_1600_RPDOMappingParameter                        0x1600

        #define OD_1600_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1600_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1600_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1600_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1600_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1600_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1600_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1600_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1600_8_RPDOMappingParameter_mappedObject8        8

/*1601 */
        #define OD_1601_RPDOMappingParameter                        0x1601

        #define OD_1601_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1601_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1601_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1601_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1601_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1601_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1601_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1601_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1601_8_RPDOMappingParameter_mappedObject8        8

/*1602 */
        #define OD_1602_RPDOMappingParameter                        0x1602

        #define OD_1602_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1602_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1602_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1602_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1602_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1602_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1602_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1602_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1602_8_RPDOMappingParameter_mappedObject8        8

/*1603 */
        #define OD_1603_RPDOMappingParameter                        0x1603

        #define OD_1603_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1603_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1603_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1603_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1603_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1603_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1603_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1603_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1603_8_RPDOMappingParameter_mappedObject8        8

/*1604 */
        #define OD_1604_RPDOMappingParameter                        0x1604

        #define OD_1604_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1604_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1604_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1604_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1604_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1604_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1604_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1604_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1604_8_RPDOMappingParameter_mappedObject8        8

/*1605 */
        #define OD_1605_RPDOMappingParameter                        0x1605

        #define OD_1605_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1605_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1605_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1605_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1605_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1605_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1605_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1605_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1605_8_RPDOMappingParameter_mappedObject8        8

/*1606 */
        #define OD_1606_RPDOMappingParameter                        0x1606

        #define OD_1606_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1606_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1606_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1606_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1606_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1606_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1606_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1606_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1606_8_RPDOMappingParameter_mappedObject8        8

/*1607 */
        #define OD_1607_RPDOMappingParameter                        0x1607

        #define OD_1607_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1607_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1607_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1607_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1607_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1607_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1607_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1607_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1607_8_RPDOMappingParameter_mappedObject8        8

/*1608 */
        #define OD_1608_RPDOMappingParameter                        0x1608

        #define OD_1608_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1608_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1608_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1608_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1608_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1608_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1608_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1608_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1608_8_RPDOMappingParameter_mappedObject8        8

/*1609 */
        #define OD_1609_RPDOMappingParameter                        0x1609

        #define OD_1609_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_1609_1_RPDOMappingParameter_mappedObject1        1
        #define OD_1609_2_RPDOMappingParameter_mappedObject2        2
        #define OD_1609_3_RPDOMappingParameter_mappedObject3        3
        #define OD_1609_4_RPDOMappingParameter_mappedObject4        4
        #define OD_1609_5_RPDOMappingParameter_mappedObject5        5
        #define OD_1609_6_RPDOMappingParameter_mappedObject6        6
        #define OD_1609_7_RPDOMappingParameter_mappedObject7        7
        #define OD_1609_8_RPDOMappingParameter_mappedObject8        8

/*160a */
        #define OD_160a_RPDOMappingParameter                        0x160a

        #define OD_160a_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160a_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160a_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160a_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160a_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160a_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160a_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160a_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160a_8_RPDOMappingParameter_mappedObject8        8

/*160b */
        #define OD_160b_RPDOMappingParameter                        0x160b

        #define OD_160b_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160b_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160b_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160b_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160b_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160b_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160b_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160b_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160b_8_RPDOMappingParameter_mappedObject8        8

/*160c */
        #define OD_160c_RPDOMappingParameter                        0x160c

        #define OD_160c_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160c_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160c_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160c_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160c_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160c_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160c_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160c_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160c_8_RPDOMappingParameter_mappedObject8        8

/*160d */
        #define OD_160d_RPDOMappingParameter                        0x160d

        #define OD_160d_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160d_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160d_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160d_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160d_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160d_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160d_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160d_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160d_8_RPDOMappingParameter_mappedObject8        8

/*160e */
        #define OD_160e_RPDOMappingParameter                        0x160e

        #define OD_160e_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160e_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160e_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160e_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160e_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160e_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160e_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160e_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160e_8_RPDOMappingParameter_mappedObject8        8

/*160f */
        #define OD_160f_RPDOMappingParameter                        0x160f

        #define OD_160f_0_RPDOMappingParameter_maxSubIndex          0
        #define OD_160f_1_RPDOMappingParameter_mappedObject1        1
        #define OD_160f_2_RPDOMappingParameter_mappedObject2        2
        #define OD_160f_3_RPDOMappingParameter_mappedObject3        3
        #define OD_160f_4_RPDOMappingParameter_mappedObject4        4
        #define OD_160f_5_RPDOMappingParameter_mappedObject5        5
        #define OD_160f_6_RPDOMappingParameter_mappedObject6        6
        #define OD_160f_7_RPDOMappingParameter_mappedObject7        7
        #define OD_160f_8_RPDOMappingParameter_mappedObject8        8

/*1800 */
        #define OD_1800_TPDOCommunicationParameter                  0x1800

        #define OD_1800_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1800_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1800_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1800_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1800_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1800_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1800_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1801 */
        #define OD_1801_TPDOCommunicationParameter                  0x1801

        #define OD_1801_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1801_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1801_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1801_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1801_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1801_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1801_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1802 */
        #define OD_1802_TPDOCommunicationParameter                  0x1802

        #define OD_1802_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1802_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1802_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1802_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1802_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1802_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1802_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1803 */
        #define OD_1803_TPDOCommunicationParameter                  0x1803

        #define OD_1803_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1803_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1803_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1803_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1803_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1803_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1803_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1804 */
        #define OD_1804_TPDOCommunicationParameter                  0x1804

        #define OD_1804_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1804_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1804_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1804_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1804_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1804_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1804_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1805 */
        #define OD_1805_TPDOCommunicationParameter                  0x1805

        #define OD_1805_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1805_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1805_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1805_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1805_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1805_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1805_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1806 */
        #define OD_1806_TPDOCommunicationParameter                  0x1806

        #define OD_1806_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1806_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1806_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1806_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1806_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1806_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1806_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1807 */
        #define OD_1807_TPDOCommunicationParameter                  0x1807

        #define OD_1807_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1807_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1807_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1807_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1807_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1807_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1807_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1808 */
        #define OD_1808_TPDOCommunicationParameter                  0x1808

        #define OD_1808_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1808_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1808_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1808_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1808_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1808_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1808_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1809 */
        #define OD_1809_TPDOCommunicationParameter                  0x1809

        #define OD_1809_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_1809_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_1809_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_1809_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_1809_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_1809_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_1809_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180a */
        #define OD_180a_TPDOCommunicationParameter                  0x180a

        #define OD_180a_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180a_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180a_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180a_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180a_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180a_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180a_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180b */
        #define OD_180b_TPDOCommunicationParameter                  0x180b

        #define OD_180b_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180b_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180b_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180b_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180b_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180b_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180b_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180c */
        #define OD_180c_TPDOCommunicationParameter                  0x180c

        #define OD_180c_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180c_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180c_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180c_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180c_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180c_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180c_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180d */
        #define OD_180d_TPDOCommunicationParameter                  0x180d

        #define OD_180d_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180d_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180d_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180d_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180d_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180d_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180d_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180e */
        #define OD_180e_TPDOCommunicationParameter                  0x180e

        #define OD_180e_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180e_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180e_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180e_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180e_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180e_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180e_6_TPDOCommunicationParameter_SYNCStartValue 6

/*180f */
        #define OD_180f_TPDOCommunicationParameter                  0x180f

        #define OD_180f_0_TPDOCommunicationParameter_maxSubIndex    0
        #define OD_180f_1_TPDOCommunicationParameter_COB_IDUsedByTPDO 1
        #define OD_180f_2_TPDOCommunicationParameter_transmissionType 2
        #define OD_180f_3_TPDOCommunicationParameter_inhibitTime    3
        #define OD_180f_4_TPDOCommunicationParameter_compatibilityEntry 4
        #define OD_180f_5_TPDOCommunicationParameter_eventTimer     5
        #define OD_180f_6_TPDOCommunicationParameter_SYNCStartValue 6

/*1a00 */
        #define OD_1a00_TPDOMappingParameter                        0x1a00

        #define OD_1a00_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a00_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a00_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a00_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a00_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a00_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a00_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a00_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a00_8_TPDOMappingParameter_mappedObject8        8

/*1a01 */
        #define OD_1a01_TPDOMappingParameter                        0x1a01

        #define OD_1a01_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a01_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a01_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a01_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a01_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a01_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a01_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a01_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a01_8_TPDOMappingParameter_mappedObject8        8

/*1a02 */
        #define OD_1a02_TPDOMappingParameter                        0x1a02

        #define OD_1a02_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a02_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a02_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a02_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a02_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a02_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a02_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a02_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a02_8_TPDOMappingParameter_mappedObject8        8

/*1a03 */
        #define OD_1a03_TPDOMappingParameter                        0x1a03

        #define OD_1a03_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a03_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a03_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a03_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a03_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a03_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a03_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a03_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a03_8_TPDOMappingParameter_mappedObject8        8

/*1a04 */
        #define OD_1a04_TPDOMappingParameter                        0x1a04

        #define OD_1a04_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a04_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a04_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a04_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a04_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a04_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a04_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a04_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a04_8_TPDOMappingParameter_mappedObject8        8

/*1a05 */
        #define OD_1a05_TPDOMappingParameter                        0x1a05

        #define OD_1a05_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a05_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a05_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a05_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a05_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a05_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a05_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a05_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a05_8_TPDOMappingParameter_mappedObject8        8

/*1a06 */
        #define OD_1a06_TPDOMappingParameter                        0x1a06

        #define OD_1a06_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a06_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a06_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a06_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a06_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a06_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a06_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a06_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a06_8_TPDOMappingParameter_mappedObject8        8

/*1a07 */
        #define OD_1a07_TPDOMappingParameter                        0x1a07

        #define OD_1a07_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a07_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a07_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a07_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a07_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a07_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a07_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a07_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a07_8_TPDOMappingParameter_mappedObject8        8

/*1a08 */
        #define OD_1a08_TPDOMappingParameter                        0x1a08

        #define OD_1a08_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a08_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a08_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a08_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a08_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a08_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a08_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a08_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a08_8_TPDOMappingParameter_mappedObject8        8

/*1a09 */
        #define OD_1a09_TPDOMappingParameter                        0x1a09

        #define OD_1a09_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a09_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a09_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a09_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a09_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a09_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a09_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a09_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a09_8_TPDOMappingParameter_mappedObject8        8

/*1a0a */
        #define OD_1a0a_TPDOMappingParameter                        0x1a0a

        #define OD_1a0a_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0a_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0a_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0a_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0a_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0a_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0a_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0a_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0a_8_TPDOMappingParameter_mappedObject8        8

/*1a0b */
        #define OD_1a0b_TPDOMappingParameter                        0x1a0b

        #define OD_1a0b_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0b_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0b_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0b_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0b_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0b_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0b_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0b_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0b_8_TPDOMappingParameter_mappedObject8        8

/*1a0c */
        #define OD_1a0c_TPDOMappingParameter                        0x1a0c

        #define OD_1a0c_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0c_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0c_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0c_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0c_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0c_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0c_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0c_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0c_8_TPDOMappingParameter_mappedObject8        8

/*1a0d */
        #define OD_1a0d_TPDOMappingParameter                        0x1a0d

        #define OD_1a0d_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0d_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0d_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0d_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0d_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0d_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0d_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0d_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0d_8_TPDOMappingParameter_mappedObject8        8

/*1a0e */
        #define OD_1a0e_TPDOMappingParameter                        0x1a0e

        #define OD_1a0e_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0e_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0e_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0e_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0e_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0e_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0e_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0e_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0e_8_TPDOMappingParameter_mappedObject8        8

/*1a0f */
        #define OD_1a0f_TPDOMappingParameter                        0x1a0f

        #define OD_1a0f_0_TPDOMappingParameter_maxSubIndex          0
        #define OD_1a0f_1_TPDOMappingParameter_mappedObject1        1
        #define OD_1a0f_2_TPDOMappingParameter_mappedObject2        2
        #define OD_1a0f_3_TPDOMappingParameter_mappedObject3        3
        #define OD_1a0f_4_TPDOMappingParameter_mappedObject4        4
        #define OD_1a0f_5_TPDOMappingParameter_mappedObject5        5
        #define OD_1a0f_6_TPDOMappingParameter_mappedObject6        6
        #define OD_1a0f_7_TPDOMappingParameter_mappedObject7        7
        #define OD_1a0f_8_TPDOMappingParameter_mappedObject8        8

/*1f80 */
        #define OD_1f80_NMTStartup                                  0x1f80

/*2100 */
        #define OD_2100_errorStatusBits                             0x2100

/*2101 */
        #define OD_2101_CANNodeID                                   0x2101

/*2102 */
        #define OD_2102_CANBitRate                                  0x2102

/*2103 */
        #define OD_2103_SYNCCounter                                 0x2103

/*2104 */
        #define OD_2104_SYNCTime                                    0x2104

/*2106 */
        #define OD_2106_powerOnCounter                              0x2106

/*2107 */
        #define OD_2107_performance                                 0x2107

        #define OD_2107_0_performance_maxSubIndex                   0
        #define OD_2107_1_performance_cyclesPerSecond               1
        #define OD_2107_2_performance_timerCycleTime                2
        #define OD_2107_3_performance_timerCycleMaxTime             3
        #define OD_2107_4_performance_mainCycleTime                 4
        #define OD_2107_5_performance_mainCycleMaxTime              5

/*2108 */
        #define OD_2108_temperature                                 0x2108

        #define OD_2108_0_temperature_maxSubIndex                   0
        #define OD_2108_1_temperature_mainPCB                       1

/*2109 */
        #define OD_2109_voltage                                     0x2109

        #define OD_2109_0_voltage_maxSubIndex                       0
        #define OD_2109_1_voltage_mainPCBSupply                     1

/*2110 */
        #define OD_2110_variableInt32                               0x2110

        #define OD_2110_0_variableInt32_maxSubIndex                 0
        #define OD_2110_1_variableInt32_int32                       1
        #define OD_2110_2_variableInt32_int32                       2
        #define OD_2110_3_variableInt32_int32                       3
        #define OD_2110_4_variableInt32_int32                       4
        #define OD_2110_5_variableInt32_int32                       5
        #define OD_2110_6_variableInt32_int32                       6
        #define OD_2110_7_variableInt32_int32                       7
        #define OD_2110_8_variableInt32_int32                       8
        #define OD_2110_9_variableInt32_int32                       9
        #define OD_2110_10_variableInt32_int32                      10
        #define OD_2110_11_variableInt32_int32                      11
        #define OD_2110_12_variableInt32_int32                      12
        #define OD_2110_13_variableInt32_int32                      13
        #define OD_2110_14_variableInt32_int32                      14
        #define OD_2110_15_variableInt32_int32                      15
        #define OD_2110_16_variableInt32_int32                      16
        #define OD_2110_17_variableInt32_int32                      17
        #define OD_2110_18_variableInt32_int32                      18
        #define OD_2110_19_variableInt32_int32                      19
        #define OD_2110_20_variableInt32_int32                      20
        #define OD_2110_21_variableInt32_int32                      21
        #define OD_2110_22_variableInt32_int32                      22
        #define OD_2110_23_variableInt32_int32                      23
        #define OD_2110_24_variableInt32_int32                      24
        #define OD_2110_25_variableInt32_int32                      25
        #define OD_2110_26_variableInt32_int32                      26
        #define OD_2110_27_variableInt32_int32                      27
        #define OD_2110_28_variableInt32_int32                      28
        #define OD_2110_29_variableInt32_int32                      29
        #define OD_2110_30_variableInt32_int32                      30
        #define OD_2110_31_variableInt32_int32                      31
        #define OD_2110_32_variableInt32_int32                      32

/*2111 */
        #define OD_2111_variableROM_Int32                           0x2111

        #define OD_2111_0_variableROM_Int32_maxSubIndex             0
        #define OD_2111_1_variableROM_Int32_int32                   1
        #define OD_2111_2_variableROM_Int32_int32                   2
        #define OD_2111_3_variableROM_Int32_int32                   3
        #define OD_2111_4_variableROM_Int32_int32                   4
        #define OD_2111_5_variableROM_Int32_int32                   5
        #define OD_2111_6_variableROM_Int32_int32                   6
        #define OD_2111_7_variableROM_Int32_int32                   7
        #define OD_2111_8_variableROM_Int32_int32                   8
        #define OD_2111_9_variableROM_Int32_int32                   9
        #define OD_2111_10_variableROM_Int32_int32                  10
        #define OD_2111_11_variableROM_Int32_int32                  11
        #define OD_2111_12_variableROM_Int32_int32                  12
        #define OD_2111_13_variableROM_Int32_int32                  13
        #define OD_2111_14_variableROM_Int32_int32                  14
        #define OD_2111_15_variableROM_Int32_int32                  15
        #define OD_2111_16_variableROM_Int32_int32                  16

/*2112 */
        #define OD_2112_variableNV_Int32                            0x2112

        #define OD_2112_0_variableNV_Int32_maxSubIndex              0
        #define OD_2112_1_variableNV_Int32_int32                    1
        #define OD_2112_2_variableNV_Int32_int32                    2
        #define OD_2112_3_variableNV_Int32_int32                    3
        #define OD_2112_4_variableNV_Int32_int32                    4
        #define OD_2112_5_variableNV_Int32_int32                    5
        #define OD_2112_6_variableNV_Int32_int32                    6
        #define OD_2112_7_variableNV_Int32_int32                    7
        #define OD_2112_8_variableNV_Int32_int32                    8
        #define OD_2112_9_variableNV_Int32_int32                    9
        #define OD_2112_10_variableNV_Int32_int32                   10
        #define OD_2112_11_variableNV_Int32_int32                   11
        #define OD_2112_12_variableNV_Int32_int32                   12
        #define OD_2112_13_variableNV_Int32_int32                   13
        #define OD_2112_14_variableNV_Int32_int32                   14
        #define OD_2112_15_variableNV_Int32_int32                   15
        #define OD_2112_16_variableNV_Int32_int32                   16

/*2130 */
        #define OD_2130_time                                        0x2130

        #define OD_2130_0_time_maxSubIndex                          0
        #define OD_2130_1_time_string                               1
        #define OD_2130_2_time_epochTimeBaseMs                      2
        #define OD_2130_3_time_epochTimeOffsetMs                    3

/*3001 */
        #define OD_3001_receiveFile                                 0x3001

        #define OD_3001_0_receiveFile_maxSubIndex                   0
        #define OD_3001_1_receiveFile_fileName                      1
        #define OD_3001_2_receiveFile_fileData                      2
        #define OD_3001_3_receiveFile_saveFile                      3

/*3002 */
        #define OD_3002_sendFile                                    0x3002

        #define OD_3002_0_sendFile_maxSubIndex                      0
        #define OD_3002_1_sendFile_fileName                         1
        #define OD_3002_2_sendFile_fileData                         2
        #define OD_3002_3_sendFile_loadFile                         3
        #define OD_3002_4_sendFile_filesAvalible                    4

/*3100 */
        #define OD_3100_starTrackerStatusControl                    0x3100

        #define OD_3100_0_starTrackerStatusControl_maxSubIndex      0
        #define OD_3100_1_starTrackerStatusControl_currentState     1
        #define OD_3100_2_starTrackerStatusControl_getLatestImage   2

/*3101 */
        #define OD_3101_orienation                                  0x3101

        #define OD_3101_0_orienation_maxSubIndex                    0
        #define OD_3101_1_orienation_rotationY                      1
        #define OD_3101_2_orienation_rotationZ                      2
        #define OD_3101_3_orienation_rotationCamera                 3

/*6000 */
        #define OD_6000_readInput8Bit                               0x6000

        #define OD_6000_0_readInput8Bit_maxSubIndex                 0
        #define OD_6000_1_readInput8Bit_input                       1
        #define OD_6000_2_readInput8Bit_input                       2
        #define OD_6000_3_readInput8Bit_input                       3
        #define OD_6000_4_readInput8Bit_input                       4
        #define OD_6000_5_readInput8Bit_input                       5
        #define OD_6000_6_readInput8Bit_input                       6
        #define OD_6000_7_readInput8Bit_input                       7
        #define OD_6000_8_readInput8Bit_input                       8

/*6200 */
        #define OD_6200_writeOutput8Bit                             0x6200

        #define OD_6200_0_writeOutput8Bit_maxSubIndex               0
        #define OD_6200_1_writeOutput8Bit_output                    1
        #define OD_6200_2_writeOutput8Bit_output                    2
        #define OD_6200_3_writeOutput8Bit_output                    3
        #define OD_6200_4_writeOutput8Bit_output                    4
        #define OD_6200_5_writeOutput8Bit_output                    5
        #define OD_6200_6_writeOutput8Bit_output                    6
        #define OD_6200_7_writeOutput8Bit_output                    7
        #define OD_6200_8_writeOutput8Bit_output                    8

/*6401 */
        #define OD_6401_readAnalogueInput16Bit                      0x6401

        #define OD_6401_0_readAnalogueInput16Bit_maxSubIndex        0
        #define OD_6401_1_readAnalogueInput16Bit_input              1
        #define OD_6401_2_readAnalogueInput16Bit_input              2
        #define OD_6401_3_readAnalogueInput16Bit_input              3
        #define OD_6401_4_readAnalogueInput16Bit_input              4
        #define OD_6401_5_readAnalogueInput16Bit_input              5
        #define OD_6401_6_readAnalogueInput16Bit_input              6
        #define OD_6401_7_readAnalogueInput16Bit_input              7
        #define OD_6401_8_readAnalogueInput16Bit_input              8
        #define OD_6401_9_readAnalogueInput16Bit_input              9
        #define OD_6401_10_readAnalogueInput16Bit_input             10
        #define OD_6401_11_readAnalogueInput16Bit_input             11
        #define OD_6401_12_readAnalogueInput16Bit_input             12

/*6411 */
        #define OD_6411_writeAnalogueOutput16Bit                    0x6411

        #define OD_6411_0_writeAnalogueOutput16Bit_maxSubIndex      0
        #define OD_6411_1_writeAnalogueOutput16Bit_output           1
        #define OD_6411_2_writeAnalogueOutput16Bit_output           2
        #define OD_6411_3_writeAnalogueOutput16Bit_output           3
        #define OD_6411_4_writeAnalogueOutput16Bit_output           4
        #define OD_6411_5_writeAnalogueOutput16Bit_output           5
        #define OD_6411_6_writeAnalogueOutput16Bit_output           6
        #define OD_6411_7_writeAnalogueOutput16Bit_output           7
        #define OD_6411_8_writeAnalogueOutput16Bit_output           8

/*******************************************************************************
   STRUCTURES FOR VARIABLES IN DIFFERENT MEMORY LOCATIONS
*******************************************************************************/
#define  CO_OD_FIRST_LAST_WORD     0x55 //Any value from 0x01 to 0xFE. If changed, EEPROM will be reinitialized.

/***** Structure for ROM variables ********************************************/
struct sCO_OD_ROM{
               UNSIGNED32     FirstWord;


               UNSIGNED32     LastWord;
};

/***** Structure for RAM variables ********************************************/
struct sCO_OD_RAM{
               UNSIGNED32     FirstWord;

/*1000      */ UNSIGNED32      deviceType;
/*1001      */ UNSIGNED8       errorRegister;
/*1002      */ UNSIGNED32      manufacturerStatusRegister;
/*1003      */ UNSIGNED32      preDefinedErrorField[8];
/*1005      */ UNSIGNED32      COB_ID_SYNCMessage;
/*1006      */ UNSIGNED32      communicationCyclePeriod;
/*1007      */ UNSIGNED32      synchronousWindowLength;
/*1008      */ VISIBLE_STRING  manufacturerDeviceName[11];
/*1009      */ VISIBLE_STRING  manufacturerHardwareVersion[4];
/*100a      */ VISIBLE_STRING  manufacturerSoftwareVersion[4];
/*1010      */ UNSIGNED32      storeParameters[1];
/*1011      */ UNSIGNED32      restoreDefaultParameters[1];
/*1014      */ UNSIGNED32      COB_ID_EMCY;
/*1015      */ UNSIGNED16      inhibitTimeEMCY;
/*1016      */ UNSIGNED32      consumerHeartbeatTime[4];
/*1017      */ UNSIGNED16      producerHeartbeatTime;
/*1018      */ OD_identity_t   identity;
/*1019      */ UNSIGNED8       synchronousCounterOverflowValue;
/*1029      */ UNSIGNED8       errorBehavior[6];
/*1200      */ OD_SDOServerParameter_t SDOServerParameter[1];
/*1280      */ OD_SDOClientParameter_t SDOClientParameter[1];
/*1400      */ OD_RPDOCommunicationParameter_t RPDOCommunicationParameter[16];
/*1600      */ OD_RPDOMappingParameter_t RPDOMappingParameter[16];
/*1800      */ OD_TPDOCommunicationParameter_t TPDOCommunicationParameter[16];
/*1a00      */ OD_TPDOMappingParameter_t TPDOMappingParameter[16];
/*1f80      */ UNSIGNED32      NMTStartup;
/*2100      */ OCTET_STRING    errorStatusBits[10];
/*2101      */ UNSIGNED8       CANNodeID;
/*2102      */ UNSIGNED16      CANBitRate;
/*2103      */ UNSIGNED16      SYNCCounter;
/*2104      */ UNSIGNED16      SYNCTime;
/*2106      */ UNSIGNED32      powerOnCounter;
/*2107      */ UNSIGNED16      performance[5];
/*2108      */ INTEGER16       temperature[1];
/*2109      */ INTEGER16       voltage[1];
/*2110      */ INTEGER32       variableInt32[32];
/*2111      */ INTEGER32       variableROM_Int32[16];
/*2112      */ INTEGER32       variableNV_Int32[16];
/*2130      */ OD_time_t       time;
/*3001      */ OD_receiveFile_t receiveFile;
/*3002      */ OD_sendFile_t   sendFile;
/*3100      */ OD_starTrackerStatusControl_t starTrackerStatusControl;
/*3101      */ OD_orienation_t orienation;
/*6000      */ UNSIGNED8       readInput8Bit[8];
/*6200      */ UNSIGNED8       writeOutput8Bit[8];
/*6401      */ INTEGER16       readAnalogueInput16Bit[12];
/*6411      */ INTEGER16       writeAnalogueOutput16Bit[8];

               UNSIGNED32     LastWord;
};

/***** Structure for EEPROM variables ********************************************/
struct sCO_OD_EEPROM{
               UNSIGNED32     FirstWord;


               UNSIGNED32     LastWord;
};

/***** Declaration of Object Dictionary variables *****************************/
extern struct sCO_OD_ROM CO_OD_ROM;

extern struct sCO_OD_RAM CO_OD_RAM;

extern struct sCO_OD_EEPROM CO_OD_EEPROM;

/*******************************************************************************
   ALIASES FOR OBJECT DICTIONARY VARIABLES
*******************************************************************************/
/*1000, Data Type: UNSIGNED32 */
        #define OD_deviceType                                       CO_OD_RAM.deviceType

/*1001, Data Type: UNSIGNED8 */
        #define OD_errorRegister                                    CO_OD_RAM.errorRegister

/*1002, Data Type: UNSIGNED32 */
        #define OD_manufacturerStatusRegister                       CO_OD_RAM.manufacturerStatusRegister

/*1003, Data Type: UNSIGNED32, Array[8] */
        #define OD_preDefinedErrorField                             CO_OD_RAM.preDefinedErrorField
        #define ODL_preDefinedErrorField_arrayLength                8
        #define ODA_preDefinedErrorField_standardErrorField         0

/*1005, Data Type: UNSIGNED32 */
        #define OD_COB_ID_SYNCMessage                               CO_OD_RAM.COB_ID_SYNCMessage

/*1006, Data Type: UNSIGNED32 */
        #define OD_communicationCyclePeriod                         CO_OD_RAM.communicationCyclePeriod

/*1007, Data Type: UNSIGNED32 */
        #define OD_synchronousWindowLength                          CO_OD_RAM.synchronousWindowLength

/*1008, Data Type: VISIBLE_STRING */
        #define OD_manufacturerDeviceName                           CO_OD_RAM.manufacturerDeviceName
        #define ODL_manufacturerDeviceName_stringLength             11

/*1009, Data Type: VISIBLE_STRING */
        #define OD_manufacturerHardwareVersion                      CO_OD_RAM.manufacturerHardwareVersion
        #define ODL_manufacturerHardwareVersion_stringLength        4

/*100a, Data Type: VISIBLE_STRING */
        #define OD_manufacturerSoftwareVersion                      CO_OD_RAM.manufacturerSoftwareVersion
        #define ODL_manufacturerSoftwareVersion_stringLength        4

/*1010, Data Type: UNSIGNED32, Array[1] */
        #define OD_storeParameters                                  CO_OD_RAM.storeParameters
        #define ODL_storeParameters_arrayLength                     1
        #define ODA_storeParameters_saveAllParameters               0

/*1011, Data Type: UNSIGNED32, Array[1] */
        #define OD_restoreDefaultParameters                         CO_OD_RAM.restoreDefaultParameters
        #define ODL_restoreDefaultParameters_arrayLength            1
        #define ODA_restoreDefaultParameters_restoreAllDefaultParameters 0

/*1014, Data Type: UNSIGNED32 */
        #define OD_COB_ID_EMCY                                      CO_OD_RAM.COB_ID_EMCY

/*1015, Data Type: UNSIGNED16 */
        #define OD_inhibitTimeEMCY                                  CO_OD_RAM.inhibitTimeEMCY

/*1016, Data Type: UNSIGNED32, Array[4] */
        #define OD_consumerHeartbeatTime                            CO_OD_RAM.consumerHeartbeatTime
        #define ODL_consumerHeartbeatTime_arrayLength               4
        #define ODA_consumerHeartbeatTime_consumerHeartbeatTime     0

/*1017, Data Type: UNSIGNED16 */
        #define OD_producerHeartbeatTime                            CO_OD_RAM.producerHeartbeatTime

/*1018, Data Type: identity_t */
        #define OD_identity                                         CO_OD_RAM.identity

/*1019, Data Type: UNSIGNED8 */
        #define OD_synchronousCounterOverflowValue                  CO_OD_RAM.synchronousCounterOverflowValue

/*1029, Data Type: UNSIGNED8, Array[6] */
        #define OD_errorBehavior                                    CO_OD_RAM.errorBehavior
        #define ODL_errorBehavior_arrayLength                       6
        #define ODA_errorBehavior_communication                     0
        #define ODA_errorBehavior_communicationOther                1
        #define ODA_errorBehavior_communicationPassive              2
        #define ODA_errorBehavior_generic                           3
        #define ODA_errorBehavior_deviceProfile                     4
        #define ODA_errorBehavior_manufacturerSpecific              5

/*1200, Data Type: SDOServerParameter_t */
        #define OD_SDOServerParameter                               CO_OD_RAM.SDOServerParameter

/*1280, Data Type: SDOClientParameter_t */
        #define OD_SDOClientParameter                               CO_OD_RAM.SDOClientParameter

/*1400, Data Type: RPDOCommunicationParameter_t */
        #define OD_RPDOCommunicationParameter                       CO_OD_RAM.RPDOCommunicationParameter

/*1600, Data Type: RPDOMappingParameter_t */
        #define OD_RPDOMappingParameter                             CO_OD_RAM.RPDOMappingParameter

/*1800, Data Type: TPDOCommunicationParameter_t */
        #define OD_TPDOCommunicationParameter                       CO_OD_RAM.TPDOCommunicationParameter

/*1a00, Data Type: TPDOMappingParameter_t */
        #define OD_TPDOMappingParameter                             CO_OD_RAM.TPDOMappingParameter

/*1f80, Data Type: UNSIGNED32 */
        #define OD_NMTStartup                                       CO_OD_RAM.NMTStartup

/*2100, Data Type: OCTET_STRING */
        #define OD_errorStatusBits                                  CO_OD_RAM.errorStatusBits
        #define ODL_errorStatusBits_stringLength                    10

/*2101, Data Type: UNSIGNED8 */
        #define OD_CANNodeID                                        CO_OD_RAM.CANNodeID

/*2102, Data Type: UNSIGNED16 */
        #define OD_CANBitRate                                       CO_OD_RAM.CANBitRate

/*2103, Data Type: UNSIGNED16 */
        #define OD_SYNCCounter                                      CO_OD_RAM.SYNCCounter

/*2104, Data Type: UNSIGNED16 */
        #define OD_SYNCTime                                         CO_OD_RAM.SYNCTime

/*2106, Data Type: UNSIGNED32 */
        #define OD_powerOnCounter                                   CO_OD_RAM.powerOnCounter

/*2107, Data Type: UNSIGNED16, Array[5] */
        #define OD_performance                                      CO_OD_RAM.performance
        #define ODL_performance_arrayLength                         5
        #define ODA_performance_cyclesPerSecond                     0
        #define ODA_performance_timerCycleTime                      1
        #define ODA_performance_timerCycleMaxTime                   2
        #define ODA_performance_mainCycleTime                       3
        #define ODA_performance_mainCycleMaxTime                    4

/*2108, Data Type: INTEGER16, Array[1] */
        #define OD_temperature                                      CO_OD_RAM.temperature
        #define ODL_temperature_arrayLength                         1
        #define ODA_temperature_mainPCB                             0

/*2109, Data Type: INTEGER16, Array[1] */
        #define OD_voltage                                          CO_OD_RAM.voltage
        #define ODL_voltage_arrayLength                             1
        #define ODA_voltage_mainPCBSupply                           0

/*2110, Data Type: INTEGER32, Array[32] */
        #define OD_variableInt32                                    CO_OD_RAM.variableInt32
        #define ODL_variableInt32_arrayLength                       32
        #define ODA_variableInt32_int32                             0

/*2111, Data Type: INTEGER32, Array[16] */
        #define OD_variableROM_Int32                                CO_OD_RAM.variableROM_Int32
        #define ODL_variableROM_Int32_arrayLength                   16
        #define ODA_variableROM_Int32_int32                         0

/*2112, Data Type: INTEGER32, Array[16] */
        #define OD_variableNV_Int32                                 CO_OD_RAM.variableNV_Int32
        #define ODL_variableNV_Int32_arrayLength                    16
        #define ODA_variableNV_Int32_int32                          0

/*2130, Data Type: time_t */
        #define OD_time                                             CO_OD_RAM.time

/*3001, Data Type: receiveFile_t */
        #define OD_receiveFile                                      CO_OD_RAM.receiveFile

/*3002, Data Type: sendFile_t */
        #define OD_sendFile                                         CO_OD_RAM.sendFile

/*3100, Data Type: starTrackerStatusControl_t */
        #define OD_starTrackerStatusControl                         CO_OD_RAM.starTrackerStatusControl

/*3101, Data Type: orienation_t */
        #define OD_orienation                                       CO_OD_RAM.orienation

/*6000, Data Type: UNSIGNED8, Array[8] */
        #define OD_readInput8Bit                                    CO_OD_RAM.readInput8Bit
        #define ODL_readInput8Bit_arrayLength                       8
        #define ODA_readInput8Bit_input                             0

/*6200, Data Type: UNSIGNED8, Array[8] */
        #define OD_writeOutput8Bit                                  CO_OD_RAM.writeOutput8Bit
        #define ODL_writeOutput8Bit_arrayLength                     8
        #define ODA_writeOutput8Bit_output                          0

/*6401, Data Type: INTEGER16, Array[12] */
        #define OD_readAnalogueInput16Bit                           CO_OD_RAM.readAnalogueInput16Bit
        #define ODL_readAnalogueInput16Bit_arrayLength              12
        #define ODA_readAnalogueInput16Bit_input                    0

/*6411, Data Type: INTEGER16, Array[8] */
        #define OD_writeAnalogueOutput16Bit                         CO_OD_RAM.writeAnalogueOutput16Bit
        #define ODL_writeAnalogueOutput16Bit_arrayLength            8
        #define ODA_writeAnalogueOutput16Bit_output                 0

