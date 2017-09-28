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

#ifndef  __private_uboot_h__
#define  __private_uboot_h__

#include "spare_head.h"
/******************************************************************************/
/*               the control information stored in file head                  */
/******************************************************************************/
struct spare_boot_ctrl_head
{
	unsigned int  jump_instruction;  
	unsigned char magic[MAGIC_SIZE]; 
	unsigned int  check_sum;         
	unsigned int  align_size;		 
	unsigned int  length;            
	unsigned int  uboot_length;      
	unsigned char version[8];        
	unsigned char platform[8];       
	int           reserved[1];       
};

/******************************************************************************/
/*                          the data stored in file head                      */
/******************************************************************************/
struct spare_boot_data_head
{
	unsigned int                dram_para[32];
	int							run_clock;			
	int							run_core_vol;		
	int							uart_port;              // UART控制器编号
	normal_gpio_cfg             uart_gpio[2];           // UART控制器(调试打印口)GPIO信息
	int							twi_port;               // TWI控制器编号
	normal_gpio_cfg             twi_gpio[2];            // TWI控制器GPIO信息，用于控制TWI
	int		                    work_mode;              // 工作模式
    int                         storage_type;           // 存储介质类型  0：nand   1：sdcard    2: spinor
    normal_gpio_cfg             nand_gpio[32];          // nand GPIO信息
    char						nand_spare_data[256];	// nand 额外信息
    normal_gpio_cfg             sdcard_gpio[32];		// sdcard GPIO信息
    char             			sdcard_spare_data[256];	// sdcard 额外信息
    int                         secureos_exist;
    uint                        uboot_start_sector_in_mmc;
    int							reserved[4];			// 保留数据位, 256bytes align

};

struct spare_boot_head_t
{
	struct spare_boot_ctrl_head    boot_head;
	struct spare_boot_data_head    boot_data;
};


#endif


