/*
 * (C) Copyright 2007-2013
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Martin zheng <zhengjiewen@allwinnertech.com>
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
/*
 ************************************************************************************************************
 *
 *                                             eGon2_int_enter_standby
 *
 *    函数名称：
 *
 *    参数列表：
 *
 *
 *
 *    返回值  ：
 *
 *    说明    ：	进入standby
 *				1) 清除DMA PENDING，保存DMA enable
 *				2) 关闭DMA AHB
 *
 ************************************************************************************************************
 */

int standby_int_init(void)
{

    asm("mrs r0, cpsr");
    asm("orr r0, r0, #(0x40|0x80)");
    asm("msr cpsr_c, r0");


    return 0;
}
/*
 ************************************************************************************************************
 *
 *                                             eGon2_int_exit_standby
 *
 *    函数名称：
 *
 *    参数列表：
 *
 *
 *
 *    返回值  ：
 *
 *    说明    ：	退出standby
 *				1) 打开DMA AHB
 *				2) 清除DMA PENDING，恢复DMA enable
 *
 *
 ************************************************************************************************************
 */
int standby_int_exit(void)
{

    asm("mrs r0, cpsr");
    asm("bic r0, r0, #(0x40|0x80)");
    asm("msr cpsr_c, r0");

    return 0;
}

#if 0
int standby_int_query(void)
{

    return 0;
}

#endif
