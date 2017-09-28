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

#include <common.h>
#include <asm/io.h>
#include <asm/arch/ccmu.h>
#include <asm/arch/cpu.h>

struct core_pll_freq_tbl {
    int FactorN;
    int FactorK;
    int FactorM;
    int FactorP;
    int pading;
};

static struct core_pll_freq_tbl pll1_table[] =
{ 
	{9 ,	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{9 , 	0,	0,	2},   
	{10,	0,	0,	2},   
	{11,	0,	0,	2},   
	{12,	0,	0,	2},   
	{13,	0,	0,	2},   
	{14,	0,	0,	2},   
	{15,	0,	0,	2},   
	{16,	0,	0,	2},   
	{17,	0,	0,	2},   
	{18,	0,	0,	2},   
	{9 ,	0,	0,	1},   
	{10,    0,	0,	1},   
	{10,    0,	0,	1},   
	{11,    0,	0,	1},   
	{11,    0,	0,	1},   
	{12,    0,	0,	1},   
	{12,    0,	0,	1},   
	{13,    0,	0,	1},   
	{13,    0,	0,	1},   
	{14,    0,	0,	1},   
	{14,    0,	0,	1},   
	{15,    0,	0,	1},   
	{15,    0,	0,	1},   
	{16,    0,	0,	1},   
	{16,    0,	0,	1},   
	{17,    0,	0,	1},   
	{17,    0,	0,	1},   
	{18,    0,	0,	1},   
	{18,    0,	0,	1},   
	{9 ,	0,	0,	0},   
	{9 ,	0,	0,	0},   
	{10,	0,	0,	0},   
	{10,	0,	0,	0},   
	{10,	0,	0,	0},   
	{10,	0,	0,	0},   
	{11,	0,	0,	0},   
	{11,	0,	0,	0},   
	{11,	0,	0,	0},   
	{11,	0,	0,	0},   
	{12,	0,	0,	0},   
	{12,	0,	0,	0},   
	{12,	0,	0,	0},   
	{12,	0,	0,	0},   
	{13,	0,	0,	0},   
	{13,	0,	0,	0},   
	{13,	0,	0,	0},   
	{13,	0,	0,	0},   
	{14,	0,	0,	0},   
	{14,	0,	0,	0},   
	{14,	0,	0,	0},   
	{14,	0,	0,	0},   
	{15,	0,	0,	0},   
	{15,	0,	0,	0},   
	{15,	0,	0,	0},   
	{15,    0,	0,	0},   
	{16,	0,	0,	0},   
	{16,	0,	0,	0},   
	{16,	0,	0,	0},   
	{16,	0,	0,	0},   
	{17,	0,	0,	0},   
	{17,	0,	0,	0},   
	{17,	0,	0,	0},   
	{17,	0,	0,	0},   
	{18,	0,	0,	0},   
	{18,	0,	0,	0},   
	{18,	0,	0,	0},   
	{18,	0,	0,	0},   
	{19,	0,	0,	0},   
	{19,	0,	0,	0},   
	{19,	0,	0,	0},   
	{19,	0,	0,	0},   
	{20,	0,	0,	0},   
	{20,	0,	0,	0},   
	{20,	0,	0,	0},   
	{20,	0,	0,	0},   
	{21,	0,	0,	0},   
	{21,	0,	0,	0},   
	{21,	0,	0,	0},   
	{21,	0,	0,	0},   
	{22,	0,	0,	0},   
	{22,	0,	0,	0},   
	{22,	0,	0,	0},   
	{22,	0,	0,	0},   
	{23,	0,	0,	0},   
	{23,	0,	0,	0},   
	{23,	0,	0,	0},   
	{23,	0,	0,	0},   
	{24,	0,	0,	0},   
	{24,	0,	0,	0},   
	{24,	0,	0,	0},   
	{24,	0,	0,	0},   
	{25,	0,	0,	0},   
	{25,	0,	0,	0},   
	{25,	0,	0,	0},   
	{25,	0,	0,	0},   
	{26,	0,	0,	0},   
	{26,	0,	0,	0},   
	{26,	0,	0,	0},   
	{26,	0,	0,	0},   
	{27,	0,	0,	0},   
	{27,	0,	0,	0},   
	{27,	0,	0,	0},   
	{27,	0,	0,	0},   
	{28,	0,	0,	0},   
	{28,	0,	0,	0},   
	{28,	0,	0,	0},   
	{28,	0,	0,	0},   
	{29,	0,	0,	0},   
	{29,	0,	0,	0},   
	{29,	0,	0,	0},   
	{29,	0,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{15,	1,	0,	0},   
	{10,	2,	0,	0},   
	{10,	2,	0,	0},   
	{10,	2,	0,	0},   
	{10,	2,	0,	0},   
	{16,	1,	0,	0},   
	{16,	1,	0,	0},   
	{16,	1,	0,	0},   
	{16,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{17,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{18,	1,	0,	0},   
	{12,	2,	0,	0},   
	{12,	2,	0,	0},   
	{12,	2,	0,	0},   
	{12,	2,	0,	0},   
	{19,	1,	0,	0},   
	{19,	1,	0,	0},   
	{19,	1,	0,	0},   
	{19,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{20,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{21,	1,	0,	0},   
	{14,	2,	0,	0},   
	{14,	2,	0,	0},   
	{14,	2,	0,	0},   
	{14,	2,	0,	0},   
	{22,	1,	0,	0},   
	{22,	1,	0,	0},   
	{22,	1,	0,	0},   
	{22,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{23,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{24,	1,	0,	0},   
	{16,	2,	0,	0},   
	{16,	2,	0,	0},   
	{16,	2,	0,	0},   
	{16,	2,	0,	0},   
	{25,	1,	0,	0},   
	{25,	1,	0,	0},   
	{25,	1,	0,	0},   
	{25,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{26,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{27,	1,	0,	0},   
	{18,	2,	0,	0},   
	{18,	2,	0,	0},   
	{18,	2,	0,	0},   
	{18,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{19,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{20,	2,	0,	0},   
	{15,	3,	0,	0},   
	{15,	3,	0,	0},   
	{15,	3,	0,	0},   
	{15,	3,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{21,	2,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{16,	3,	0,	0},   
	{22,	2,	0,	0},   
	{22,	2,	0,	0},   
	{22,	2,	0,	0},   
	{22,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{23,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{24,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{25,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{26,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
	{27,	2,	0,	0},   
};

static int clk_get_pll_para(struct core_pll_freq_tbl *factor, int rate);
int sunxi_clock_get_pll6( void );

/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int sunxi_clock_get_corepll(void)
{
	unsigned int reg_val;
	int 	div_m, div_p;
	int 	factor_k, factor_n;
	int 	clock;

	reg_val  = readl(CCM_PLL1_CPUX_CTRL);
	div_p    = ((reg_val >>16) & 0x3);
	factor_n = ((reg_val >> 8) & 0x1f) + 1;
	factor_k = ((reg_val >> 4) & 0x3) + 1;
	div_m    = ((reg_val >> 0) & 0x3) + 1;

	clock = 24 * factor_n * factor_k/div_m/(1<<div_p);

	return clock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int sunxi_clock_get_axi(void)
{
	int clock;
	unsigned int reg_val;
	int clock_src, factor;

	reg_val   = readl(CCM_CPU_L2_AXI_CTRL);
	clock_src = (reg_val >> 16) & 0x03;
	factor    = (reg_val >> 0) & 0x03;

	if(factor >= 3)
	{
		factor = 4;
	}
	else
	{
		factor ++;
	}

	switch(clock_src)
	{
		case 0:
			clock = 32000;
			break;
		case 1:
			clock = 24;
			break;
		case 2:
			clock =  sunxi_clock_get_corepll();
			break;
		default:
			return 0;
	}

	return clock/factor;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int sunxi_clock_get_ahb(void)
{
	unsigned int reg_val;
	int factor;
	int clock;
    int src = 0;

	reg_val = readl(CCM_AHB1_APB1_CTRL);
	
    src = (reg_val >> 12)&0x3;
    clock = 0;
    switch(src)
    {
        case 2://src is axi
            factor  = (reg_val >> 4) & 0x03;
            clock   = sunxi_clock_get_axi()>>factor;
            break;
        case 3://src is pll6
            factor  = (reg_val >> 6) & 0x03;
            clock   = sunxi_clock_get_pll6()/(factor+1);
        break;
    }

	return clock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int sunxi_clock_get_apb1(void)
{
	unsigned int reg_val;
	int          clock, factor;

	reg_val = readl(CCM_AHB1_APB1_CTRL);
	factor  = (reg_val >> 8) & 0x03;
	clock   = sunxi_clock_get_ahb();

	if(factor)
	{
		clock >>= factor;
	}
	else
	{
		clock >>= 1;
	}

	return clock;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：
*
*
************************************************************************************************************
*/
int sunxi_clock_get_apb2(void)
{
	return 24;
}
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：只限于调整COREPLL，固定分频比，4:2:1
*
*
************************************************************************************************************
*/
static int clk_get_pll_para(struct core_pll_freq_tbl *factor, int pll_clk)
{
    int                       index;
	struct core_pll_freq_tbl *target_factor;

    index = pll_clk / 6;
	target_factor = &pll1_table[index];

    factor->FactorN = target_factor->FactorN;
    factor->FactorK = target_factor->FactorK;
    factor->FactorM = target_factor->FactorM;
    factor->FactorP = target_factor->FactorP;

    return 0;
}
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
static int clk_set_divd(void)
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
/*
************************************************************************************************************
*
*                                             function
*
*    函数名称：
*
*    参数列表：
*
*
*
*    返回值  ：
*
*    说明    ：只限于调整COREPLL，固定分频比，4:2:1
*
*
************************************************************************************************************
*/
int sunxi_clock_set_corepll(int frequency, int core_vol)
{
    unsigned int reg_val;
    unsigned int i;
    struct core_pll_freq_tbl  pll_factor;
    if(!frequency)
    {
        frequency = 408;
    }
    else if(frequency < 24)
    {
		frequency = 24;
    }
    reg_val = readl(CCM_CPU_L2_AXI_CTRL);
    reg_val &= ~(0x03 << 16);
    reg_val |=  (0x01 << 16);
    writel(reg_val, CCM_CPU_L2_AXI_CTRL);
    for(i=0; i<0x400; i++);
	clk_get_pll_para(&pll_factor, frequency);
    reg_val = readl(CCM_PLL1_CPUX_CTRL);
    reg_val &= ~((0x03 << 16) | (0x1f << 8) | (0x03 << 4) | (0x03 << 0));
	reg_val |=  (pll_factor.FactorP << 16) | (pll_factor.FactorN<<8) | (pll_factor.FactorK<<4) | (0 << 0) ;
    writel(reg_val, CCM_PLL1_CPUX_CTRL);
#ifndef CONFIG_A67_FPGA
	do
	{
		reg_val = readl(CCM_PLL1_CPUX_CTRL);
	}
	while(!(reg_val & (0x1 << 28)));
#endif
    clk_set_divd();
    reg_val = readl(CCM_CPU_L2_AXI_CTRL);
    reg_val &= ~(0x03 << 16);
    reg_val |=  (0x02 << 16);
    writel(reg_val, CCM_CPU_L2_AXI_CTRL);

    return  0;
}
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
int sunxi_clock_get_pll6(void)
{
	unsigned int reg_val;
	int factor_n, factor_k, pll6;

	reg_val = readl(CCM_PLL6_MOD_CTRL);
	factor_n = ((reg_val >> 8) & 0x1f) + 1;
	factor_k = ((reg_val >> 4) & 0x03) + 1;
	pll6 = 24 * factor_n * factor_k/2;
	return pll6;
}
