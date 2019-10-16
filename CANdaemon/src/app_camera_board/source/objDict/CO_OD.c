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


#include "CO_driver.h"
#include "CO_OD.h"
#include "CO_SDO.h"


/*******************************************************************************
   DEFINITION AND INITIALIZATION OF OBJECT DICTIONARY VARIABLES
*******************************************************************************/


/***** Definition for ROM variables ********************************************/
struct sCO_OD_ROM CO_OD_ROM = {
           CO_OD_FIRST_LAST_WORD,


           CO_OD_FIRST_LAST_WORD,
};


/***** Definition for RAM variables ********************************************/
struct sCO_OD_RAM CO_OD_RAM = {
           CO_OD_FIRST_LAST_WORD,

/*1000*/ 0x0000L,
/*1001*/ 0x0L,
/*1002*/ 0x0000L,
/*1003*/ {0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1005*/ 0x0080L,
/*1006*/ 0x0000L,
/*1007*/ 0x0000L,
/*1008*/ {'C', 'A', 'N', 'o', 'p', 'e', 'n', 'N', 'o', 'd', 'e'},
/*1009*/ {'3', '.', '0', '0'},
/*100a*/ {'3', '.', '0', '0'},
/*1010*/ {0x0003L},
/*1011*/ {0x0001L},
/*1014*/ 0x0080L,
/*1015*/ 0x64,
/*1016*/ {0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1017*/ 0x3e8,
/*1018*/ {0x4L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1019*/ 0x0L,
/*1029*/ {0x0L, 0x0L, 0x1L, 0x0L, 0x0L, 0x0L},
/*1200*/ {{0x2L, 0x0600L, 0x0580L}},
/*1280*/ {{0x3L, 0x0000L, 0x0000L, 0x0L}},
/*1400*/ {{0x2L, 0x0200L, 0xffL},
/*1401*/ {0x2L, 0x0300L, 0xfeL},
/*1402*/ {0x2L, 0x0400L, 0xfeL},
/*1403*/ {0x2L, 0x0500L, 0xfeL},
/*1404*/ {0x2L, 0x80000000L, 0xfeL},
/*1405*/ {0x2L, 0x80000000L, 0xfeL},
/*1406*/ {0x2L, 0x80000000L, 0xfeL},
/*1407*/ {0x2L, 0x80000000L, 0xfeL},
/*1408*/ {0x2L, 0x80000000L, 0xfeL},
/*1409*/ {0x2L, 0x80000000L, 0xfeL},
/*140a*/ {0x2L, 0x80000000L, 0xfeL},
/*140b*/ {0x2L, 0x80000000L, 0xfeL},
/*140c*/ {0x2L, 0x80000000L, 0xfeL},
/*140d*/ {0x2L, 0x80000000L, 0xfeL},
/*140e*/ {0x2L, 0x80000000L, 0xfeL},
/*140f*/ {0x2L, 0x80000000L, 0xfeL}},
/*1600*/ {{0x2L, 0x62000108L, 0x62000208L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1601*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1602*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1603*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1604*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1605*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1606*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1607*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1608*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1609*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160a*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160b*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160c*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160d*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160e*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*160f*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L}},
/*1800*/ {{0x6L, 0x0180L, 0xffL, 0x64, 0x0L, 0x00, 0x0L},
/*1801*/ {0x6L, 0x0280L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1802*/ {0x6L, 0x0380L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1803*/ {0x6L, 0x0480L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1804*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1805*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1806*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1807*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1808*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*1809*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180a*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180b*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180c*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180d*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180e*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L},
/*180f*/ {0x6L, 0x80000000L, 0xfeL, 0x00, 0x0L, 0x00, 0x0L}},
/*1a00*/ {{0x2L, 0x60000108L, 0x60000208L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a01*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a02*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a03*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a04*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a05*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a06*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a07*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a08*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a09*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0a*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0b*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0c*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0d*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0e*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*1a0f*/ {0x0L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L}},
/*1f80*/ 0x0001L,
/*2100*/ {0x0L},
/*2101*/ 0x12L,
/*2102*/ 0x3e8,
/*2103*/ 0x00,
/*2104*/ 0x00,
/*2106*/ 0x0000L,
/*2107*/ {0x3e8, 0x00, 0x00, 0x00, 0x00},
/*2108*/ {0x00},
/*2109*/ {0x00},
/*2110*/ {0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*2111*/ {0x0001L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*2112*/ {0x0001L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L, 0x0000L},
/*2130*/ {0x3L, {'-'}, 0x00000000L, 0x0000L},
/*3001*/ {0x3L, 0, 0, 0x0L},
/*3002*/ {0x4L, 0, 0, 0x0L, 0x0L},
/*3100*/ 0x0,
/*6000*/ {0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
/*6200*/ {0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
/*6401*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
/*6411*/ {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},

           CO_OD_FIRST_LAST_WORD,
};


/***** Definition for EEPROM variables ********************************************/
struct sCO_OD_EEPROM CO_OD_EEPROM = {
           CO_OD_FIRST_LAST_WORD,


           CO_OD_FIRST_LAST_WORD,
};




/*******************************************************************************
   STRUCTURES FOR RECORD TYPE OBJECTS
*******************************************************************************/


/*0x1018*/ const CO_OD_entryRecord_t OD_record1018[5] = {
           {(void*)&CO_OD_RAM.identity.maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.identity.vendorID, 0x86, 0x4 },
           {(void*)&CO_OD_RAM.identity.productCode, 0x86, 0x4 },
           {(void*)&CO_OD_RAM.identity.revisionNumber, 0x86, 0x4 },
           {(void*)&CO_OD_RAM.identity.serialNumber, 0x86, 0x4 },
};

/*0x1200*/ const CO_OD_entryRecord_t OD_record1200[3] = {
           {(void*)&CO_OD_RAM.SDOServerParameter[0].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.SDOServerParameter[0].COB_IDClientToServer, 0x86, 0x4 },
           {(void*)&CO_OD_RAM.SDOServerParameter[0].COB_IDServerToClient, 0x86, 0x4 },
};

/*0x1280*/ const CO_OD_entryRecord_t OD_record1280[4] = {
           {(void*)&CO_OD_RAM.SDOClientParameter[0].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].COB_IDClientToServer, 0x9e, 0x4 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].COB_IDServerToClient, 0x9e, 0x4 },
           {(void*)&CO_OD_RAM.SDOClientParameter[0].nodeIDOfTheSDOServer, 0x0e, 0x1 },
};

/*0x1400*/ const CO_OD_entryRecord_t OD_record1400[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[0].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[0].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[0].transmissionType, 0x0e, 0x1 },
};

/*0x1401*/ const CO_OD_entryRecord_t OD_record1401[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[1].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[1].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[1].transmissionType, 0x0e, 0x1 },
};

/*0x1402*/ const CO_OD_entryRecord_t OD_record1402[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[2].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[2].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[2].transmissionType, 0x0e, 0x1 },
};

/*0x1403*/ const CO_OD_entryRecord_t OD_record1403[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[3].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[3].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[3].transmissionType, 0x0e, 0x1 },
};

/*0x1404*/ const CO_OD_entryRecord_t OD_record1404[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[4].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[4].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[4].transmissionType, 0x0e, 0x1 },
};

/*0x1405*/ const CO_OD_entryRecord_t OD_record1405[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[5].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[5].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[5].transmissionType, 0x0e, 0x1 },
};

/*0x1406*/ const CO_OD_entryRecord_t OD_record1406[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[6].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[6].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[6].transmissionType, 0x0e, 0x1 },
};

/*0x1407*/ const CO_OD_entryRecord_t OD_record1407[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[7].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[7].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[7].transmissionType, 0x0e, 0x1 },
};

/*0x1408*/ const CO_OD_entryRecord_t OD_record1408[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[8].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[8].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[8].transmissionType, 0x0e, 0x1 },
};

/*0x1409*/ const CO_OD_entryRecord_t OD_record1409[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[9].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[9].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[9].transmissionType, 0x0e, 0x1 },
};

/*0x140a*/ const CO_OD_entryRecord_t OD_record140a[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[10].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[10].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[10].transmissionType, 0x0e, 0x1 },
};

/*0x140b*/ const CO_OD_entryRecord_t OD_record140b[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[11].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[11].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[11].transmissionType, 0x0e, 0x1 },
};

/*0x140c*/ const CO_OD_entryRecord_t OD_record140c[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[12].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[12].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[12].transmissionType, 0x0e, 0x1 },
};

/*0x140d*/ const CO_OD_entryRecord_t OD_record140d[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[13].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[13].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[13].transmissionType, 0x0e, 0x1 },
};

/*0x140e*/ const CO_OD_entryRecord_t OD_record140e[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[14].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[14].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[14].transmissionType, 0x0e, 0x1 },
};

/*0x140f*/ const CO_OD_entryRecord_t OD_record140f[3] = {
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[15].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[15].COB_IDUsedByRPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOCommunicationParameter[15].transmissionType, 0x0e, 0x1 },
};

/*0x1600*/ const CO_OD_entryRecord_t OD_record1600[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[0].mappedObject8, 0x8e, 0x4 },
};

/*0x1601*/ const CO_OD_entryRecord_t OD_record1601[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[1].mappedObject8, 0x8e, 0x4 },
};

/*0x1602*/ const CO_OD_entryRecord_t OD_record1602[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[2].mappedObject8, 0x8e, 0x4 },
};

/*0x1603*/ const CO_OD_entryRecord_t OD_record1603[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[3].mappedObject8, 0x8e, 0x4 },
};

/*0x1604*/ const CO_OD_entryRecord_t OD_record1604[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[4].mappedObject8, 0x8e, 0x4 },
};

/*0x1605*/ const CO_OD_entryRecord_t OD_record1605[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[5].mappedObject8, 0x8e, 0x4 },
};

/*0x1606*/ const CO_OD_entryRecord_t OD_record1606[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[6].mappedObject8, 0x8e, 0x4 },
};

/*0x1607*/ const CO_OD_entryRecord_t OD_record1607[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[7].mappedObject8, 0x8e, 0x4 },
};

/*0x1608*/ const CO_OD_entryRecord_t OD_record1608[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[8].mappedObject8, 0x8e, 0x4 },
};

/*0x1609*/ const CO_OD_entryRecord_t OD_record1609[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[9].mappedObject8, 0x8e, 0x4 },
};

/*0x160a*/ const CO_OD_entryRecord_t OD_record160a[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[10].mappedObject8, 0x8e, 0x4 },
};

/*0x160b*/ const CO_OD_entryRecord_t OD_record160b[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[11].mappedObject8, 0x8e, 0x4 },
};

/*0x160c*/ const CO_OD_entryRecord_t OD_record160c[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[12].mappedObject8, 0x8e, 0x4 },
};

/*0x160d*/ const CO_OD_entryRecord_t OD_record160d[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[13].mappedObject8, 0x8e, 0x4 },
};

/*0x160e*/ const CO_OD_entryRecord_t OD_record160e[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[14].mappedObject8, 0x8e, 0x4 },
};

/*0x160f*/ const CO_OD_entryRecord_t OD_record160f[9] = {
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.RPDOMappingParameter[15].mappedObject8, 0x8e, 0x4 },
};

/*0x1800*/ const CO_OD_entryRecord_t OD_record1800[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[0].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1801*/ const CO_OD_entryRecord_t OD_record1801[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[1].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1802*/ const CO_OD_entryRecord_t OD_record1802[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[2].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1803*/ const CO_OD_entryRecord_t OD_record1803[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[3].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1804*/ const CO_OD_entryRecord_t OD_record1804[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[4].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1805*/ const CO_OD_entryRecord_t OD_record1805[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[5].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1806*/ const CO_OD_entryRecord_t OD_record1806[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[6].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1807*/ const CO_OD_entryRecord_t OD_record1807[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[7].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1808*/ const CO_OD_entryRecord_t OD_record1808[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[8].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1809*/ const CO_OD_entryRecord_t OD_record1809[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[9].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180a*/ const CO_OD_entryRecord_t OD_record180a[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[10].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180b*/ const CO_OD_entryRecord_t OD_record180b[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[11].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180c*/ const CO_OD_entryRecord_t OD_record180c[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[12].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180d*/ const CO_OD_entryRecord_t OD_record180d[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[13].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180e*/ const CO_OD_entryRecord_t OD_record180e[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[14].SYNCStartValue, 0x0e, 0x1 },
};

/*0x180f*/ const CO_OD_entryRecord_t OD_record180f[7] = {
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].COB_IDUsedByTPDO, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].transmissionType, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].inhibitTime, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].compatibilityEntry, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].eventTimer, 0x8e, 0x2 },
           {(void*)&CO_OD_RAM.TPDOCommunicationParameter[15].SYNCStartValue, 0x0e, 0x1 },
};

/*0x1a00*/ const CO_OD_entryRecord_t OD_record1a00[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[0].mappedObject8, 0x8e, 0x4 },
};

/*0x1a01*/ const CO_OD_entryRecord_t OD_record1a01[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[1].mappedObject8, 0x8e, 0x4 },
};

/*0x1a02*/ const CO_OD_entryRecord_t OD_record1a02[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[2].mappedObject8, 0x8e, 0x4 },
};

/*0x1a03*/ const CO_OD_entryRecord_t OD_record1a03[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[3].mappedObject8, 0x8e, 0x4 },
};

/*0x1a04*/ const CO_OD_entryRecord_t OD_record1a04[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[4].mappedObject8, 0x8e, 0x4 },
};

/*0x1a05*/ const CO_OD_entryRecord_t OD_record1a05[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[5].mappedObject8, 0x8e, 0x4 },
};

/*0x1a06*/ const CO_OD_entryRecord_t OD_record1a06[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[6].mappedObject8, 0x8e, 0x4 },
};

/*0x1a07*/ const CO_OD_entryRecord_t OD_record1a07[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[7].mappedObject8, 0x8e, 0x4 },
};

/*0x1a08*/ const CO_OD_entryRecord_t OD_record1a08[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[8].mappedObject8, 0x8e, 0x4 },
};

/*0x1a09*/ const CO_OD_entryRecord_t OD_record1a09[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[9].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0a*/ const CO_OD_entryRecord_t OD_record1a0a[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[10].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0b*/ const CO_OD_entryRecord_t OD_record1a0b[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[11].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0c*/ const CO_OD_entryRecord_t OD_record1a0c[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[12].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0d*/ const CO_OD_entryRecord_t OD_record1a0d[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[13].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0e*/ const CO_OD_entryRecord_t OD_record1a0e[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[14].mappedObject8, 0x8e, 0x4 },
};

/*0x1a0f*/ const CO_OD_entryRecord_t OD_record1a0f[9] = {
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].numberOfMappedObjects, 0x0e, 0x1 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject1, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject2, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject3, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject4, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject5, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject6, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject7, 0x8e, 0x4 },
           {(void*)&CO_OD_RAM.TPDOMappingParameter[15].mappedObject8, 0x8e, 0x4 },
};

/*0x2130*/ const CO_OD_entryRecord_t OD_record2130[4] = {
           {(void*)&CO_OD_RAM.time.maxSubIndex, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.time.string, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.time.epochTimeBaseMs, 0x86, 0x8 },
           {(void*)&CO_OD_RAM.time.epochTimeOffsetMs, 0x9e, 0x4 },
};

/*0x3001*/ const CO_OD_entryRecord_t OD_record3001[4] = {
           {(void*)&CO_OD_RAM.receiveFile.maxSubIndex, 0x06, 0x1 },
           {(void*)0, 0x0a, 0x0 },
           {(void*)0, 0x0a, 0x0 },
           {(void*)&CO_OD_RAM.receiveFile.saveFile, 0x0a, 0x1 },
};

/*0x3002*/ const CO_OD_entryRecord_t OD_record3002[5] = {
           {(void*)&CO_OD_RAM.sendFile.maxSubIndex, 0x06, 0x1 },
           {(void*)0, 0x06, 0x0 },
           {(void*)0, 0x06, 0x0 },
           {(void*)&CO_OD_RAM.sendFile.loadFile, 0x06, 0x1 },
           {(void*)&CO_OD_RAM.sendFile.filesAvalible, 0x06, 0x1 },
};

/*******************************************************************************
   OBJECT DICTIONARY
*******************************************************************************/
const CO_OD_entry_t CO_OD[106] = {

{0x1000, 0x00, 0x86, 4, (void*)&CO_OD_RAM.deviceType},
{0x1001, 0x00, 0x26, 1, (void*)&CO_OD_RAM.errorRegister},
{0x1002, 0x00, 0xa6, 4, (void*)&CO_OD_RAM.manufacturerStatusRegister},
{0x1003, 0x08, 0x8e, 4, (void*)&CO_OD_RAM.preDefinedErrorField[0]},
{0x1005, 0x00, 0x8e, 4, (void*)&CO_OD_RAM.COB_ID_SYNCMessage},
{0x1006, 0x00, 0x8e, 4, (void*)&CO_OD_RAM.communicationCyclePeriod},
{0x1007, 0x00, 0x8e, 4, (void*)&CO_OD_RAM.synchronousWindowLength},
{0x1008, 0x00, 0x06, 11, (void*)&CO_OD_RAM.manufacturerDeviceName},
{0x1009, 0x00, 0x06, 4, (void*)&CO_OD_RAM.manufacturerHardwareVersion},
{0x100a, 0x00, 0x06, 4, (void*)&CO_OD_RAM.manufacturerSoftwareVersion},
{0x1010, 0x01, 0x8e, 4, (void*)&CO_OD_RAM.storeParameters[0]},
{0x1011, 0x01, 0x8e, 4, (void*)&CO_OD_RAM.restoreDefaultParameters[0]},
{0x1014, 0x00, 0x86, 4, (void*)&CO_OD_RAM.COB_ID_EMCY},
{0x1015, 0x00, 0x8e, 2, (void*)&CO_OD_RAM.inhibitTimeEMCY},
{0x1016, 0x04, 0x8e, 4, (void*)&CO_OD_RAM.consumerHeartbeatTime[0]},
{0x1017, 0x00, 0x8e, 2, (void*)&CO_OD_RAM.producerHeartbeatTime},
{0x1018, 0x04, 0x00, 0, (void*)&OD_record1018},
{0x1019, 0x00, 0x0e, 1, (void*)&CO_OD_RAM.synchronousCounterOverflowValue},
{0x1029, 0x06, 0x0e, 1, (void*)&CO_OD_RAM.errorBehavior[0]},
{0x1200, 0x02, 0x00, 0, (void*)&OD_record1200},
{0x1280, 0x03, 0x00, 0, (void*)&OD_record1280},
{0x1400, 0x02, 0x00, 0, (void*)&OD_record1400},
{0x1401, 0x02, 0x00, 0, (void*)&OD_record1401},
{0x1402, 0x02, 0x00, 0, (void*)&OD_record1402},
{0x1403, 0x02, 0x00, 0, (void*)&OD_record1403},
{0x1404, 0x02, 0x00, 0, (void*)&OD_record1404},
{0x1405, 0x02, 0x00, 0, (void*)&OD_record1405},
{0x1406, 0x02, 0x00, 0, (void*)&OD_record1406},
{0x1407, 0x02, 0x00, 0, (void*)&OD_record1407},
{0x1408, 0x02, 0x00, 0, (void*)&OD_record1408},
{0x1409, 0x02, 0x00, 0, (void*)&OD_record1409},
{0x140a, 0x02, 0x00, 0, (void*)&OD_record140a},
{0x140b, 0x02, 0x00, 0, (void*)&OD_record140b},
{0x140c, 0x02, 0x00, 0, (void*)&OD_record140c},
{0x140d, 0x02, 0x00, 0, (void*)&OD_record140d},
{0x140e, 0x02, 0x00, 0, (void*)&OD_record140e},
{0x140f, 0x02, 0x00, 0, (void*)&OD_record140f},
{0x1600, 0x08, 0x00, 0, (void*)&OD_record1600},
{0x1601, 0x08, 0x00, 0, (void*)&OD_record1601},
{0x1602, 0x08, 0x00, 0, (void*)&OD_record1602},
{0x1603, 0x08, 0x00, 0, (void*)&OD_record1603},
{0x1604, 0x08, 0x00, 0, (void*)&OD_record1604},
{0x1605, 0x08, 0x00, 0, (void*)&OD_record1605},
{0x1606, 0x08, 0x00, 0, (void*)&OD_record1606},
{0x1607, 0x08, 0x00, 0, (void*)&OD_record1607},
{0x1608, 0x08, 0x00, 0, (void*)&OD_record1608},
{0x1609, 0x08, 0x00, 0, (void*)&OD_record1609},
{0x160a, 0x08, 0x00, 0, (void*)&OD_record160a},
{0x160b, 0x08, 0x00, 0, (void*)&OD_record160b},
{0x160c, 0x08, 0x00, 0, (void*)&OD_record160c},
{0x160d, 0x08, 0x00, 0, (void*)&OD_record160d},
{0x160e, 0x08, 0x00, 0, (void*)&OD_record160e},
{0x160f, 0x08, 0x00, 0, (void*)&OD_record160f},
{0x1800, 0x06, 0x00, 0, (void*)&OD_record1800},
{0x1801, 0x06, 0x00, 0, (void*)&OD_record1801},
{0x1802, 0x06, 0x00, 0, (void*)&OD_record1802},
{0x1803, 0x06, 0x00, 0, (void*)&OD_record1803},
{0x1804, 0x06, 0x00, 0, (void*)&OD_record1804},
{0x1805, 0x06, 0x00, 0, (void*)&OD_record1805},
{0x1806, 0x06, 0x00, 0, (void*)&OD_record1806},
{0x1807, 0x06, 0x00, 0, (void*)&OD_record1807},
{0x1808, 0x06, 0x00, 0, (void*)&OD_record1808},
{0x1809, 0x06, 0x00, 0, (void*)&OD_record1809},
{0x180a, 0x06, 0x00, 0, (void*)&OD_record180a},
{0x180b, 0x06, 0x00, 0, (void*)&OD_record180b},
{0x180c, 0x06, 0x00, 0, (void*)&OD_record180c},
{0x180d, 0x06, 0x00, 0, (void*)&OD_record180d},
{0x180e, 0x06, 0x00, 0, (void*)&OD_record180e},
{0x180f, 0x06, 0x00, 0, (void*)&OD_record180f},
{0x1a00, 0x08, 0x00, 0, (void*)&OD_record1a00},
{0x1a01, 0x08, 0x00, 0, (void*)&OD_record1a01},
{0x1a02, 0x08, 0x00, 0, (void*)&OD_record1a02},
{0x1a03, 0x08, 0x00, 0, (void*)&OD_record1a03},
{0x1a04, 0x08, 0x00, 0, (void*)&OD_record1a04},
{0x1a05, 0x08, 0x00, 0, (void*)&OD_record1a05},
{0x1a06, 0x08, 0x00, 0, (void*)&OD_record1a06},
{0x1a07, 0x08, 0x00, 0, (void*)&OD_record1a07},
{0x1a08, 0x08, 0x00, 0, (void*)&OD_record1a08},
{0x1a09, 0x08, 0x00, 0, (void*)&OD_record1a09},
{0x1a0a, 0x08, 0x00, 0, (void*)&OD_record1a0a},
{0x1a0b, 0x08, 0x00, 0, (void*)&OD_record1a0b},
{0x1a0c, 0x08, 0x00, 0, (void*)&OD_record1a0c},
{0x1a0d, 0x08, 0x00, 0, (void*)&OD_record1a0d},
{0x1a0e, 0x08, 0x00, 0, (void*)&OD_record1a0e},
{0x1a0f, 0x08, 0x00, 0, (void*)&OD_record1a0f},
{0x1f80, 0x00, 0x8e, 4, (void*)&CO_OD_RAM.NMTStartup},
{0x2100, 0x00, 0x26, 10, (void*)&CO_OD_RAM.errorStatusBits},
{0x2101, 0x00, 0x0e, 1, (void*)&CO_OD_RAM.CANNodeID},
{0x2102, 0x00, 0x8e, 2, (void*)&CO_OD_RAM.CANBitRate},
{0x2103, 0x00, 0x8e, 2, (void*)&CO_OD_RAM.SYNCCounter},
{0x2104, 0x00, 0x86, 2, (void*)&CO_OD_RAM.SYNCTime},
{0x2106, 0x00, 0x86, 4, (void*)&CO_OD_RAM.powerOnCounter},
{0x2107, 0x05, 0x8e, 2, (void*)&CO_OD_RAM.performance[0]},
{0x2108, 0x01, 0x8e, 2, (void*)&CO_OD_RAM.temperature[0]},
{0x2109, 0x01, 0x8e, 2, (void*)&CO_OD_RAM.voltage[0]},
{0x2110, 0x20, 0x8e, 4, (void*)&CO_OD_RAM.variableInt32[0]},
{0x2111, 0x10, 0x8e, 4, (void*)&CO_OD_RAM.variableROM_Int32[0]},
{0x2112, 0x10, 0x8e, 4, (void*)&CO_OD_RAM.variableNV_Int32[0]},
{0x2130, 0x03, 0x00, 0, (void*)&OD_record2130},
{0x3001, 0x03, 0x00, 0, (void*)&OD_record3001},
{0x3002, 0x04, 0x00, 0, (void*)&OD_record3002},
{0x3100, 0x00, 0x0e, 1, (void*)&CO_OD_RAM.getLatestImage},
{0x6000, 0x08, 0x0e, 1, (void*)&CO_OD_RAM.readInput8Bit[0]},
{0x6200, 0x08, 0x0e, 1, (void*)&CO_OD_RAM.writeOutput8Bit[0]},
{0x6401, 0x0c, 0x8e, 2, (void*)&CO_OD_RAM.readAnalogueInput16Bit[0]},
{0x6411, 0x08, 0x8e, 2, (void*)&CO_OD_RAM.writeAnalogueOutput16Bit[0]},
};
