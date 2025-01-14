/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Jerry Wang <wangflord@allwinnertech.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <config.h>
#include <common.h>
#include <sunxi_mbr.h>
#include <sys_config.h>
#include "sprite_privatedata.h"
#include "sprite_verify.h"
#include <securestorage.h>
/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_erase_flash(void  *img_mbr_buffer)
{
	int need_erase_flag = 0;
    char buf[SUNXI_MBR_SIZE * SUNXI_MBR_COPY_NUM];
    int  ret;

	if(sunxi_sprite_erase(0, img_mbr_buffer) > 0)
	{
		printf("flash already erased\n");
		return 0;
	}
    ret = script_parser_fetch("platform", "eraseflag", &need_erase_flag, 1);
    if((!ret) && (need_erase_flag))
    {
    	printf("do need erase flash\n");
    }
    else
    {
    	printf("do not need erase flash\n");

    	return 0;
    }
	if(need_erase_flag == 0x11)
	{
		printf("force erase flash\n");
		sunxi_sprite_erase(1, img_mbr_buffer);

		return 0;
	}
	if(!sunxi_sprite_probe_prvt(img_mbr_buffer))
	{
		printf("no part need to protect user data\n");
		sunxi_sprite_erase(need_erase_flag, img_mbr_buffer);

		return 0;
	}
	if(sunxi_sprite_init(1))
	{
		debug("sunxi sprite pre init fail, we have to erase it\n");
		sunxi_sprite_exit(1);
		sunxi_sprite_erase(need_erase_flag, img_mbr_buffer);

		return 0;
	}
	debug("nand pre init ok\n");
	if(!sunxi_sprite_read(0, (SUNXI_MBR_SIZE * SUNXI_MBR_COPY_NUM)/512, buf))
	{
		printf("read local mbr on flash failed\n");
		sunxi_sprite_exit(1);
		sunxi_sprite_erase(need_erase_flag, img_mbr_buffer);

		return 0;
	}
	if(sunxi_sprite_verify_mbr(buf))
	{
		printf("the mbr on flash is bad\n");
		sunxi_sprite_exit(1);
		sunxi_sprite_erase(need_erase_flag, img_mbr_buffer);

		return 0;
	}
	printf("begin to store data\n");
	if(sunxi_sprite_store_part_data(buf) < 0)
	{
		sunxi_sprite_exit(1);

		return -1;
	}
	sunxi_sprite_exit(1);
	printf("need_erase_flag = %d\n", need_erase_flag);
	printf("begin to erase\n");
	sunxi_sprite_erase(need_erase_flag, img_mbr_buffer);
	printf("finish erase\n");
	sunxi_sprite_init(0);
	printf("rewrite\n");
	if(sunxi_sprite_restore_part_data(img_mbr_buffer))
    {
    	sunxi_sprite_exit(0);

    	return -1;
    }
	printf("flash exit\n");
	sunxi_sprite_exit(0);

    return 0;
}

/*
************************************************************************************************************
*
*                                             function
*
*    name          :
*
*    parmeters     :
*
*    return        :
*
*    note          :
*
*
************************************************************************************************************
*/
int sunxi_sprite_force_erase_key(void)
{
	char buf[SUNXI_MBR_SIZE * SUNXI_MBR_COPY_NUM];
	if(sunxi_sprite_init(1))
	{
		printf("sunxi sprite pre init fail\n");
		return -1;
	}
	if(!sunxi_sprite_read(0, (SUNXI_MBR_SIZE * SUNXI_MBR_COPY_NUM)/512, buf))
	{
		printf("read local mbr on flash failed\n");
		sunxi_sprite_exit(1);

		return -1;
	}
	if(sunxi_sprite_verify_mbr(buf))
	{
		printf("the mbr on flash is bad\n");
		sunxi_sprite_exit(1);

		return -1;
	}
	if(sunxi_sprite_erase_private_key(buf))
	{
		printf("erase key fail \n");
		return -1;
	}
#ifdef CONFIG_SUNXI_SECURE_STORAGE
    if(sunxi_secure_storage_init())
        return -1;	
	if(sunxi_secure_storage_erase("key_burned_flag") == -1)
        return -1;
#endif
	printf("erase key success \n");
	return 0;
}
