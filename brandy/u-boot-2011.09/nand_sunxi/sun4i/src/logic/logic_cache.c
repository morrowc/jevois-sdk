/*
************************************************************************************************************************
*                                                      eNand
*                                         Nand flash driver logic manage module
*
*                             Copyright(C), 2008-2009, SoftWinners Microelectronic Co., Ltd.
*											       All Rights Reserved
*
* File Name : logic_cache.c
*
* Author : Richard.x
*
* Version : v0.1
*
* Date : 2008.03.29
*
* Description : This file descripe the cache read / cache write nand interface.
*
* Others : None at present.
*
*
* History :
*
*  <Author>        <time>       <version>      <description>
*
* Richard.x         2008.03.29      0.1          build the file
*
************************************************************************************************************************
*/
#include "../include/nand_logic.h"


#define NAND_W_CACHE_EN
#define N_NAND_W_CACHE  8


typedef struct
{
	__u8	*data;
	__u32	size;

	__u32	hit_page;
	__u64   secbitmap;

	__u32	access_count;
	__u32   dev_num;
}__nand_cache_t;

__u32 g_w_access_cnt;

__nand_cache_t nand_w_cache[N_NAND_W_CACHE];
__nand_cache_t nand_r_cache;
__u32 nand_current_dev_num;

__u32 _get_valid_bits(__u64 secbitmap)
{
	__u32 validbit = 0;

	while(secbitmap)
	{
		if(secbitmap & (__u64)0x1)
			validbit++;
		secbitmap >>= 1;
	}

	return validbit;
}

__u32 _get_first_valid_bit(__u64 secbitmap)
{
	__u32 firstbit = 0;

	while(!(secbitmap & (__u64)0x1))
	{
		secbitmap >>= 1;
		firstbit++;
	}

	return firstbit;
}

__s32 _flush_w_cache(void)
{
	__u32	i;

	for(i = 0; i < N_NAND_W_CACHE; i++)
	{
		if(nand_w_cache[i].hit_page != 0xffffffff)
		{
			if(nand_w_cache[i].secbitmap != FULL_BITMAP_OF_LOGIC_PAGE)
				LML_PageRead(nand_w_cache[i].hit_page,(nand_w_cache[i].secbitmap ^ FULL_BITMAP_OF_LOGIC_PAGE)&FULL_BITMAP_OF_LOGIC_PAGE,nand_w_cache[i].data);
			
			LML_PageWrite(nand_w_cache[i].hit_page,FULL_BITMAP_OF_LOGIC_PAGE,nand_w_cache[i].data);
			

			/*disable read cache with current page*/
			if (nand_r_cache.hit_page == nand_w_cache[i].hit_page){
					nand_r_cache.hit_page = 0xffffffff;
					nand_r_cache.secbitmap = 0;
			}
			
			
			nand_w_cache[i].hit_page = 0xffffffff;
			nand_w_cache[i].secbitmap = 0;
			nand_w_cache[i].access_count = 0;
			
		}
	}


	return 0;

}

__s32 _flush_w_cache_simple(__u32 i)
{
	if(nand_w_cache[i].hit_page != 0xffffffff)
	{
		if(nand_w_cache[i].secbitmap != FULL_BITMAP_OF_LOGIC_PAGE)
			LML_PageRead(nand_w_cache[i].hit_page,(nand_w_cache[i].secbitmap ^ FULL_BITMAP_OF_LOGIC_PAGE)&FULL_BITMAP_OF_LOGIC_PAGE,nand_w_cache[i].data);

		LML_PageWrite(nand_w_cache[i].hit_page,FULL_BITMAP_OF_LOGIC_PAGE,nand_w_cache[i].data);
		

		/*disable read cache with current page*/
		if (nand_r_cache.hit_page == nand_w_cache[i].hit_page){
				nand_r_cache.hit_page = 0xffffffff;
				nand_r_cache.secbitmap = 0;
		}
		
		nand_w_cache[i].hit_page = 0xffffffff;
		nand_w_cache[i].secbitmap = 0;
		nand_w_cache[i].access_count = 0;
		nand_w_cache[i].dev_num= 0xffffffff;

	}

	return 0;

}


__s32 NAND_CacheFlush(void)
{

	_flush_w_cache();

	return 0;

}

__s32 NAND_CacheFlushDev(__u32 dev_num)
{
	__u32	i;

	for(i = 0; i < N_NAND_W_CACHE; i++)
	{
		if(nand_w_cache[i].dev_num == dev_num)
		{
			_flush_w_cache_simple(i);
		}


	}

	return 0;

}

void _get_data_from_cache(__u32 blk, __u32 nblk, void *buf)
{
	__u32 i;
	__u32 sec;
	__u32 page,SecWithinPage;
	__u64 SecBitmap;

	for(sec = blk; sec < blk + nblk; sec++)
	{
		SecWithinPage = sec % SECTOR_CNT_OF_LOGIC_PAGE;
		SecBitmap = ((__u64)1 << SecWithinPage);
		page = sec / SECTOR_CNT_OF_LOGIC_PAGE;
		for (i = 0; i < N_NAND_W_CACHE; i++)
		{
			if ((nand_w_cache[i].hit_page == page) && (nand_w_cache[i].secbitmap & SecBitmap))
			{
				MEMCPY((__u8 *)buf + (sec - blk) * 512, nand_w_cache[i].data + SecWithinPage * 512,512);
				break;
			}
		}
	}
}

void _get_one_page(__u32 page,__u64 SecBitmap,__u8 *data)
{
	__u32 i;
	__u8 *tmp = data;


	if(page == nand_r_cache.hit_page)
	{
		for(i = 0;i < SECTOR_CNT_OF_LOGIC_PAGE; i++)
		{
			if(SecBitmap & ((__u64)1<<i))
			{
				MEMCPY(tmp + (i<<9),nand_r_cache.data + (i<<9),512);
			}
		}
	}

	else
	{
		if(SecBitmap == FULL_BITMAP_OF_LOGIC_PAGE)
		{
			LML_PageRead(page,FULL_BITMAP_OF_LOGIC_PAGE,tmp);
		}
		else
		{
			LML_PageRead(page,FULL_BITMAP_OF_LOGIC_PAGE,nand_r_cache.data);
			nand_r_cache.hit_page = page;
			nand_r_cache.secbitmap = FULL_BITMAP_OF_LOGIC_PAGE;

			for(i = 0;i < SECTOR_CNT_OF_LOGIC_PAGE; i++)
			{
				if(SecBitmap & ((__u64)1<<i))
				{
					MEMCPY(tmp + (i<<9),nand_r_cache.data + (i<<9),512);
				}
			}
		}
	}

	SecBitmap = 0;
}

__s32 NAND_CacheRead(__u32 blk, __u32 nblk, void *buf)
{
	__u32	nSector,StartSec;
	__u32	page;
	__u32	SecWithinPage;
	__u64	SecBitmap;
	__u8 	*pdata;

	nSector 	= nblk;
	StartSec 	= blk;
	SecBitmap 	= 0;
	page 		= 0xffffffff;
	pdata		= (__u8 *)buf;

	/*combind sectors to pages*/
	while(nSector)
	{
		SecWithinPage = StartSec % SECTOR_CNT_OF_LOGIC_PAGE;
		SecBitmap |= ((__u64)1 << SecWithinPage);
		page = StartSec / SECTOR_CNT_OF_LOGIC_PAGE;

		/*close page if last sector*/
		if (SecWithinPage == (SECTOR_CNT_OF_LOGIC_PAGE - 1))
		{

			__u8 *tmp = pdata + 512 - 512*_get_valid_bits(SecBitmap) - 512 * _get_first_valid_bit(SecBitmap);
			_get_one_page(page, SecBitmap, tmp);
			SecBitmap = 0;
		}

		/*reset variable*/
		nSector--;
		StartSec++;
		pdata += 512;
	}

	/*fill opened page*/
	if (SecBitmap)
	{
		__u8	*tmp = pdata - 512*_get_valid_bits(SecBitmap) - 512 * _get_first_valid_bit(SecBitmap);
		_get_one_page(page, SecBitmap, tmp);
	}

	/*renew data from cache*/
	_get_data_from_cache(blk,nblk,buf);

	return 0;

}

__s32 _fill_nand_cache(__u32 page, __u64 secbitmap, __u8 *pdata)
{
	__u8	hit;
	__u8	i;
	__u8 	pos = 0xff;

	g_w_access_cnt++;

	hit = 0;

	for (i = 0; i < N_NAND_W_CACHE; i++)
	{
		/*merge data if cache hit*/
		if (nand_w_cache[i].hit_page == page){
			hit = 1;
			MEMCPY(nand_w_cache[i].data + 512 * _get_first_valid_bit(secbitmap),pdata, 512 * _get_valid_bits(secbitmap));
			nand_w_cache[i].secbitmap |= secbitmap;
			nand_w_cache[i].access_count = g_w_access_cnt;
			pos = i;
			break;
		}
	}

	/*post data if cache miss*/
	if (!hit)
	{
		/*find cache to post*/
		for (i = 0; i < N_NAND_W_CACHE; i++)
		{
			if (nand_w_cache[i].hit_page == 0xffffffff)
			{
				pos = i;
				break;
			}
		}

		if (pos == 0xff)
		{
			__u32 access_cnt = nand_w_cache[0].access_count;
			pos = 0;

			for (i = 1; i < N_NAND_W_CACHE; i++)
			{
				if (access_cnt > nand_w_cache[i].access_count)
				{
					pos = i;
					access_cnt = nand_w_cache[i].access_count;
				}
				
				if((nand_w_cache[i].hit_page == page-1)&&(page>0))
				{
				    pos = i;
				    break;
				}
			}

			if(nand_w_cache[pos].secbitmap != FULL_BITMAP_OF_LOGIC_PAGE)
				LML_PageRead(nand_w_cache[pos].hit_page,nand_w_cache[pos].secbitmap ^ FULL_BITMAP_OF_LOGIC_PAGE,nand_w_cache[pos].data);

			LML_PageWrite(nand_w_cache[pos].hit_page, FULL_BITMAP_OF_LOGIC_PAGE, nand_w_cache[pos].data);
			nand_w_cache[pos].access_count = 0;

			/*disable read cache with current page*/
			if (nand_r_cache.hit_page == nand_w_cache[pos].hit_page){
				nand_r_cache.hit_page = 0xffffffff;
				nand_r_cache.secbitmap = 0;
			}

		}

		/*merge data*/
		MEMCPY(nand_w_cache[pos].data + 512 * _get_first_valid_bit(secbitmap),pdata, 512 * _get_valid_bits(secbitmap));
		nand_w_cache[pos].hit_page = page;
		nand_w_cache[pos].secbitmap = secbitmap;
		nand_w_cache[pos].access_count = g_w_access_cnt;
		nand_w_cache[i].dev_num= nand_current_dev_num;

	}

	if (g_w_access_cnt == 0)
	{
		for (i = 0; i < N_NAND_W_CACHE; i++)
			nand_w_cache[i].access_count = 0;
		g_w_access_cnt = 1;
		nand_w_cache[pos].access_count = g_w_access_cnt;
	}

	return 0;
}

__s32 NAND_CacheWrite(__u32 blk, __u32 nblk, void *buf)
{
	__u32	nSector,StartSec;
	__u32	page;
	__u32	SecWithinPage;
	__u64	SecBitmap;
	__u32	i;
	__u8 	*pdata;

	nSector 	= nblk;
	StartSec 	= blk;
	SecBitmap 	= 0;
	page 		= 0xffffffff;
	pdata		= (__u8 *)buf;

	/*combind sectors to pages*/
	while(nSector)
	{
		SecWithinPage = StartSec % SECTOR_CNT_OF_LOGIC_PAGE;
		SecBitmap |= ((__u64)1 << SecWithinPage);
		page = StartSec / SECTOR_CNT_OF_LOGIC_PAGE;


		/*close page if last sector*/
		if (SecWithinPage == (SECTOR_CNT_OF_LOGIC_PAGE - 1))
		{
			/*write to nand flash if align one logic page*/
			if(SecBitmap == FULL_BITMAP_OF_LOGIC_PAGE)
			{
				/*disable write cache with current page*/
				for (i = 0; i < N_NAND_W_CACHE; i++)
				{
					if(nand_w_cache[i].hit_page == page)
					{
						nand_w_cache[i].hit_page = 0xffffffff;
						nand_w_cache[i].secbitmap = 0;
						nand_w_cache[i].dev_num= 0xffffffff;
					}
					else if((nand_w_cache[i].hit_page == page-1)&&(page>0))
					{
					    _flush_w_cache_simple(i);
					}
					
					
				}
				/*disable read cache with current page*/
				if (nand_r_cache.hit_page == page){
					nand_r_cache.hit_page = 0xffffffff;
					nand_r_cache.secbitmap = 0;
				}


				LML_PageWrite(page,FULL_BITMAP_OF_LOGIC_PAGE,pdata + 512 - 512*SECTOR_CNT_OF_LOGIC_PAGE);
			}

			/*fill to cache if unalign one logic page*/
			else
				_fill_nand_cache(page, SecBitmap, pdata + 512 - 512*_get_valid_bits(SecBitmap));

			SecBitmap = 0;
		}


		/*reset variable*/
		nSector--;
		StartSec++;
		pdata += 512;
	}

	/*fill opened page*/
	if (SecBitmap)
		_fill_nand_cache(page,SecBitmap,pdata - 512*_get_valid_bits(SecBitmap));

	return 0;
}


__s32 NAND_CacheOpen(void)
{
	__u32 i;

	g_w_access_cnt = 0;

	for(i = 0; i < N_NAND_W_CACHE; i++)
	{
		nand_w_cache[i].size = 512 * SECTOR_CNT_OF_LOGIC_PAGE;
		nand_w_cache[i].data = MALLOC(nand_w_cache[i].size);
		nand_w_cache[i].hit_page = 0xffffffff;
		nand_w_cache[i].secbitmap = 0;
		nand_w_cache[i].access_count = 0;
		nand_w_cache[i].dev_num= 0xffffffff;
	}

	nand_r_cache.size = 512 * SECTOR_CNT_OF_LOGIC_PAGE;
	nand_r_cache.data = MALLOC(nand_r_cache.size);
	nand_r_cache.hit_page = 0xffffffff;
	nand_r_cache.secbitmap = 0;
	nand_r_cache.access_count = 0;
	nand_r_cache.dev_num= 0xffffffff;

	return 0;
}

__s32 NAND_CacheClose(void)
{
	__u32 i;

	NAND_CacheFlush();

	#ifdef NAND_W_CACHE_EN
		for(i = 0; i < N_NAND_W_CACHE; i++)
			FREE(nand_w_cache[i].data,nand_w_cache[i].size);
	#endif
	FREE(nand_r_cache.data,nand_r_cache.size);
	return 0;
}
