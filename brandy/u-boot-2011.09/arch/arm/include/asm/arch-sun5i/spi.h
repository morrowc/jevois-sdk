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
#ifndef   __SPI_H__
#define   __SPI_H__

#include  <asm/arch/cpu.h>

#define SPI_CTL_MSDC_OFFSET     0x13
#define SPI_CTL_TPE_OFFSET      0x12
#define SPI_CTL_SS_LEVEL_OFFSET 0x11
#define SPI_CTL_SS_CTL_OFFSET   0x10
#define SPI_CTL_DHB_OFFSET      0x0f
#define SPI_CTL_DDB_OFFSET      0x0e
#define SPI_CTL_SS_OFFSET       0x0c
#define SPI_CTL_SMC_OFFSET      0x0b
#define SPI_CTL_XCH_OFFSET      0x0a
#define SPI_CTL_RFRST_OFFSET    0x09
#define SPI_CTL_TFRST_OFFSET    0x08
#define SPI_CTL_SSCTL_OFFSET    0x07
#define SPI_CTL_LMTF_OFFSET     0x06
#define SPI_CTL_TBW_OFFSET      0x05
#define SPI_CTL_SSPOL_OFFSET    0x04
#define SPI_CTL_POL_OFFSET      0x03
#define SPI_CTL_PHA_OFFSET      0x02
#define SPI_CTL_MODE_OFFSET     0x01
#define SPI_CTL_EN_OFFSET       0x00

#define SPI_DDMA_PARM   0x7070707

#define SPI_RX_DATA      (SUNXI_SPI0_BASE + 0x00)       
#define SPI_TX_DATA      (SUNXI_SPI0_BASE + 0x04)       
#define SPI_CONTROL      (SUNXI_SPI0_BASE + 0x08)       
#define SPI_INTCTRL      (SUNXI_SPI0_BASE + 0x0c)       
#define SPI_INTSTAT      (SUNXI_SPI0_BASE + 0x10)       
#define SPI_DMACTRL      (SUNXI_SPI0_BASE + 0x14)       
#define SPI_WATCLCK      (SUNXI_SPI0_BASE + 0x18)       
#define SPI_CLCKRATE     (SUNXI_SPI0_BASE + 0x1c)       
#define SPI_BURSTCNT     (SUNXI_SPI0_BASE + 0x20)       
#define SPI_TRANSCNT     (SUNXI_SPI0_BASE + 0x24)       
#define SPI_FIFOSTAT     (SUNXI_SPI0_BASE + 0x28)       

#define SPI_MCLK        36         
#define SPI_SCLK        18         

int   spic_init(unsigned int spi_no);
int   spic_exit(unsigned int spi_no);
int   spic_rw  (unsigned int tcnt, void* txbuf, unsigned int rcnt, void* rxbuf);


#endif

