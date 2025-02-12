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

#include "standby_i.h"
#include <asm/arch/ccmu.h>


/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
static int standby_set_divd_to_pll1(void)
{
	unsigned int reg_val;

	reg_val = readl(CCM_CPU_L2_AXI_CTRL);
	reg_val &= ~(0x03 << 0);
	reg_val |=  (0x02 << 0);
	writel(reg_val, CCM_CPU_L2_AXI_CTRL);
	reg_val = readl(CCM_AHB1_APB1_CTRL);;
	reg_val &= ~((0x03 << 12) | (0x03 << 8) | (0x03 << 6) | (0x03 << 4));
	reg_val |=   (0x03 << 12);
	reg_val |=  (2 << 6);
	reg_val |=  (1 << 8);

	writel(reg_val, CCM_AHB1_APB1_CTRL);

	return 0;
}

static int standby_set_divd_to_24M(void)
{
	unsigned int reg_val;

	reg_val = readl(CCM_CPU_L2_AXI_CTRL);
	reg_val &= ~(0x03 << 0);
	reg_val |=  (0x01 << 0);
	writel(reg_val, CCM_CPU_L2_AXI_CTRL);
	reg_val = readl(CCM_AHB1_APB1_CTRL);;
	reg_val &= ~((0x03 << 12) | (0x03 << 8) | (0x03 << 6) | (0x03 << 4));
	reg_val |=   (0x02 << 12);
	reg_val |=  (2 << 6);
	reg_val |=  (1 << 8);

	writel(reg_val, CCM_AHB1_APB1_CTRL);

	return 0;
}


__s32 standby_clock_to_24M(void)
{
	__u32 reg_val;
	int   i;

	reg_val = readl(CCM_CPU_L2_AXI_CTRL);
    reg_val &= ~(0x03 << 16);
    reg_val |=  (0x01 << 16);
    writel(reg_val, CCM_CPU_L2_AXI_CTRL);

	standby_set_divd_to_24M();

	for(i=0; i<0x4000; i++);

	return 0;
}

__s32 standby_clock_to_pll1(void)
{
	__u32 reg_val;

	standby_set_divd_to_pll1();

	reg_val = readl(CCM_CPU_L2_AXI_CTRL);
    reg_val &= ~(0x03 << 16);
    reg_val |=  (0x02 << 16);
    writel(reg_val, CCM_CPU_L2_AXI_CTRL);

	return 0;
}


void standby_clock_plldisable(void)
{
	uint reg_val;

	reg_val = readl(CCM_PLL1_CPUX_CTRL);
	reg_val &= ~(1U << 31);
	writel(reg_val, CCM_PLL1_CPUX_CTRL);

	reg_val = readl(CCM_PLL3_VIDEO_CTRL);
	reg_val &= ~(1U << 31);
	writel(reg_val, CCM_PLL3_VIDEO_CTRL);

	reg_val = readl(CCM_PLL6_MOD_CTRL);
	reg_val &= ~(1U << 31);
	writel(reg_val, CCM_PLL6_MOD_CTRL);

	reg_val = readl(CCM_PLL7_VIDEO1_CTRL);
	reg_val &= ~(1U << 31);
	writel(reg_val, CCM_PLL7_VIDEO1_CTRL);
}

void standby_clock_pllenable(void)
{
	__u32 reg_val;

	reg_val = readl(CCM_PLL1_CPUX_CTRL);
	reg_val |= (1U << 31);
	writel(reg_val, CCM_PLL1_CPUX_CTRL);
	do
	{
		reg_val = readl(CCM_PLL1_CPUX_CTRL);
	}
	while(!(reg_val & (0x1 << 28)));


	reg_val = readl(CCM_PLL3_VIDEO_CTRL);
	reg_val |= (1U << 31);
	writel(reg_val, CCM_PLL3_VIDEO_CTRL);
	do
	{
		reg_val = readl(CCM_PLL3_VIDEO_CTRL);
	}
	while(!(reg_val & (0x1 << 28)));

	reg_val = readl(CCM_PLL6_MOD_CTRL);
	reg_val |= (1U << 31);
	writel(reg_val, CCM_PLL6_MOD_CTRL);
	do
	{
		reg_val = readl(CCM_PLL6_MOD_CTRL);
	}
	while(!(reg_val & (0x1 << 28)));

	reg_val = readl(CCM_PLL7_VIDEO1_CTRL);
	reg_val |= (1U << 31);
	writel(reg_val, CCM_PLL7_VIDEO1_CTRL);
	do
	{
		reg_val = readl(CCM_PLL7_VIDEO1_CTRL);
	}
	while(!(reg_val & (0x1 << 28)));
}


