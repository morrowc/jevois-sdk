/*
************************************************************************************************************************
*                                                      eNand
*                                         Nand flash driver logic manage module
*
*                             Copyright(C), 2008-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : wear_levelling.c
*
* Author : Kevin.z
*
* Version : v0.1
*
* Date : 2008.04.07
*
* Description : This file is the wear-levelling module.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Kevin.z         2008.04.07      0.1          build the file
*
************************************************************************************************************************
*/
#include "../include/nand_drv_cfg.h"
#include "../include/nand_type.h"
#include "../include/nand_physic.h"
#include "../include/nand_logic.h"

extern struct __NandDriverGlobal_t     NandDriverInfo;

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
__s32 LML_WearLevelling(void)
{
    #if CFG_SUPPORT_WEAR_LEVELLING

    __s32   i, result;

    __u32   tmpLogicBlk;
    __u16   tmpLowEc = 0xffff;
    struct __SuperPhyBlkType_t tmpFreeBlk, tmpDataBlk;
    struct __NandUserData_t tmpSpare[2];
    struct __PhysicOpPara_t tmpSrcPage, tmpDstPage;
    BLK_ERASE_CNTER = 0;

    for(i=DATA_BLK_CNT_OF_ZONE-1; i>=0; i--)
    {
        if(DATA_BLK_TBL[i].BlkEraseCnt < tmpLowEc)
        {
            tmpLowEc = DATA_BLK_TBL[i].BlkEraseCnt;
            tmpLogicBlk = i;
        }
    }

    result = BMM_GetFreeBlk(HIGHEST_EC_TYPE, &tmpFreeBlk);
    if(result < 0)
    {
        LOGICCTL_ERR("[LOGICCTL_ERR] Get free block failed when do wear-levelling!\n");
        return -1;
    }

    BLK_ERASE_CNTER = 0;

    if(tmpLowEc >= tmpFreeBlk.BlkEraseCnt)
    {
        if(tmpLowEc == 0xffff)
        {
            for(i=0; i<DATA_BLK_CNT_OF_ZONE; i++)
            {
                DATA_BLK_TBL[i].BlkEraseCnt = 0x00;
            }

            for(i=0; i<FREE_BLK_CNT_OF_ZONE; i++)
            {
                if(FREE_BLK_TBL[i].PhyBlkNum != 0xffff)
                {
                    FREE_BLK_TBL[i].BlkEraseCnt = 0x00;
                }
            }

            for(i=0; i<MAX_LOG_BLK_CNT; i++)
            {
                if(LOG_BLK_TBL[i].LogicBlkNum != 0xffff)
                {
                    LOG_BLK_TBL[i].PhyBlk.BlkEraseCnt = 0x00;
                }
            }
        }

        BMM_SetFreeBlk(&tmpFreeBlk);

        return 0;
    }

    BMM_GetDataBlk(tmpLogicBlk, &tmpDataBlk);
    result = BMM_GetLogBlk(tmpLogicBlk, NULL);
    if(result < 0)
    {
        LML_CalculatePhyOpPar(&tmpSrcPage, CUR_MAP_ZONE, tmpDataBlk.PhyBlkNum, 0);
        tmpSrcPage.SectBitmap = 0x03;
        tmpSrcPage.MDataPtr = LML_TEMP_BUF;
        tmpSrcPage.SDataPtr = (void *)tmpSpare;
        LML_VirtualPageRead(&tmpSrcPage);

        if(tmpSpare[0].LogicInfo != 0xffff)
        {
            tmpSrcPage.SectBitmap = FULL_BITMAP_OF_SUPER_PAGE;
            tmpSrcPage.MDataPtr = NULL;
            tmpSrcPage.SDataPtr = NULL;

            tmpDstPage.SectBitmap = FULL_BITMAP_OF_SUPER_PAGE;
            tmpDstPage.MDataPtr = NULL;
            tmpDstPage.SDataPtr = NULL;

            for(i=0; i<PAGE_CNT_OF_SUPER_BLK; i++)
            {
                LML_CalculatePhyOpPar(&tmpSrcPage, CUR_MAP_ZONE, tmpDataBlk.PhyBlkNum, i);
                LML_CalculatePhyOpPar(&tmpDstPage, CUR_MAP_ZONE, tmpFreeBlk.PhyBlkNum, i);

                PHY_PageCopyback(&tmpSrcPage, &tmpDstPage);
                result = PHY_SynchBank(tmpDstPage.BankNum, SYNC_CHIP_MODE);
                if(result < 0)
                {
                    LOGICCTL_DBG("[LOGICCTL_DBG] Copy page failed when doing wear-levelling!\n");
                    result = LML_BadBlkManage(&tmpFreeBlk, CUR_MAP_ZONE, 0, NULL);
                    if(result < 0)
                    {
                        LOGICCTL_ERR("[LOGICCTL_ERR] Bad block manage failed when doing wear-levelling!\n");
                        return -1;
                    }

                    return 0;
                }
            }
        }

        BMM_SetDataBlk(tmpLogicBlk, &tmpFreeBlk);

        if(tmpSpare[0].LogicInfo != 0xffff)
        {
            result = LML_VirtualBlkErase(CUR_MAP_ZONE, tmpDataBlk.PhyBlkNum);
            if(result < 0)
            {
                LOGICCTL_DBG("[LOGICCTL_DBG] Erase super block failed when doing wear-levelling!\n");
                result = LML_BadBlkManage(&tmpDataBlk, CUR_MAP_ZONE, 0, NULL);
                if(result < 0)
                {
                    LOGICCTL_ERR("[LOGICCTL_ERR] Bad block manage failed when doing wear-levelling!\n");
                   return -1;
                }

                return 0;
            }
        }

        tmpDataBlk.BlkEraseCnt++;
        BMM_SetFreeBlk(&tmpDataBlk);
    }
    else
    {
        BMM_SetFreeBlk(&tmpFreeBlk);
    }

    #endif
    return 0;
}

