/*
************************************************************************************************************************
*                                                      eNand
*                                     Nand flash driver logic control module define
*
*                             Copyright(C), 2008-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : nand_logic.h
*
* Author : Kevin.z
*
* Version : v0.1
*
* Date : 2008.03.25
*
* Description : This file define the function interface and some data structure export
*               for the logical control module.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Kevin.z         2008.03.25      0.1          build the file
*
************************************************************************************************************************
*/
#ifndef __NAND_LOGIC_H__
#define __NAND_LOGIC_H__

#include "nand_type.h"
#include "nand_physic.h"



#define SECTOR_CNT_OF_LOGIC_PAGE    SECTOR_CNT_OF_SUPER_PAGE

#define FULL_BITMAP_OF_LOGIC_PAGE   FULL_BITMAP_OF_SUPER_PAGE

#define PAGE_CNT_OF_LOGIC_BLK       (NandDriverInfo.LogicalArchitecture->PageCntPerLogicBlk)

#define PAGE_CNT_OF_SUPER_BLK       PAGE_CNT_OF_LOGIC_BLK

#define DATA_BLK_CNT_OF_ZONE        (NandDriverInfo.LogicalArchitecture->LogicBlkCntPerZone)

#define FREE_BLK_CNT_OF_ZONE        (BLOCK_CNT_OF_ZONE - DATA_BLK_CNT_OF_ZONE - 1)


#define LOG_BLK_CNT_OF_ZONE         MAX_LOG_BLK_CNT

#define FREE_BLK_CNT_OF_ZONE        (BLOCK_CNT_OF_ZONE - DATA_BLK_CNT_OF_ZONE - 1)

#define ZONE_CNT_OF_DIE             (NandDriverInfo.LogicalArchitecture->ZoneCntPerDie)

#define INTERLEAVE_BANK_CNT         (PAGE_CNT_OF_LOGIC_BLK / NandDriverInfo.NandStorageInfo->PageCntPerPhyBlk)


#define LML_PROCESS_TBL_BUF         (NandDriverInfo.PageCachePool->PageCache2)

#define LML_TEMP_BUF                (NandDriverInfo.PageCachePool->PageCache0)
#define LML_WRITE_PAGE_CACHE        (NandDriverInfo.PageCachePool->PageCache1)

#define LML_SPARE_BUF               (NandDriverInfo.PageCachePool->SpareCache)


#define BLK_MAP_CACHE_POOL          (NandDriverInfo.BlkMapTblCachePool)

#define BLK_ERASE_CNTER             (BLK_MAP_CACHE_POOL->SuperBlkEraseCnt)

#define LOG_ACCESS_TIMER            (BLK_MAP_CACHE_POOL->LogBlkAccessTimer)

#define LOG_ACCESS_AGE              (BLK_MAP_CACHE_POOL->LogBlkAccessAge)

#define BLK_MAP_CACHE               (BLK_MAP_CACHE_POOL->ActBlkMapTbl)

#define LAST_FREE_BLK_PST           (BLK_MAP_CACHE->LastFreeBlkPst)

#define DATA_BLK_TBL                (BLK_MAP_CACHE->DataBlkTbl)

#define LOG_BLK_TBL                 (BLK_MAP_CACHE->LogBlkTbl)

#define FREE_BLK_TBL                (BLK_MAP_CACHE->FreeBlkTbl)

#define CUR_MAP_ZONE                (BLK_MAP_CACHE->ZoneNum)

#define PAGE_MAP_CACHE_POOL         (NandDriverInfo.PageMapTblCachePool)

#define PAGE_MAP_CACHE              (NandDriverInfo.PageMapTblCachePool->ActPageMapTbl)

#define PAGE_MAP_TBL                (PAGE_MAP_CACHE->PageMapTbl)

#define NORMAL_MERGE_MODE       0x00               
#define SPECIAL_MERGE_MODE      0x01               

#define INVALID_PAGE_NUM        0xffff

#define LOWEST_EC_TYPE          0x00
#define HIGHEST_EC_TYPE         0x01



/*
************************************************************************************************************************
*                           INITIATE NAND FLASH LOGIC MANAGE LAYER
*
*Description: initiate the logic manage layer for nand flash driver.
*
*Arguments  : none;
*
*Return     : intiate result;
*               = 0     init successful;
*               = -1    init failed.
************************************************************************************************************************
*/
__s32 LML_Init(void);


/*
************************************************************************************************************************
*                           NAND FLASH LOGIC MANAGE LAYER EXIT
*
*Description: exit nand flash logic manage layer.
*
*Arguments  : none;
*
*Return     : exit result;
*               = 0     exit successfu;
*               = -1    exit failed.
************************************************************************************************************************
*/
__s32 LML_Exit(void);


/*
************************************************************************************************************************
*                           NAND FLASH LOGIC MANAGE LAYER READ
*
*Description: Read data from logic disk area to buffer.
*
*Arguments  : nLba      the logic block address on the logic area from where to read;
*             nLength   the size of the data need be read, based on sector;
*             pBuf      the pointer to the buffer where will store the data readout of nand.
*
*Return     : read result;
*               = 0     read successful;
*               = -1    read failed.
************************************************************************************************************************
*/
__s32 LML_Read(__u32 nLba, __u32 nLength, void* pBuf);


/*
************************************************************************************************************************
*                           NAND FLASH LOGIC MANAGE LAYER WRITE
*
*Description: Write data from buffer to logic disk area.
*
*Arguments  : nLba      the logic block address on the logic area from where to write;
*             nLength   the size of the data need to be write, based on sector;
*             pBuf      the pointer to the buffer where stored the data write to nand flash.
*
*Return     : write result;
*               = 0     write successful;
*               = -1    write failed.
************************************************************************************************************************
*/
__s32 LML_Write(__u32 nLba, __u32 nLength, void* pBuf);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER FLUSH PAGE CACHE
*
*Description: Flush the data in the cache buffer to nand flash.
*
*Arguments  : none

*Return     : flush result;
*               = 0     flush successful;
*               = -1    flush failed.
************************************************************************************************************************
*/
__s32 LML_FlushPageCache(void);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER PAGE READ
*
*Description: Read data from logic disk to buffer based page.
*
*Arguments  : nPage     the page address which need be read;
*             nBitmap   the bitmap of the sectors in the page which need be read data;
*             pBuf      the pointer to the buffer where will store the data read out.
*
*Return     : page read result;
*               = 0     read successful;
*               > 0     read successful, but need do some process;
*               < 0     read failed.
************************************************************************************************************************
*/
__s32 LML_PageRead(__u32 nPage, __u32 nBitmap, void* pBuf);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER PAGE WRITE
*
*Description: Write data from buffer to logic area based on page.
*
*Arguments  : nPage     the page address which need be write;
*             nBitmap   the bitmap of sectors in the page which need be write, it is always full;
*             pBuf      the pointer to the buffer where is storing the data.
*
*Return     : write result;
*               = 0     write successful;
*               > 0     write successful, but need do some process;
*               < 0     write failed.
************************************************************************************************************************
*/
__s32 LML_PageWrite(__u32 nPage, __u32 nBitmap, void* pBuf);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER READ-RECLAIM
*
*Description: Repair the logic block whose data has reach the limit of the ability of
*             the HW ECC module correct.
*
*Arguments  : nPage     the page address where need be repaired.
*
*Return     : read-reclaim result;
*               = 0     do read-reclaim successful;
*               = -1    do read-reclaim failed.
*
*Notes      : if read a physical page millions of times, there may be some bit error in
*             the page, and the error bit number will increase along with the read times,
*             so, if the number of the error bit reachs the ECC limit, the data should be
*             read out and write to another physical blcok.
************************************************************************************************************************
*/
__s32 LML_ReadReclaim(__u32 nPage);


/*
************************************************************************************************************************
*                   NAND FLASH LOGIC MANAGE LAYER WEAR-LEVELLING
*
*Description: Equate the erase cycles among all physical blocks.
*
*Arguments  : none
*
*Return     : do wear-levelling result;
*               = 0     do wear-levelling successful;
*               = -1    do wear-levelling failed.
*
*Notes      : The erase cycle of a physical block is limited, if the erase cycle overun this
*             limit, the physical block may be invalid. so a policy is needed to equate the
*             millions of erase cycles to ervery physical block.
************************************************************************************************************************
*/
__s32 LML_WearLevelling(void);


/*
************************************************************************************************************************
*                   NAND FLASH LOGIC MANAGE LAYER CALCULATE PHYSICAL ADDRESS
*
*Description: Calculate the physical address parameter.
*
*Arguments  : pPhyPar   the pointer to the physical address parameter;
*             nZone     the zone number which the superblock belonged to;
*             nBlock    the number of the super block in a DIE;
*             nPage     the page number in the super block.
*
*Return     : calculate result;
*               = 0     calculate successful;
*               = -1    calculate failed.
************************************************************************************************************************
*/
__s32 LML_CalPhyPar(struct __PhysicOpPara_t *pPhyPar, __u32 nZone, __u32 nBlock, __u32 nPage);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER BAD BLOCK MANAGE
*
*Description: Nand flash bad block manage.
*
*Arguments  : pBadBlk   the pointer to the bad physical block parameter;
*             nZoneNum  the number of the zone which the bad block belonged to;
*             nErrPage  the number of the error page;
*             pNewBlk   the pointer to the new valid block parameter.
*
*Return     : bad block manage result;
*               = 0     do bad block manage successful;
*               = -1    do bad block manage failed.
************************************************************************************************************************
*/
__s32 LML_BadBlkManage(struct __SuperPhyBlkType_t *pBadBlk, __u32 nZoneNum, __u32 nErrPage, struct __SuperPhyBlkType_t *pNewBlk);


/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER MERGE LOG BLOCK
*
*Description: Merge the log block whoes mapping table is active.
*
*Arguments  : nMode         the type of the merge;
*                           = 0     normal merge, the log block  table is not full;
*                           = 1     special merge, the log block table is full;
*             nLogicBlk     the number of the logical block, which need be merged.
*
*Return     : merge result;
*               = 0     merge log successful;
*               = -1    do bad block manage failed.
************************************************************************************************************************
*/
__s32 LML_MergeLogBlk(__u32 nMode, __u32 nLogicBlk);

/*
************************************************************************************************************************
*                       NAND FLASH LOGIC MANAGE LAYER ERASE SUPER BLOCK
*
*Description: Erase the given super block.
*
*Arguments  : nZone         the number of the zone which the super block belonged to;
*             nSuperBlk     the number of the super block which need be erased.
*
*Return     : erase result
*               = 0     super block erase successful;
*               =-1     super block erase failed.
************************************************************************************************************************
*/
__s32 LML_SuperBlkErase(__u32 nZone, __u32 nSuperBlk);


/*
************************************************************************************************************************
*                       CALCULATE PHYSICAL OPERATION PARAMETER
*
*Description: Calculate the paramter for physical operation with the number of zone, number of
*             super block and number of page in the super block.
*
*Arguments  : pPhyPar   the pointer to the physical operation parameter;
*             nZone     the number of the zone which the super block blonged to;
*             nBlock    the number of the super block;
*             nPage     the number of the super page in the super block.
*
*Return     : calculate parameter result;
*               = 0     calculate parameter successful;
*               < 0     calcualte parameter failed.
************************************************************************************************************************
*/
__s32 LML_CalculatePhyOpPar(struct __PhysicOpPara_t *pPhyPar, __u32 nZone, __u32 nBlock, __u32 nPage);


/*
************************************************************************************************************************
*                       INIT BLOCK MAPPING TABLE CACHE
*
*Description: Initiate block mapping talbe cache.
*
*Arguments  : none.
*
*Return     : init result;
*               = 0     init successful;
*               = -1    init failed.
************************************************************************************************************************
*/
__s32 BMM_InitMapTblCache(void);


/*
************************************************************************************************************************
*                       BLOCK MAPPING TABLE CACHE EXIT
*
*Description: exit block mapping table cache.
*
*Arguments  : none.
*
*Return     : exit result;
*               = 0     exit successful;
*               = -1    exit failed.
************************************************************************************************************************
*/
__s32 BMM_ExitMapTblCache(void);


/*
************************************************************************************************************************
*                       SWITCH BLOCK MAPPING TABLE
*
*Description: Switch block mapping table.
*
*Arguments  : nZone     zone number which block mapping table need be accessed.
*
*Return     : switch result;
*               = 0     switch successful;
*               = -1    switch failed.
************************************************************************************************************************
*/
__s32 BMM_SwitchMapTbl(__u32 nZone);


/*
************************************************************************************************************************
*                           WRITE BACK ALL MAPPING TABLE
*
*Description: Write back all mapping table.
*
*Arguments  : none.
*
*Return     : write table result;
*               = 0     write successful;
*               = -1    write failed.
************************************************************************************************************************
*/
__s32 BMM_WriteBackAllMapTbl(void);


/*
************************************************************************************************************************
*                       SET DIRTY FLAG FOR BLOCK MAPPING TABLE
*
*Description: Set dirty flag for block mapping table.
*
*Arguments  : none.
*
*Return     : set dirty flag result;
*               = 0     set dirty flag successful;
*               = -1    set dirty flag failed.
************************************************************************************************************************
*/
__s32 BMM_SetDirtyFlag(void);


/*
************************************************************************************************************************
*                       CALCULATE BLOCK MAPPING TABLE ACCESS COUNT
*
*Description: Calculate block mapping table access count for cache switch.
*
*Arguments  : none.
*
*Return     : none;
************************************************************************************************************************
*/
void BMM_CalAccessCount(void);


/*
************************************************************************************************************************
*                       INIT PAGE MAPPING TABLE CACHE
*
*Description: Init page mapping table cache.
*
*Arguments  : none.
*
*Return     : init result;
*               = 0         init page mapping table cache successful;
*               = -1        init page mapping table cache failed.
************************************************************************************************************************
*/
__s32 PMM_InitMapTblCache(void);


/*
************************************************************************************************************************
*                       EXIT PAGE MAPPING TABLE CACHE
*
*Description: Exit page mapping table cache.
*
*Arguments  : none.
*
*Return     : exit result;
*               = 0         exit page mapping table cache successful;
*               = -1        exit page mapping table cache failed.
************************************************************************************************************************
*/
__s32 PMM_ExitMapTblCache(void);


/*
************************************************************************************************************************
*                      SWITCH PAGE MAPPING TABLE
*
*Description: Switch page mapping table cache.
*
*Arguments  : nLogBlkPst    the position of the log block in the log block table.
*
*Return     : switch result;
*               = 0     switch table successful;
*               = -1    switch table failed.
************************************************************************************************************************
*/
__s32 PMM_SwitchMapTbl(__u32 nLogBlkPst);


/*
************************************************************************************************************************
*                      CALCUALTE PAGE MAPPING TABLE ACCESS COUNT
*
*Description: Calculate page mapping table access count for table cache switch.
*
*Arguments  : none.
*
*Return     : none.
************************************************************************************************************************
*/
void PMM_CalAccessCount(void);

__s32 BMM_GetDataBlk(__u32 nBlk, struct __SuperPhyBlkType_t *pDataBlk);
__s32 BMM_SetDataBlk(__u32 nBlk, struct __SuperPhyBlkType_t *pDataBlk);
__s32 BMM_GetFreeBlk(__u32 nType, struct __SuperPhyBlkType_t *pFreeBlk);
__s32 BMM_SetFreeBlk(struct __SuperPhyBlkType_t *pFreeBlk);
__s32 BMM_GetLogBlk(__u32 nLogicBlk, struct __LogBlkType_t *pLogBlk);
__s32 BMM_SetLogBlk(__u32 nLogicBlk, struct __LogBlkType_t *pLogBlk);
__u32 PMM_GetLogPage(__u32 nBlk, __u32 nPage, __u8 nMode);
void PMM_ClearCurMapTbl(void);
__u32 PMM_GetCurMapPage(__u16 nLogicalPage);
void PMM_SetCurMapPage(__u16 nLogicalPage,__u16 nPhysicPage);
__s32 LML_VirtualBlkErase(__u32 nZone, __u32 nSuperBlk);
__s32 LML_VirtualPageWrite( struct __PhysicOpPara_t *pVirtualPage);
__s32 LML_VirtualPageRead(struct __PhysicOpPara_t *pVirtualPage);

__s32 NAND_CacheFlush(void);
__s32 NAND_CacheFlushDev(__u32 dev_num);
__s32 NAND_CacheRead(__u32 blk, __u32 nblk, void *buf);
__s32 NAND_CacheWrite(__u32 blk, __u32 nblk, void *buf);
__s32 NAND_CacheOpen(void);
__s32 NAND_CacheClose(void);


__u32 NAND_GetDiskSize(void);


#endif 

