/*
 * Copyright 2008 Freescale Semiconductor, Inc.
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
#include <config.h>

#include <openssl/sha.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/bn.h>
#include <openssl/pem.h>

#include "../fs/aw_fs/ff.h"

extern RSA *PEM_read_RSA_PUBKEY(FILE *fp, RSA **x, void *cb, void *u);
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
int sunxi_rsa_publickey_decrypt(char *source_str, char *decryped_data, int data_bytes, char *key_path)
{
    RSA *p_rsa;
    FILE file;
    int rsa_len, ret = -1;

	printf("sunxi_rsa_publickey_decrypt key name=%s\n", key_path);

	ret = f_open (&file, key_path, FA_OPEN_EXISTING | FA_READ );
    if(ret)
    {
        printf("open key file error\n");

        return -1;
    }
    if((p_rsa=PEM_read_RSA_PUBKEY(&file,NULL,NULL,NULL))==NULL)
    {
        printf("unable to get public key\n");

        goto __sunxi_rsa_publickey_decrypt_err;
    }

    rsa_len=RSA_size(p_rsa);

    if(RSA_public_decrypt(rsa_len,(unsigned char *)source_str, (unsigned char*)decryped_data, p_rsa, RSA_NO_PADDING)<0)
    {
        goto __sunxi_rsa_publickey_decrypt_err;
    }
    ret = 0;

__sunxi_rsa_publickey_decrypt_err:
	f_close(&file);

	if(p_rsa != NULL)
		RSA_free(p_rsa);

    return ret;
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
