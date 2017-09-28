/*
**********************************************************************************************************************
*
*						           the Embedded Secure Bootloader System
*
*
*						       Copyright(C), 2006-2014, Allwinnertech Co., Ltd.
*                                           All Rights Reserved
*
* File    :
*
* By      :
*
* Version : V2.00
*
* Date	  :
*
* Descript:
**********************************************************************************************************************
*/
#include "common.h"
#include "asm/io.h"
#include "asm/arch/ccmu.h"
#include "asm/arch/ss.h"
#include "asm/arch/mmu.h"
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
static u32 __aw_endian4(u32 data)
{
	u32 d1, d2, d3, d4;
	d1= (data&0xff)<<24;
	d2= (data&0xff00)<<8;
	d3= (data&0xff0000)>>8;
	d4= (data&0xff000000)>>24;

	return (d1|d2|d3|d4);
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
static u32 __sha256_padding(u32 data_size, u8* text)
{
	u32 i;
	u32 k, q;
	u32 size;
	u32 padding_buf[16];
    u8 *ptext;

	k = data_size/64;
	q = data_size%64;

	ptext = (u8*)padding_buf;
	if(q==0){
		for(i=0; i<16; i++){
			padding_buf[i] = 0x0;
		}
		padding_buf[0] = 0x00000080;

		padding_buf[14] = data_size>>29;
		padding_buf[15] = data_size<<3;
		padding_buf[14] = __aw_endian4(padding_buf[14]);
		padding_buf[15] = __aw_endian4(padding_buf[15]);

		for(i=0; i<64; i++){
			text[k*64 + i] = ptext[i];
		}
		size = (k + 1)*64;
	}else if(q<56){
		for(i=0; i<16; i++){
			padding_buf[i] = 0x0;
		}
		for(i=0; i<q; i++){
			ptext[i] = text[k*64 + i];
		}
		ptext[q] = 0x80;


		padding_buf[14] = data_size>>29;
		padding_buf[15] = data_size<<3;
		padding_buf[14] = __aw_endian4(padding_buf[14]);
		padding_buf[15] = __aw_endian4(padding_buf[15]);

		for(i=0; i<64; i++){
			text[k*64 + i] = ptext[i];
		}
		size = (k + 1)*64;
	}else{
		for(i=0; i<16; i++){
			padding_buf[i] = 0x0;
		}
		for(i=0; i<q; i++){
			ptext[i] = text[k*64 + i];
		}
		ptext[q] = 0x80;
		for(i=0; i<64; i++){
			text[k*64 + i] = ptext[i];
		}

		for(i=0; i<16; i++){
			padding_buf[i] = 0x0;
		}
		padding_buf[14] = data_size>>29;
		padding_buf[15] = data_size<<3;
		padding_buf[14] = __aw_endian4(padding_buf[14]);
		padding_buf[15] = __aw_endian4(padding_buf[15]);

		for(i=0; i<64; i++){
			text[(k + 1)*64 + i] = ptext[i];
		}
		size = (k + 2)*64;
	}

	return size;
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
static void __rsa_padding(u8 *dst_buf, u8 *src_buf, u32 data_len, u32 group_len)
{
	int i = 0;

	memset(dst_buf, 0, group_len);
	for(i = group_len - data_len; i < group_len; i++)
	{
		dst_buf[i] = src_buf[group_len - 1 - i];
	}
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
void sunxi_ss_open(void)
{
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
void sunxi_ss_close(void)
{
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
int  sunxi_sha_calc(u8 *dst_addr, u32 dst_len,
					u8 *src_addr, u32 src_len)
{
	u32 reg_val = 0;
	u32 total_len = 0;
	u32 md_size = 0;
	s32 i = 0;
	u8  iv_buff[32 + 32], *p_iv;
	u8  sign_buff[32 + 32], *p_sign;

	memset(sign_buff, 0, sizeof(sign_buff));
	memset(iv_buff, 0, sizeof(iv_buff));
	p_iv 	= (u8 *)(((u32)iv_buff + 31)&(~31));
	p_sign =  (u8 *)(((u32)sign_buff + 31)&(~31));
	reg_val = readl(SS_CTL);
	reg_val &= ~(0xf<<2);
	reg_val &= ~(0x1<<17);		
	reg_val |= 0x1<<30;        	

	reg_val |= 0x8<<2;	      
	md_size = 32;

	writel(reg_val, SS_CTL);
	writel(va2pa((u32)src_addr)	, SS_DATA_SRC_LOW_ADR);
	writel(0		        , SS_DATA_SRC_HIGH_ADR);
	writel((u32)p_sign, SS_DATA_DST_LOW_ADR);
	writel(0	      , SS_DATA_DST_HIGH_ADR);
	total_len = __sha256_padding(src_len,(u8 *)src_addr);	//计算明文长度

	writel(total_len/4,SS_DATA_LEN);
	writel((u32)p_iv, SS_PM_LOW_ADR);
	writel(0	    , SS_PM_HIGH_ADR);
	reg_val = readl(SS_INT_CTRL);
	reg_val &= ~0x3;
	reg_val |= 1;
	writel(reg_val , SS_INT_CTRL);
	reg_val = readl(SS_CTL);
	reg_val &= ~(0x1U<<31);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1;
	writel(reg_val,SS_CTL);
	while((readl(SS_INT_STATUS)&0x01)==0);

	for(i=0; i< md_size; i++)
	{
	    dst_addr[i] = p_sign[i];   //从目的地址读生成的消息摘要
	}
	reg_val = readl(SS_INT_STATUS);
	if((reg_val&0x1)==1)
	{
		reg_val &= ~(0x3);
		reg_val |= 0x1;
	}
	writel(reg_val,SS_INT_STATUS);
	reg_val = readl(SS_CTL);
	reg_val &= ~0x1;
	writel(reg_val,SS_CTL);

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
s32 sunxi_rsa_calc(u8 * n_addr,   u32 n_len,
				   u8 * e_addr,   u32 e_len,
				   u8 * dst_addr, u32 dst_len,
				   u8 * src_addr, u32 src_len)
{
#define	TEMP_BUFF_LEN	((2048>>3) + 32)

	u32 reg_val = 0;
	u8	temp_n_addr[TEMP_BUFF_LEN],   *p_n;
	u8	temp_e_addr[TEMP_BUFF_LEN],   *p_e;
	u8	temp_src_addr[TEMP_BUFF_LEN], *p_src;
	u8	temp_dst_addr[TEMP_BUFF_LEN], *p_dst;
	u32 mod_bit_size = 2048;

	u32 mod_size_len_inbytes = mod_bit_size/8;

	p_n = (u8 *)(((u32)temp_n_addr + 31)&(~31));
	p_e = (u8 *)(((u32)temp_e_addr + 31)&(~31));
	p_src = (u8 *)(((u32)temp_src_addr + 31)&(~31));
	p_dst = (u8 *)(((u32)temp_dst_addr + 31)&(~31));

	__rsa_padding(p_src, src_addr, src_len, mod_size_len_inbytes);
	__rsa_padding(p_n, n_addr, n_len, mod_size_len_inbytes);

	memset(p_e, 0, mod_size_len_inbytes);
	memcpy(p_e, e_addr, e_len);
	reg_val = readl(SS_CTL);
	reg_val &= ~(0xf<<2);
	reg_val |= 9<<2;
	reg_val &= ~(0x1<<6);	
	reg_val &= ~(0x3<<9);
	reg_val |= 0x2<<9;		
	reg_val &= ~(0x3U<<30);
	reg_val |= 0x1<<30;
	writel(reg_val, SS_CTL);
	writel(mod_bit_size/32, SS_DATA_LEN);
	writel((u32)p_src, SS_DATA_SRC_LOW_ADR);
	writel(0		 , SS_DATA_SRC_HIGH_ADR);
	writel((u32)p_e  , SS_KEY_LOW_ADR);
	writel(0		 , SS_KEY_HIGH_ADR);
	writel((u32)p_dst, SS_DATA_DST_LOW_ADR);
	writel(0		 , SS_DATA_DST_HIGH_ADR);
	writel((u32)p_n  , SS_PM_LOW_ADR);
	writel(0		 , SS_PM_HIGH_ADR);

	reg_val = readl(SS_INT_CTRL);
	reg_val &= ~0x3;
	reg_val |= 1;
	writel(reg_val, SS_INT_CTRL);
	reg_val = readl(SS_CTL);
	reg_val &= ~(0x1U<<31);
	reg_val &= ~(0x1<<30);
	reg_val |= 0x1;
	writel(reg_val,SS_CTL);
	while((readl(SS_INT_STATUS)&0x01)==0);
	__rsa_padding(dst_addr, p_dst, mod_bit_size/64, mod_bit_size/64);
	reg_val = readl(SS_INT_STATUS);
	if((reg_val&0x1)==1)
	{
		reg_val &= ~(0x3);
		reg_val |= 0x1;
	}
	writel(reg_val,SS_INT_STATUS);
	reg_val = readl(SS_CTL);
	reg_val &= ~0x1;
	writel(reg_val,SS_CTL);

	return 0;
}


