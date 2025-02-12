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
#include "dram_i.h"
#include "mctl_reg.h"
#include "mctl_hal.h"

#ifdef LINUX_CONFIG
	 #include <mach/sys_config.h>
#endif



unsigned int DRAMC_init(__dram_para_t *para)
{
	unsigned int dram_size = 0;
	unsigned int reg_val;
  	if(!para)
	{
    	return 0;
	}

	standby_serial_putc('s');
	standby_serial_putc('\n');


  	mctl_sys_init(para);

	if(((para->dram_para2>>8)&0xF) == 2)
	{
		mctl_dll_init(0, para);
		mctl_dll_init(1, para);
	}
	else
	{
		mctl_dll_init(0, para);
	}

	mctl_reset_release();

	reg_val = mctl_read_w(SDR_COM_CCR);
	reg_val |= 0x7;//modify 12/2
	mctl_write_w(SDR_COM_CCR, reg_val);

	if((mctl_read_w(R_VDD_SYS_PWROFF_GATE) & 0x3) == 0)
	{
	}

	if(((para->dram_para2>>8)&0xF) == 2)
	{
		mctl_channel_init(0, para);
		mctl_channel_init(1, para);
	}
	else
	{
		mctl_channel_init(0, para);
	}
	mctl_com_init(para);

	mctl_port_cfg();

/*
	mctl_write_w(SDR_PIR, 0x81);//modify 12/3
	if(mctl_read_w(SDR_COM_CR)&(0x1<<19))
		mctl_write_w(SDR_PIR + 0x1000, 0x81);
*/
   	if(mctl_read_w(SDR_PGSR) & (0x3<<5))
 	{
 		standby_serial_putc('3');
		return 0;
	}
   	if(mctl_read_w(SDR_COM_CR)&(0x1<<19))
   	{
   		if(mctl_read_w(SDR_PGSR + 0x1000) & (0x3<<5))
   		{
   			standby_serial_putc('4');
   			return 0;
   		}
   	}
/*
	reg_val = 0x1<<24;
	reg_val |= 0x2<<0;
	mctl_write_w(0x01c20000 + 0x15c, reg_val);
	reg_val = mctl_read_w(0x01c20000 + 0x15c);
	reg_val |= 0x1u<<31;
	mctl_write_w(0x01c20000 + 0x15c, reg_val);
	reg_val = 0x1<<24;
	reg_val |= 0x2<<0;
	mctl_write_w(0x01c20000 + 0x160, reg_val);
	reg_val = mctl_read_w(0x01c20000 + 0x15c);
	reg_val |= 0x1u<<31;
	mctl_write_w(0x01c20000 + 0x160, reg_val);
*/

	*(volatile unsigned int *)(0x01c20000 + 0x15c) = 0x82000001U;
	*(volatile unsigned int *)(0x01c20000 + 0x160) = 0x81000000U;

	dram_size = (para->dram_para1>>16)&0xF;
	dram_size *= (1<<(((para->dram_para1>>20)&0xFF)-10));
	dram_size *= (4<<(((para->dram_para1)>>28)&0xF));
	dram_size *= ((para->dram_para2>>8)&0xf);
	dram_size *= ((para->dram_para2>>12)&0xf);
	paraconfig(&(para->dram_para1), 0xFFFF<<0, dram_size<<0);
	standby_serial_putc('3');
	standby_serial_putc('\n');
	return (dram_size);
}
unsigned int DRAMC_init_auto(__dram_para_t *para)
{
#if 0
	unsigned int i, j;
	unsigned int ch_lock = 0;
	unsigned int bus_lock = 0;
	unsigned int size_max = 2048;
	unsigned int dram_size = 0;
	int ret = 0;


	paraconfig(&(para->dram_para2), 0xF<<0, 0<<0);
	paraconfig(&(para->dram_para1), 0xF<<16, 2<<16);
	paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
	paraconfig(&(para->dram_para1), 0xFFFF<<0, 0<<0);
	paraconfig(&(para->dram_para1), 0xFF<<20, 14<<20);
	paraconfig(&(para->dram_para1), 0xFu<<28, 1u<<28);

	if((para->dram_tpr13 & (0x3<<3)) == 0x0)//A31
	{
		if((para->dram_tpr13 & (0x1<<2)) == 1)//bus width lock
		{
			bus_lock = 1;
			paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
			paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		}

		if((para->dram_tpr13 & (0x1<<1)) == 1)//channel lock
		{
			ch_lock = 1;
			paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);
		}
	}else if((para->dram_tpr13 & (0x3<<3)) == 0x1)//A31S
	{
		if(para->dram_tpr13 & 0x8)
		{
			size_max = 1024;
		}
		if((para->dram_tpr13 & (0x1<<2)) == 1)//bus width lock
		{
			bus_lock = 1;
			paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
			paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		}

		if((para->dram_tpr13 & (0x1<<1)) == 1)//channel lock
		{
			ch_lock = 1;
			paraconfig(&(para->dram_para2), 0xF<<8, 1<<8);
		}
	}else if((para->dram_tpr13 & (0x3<<3)) == 0x2)//A3X PHONE
	{
	}
	else
	{
	}

	if(ch_lock == 0)// channel num auto detect
	{
		paraconfig(&(para->dram_para2), 0xF<<8, 2<<8);

		DRAMC_init(para);

		for(i=0;i<4;i++)
		{
			if(mctl_read_w(0x40000000 + i*4) != mctl_read_w(0x40000040 + i*4))
				break;
		}
		if(i<4)
		{
			paraconfig(&(para->dram_para2), 0xF<<8, 1<<8);
		}
	}

	if(bus_lock == 0)//bus width auto detect
	{
		paraconfig(&(para->dram_para2), 0xF<<0, 1<<0);
		paraconfig(&(para->dram_para1), 0xF<<16, 4<<16);
		if(!DRAMC_init(para))
		{
			paraconfig(&(para->dram_para2), 0xF<<0, 0<<0);
			paraconfig(&(para->dram_para1), 0xF<<16, 2<<16);
		}
	}


	paraconfig(&(para->dram_para2), 0xF<<12, 2<<12);
	{
		if(!DRAMC_init(para))
		{
			paraconfig(&(para->dram_para2), 0xF<<12, 1<<12);
		}
	}


	paraconfig(&(para->dram_para1), 0xFF<<20, 16<<20);

	DRAMC_init(para);

	for(i=0x10000000;i<(size_max<<10);i+=0x10000000)
	{
		for(j=0;j<32;j++)
		{
			mctl_write_w(0x40000000+i+j*4, 0x40000000+i+j*4);
		}
	}

	dram_size = size_max;
	for(i=0x10000000;i<(size_max<<10);i+=0x10000000)
	{
		for(j=0;j<32;j++)
		{
			if(mctl_read_w(0x40000000+i+j*4) != (0x40000000+i+j*4))
			{
				ret = 1;
				dram_size = (i>>20);
				break;
			}
		}
		if(ret == 1)
			break;
	}

	paraconfig(&(para->dram_para1), 0xFFFF<<0, dram_size<<0);
	dram_size >>= ((para->dram_para2>>8)&0xf);
	dram_size >>= ((para->dram_para2>>12)&0xf);
	dram_size /= (4<<(((para->dram_para1)>>28)&0xF));
	dram_size /= (para->dram_para1>>16)&0xF;
	dram_size += 10;
	paraconfig(&(para->dram_para1), 0xFF<<20, dram_size<<20);
	if(!DRAMC_init(para))
		return 0;
	return ((para->dram_para1)&0xFFFF);
#else
	return 0;
#endif
}


unsigned int mctl_sys_init(__dram_para_t *dram_para)
{
	unsigned int reg_val;

	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val &= ~(0x1U<<31);
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
	reg_val &= ~((0x3<<0) | (0x3<<4) | (0x1F<<8));
	reg_val |= ((0x1<<0) | (0x1<<4));
	reg_val |= ((dram_para->dram_clk/24-1)<<0x8);//N
	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val |= 0x1U<<31;
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

  	reg_val = mctl_read_w(CCM_PLL5_DDR_CTRL);
  	reg_val |= 0x1U<<20;
  	mctl_write_w(CCM_PLL5_DDR_CTRL, reg_val);

#ifndef SYSTEM_SIMULATION
  	standby_timer_delay(1000);
#else
  	aw_delay(0x20);
#endif

  		reg_val = mctl_read_w(CCM_MDFS_CLK_CTRL);
  		reg_val &= ~((0x3<<24) | (0x3<<16) | (0xf<<0));
  		reg_val |= (0x1u<<31) | (0x1<<24) | (0x0<<16) | (0x2<<0);
  		mctl_write_w(CCM_MDFS_CLK_CTRL, reg_val);


	standby_serial_putc('1');

  	reg_val = mctl_read_w(CCM_AHB1_GATE0_CTRL);
  	reg_val |= 0x1<<14;
  	mctl_write_w(CCM_AHB1_GATE0_CTRL, reg_val);

	standby_serial_putc('2');

  	reg_val = mctl_read_w(CCM_AHB1_RST_REG0);
  	reg_val &= ~(0x1<<14);
  	mctl_write_w(CCM_AHB1_RST_REG0, reg_val);

	standby_serial_putc('3');

	mctl_write_w(CCM_DRAM_GATING, 0);

	reg_val = mctl_read_w(CCM_DRAMCLK_CFG_CTRL);
  	reg_val &= ~(0x1U<<31);
  	mctl_write_w(CCM_DRAMCLK_CFG_CTRL, reg_val);

	standby_serial_putc('4');

	standby_timer_delay(1);
  	reg_val = mctl_read_w(CCM_AHB1_RST_REG0);
  	reg_val |= 0x1<<14;
  	mctl_write_w(CCM_AHB1_RST_REG0, reg_val);

	standby_serial_putc('4');
  	return (1);
}

unsigned int mctl_reset_release(void)
{
	unsigned int reg_val;

	standby_serial_putc('5');

  	reg_val = mctl_read_w(CCM_DRAMCLK_CFG_CTRL);
  	reg_val |= 0x1U<<31;
  	mctl_write_w(CCM_DRAMCLK_CFG_CTRL, reg_val);

#ifndef SYSTEM_SIMULATION
  	standby_timer_delay(10);
#endif

  	reg_val = mctl_read_w(CCM_DRAMCLK_CFG_CTRL);
  	reg_val |= 0x1U<<16;
  	mctl_write_w(CCM_DRAMCLK_CFG_CTRL, reg_val);

  	while(mctl_read_w(CCM_DRAMCLK_CFG_CTRL) & (0x1<<16)){};

	standby_serial_putc('6');

  	return (1);
}

unsigned int mctl_dll_init(unsigned int ch_index, __dram_para_t *para)
{
	unsigned int ch_id;
	unsigned int reg_val;

	if(ch_index == 1)
		ch_id = 0x1000;
	else
		ch_id = 0x0;

	mctl_write_w(ch_id + SDR_ACDLLCR,0x80000000);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x80000000);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x80000000);
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x80000000);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x80000000);
	}

#ifndef SYSTEM_SIMULATION
	standby_timer_delay(10);
#else
	aw_delay(0x10);
#endif

	mctl_write_w(ch_id + SDR_ACDLLCR,0x0);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x0);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x0);
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x0);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x0);
	}

#ifndef SYSTEM_SIMULATION
	standby_timer_delay(10);
#else
	aw_delay(0x10);
#endif

	mctl_write_w(ch_id + SDR_ACDLLCR,0x40000000);
	mctl_write_w(ch_id + SDR_DX0DLLCR,0x40000000);
	mctl_write_w(ch_id + SDR_DX1DLLCR,0x40000000);
	if(((para->dram_para2>>0)&0xF) == 1)
	{
		mctl_write_w(ch_id + SDR_DX2DLLCR,0x40000000);
		mctl_write_w(ch_id + SDR_DX3DLLCR,0x40000000);
	}
#ifndef SYSTEM_SIMULATION
	standby_timer_delay(10);
#else
	aw_delay(0x10);
#endif

	reg_val = mctl_read_w(ch_id + SDR_DX0DLLCR);
	reg_val &= ~(0xF<<14);
	reg_val |= ((para->dram_tpr13>>16)&0xF)<<14;
	mctl_write_w(ch_id + SDR_DX0DLLCR, reg_val);

	reg_val = mctl_read_w(ch_id + SDR_DX1DLLCR);
	reg_val &= ~(0xF<<14);
	reg_val |= ((para->dram_tpr13>>16)&0xF)<<14;
	mctl_write_w(ch_id + SDR_DX1DLLCR, reg_val);

	reg_val = mctl_read_w(ch_id + SDR_DX2DLLCR);
	reg_val &= ~(0xF<<14);
	reg_val |= ((para->dram_tpr13>>16)&0xF)<<14;
	mctl_write_w(ch_id + SDR_DX2DLLCR, reg_val);

	reg_val = mctl_read_w(ch_id + SDR_DX3DLLCR);
	reg_val &= ~(0xF<<14);
	reg_val |= ((para->dram_tpr13>>16)&0xF)<<14;
	mctl_write_w(ch_id + SDR_DX3DLLCR, reg_val);


	return (1);
}

unsigned int mctl_channel_init(unsigned int ch_index, __dram_para_t *para)
{
	unsigned int reg_val = 0;
	unsigned int clkmhz = 0;
	unsigned int ch_id = 0;
	unsigned int hold_flag = 0;
	unsigned int trefi = 0;
	unsigned int tmrd = 0;
	unsigned int trfc = 0;
	unsigned int trp = 0;
	unsigned int tprea = 0;
	unsigned int trtw = 0;
	unsigned int tal = 0;
	unsigned int tcl = 0;
	unsigned int tcwl = 0;
	unsigned int tras = 0;
	unsigned int trc = 0;
	unsigned int trcd = 0;
	unsigned int trrd = 0;
	unsigned int trtp = 0;
	unsigned int twr = 0;
	unsigned int twtr = 0;
	unsigned int texsr = 0;
	unsigned int txp = 0;
	unsigned int txpdll = 0;
	unsigned int tzqcs = 0;
	unsigned int tzqcsi = 0;
	unsigned int tdqs = 0;
	unsigned int tcksre = 0;
	unsigned int tcksrx = 0;
	unsigned int tcke = 0;
	unsigned int tmod = 0;
	unsigned int trstl = 0;
	unsigned int tzqcl = 0;
	unsigned int tmrr = 0;
	unsigned int tckesr = 0;
	unsigned int tdpd = 0;
	unsigned int tccd = 0;
	unsigned int taond = 0;
	unsigned int tfaw = 0;
	unsigned int trtodt = 0;
	unsigned int tdqsck = 0;
	unsigned int tdqsckmax = 0;
	unsigned int tdllk = 0;
	unsigned int titmsrst = 0;
	unsigned int tdlllock = 0;
	unsigned int tdllsrst = 0;
	unsigned int tdinit0 = 0;
	unsigned int tdinit1 = 0;
	unsigned int tdinit2 = 0;
	unsigned int tdinit3 = 0;

	if((para->dram_tpr13 & 0x1) == 0)//auto detect
	{
		if(para->dram_type == 6)//LPDDR2
		{

		}else if(para->dram_type == 3)//DDR3
		{
			trefi 	= 78;
			tmrd  	= 4;
			trfc	= 140;
			trp		= 9;
			tprea	= 0;
			trtw	= 2;
			tal		= 0;
			tcl		= 9;
			tcwl    = 8;
			tras	= 24;
			trc		= 33;
			trcd	= 9;
			trrd	= 4;
			trtp	= 5;
			twr		= 10;
			twtr	= 5;
			texsr	= 512;
			txp		= 5;
			txpdll	= 16;
			tzqcs	= 64;
			tzqcsi	= 0;
			tdqs	= 1;
			tcksre	= 7;
			tcksrx	= 7;
			tcke	= 4;
			tmod 	= 12;
			trstl	= 80;
			tzqcl	= 512;
			tmrr	= 2;
			tckesr	= 5;
			tdpd	= 0;
			tccd 	= 0;
			taond	= 0;
			tfaw	= 22;
			trtodt	= 0;
			tdqsck	= 1;
			tdqsckmax = 1;
			tdllk	= 512;
			titmsrst= 10;
			tdlllock = 2750;
			tdllsrst= 27;
			tdinit0 = 266525;
			tdinit1 = 192;
			tdinit2	= 106610;
			tdinit3 = 534;
			if(para->dram_clk <= 480)
			{
				trfc	= 115;
				tras	= 18;
				trc		= 23;
				trtp	= 4;
				twr		= 8;
				twtr	= 4;
				txp		= 4;
				txpdll	= 14;
				tcksre	= 5;
				tcksrx  = 5;
				tfaw	= 20;
				tdlllock = 2250;
				tdllsrst= 23;
				tdinit0 = 217000;
				tdinit1 = 160;
				tdinit2	= 87000;
				tdinit3 = 433;
			}
			para->dram_mr0		=	0x1A00;
			{
				if(tcl >= 12)
				{
					para->dram_mr0 |= 0x1<<2;
					para->dram_mr0 |= (tcl-12)<<4;
				}else
				{
					para->dram_mr0 |= (tcl-4)<<4;
				}
			}
			para->dram_mr1		=	0x4;
			if(tal != 0)
			{
				para->dram_mr1	|= (tcl - tal)<<3;
			}
			para->dram_mr2		=	(tcwl-5)<<3;
			para->dram_mr3		=	0;

			para->dram_tpr0		=	tzqcsi;
			para->dram_tpr1		= 	(texsr<<22)|(tdpd<<12)|(tzqcl<<2)|(tprea<<0);
			para->dram_tpr2		=	(trfc<<23)|(trefi<<15)|(tmrr<<7)|(tzqcs<<0);
			para->dram_tpr3		=	(trstl<<25)|(tras<<19)|(trc<<13)|(txpdll<<7)|\
									(trp<<3)|(tmrd<<0);
			para->dram_tpr4		=	(tcksre<<27)|(tcksrx<<22)|(tcke<<17)|(tmod<<12)|\
									(trtw<<8)|(tal<<4)|(tcl<<0);
			para->dram_tpr5		=	(tcwl<<28)|(trcd<<24)|(trrd<<20)|(trtp<<16)|\
									(twr<<11)|(twtr<<7)|(tckesr<<3)|(txp<<0);
			para->dram_tpr6		=	(tdqs<<29) | (tdllk<<19) | (tfaw<<13) | (tdqsck<<10) |\
									(tdqsckmax<<7) | (tccd<<6) | (trtodt<<5) | (trtw<<4) |\
									(taond<<2);
			para->dram_tpr7		=	(tdllsrst<<0) | (tdlllock<<6) | (titmsrst<<18);
			para->dram_tpr8		=	(tdinit0<<0) | (tdinit1<<19);
			para->dram_tpr9		=	(tdinit2<<0) | (tdinit3<<17);
		}

	}else
	{
		trefi = ((para->dram_tpr2)>>15)&0xFF;
		tmrd  = ((para->dram_tpr3)>>0)&0x7;
		trfc  = ((para->dram_tpr2)>>23)&0x1FF;
		trp   = ((para->dram_tpr3)>>3)&0xF;
		tprea = ((para->dram_tpr1)>>0)&0x3;
		trtw  = ((para->dram_tpr4)>>8)&0xF;
		tal   = ((para->dram_tpr4)>>4)&0xF;
		tcl   = ((para->dram_tpr4)>>0)&0xF;
		tcwl  = ((para->dram_tpr5)>>28)&0xF;
		tras  = ((para->dram_tpr3)>>19)&0x3F;
		trc   = ((para->dram_tpr3)>>13)&0x3F;
		trcd  = ((para->dram_tpr5)>>24)&0xF;
		trrd  = ((para->dram_tpr5)>>20)&0xF;
		trtp  = ((para->dram_tpr5)>>16)&0xF;
		twr   = ((para->dram_tpr5)>>11)&0x1F;
		twtr  = ((para->dram_tpr5)>>7)&0xF;
		texsr = ((para->dram_tpr1)>>22)&0x3FF;
		txp   = ((para->dram_tpr5)>>0)&0x7;
		txpdll= ((para->dram_tpr3)>>7)&0x3F;
		tzqcs = ((para->dram_tpr2)>>0)&0x7F;
		tzqcsi= (para->dram_tpr0);
		tdqs  = ((para->dram_tpr6)>>29)&0x7;
		tcksre= ((para->dram_tpr4)>>27)&0x1F;
		tcksrx= ((para->dram_tpr4)>>22)&0x1F;
		tcke  = ((para->dram_tpr4)>>17)&0x1F;
		tmod  = ((para->dram_tpr4)>>12)&0x1F;
		trstl = ((para->dram_tpr3)>>25)&0x7F;
		tzqcl = ((para->dram_tpr1)>>2)&0x3FF;
		tmrr  = ((para->dram_tpr2)>>7)&0xFF;
		tckesr= ((para->dram_tpr5)>>3)&0xF;
		tdpd  =	((para->dram_tpr1)>>12)&0x3FF;
		tccd 	= ((para->dram_tpr6)>>6)&0x1;
		taond	= ((para->dram_tpr6)>>2)&0x3;
		tfaw	= ((para->dram_tpr6)>>13)&0x3F;
		trtodt	= ((para->dram_tpr6)>>5)&0x1;
		tdqsck	= ((para->dram_tpr6)>>10)&0x7;
		tdqsckmax = ((para->dram_tpr6)>>7)&0x7;
		tdllk	= ((para->dram_tpr6)>>19)&0x3FF;
		titmsrst= ((para->dram_tpr7)>>18)&0xF;
		tdlllock = ((para->dram_tpr7)>>6)&0xFFF;
		tdllsrst= ((para->dram_tpr7)>>0)&0x3F;
		tdinit0 = ((para->dram_tpr8)>>0)&0x7FFFF;
		tdinit1 = ((para->dram_tpr8)>>19)&0xFF;
		tdinit2	= ((para->dram_tpr9)>>0)&0x1FFFF;
		tdinit3 = ((para->dram_tpr9)>>17)&0x3FF;
	}

#if 0
	reg_val = 0;
#else
	reg_val = mctl_read_w(R_VDD_SYS_PWROFF_GATE);
#endif

	if(ch_index == 1)
	{
		ch_id = 0x1000;
		hold_flag = (reg_val)&0x1;
		standby_serial_putc('c');
		standby_serial_putc('1');
		standby_serial_putc('\n');
	}
	else
	{
		ch_id = 0x0;
		hold_flag = (reg_val>>1)&0x1;
		standby_serial_putc('c');
		standby_serial_putc('0');
		standby_serial_putc('\n');
	}


	reg_val = 0x83000000;
	mctl_write_w(ch_id + SDR_MCMD, reg_val);

	while(mctl_read_w(ch_id + SDR_MCMD) & 0x80000000)
		continue;

   reg_val = 0x01042202;
   reg_val |= 0x2<<22;
   reg_val &= ~(0x3<<12);
   if(((para->dram_para2>>12)&0xF) == 2)
   		reg_val |= (0x1<<19);
   mctl_write_w(ch_id + SDR_PGCR, reg_val);

	mctl_write_w(ch_id + SDR_MR0, para->dram_mr0);
	mctl_write_w(ch_id + SDR_MR1, para->dram_mr1);
	mctl_write_w(ch_id + SDR_MR2, para->dram_mr2);
	mctl_write_w(ch_id + SDR_MR3, para->dram_mr3);

	reg_val = titmsrst<<18;
	reg_val |= tdlllock<<6;
	reg_val |= tdllsrst<<0;
	mctl_write_w(ch_id + SDR_PTR0, reg_val);

	reg_val = tdinit0<<0;
	reg_val |= tdinit1<<19;
	mctl_write_w(ch_id + SDR_PTR1, reg_val);

	reg_val = tdinit2<<0;
	reg_val |= tdinit3<<17;
	mctl_write_w(ch_id + SDR_PTR2, reg_val);

	reg_val = (tccd)<<31;
	reg_val |= (trc)<<25;
	reg_val |= (trrd)<<21;
	reg_val |= (tras)<<16;
	reg_val |= (trcd)<<12;
	reg_val |= (trp)<<8;
	reg_val |= (twtr)<<5;
	reg_val |= (trtp)<<2;
	reg_val |= (tmrd)<<0;
	mctl_write_w(ch_id + SDR_DTPR0, reg_val);

	reg_val = tdqsckmax<<27;
	reg_val |= tdqsck<<24;
	reg_val |= trfc<<16;
	reg_val |= trtodt<<11;
	reg_val |= (tmod-12)<<9;
	reg_val |= 0<<2;
	reg_val |= tfaw<<3;
	reg_val |= taond<<0;
	mctl_write_w(ch_id + SDR_DTPR1, reg_val);

	reg_val = tdllk<<19;
	reg_val |= tcke<<15;
	reg_val |= txpdll<<10;
	reg_val |= texsr<<0;
	mctl_write_w(ch_id + SDR_DTPR2, reg_val);

	mctl_write_w(ch_id + SDR_DFITPHYUPDTYPE0, 1);

	if(para->dram_type == 2)	
		reg_val = 0xa;
	else if(para->dram_type == 3)
		reg_val = 0xb;
	else if(para->dram_type == 5)
		reg_val = 0x8;
	else						
		reg_val = 0xc;
	mctl_write_w(ch_id + SDR_DCR, reg_val);

	reg_val = 0xd200001b;
	if((para->dram_mr1 & 0x244) != 0)
		reg_val |= 0x1<<29;
	mctl_write_w(ch_id + SDR_DSGCR, reg_val);

	reg_val = 0x800;
	mctl_write_w(ch_id + SDR_DXCCR, reg_val);

if(para->dram_odt_en == 0){
	mctl_write_w(ch_id + SDR_DX0GCR, 0x881);
	mctl_write_w(ch_id + SDR_DX1GCR, 0x881);
	mctl_write_w(ch_id + SDR_DX2GCR, 0x881);
	mctl_write_w(ch_id + SDR_DX3GCR, 0x881);
}else{
	mctl_write_w(ch_id + SDR_DX0GCR, 0x2e81);
	mctl_write_w(ch_id + SDR_DX1GCR, 0x2e81);
	mctl_write_w(ch_id + SDR_DX2GCR, 0x2e81);
	mctl_write_w(ch_id + SDR_DX3GCR, 0x2e81);
}

	while( (mctl_read_w(ch_id + SDR_PGSR)&0x3)!= 0x3 ) {};

	mctl_write_w(ch_id + SDR_ZQ0CR1, para->dram_zq);
	reg_val = mctl_read_w(ch_id + SDR_PIR);
	reg_val |= 0x1<<28;
	mctl_write_w(ch_id + SDR_PIR, reg_val);

#if 0
	reg_val = 0x69;
#else
	if(hold_flag)
	{
		reg_val = 0x41;
	}
	else
	{
		reg_val = 0xe9;
	}
#endif
	mctl_write_w(ch_id + SDR_PIR, reg_val);

#ifndef SYSTEM_SIMULATION
	standby_timer_delay(10);
#endif

	if(!hold_flag)
	{
		while( (mctl_read_w(ch_id + SDR_PGSR)&0x1F) != 0x1F) {};//modify 12/3
	}else
	{
		standby_serial_putc('a');
		while( (mctl_read_w(ch_id + SDR_PGSR)&0x1F) != 0xB) {};//modify 12/3
		standby_serial_putc('b');
	}

	reg_val = 0x1;
	mctl_write_w(ch_id + SDR_SCTL, reg_val);
	while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x1 ) {};

	clkmhz = para->dram_clk;
	reg_val = clkmhz;
	mctl_write_w(ch_id + SDR_TOGCNT1U, reg_val);	
	reg_val = clkmhz/10;
	mctl_write_w(ch_id + SDR_TOGCNT100N, reg_val);	
	mctl_write_w(ch_id + SDR_TREFI	,trefi);
	mctl_write_w(ch_id + SDR_TMRD	,tmrd);
	mctl_write_w(ch_id + SDR_TRFC	,trfc);
	mctl_write_w(ch_id + SDR_TRP	,trp | (tprea<<16));
	mctl_write_w(ch_id + SDR_TRTW	,trtw);
	mctl_write_w(ch_id + SDR_TAL	,tal);
	mctl_write_w(ch_id + SDR_TCL	,tcl);
	mctl_write_w(ch_id + SDR_TCWL	,tcwl);
	mctl_write_w(ch_id + SDR_TRAS	,tras);
	mctl_write_w(ch_id + SDR_TRC	,trc);
	mctl_write_w(ch_id + SDR_TRCD	,trcd);
	mctl_write_w(ch_id + SDR_TRRD	,trrd);
	mctl_write_w(ch_id + SDR_TRTP	,trtp);
	mctl_write_w(ch_id + SDR_TWR	,twr);
	mctl_write_w(ch_id + SDR_TWTR	,twtr);
	mctl_write_w(ch_id + SDR_TEXSR	,texsr);
	mctl_write_w(ch_id + SDR_TXP	,txp);
	mctl_write_w(ch_id + SDR_TXPDLL ,txpdll);
	mctl_write_w(ch_id + SDR_TZQCS	,tzqcs);
	mctl_write_w(ch_id + SDR_TZQCSI ,tzqcsi);
	mctl_write_w(ch_id + SDR_TDQS   ,tdqs);
	mctl_write_w(ch_id + SDR_TCKSRE ,tcksre);
	mctl_write_w(ch_id + SDR_TCKSRX ,tcksrx);
	mctl_write_w(ch_id + SDR_TCKE 	,tcke);
	mctl_write_w(ch_id + SDR_TMOD 	,tmod);
	mctl_write_w(ch_id + SDR_TRSTL  ,trstl);
	mctl_write_w(ch_id + SDR_TZQCL  ,tzqcl);
	mctl_write_w(ch_id + SDR_TMRR 	,tmrr);
	mctl_write_w(ch_id + SDR_TCKESR ,tckesr);
	mctl_write_w(ch_id + SDR_TDPD 	,tdpd);

if((para->dram_mr1 & 0x244) != 0){
	reg_val = mctl_read_w(ch_id + SDR_DFIODTCFG);
	reg_val |= (0x1<<3);
	mctl_write_w(ch_id + SDR_DFIODTCFG, reg_val);

	reg_val = mctl_read_w(ch_id + SDR_DFIODTCFG1);
	reg_val &= ~((0x1f<<0));
	reg_val |= (0x0<<0);
	mctl_write_w(ch_id + SDR_DFIODTCFG1, reg_val);
}
	reg_val = 0x0;
	if(((para->dram_para2>>0)&0xF) == 0)
		reg_val = 0x1;
	mctl_write_w(ch_id + SDR_PPCFG, reg_val);

	if((((para->dram_tpr13>>5)&0x1) == 0) && (para->dram_type != 6))
	{
		reg_val = tcwl - 0;
		mctl_write_w(ch_id + SDR_DFITPHYWRL, reg_val);
		reg_val = tcl - 1;
		mctl_write_w(ch_id + SDR_DFITRDDEN, reg_val);
	}
	else
	{
		reg_val = tcwl - 1;
		mctl_write_w(ch_id + SDR_DFITPHYWRL, reg_val);
		reg_val = tcl - 2;
		mctl_write_w(ch_id + SDR_DFITRDDEN, reg_val);
	}
	mctl_write_w(ch_id + SDR_DFITPHYRDL, 15);

	reg_val = 0x5;
	mctl_write_w(ch_id + SDR_DFISTCFG0, reg_val);

	if(para->dram_type == 2)		
		reg_val = 0x70040;
	else if(para->dram_type == 3)	
		reg_val = 0x70061;
	else if(para->dram_type == 5)	
		reg_val = 0x970040;
	else							
		reg_val = 0xd70040;
	mctl_write_w(ch_id + SDR_MCFG, reg_val);

	reg_val = 0x2;
	mctl_write_w(ch_id + SDR_DFIUPDCFG, reg_val);

	reg_val = 0x2;
	mctl_write_w(ch_id + SDR_SCTL, reg_val);

	while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x3 ) {};

	if(hold_flag)
	{
		standby_serial_putc('d');

		reg_val = 0x3;
		mctl_write_w(ch_id + SDR_SCTL, reg_val);
		while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x5 ) {};

		standby_serial_putc('e');
		reg_val = mctl_read_w(R_VDD_SYS_PWROFF_GATE);
		if(ch_index == 1)
			reg_val &= ~(0x1);
		else
			reg_val &= ~(0x1<<1);
		mctl_write_w(R_VDD_SYS_PWROFF_GATE, reg_val);

		reg_val = 0x4;
		mctl_write_w(ch_id + SDR_SCTL, reg_val);
		while(  (mctl_read_w(ch_id + SDR_SSTAT)&0x7) != 0x3 ) {};

		standby_serial_putc('f');
		reg_val = 0x89;
		mctl_write_w(ch_id + SDR_PIR, reg_val);
		while( (mctl_read_w(ch_id + SDR_PGSR)&0x1) == 0x0) {};

		standby_serial_putc('g');
	}

	reg_val = mctl_read_w(ch_id + SDR_MCFG);
	reg_val |= 0x10 << 8;
	mctl_write_w(ch_id + SDR_MCFG, reg_val);

	return (1);
}

unsigned int mctl_com_init(__dram_para_t *para)
{
	unsigned int reg_val;

	reg_val = 0;
	if(((para->dram_para2>>12)&0xF) == 2)
		reg_val |= 0x1;

	if(((para->dram_para1>>28)&0xF) == 1)
		reg_val |= 0x1<<2;

	reg_val |= ((((para->dram_para1>>20)&0xFF) -1)&0xf)<<4;

/*
	if(para->dram_page_size == 8)
		reg_val |= 0xa<<8;
	else if(para->dram_page_size == 4)
		reg_val |= 0x9<<8;
	else if(para->dram_page_size == 2)
		reg_val |= 0x8<<8;
	else if(para->dram_page_size == 1)
		reg_val |= 0x7<<8;
	else
		reg_val |= 0x6<<8;
*/
	if(((para->dram_para1>>16)&0xF) == 8)
		reg_val |= 0xa<<8;
	else if(((para->dram_para1>>16)&0xF) == 4)
		reg_val |= 0x9<<8;
	else if(((para->dram_para1>>16)&0xF) == 2)
		reg_val |= 0x8<<8;
	else if(((para->dram_para1>>16)&0xF) == 1)
		reg_val |= 0x7<<8;
	else
		reg_val |= 0x6<<8;

/*
	if(para->dram_bus_width == 32)
		reg_val |= 0x3<<12;
	else
		reg_val |= 0x1<<12;
*/
	if(((para->dram_para2>>0)&0xF) == 1)
		reg_val |= 0x3<<12;
	else
		reg_val |= 0x1<<12;

/*
	if(para->dram_access_mode == 0)
		reg_val |= 0x1<<15;
*/
	if(((para->dram_para2>>4)&0xF) == 0)
		reg_val |= 0x1<<15;

	reg_val |= (para->dram_type)<<16;

/*
	if(para->dram_ch_num == 2)
		reg_val |= 0x1<<19;
*/
	if(((para->dram_para2>>8)&0xF) == 2)
		reg_val |= 0x1<<19;

	reg_val |= 0x1<<20;
#if 0
	if(para->dram_type != 6)
	{
		reg_val |= 0x1<<22;
	}else
	{
		reg_val |= 0x319<<20;
	}
#else
	reg_val |= 0x1<<22;
#endif
	mctl_write_w(SDR_COM_CR, reg_val);

	if((((para->dram_tpr13>>5)&0x1) == 0) && (para->dram_type != 6))
	{
		reg_val = mctl_read_w(SDR_COM_DBGCR);
		reg_val |= 1U << 6;
		mctl_write_w(SDR_COM_DBGCR, reg_val);
	}

#if 0
	if(para->dram_type == 2)
	{// for DDR2
		reg_val = 0x9;
		reg_val |= 0x9<<8;
	}
	else
	{// for LPDDR2
	}
	mctl_write_w(SDR_COM_DBGCR1, reg_val);

#endif


	return (1);
}

unsigned int mctl_port_cfg(void)
{
	unsigned int reg_val;

	reg_val = mctl_read_w(CCM_AXI_GATE_CTRL);
	reg_val |= 0x1;
	mctl_write_w(CCM_AXI_GATE_CTRL, reg_val);

	reg_val = 0x0400302;
	mctl_write_w(0x01c62010, reg_val);
	reg_val = 0x1000307;
	mctl_write_w(0x01c62014, reg_val);
	reg_val = 0x0400302;
	mctl_write_w(0x01c62018, reg_val);
	reg_val = 0x1000307;
	mctl_write_w(0x01c6201c, reg_val);
	reg_val = 0x1000307;
	mctl_write_w(0x01c62020, reg_val);
	reg_val = 0x1000303;
	mctl_write_w(0x01c62028, reg_val);

	reg_val = 0x1000303;
	mctl_write_w(0x01c62030, reg_val);
	reg_val = 0x0400310;
	mctl_write_w(0x01c62034, reg_val);
	reg_val = 0x1000307;
	mctl_write_w(0x01c62038, reg_val);
	reg_val = 0x1000303;
	mctl_write_w(0x01c6203c, reg_val);
	reg_val = 0x1800303;
	mctl_write_w(0x01c62040, reg_val);
	reg_val = 0x1800303;
	mctl_write_w(0x01c62044, reg_val);
	reg_val = 0x1800303;
	mctl_write_w(0x01c62048, reg_val);
	reg_val = 0x1800303;
	mctl_write_w(0x01c6204C, reg_val);
	reg_val = 0x1000303;
	mctl_write_w(0x01c62050, reg_val);

	reg_val = 0x00000002;
	mctl_write_w(0x01c6206C, reg_val);

	reg_val = 0x00000310;
	mctl_write_w(0x01c62070, reg_val);
	reg_val = 0x00400310;
	mctl_write_w(0x01c62074, reg_val);
	reg_val = 0x00400310;
	mctl_write_w(0x01c62078, reg_val);
	reg_val = 0x00000307;
	mctl_write_w(0x01c6207C, reg_val);
	reg_val = 0x00000317;
	mctl_write_w(0x01c62080, reg_val);
	reg_val = 0x00000307;
	mctl_write_w(0x01c62084, reg_val);

  	return (1);
}


signed int init_DRAM(int type, __dram_para_t *para)
{
	signed int ret_val;
	unsigned int id = 0;
	__dram_para_t *dram_para;

	dram_para = (__dram_para_t *)para;

#ifdef LINUX_CONFIG
	script_item_u val;
	script_item_value_type_e type;

	type = script_get_item("dram_para", "dram_clk", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_clk = val.val;

	type = script_get_item("dram_para", "dram_type", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_type = val.val;

	type = script_get_item("dram_para", "dram_zq", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_zq = val.val;

	type = script_get_item("dram_para", "dram_odt_en", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_odt_en = val.val;

	type = script_get_item("dram_para", "dram_para1", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_para1 = val.val;

	type = script_get_item("dram_para", "dram_para2", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_para2 = val.val;

	type = script_get_item("dram_para", "dram_mr0", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_mr0 = val.val;

	type = script_get_item("dram_para", "dram_mr1", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_mr1 = val.val;

	type = script_get_item("dram_para", "dram_mr2", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_mr2 = val.val;

	type = script_get_item("dram_para", "dram_mr3", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_mr3 = val.val;

	type = script_get_item("dram_para", "dram_tpr0", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr0 = val.val;

	type = script_get_item("dram_para", "dram_tpr1", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr1 = val.val;

	type = script_get_item("dram_para", "dram_tpr2", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr2 = val.val;

	type = script_get_item("dram_para", "dram_tpr3", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr3 = val.val;

	type = script_get_item("dram_para", "dram_tpr4", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr4 = val.val;

	type = script_get_item("dram_para", "dram_tpr5", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr5 = val.val;

	type = script_get_item("dram_para", "dram_tpr6", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr6 = val.val;

	type = script_get_item("dram_para", "dram_tpr7", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr7 = val.val;

	type = script_get_item("dram_para", "dram_tpr8", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr8 = val.val;

	type = script_get_item("dram_para", "dram_tpr9", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr9 = val.val;

	type = script_get_item("dram_para", "dram_tpr10", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr10 = val.val;

	type = script_get_item("dram_para", "dram_tpr11", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr11 = val.val;

	type = script_get_item("dram_para", "dram_tpr12", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr12 = val.val;

	type = script_get_item("dram_para", "dram_tpr13", &val);
	if(SCIRPT_ITEM_VALUE_TYPE_INT != type)
	{
		printk("type err!");
	}
	printk("value is %#x\n", val.val);
	dram_para->dram_tpr13 = val.val;
#endif

#if 0
#if 0
	dram_para->dram_clk        = 480;
	dram_para->dram_type       = 3;
	dram_para->dram_zq         = 0x17b;
	dram_para->dram_odt_en     = 0;
	dram_para->dram_para1      = 0x10F40800;
	dram_para->dram_para2      = 0x1111;
	dram_para->dram_mr0        = 0x1A50;
	dram_para->dram_mr1        = 0x4;
	dram_para->dram_mr2        = 0x8;
	dram_para->dram_mr3        = 0;
	dram_para->dram_tpr0       = 0;
	dram_para->dram_tpr1       = 0x80000800;
	dram_para->dram_tpr2       = 0x39a70140;
	dram_para->dram_tpr3       = 0xa092e74c;
	dram_para->dram_tpr4       = 0x2948c209;
	dram_para->dram_tpr5       = 0x6944422c;
	dram_para->dram_tpr6       = 0x300284a0;
	dram_para->dram_tpr7       = 0x2a3297;
	dram_para->dram_tpr8       = 0x5034fa8;
	dram_para->dram_tpr9       = 0x36353d8;
	dram_para->dram_tpr10      = 0;
	dram_para->dram_tpr11      = 0;
	dram_para->dram_tpr12      = 0;
	dram_para->dram_tpr13      = 0x7;
#else
	dram_para->dram_clk        = 360;
	dram_para->dram_type       = 3;
	dram_para->dram_zq         = 0x0bb;
	dram_para->dram_odt_en     = 0;
	dram_para->dram_para1      = 0x10F40800;
	dram_para->dram_para2      = 0x1211;
	dram_para->dram_mr0        = 0x1A50;
	dram_para->dram_mr1        = 0;
	dram_para->dram_mr2        = 0x18;
	dram_para->dram_mr3        = 0;
	dram_para->dram_tpr0       = 0;
	dram_para->dram_tpr1       = 0x80000800;
	dram_para->dram_tpr2       = 0x46270140;
	dram_para->dram_tpr3       = 0xA0C4284C;
	dram_para->dram_tpr4       = 0x39c8c209;
	dram_para->dram_tpr5       = 0x694552AD;
	dram_para->dram_tpr6       = 0x3002c4a0;
	dram_para->dram_tpr7       = 0x2aaf9b;
	dram_para->dram_tpr8       = 0x604111d;
	dram_para->dram_tpr9       = 0x42da072;
	dram_para->dram_tpr10      = 0;
	dram_para->dram_tpr11      = 0;
	dram_para->dram_tpr12      = 0;
	dram_para->dram_tpr13      = 0;
#endif
#endif

#if 0
	msg("dram_para->dram_type       = %x\n", dram_para->dram_type );
	msg("dram_para->dram_zq         = %x\n", dram_para->dram_zq   );
	msg("dram_para->dram_odt_en     = %x\n", dram_para->dram_odt_en);
	msg("dram_para->dram_para1      = %x\n", dram_para->dram_para1);
	msg("dram_para->dram_para2      = %x\n", dram_para->dram_para2);
	msg("dram_para->dram_mr0        = %x\n", dram_para->dram_mr0  );
	msg("dram_para->dram_mr1        = %x\n", dram_para->dram_mr1  );
	msg("dram_para->dram_mr2        = %x\n", dram_para->dram_mr2  );
	msg("dram_para->dram_mr3        = %x\n", dram_para->dram_mr3  );
	msg("dram_para->dram_tpr0       = %x\n", dram_para->dram_tpr0 );
	msg("dram_para->dram_tpr1       = %x\n", dram_para->dram_tpr1 );
	msg("dram_para->dram_tpr2       = %x\n", dram_para->dram_tpr2 );
	msg("dram_para->dram_tpr3       = %x\n", dram_para->dram_tpr3 );
	msg("dram_para->dram_tpr4       = %x\n", dram_para->dram_tpr4 );
	msg("dram_para->dram_tpr5       = %x\n", dram_para->dram_tpr5 );
	msg("dram_para->dram_tpr6       = %x\n", dram_para->dram_tpr6 );
	msg("dram_para->dram_tpr7       = %x\n", dram_para->dram_tpr7 );
	msg("dram_para->dram_tpr8       = %x\n", dram_para->dram_tpr8 );
	msg("dram_para->dram_tpr9       = %x\n", dram_para->dram_tpr9 );
	msg("dram_para->dram_tpr10      = %x\n", dram_para->dram_tpr10);
	msg("dram_para->dram_tpr11      = %x\n", dram_para->dram_tpr11);
	msg("dram_para->dram_tpr12      = %x\n", dram_para->dram_tpr12);
	msg("dram_para->dram_tpr13      = %x\n", dram_para->dram_tpr13);
#endif
	id = ss_bonding_id();

	if(id == 0)
	{
		dram_para->dram_tpr13 |= 0x1<<7;
		if(type == 1)
		{
			dram_para->dram_tpr13 |= 0x3<<1;
		}

		paraconfig(&(dram_para->dram_para2), 0xF<<8, 2<<8);
		paraconfig(&(dram_para->dram_para2), 0xF<<0, 1<<0);


	}else if(id == 1)
	{
		dram_para->dram_tpr13 |= 0x1<<8;
		dram_para->dram_tpr13 |= 0x1<<3;
		if(type == 1)
		{
			dram_para->dram_tpr13 |= 0x3<<1;
		}

		paraconfig(&(dram_para->dram_para2), 0xF<<8, 1<<8);
		paraconfig(&(dram_para->dram_para2), 0xF<<0, 1<<0);


	}else if(id == 2)
	{
		dram_para->dram_tpr13 |= 0x1<<4;
	}

	ret_val = DRAMC_init(dram_para);

	standby_serial_putc('a');
	standby_serial_putc('\n');

	return ret_val;
}

unsigned int ss_bonding_id(void)
{
	unsigned int reg_val;
	unsigned int id;
	reg_val = mctl_read_w(0x01c20000 + 0x9C);
	reg_val &= ~(0x3<<24);
	reg_val &= ~(0x3<<16);
	reg_val &= ~(0xf);
	reg_val |= 0x0<<16;
	reg_val |= 0;
	reg_val |= 0x1U<<31;
	mctl_write_w(0x01c20000 + 0x9C, reg_val);

	reg_val = mctl_read_w(0x01c20000 + 0x60);
	reg_val |= 0x1<<5;	
	mctl_write_w(0x01c20000 + 0x60, reg_val);

	reg_val = mctl_read_w(0x01C15000 + 0x00);
	reg_val >>=16;
	reg_val &=0x3;
	mctl_write_w(0x01C15000 + 0x00,reg_val);

	id = reg_val;

	reg_val = mctl_read_w(0x01C15000 + 0x00);
	reg_val &= ~0x1;
	mctl_write_w(0x01C15000 + 0x00,reg_val);

	return id;
}

void paraconfig(unsigned int *para, unsigned int mask, unsigned int value)
{
	unsigned int reg_val = *para;

	reg_val &= ~(mask);
	reg_val |= value;

	*para = reg_val;
}
