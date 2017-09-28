#include <linux/slab.h>
#include "usb.h"
#include "scsiglue.h"
#include "smcommon.h"
#include "smil.h"

int         Check_D_LogCHS              (WORD *,BYTE *,BYTE *);
void        Initialize_D_Media          (void);
void        PowerOff_D_Media            (void);
int         Check_D_MediaPower          (void);
int         Check_D_MediaExist          (void);
int         Check_D_MediaWP             (void);
int         Check_D_MediaFmt            (struct us_data *);
int         Check_D_MediaFmtForEraseAll (struct us_data *);
int         Conv_D_MediaAddr            (struct us_data *, DWORD);
int         Inc_D_MediaAddr             (struct us_data *);
int         Check_D_FirstSect           (void);
int         Check_D_LastSect            (void);
int         Media_D_ReadOneSect         (struct us_data *, WORD, BYTE *);
int         Media_D_WriteOneSect        (struct us_data *, WORD, BYTE *);
int         Media_D_CopyBlockHead       (struct us_data *);
int         Media_D_CopyBlockTail       (struct us_data *);
int         Media_D_EraseOneBlock       (void);
int         Media_D_EraseAllBlock       (void);

int  Copy_D_BlockAll             (struct us_data *, DWORD);
int  Copy_D_BlockHead            (struct us_data *);
int  Copy_D_BlockTail            (struct us_data *);
int  Reassign_D_BlockHead        (struct us_data *);

int  Assign_D_WriteBlock         (void);
int  Release_D_ReadBlock         (struct us_data *);
int  Release_D_WriteBlock        (struct us_data *);
int  Release_D_CopySector        (struct us_data *);

int  Copy_D_PhyOneSect           (struct us_data *);
int  Read_D_PhyOneSect           (struct us_data *, WORD, BYTE *);
int  Write_D_PhyOneSect          (struct us_data *, WORD, BYTE *);
int  Erase_D_PhyOneBlock         (struct us_data *);

int  Set_D_PhyFmtValue           (struct us_data *);
int  Search_D_CIS                (struct us_data *);
int  Make_D_LogTable             (struct us_data *);
void Check_D_BlockIsFull         (void);

int  MarkFail_D_PhyOneBlock      (struct us_data *);

DWORD ErrXDCode;
DWORD ErrCode;
static BYTE  WorkBuf[SECTSIZE];
static BYTE  Redundant[REDTSIZE];
static BYTE  WorkRedund[REDTSIZE];
static WORD  *Log2Phy[MAX_ZONENUM];                
static BYTE  Assign[MAX_ZONENUM][MAX_BLOCKNUM/8];
static WORD  AssignStart[MAX_ZONENUM];
WORD  ReadBlock;
WORD  WriteBlock;
DWORD MediaChange;
static DWORD SectCopyMode;

static BYTE BitData[] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80 } ;
#define Set_D_Bit(a,b)    (a[(BYTE)((b)/8)]|= BitData[(b)%8])
#define Clr_D_Bit(a,b)    (a[(BYTE)((b)/8)]&=~BitData[(b)%8])
#define Chk_D_Bit(a,b)    (a[(BYTE)((b)/8)] & BitData[(b)%8])

BYTE     IsSSFDCCompliance;
BYTE     IsXDCompliance;



int SM_FreeMem(void)
{
	int	i;

	pr_info("SM_FreeMem start\n");
	for (i=0; i<MAX_ZONENUM; i++)
	{
		if (Log2Phy[i]!=NULL)
		{
			pr_info("Free Zone = %x, Addr = %p\n", i, Log2Phy[i]);
			kfree(Log2Phy[i]);
			Log2Phy[i] = NULL;
		}
	}
	return(NO_ERROR);
}


int Media_D_ReadSector(struct us_data *us, DWORD start,WORD count,BYTE *buf)
{
	WORD len, bn;

	if (Conv_D_MediaAddr(us, start))
		return(ErrCode);

	while(1)
	{
		len = Ssfdc.MaxSectors - Media.Sector;
		if (count > len)
			bn = len;
		else
			bn = count;
		if (Media_D_ReadOneSect(us, bn, buf))
		{
			ErrCode = ERR_EccReadErr;
			return(ErrCode);
		}

		Media.Sector += bn;
		count -= bn;

		if (count<=0)
			break;

		buf += bn * SECTSIZE;

		if (Inc_D_MediaAddr(us))
			return(ErrCode);
	}

	return(NO_ERROR);
}
int Media_D_CopySector(struct us_data *us, DWORD start,WORD count,BYTE *buf)
{
	WORD len, bn;

	/* pr_info("Media_D_CopySector !!!\n"); */
	if (Conv_D_MediaAddr(us, start))
		return(ErrCode);

	while(1)
	{
		if (Assign_D_WriteBlock())
			return(ERROR);

		len = Ssfdc.MaxSectors - Media.Sector;
		if (count > len)
			bn = len;
		else
		bn = count;

		if (Ssfdc_D_CopyBlock(us,bn,buf,Redundant))
		{
			ErrCode = ERR_WriteFault;
			return(ErrCode);
		}

		Media.Sector = 0x1F;
		if (Release_D_CopySector(us))
		{
			if (ErrCode==ERR_HwError)
			{
				ErrCode = ERR_WriteFault;
				return(ErrCode);
			}
		}
		count -= bn;

		if (count<=0)
			break;

		buf += bn * SECTSIZE;

		if (Inc_D_MediaAddr(us))
			return(ErrCode);

	}
	return(NO_ERROR);
}

int Release_D_CopySector(struct us_data *us)
{

	Log2Phy[Media.Zone][Media.LogBlock]=WriteBlock;
	Media.PhyBlock=ReadBlock;

	if (Media.PhyBlock==NO_ASSIGN)
	{
		Media.PhyBlock=WriteBlock;
		return(SMSUCCESS);
	}

	Clr_D_Bit(Assign[Media.Zone],Media.PhyBlock);
	Media.PhyBlock=WriteBlock;

	return(SMSUCCESS);
}
/*
int Media_D_WriteSector(PFDO_DEVICE_EXTENSION fdoExt, DWORD start,WORD count,BYTE *buf)
{
    int i;
    WORD len, bn;
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;


    if (Conv_D_MediaAddr(fdoExt, start))
        return(ErrCode);

    if (Check_D_FirstSect())
    {
        if (Media_D_CopyBlockHead(fdoExt))
        {
            ErrCode = ERR_WriteFault;
            return(ErrCode);
        }
    }

    while(1)
    {
        if (!Check_D_FirstSect())
        {
            if (Assign_D_WriteBlock())
                return(ErrCode);
        }

        len = Ssfdc.MaxSectors - Media.Sector;
        if (count > len)
           bn = len;
        else
           bn = count;

        if (Media_D_WriteOneSect(fdoExt, bn, buf))
        {
            ErrCode = ERR_WriteFault;
            return(ErrCode);
        }

        Media.Sector += bn - 1;

        if (!Check_D_LastSect())
        {
            if (Release_D_ReadBlock(fdoExt))

            {    if (ErrCode==ERR_HwError)
                {
                    ErrCode = ERR_WriteFault;
                    return(ErrCode);
                }
            }
        }

        count -= bn;

        if (count<=0)
            break;

        buf += bn * SECTSIZE;


        if (Inc_D_MediaAddr(fdoExt))
            return(ErrCode);
    }

    if (!Check_D_LastSect())
        return(NO_ERROR);

    if (Inc_D_MediaAddr(fdoExt))
        return(ErrCode);

    if (Media_D_CopyBlockTail(fdoExt))
    {
        ErrCode = ERR_WriteFault;
        return(ErrCode);
    }

    return(NO_ERROR);
}

int Media_D_OneSectWriteStart(PFDO_DEVICE_EXTENSION fdoExt,DWORD start,BYTE *buf)
{

    return(NO_ERROR);
}

int Media_D_OneSectWriteNext(PFDO_DEVICE_EXTENSION fdoExt, BYTE *buf)
{

    return(NO_ERROR);
}

int Media_D_OneSectWriteFlush(PFDO_DEVICE_EXTENSION fdoExt)
{
    if (!Check_D_LastSect())
        return(NO_ERROR);

    if (Inc_D_MediaAddr(fdoExt))
        return(ErrCode);

    if (Media_D_CopyBlockTail(fdoExt))
    {
        ErrCode = ERR_WriteFault;
        return(ErrCode);
    }

    return(NO_ERROR);
}
*/
int Check_D_MediaFmt(struct us_data *us)
{
	pr_info("Check_D_MediaFmt\n");

	if (!MediaChange)
		return(SMSUCCESS);

	MediaChange  = ERROR;
	SectCopyMode = COMPLETED;

	if (Set_D_PhyFmtValue(us))
	{
		ErrCode = ERR_UnknownMedia;
		return(ERROR);
	}
	
	if (Search_D_CIS(us))
	{
		ErrCode = ERR_IllegalFmt;
		return(ERROR);
	}


    MediaChange = SMSUCCESS;
    return(SMSUCCESS);
}
/*
*/
int Conv_D_MediaAddr(struct us_data *us, DWORD addr)
{
	DWORD temp;

	temp           = addr/Ssfdc.MaxSectors;
	Media.Zone     = (BYTE) (temp/Ssfdc.MaxLogBlocks);

	if (Log2Phy[Media.Zone]==NULL)
	{
		if (Make_D_LogTable(us))
		{
			ErrCode = ERR_IllegalFmt;
			return(ERROR);
		}
	}

	Media.Sector   = (BYTE) (addr%Ssfdc.MaxSectors);
	Media.LogBlock = (WORD) (temp%Ssfdc.MaxLogBlocks);

	if (Media.Zone<Ssfdc.MaxZones)
	{
		Clr_D_RedundantData(Redundant);
		Set_D_LogBlockAddr(Redundant);
		Media.PhyBlock = Log2Phy[Media.Zone][Media.LogBlock];
		return(SMSUCCESS);
	}

	ErrCode = ERR_OutOfLBA;
	return(ERROR);
}

int Inc_D_MediaAddr(struct us_data *us)
{
	WORD        LogBlock = Media.LogBlock;

	if (++Media.Sector<Ssfdc.MaxSectors)
		return(SMSUCCESS);

	if (Log2Phy[Media.Zone]==NULL)
	{
		if (Make_D_LogTable(us))
		{
			ErrCode = ERR_IllegalFmt;
			return(ERROR);
		}
	}

	Media.Sector=0;
	Media.LogBlock = LogBlock;

	if (++Media.LogBlock<Ssfdc.MaxLogBlocks)
	{
		Clr_D_RedundantData(Redundant);
		Set_D_LogBlockAddr(Redundant);
		Media.PhyBlock=Log2Phy[Media.Zone][Media.LogBlock];
		return(SMSUCCESS);
	}

	Media.LogBlock=0;

	if (++Media.Zone<Ssfdc.MaxZones)
	{
		if (Log2Phy[Media.Zone]==NULL)
		{
			if (Make_D_LogTable(us))
			{
				ErrCode = ERR_IllegalFmt;
				return(ERROR);
			}
		}

		Media.LogBlock = 0;

		Clr_D_RedundantData(Redundant);
		Set_D_LogBlockAddr(Redundant);
		Media.PhyBlock=Log2Phy[Media.Zone][Media.LogBlock];
		return(SMSUCCESS);
	}

	Media.Zone=0;
	ErrCode = ERR_OutOfLBA;

	return(ERROR);
}
/*
int Check_D_FirstSect(void)
{
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    if (!Media.Sector)
        return(SMSUCCESS);

    return(ERROR);
}

int Check_D_LastSect(void)
{
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    if (Media.Sector<(Ssfdc.MaxSectors-1))
        return(ERROR);

    return(SMSUCCESS);
}
*/
int Media_D_ReadOneSect(struct us_data *us, WORD count, BYTE *buf)
{
	DWORD err, retry;

	if (!Read_D_PhyOneSect(us, count, buf))
		return(SMSUCCESS);
	if (ErrCode==ERR_HwError)
		return(ERROR);
	if (ErrCode==ERR_DataStatus)
		return(ERROR);

#ifdef RDERR_REASSIGN
	if (Ssfdc.Attribute &MWP)
	{
		if (ErrCode==ERR_CorReadErr)
			return(SMSUCCESS);
		return(ERROR);
	}

	err=ErrCode;
	for(retry=0; retry<2; retry++)
	{
		if (Copy_D_BlockAll(us, (err==ERR_EccReadErr)?REQ_FAIL:REQ_ERASE))
		{
			if (ErrCode==ERR_HwError)
				return(ERROR);
			continue;
		}

		ErrCode = err;
		if (ErrCode==ERR_CorReadErr)
			return(SMSUCCESS);
		return(ERROR);
	}

	MediaChange = ERROR;
#else
	if (ErrCode==ERR_CorReadErr) return(SMSUCCESS);
#endif

	return(ERROR);
}
/*
int Media_D_WriteOneSect(PFDO_DEVICE_EXTENSION fdoExt, WORD count, BYTE *buf)
{
    DWORD retry;
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    if (!Write_D_PhyOneSect(fdoExt, count, buf))
        return(SMSUCCESS);
    if (ErrCode==ERR_HwError)
        return(ERROR);

    for(retry=1; retry<2; retry++)
    {
        if (Reassign_D_BlockHead(fdoExt))
        {
            if (ErrCode==ERR_HwError)
                return(ERROR);
            continue;
        }

        if (!Write_D_PhyOneSect(fdoExt, count, buf))
            return(SMSUCCESS);
        if (ErrCode==ERR_HwError)
            return(ERROR);
    }

    if (Release_D_WriteBlock(fdoExt))
        return(ERROR);

    ErrCode        = ERR_WriteFault;
    MediaChange = ERROR;
    return(ERROR);
}

int Media_D_CopyBlockHead(PFDO_DEVICE_EXTENSION fdoExt)
{
    DWORD retry;

    for(retry=0; retry<2; retry++)
    {
        if (!Copy_D_BlockHead(fdoExt))
            return(SMSUCCESS);
        if (ErrCode==ERR_HwError)
            return(ERROR);
    }

    MediaChange = ERROR;
    return(ERROR);
}

int Media_D_CopyBlockTail(PFDO_DEVICE_EXTENSION fdoExt)
{
    DWORD retry;

    if (!Copy_D_BlockTail(fdoExt))
        return(SMSUCCESS);
    if (ErrCode==ERR_HwError)
        return(ERROR);

    for(retry=1; retry<2; retry++)
    {
        if (Reassign_D_BlockHead(fdoExt))
        {
            if (ErrCode==ERR_HwError)
                return(ERROR);
            continue;
        }

        if (!Copy_D_BlockTail(fdoExt))
            return(SMSUCCESS);
        if (ErrCode==ERR_HwError)
            return(ERROR);
    }

    if (Release_D_WriteBlock(fdoExt))
        return(ERROR);

    ErrCode        = ERR_WriteFault;
    MediaChange = ERROR;
    return(ERROR);
}
*/
int Copy_D_BlockAll(struct us_data *us, DWORD mode)
{
	BYTE sect;

	sect=Media.Sector;

	if (Assign_D_WriteBlock())
		return(ERROR);
	if (mode==REQ_FAIL)
		SectCopyMode=REQ_FAIL;

	for(Media.Sector=0; Media.Sector<Ssfdc.MaxSectors; Media.Sector++)
	{
		if (Copy_D_PhyOneSect(us))
		{
			if (ErrCode==ERR_HwError)
				return(ERROR);
			if (Release_D_WriteBlock(us))
				return(ERROR);

			ErrCode = ERR_WriteFault;
			Media.PhyBlock=ReadBlock;
			Media.Sector=sect;

			return(ERROR);
		}
	}

	if (Release_D_ReadBlock(us))
		return(ERROR);

	Media.PhyBlock=WriteBlock;
	Media.Sector=sect;
	return(SMSUCCESS);
}
/*
int Copy_D_BlockHead(PFDO_DEVICE_EXTENSION fdoExt)
{
    BYTE sect;
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    sect=Media.Sector;
    if (Assign_D_WriteBlock())
        return(ERROR);

    for(Media.Sector=0; Media.Sector<sect; Media.Sector++)
    {
        if (Copy_D_PhyOneSect(fdoExt))
        {
            if (ErrCode==ERR_HwError)
                return(ERROR);
            if (Release_D_WriteBlock(fdoExt))
                return(ERROR);

            ErrCode = ERR_WriteFault;
            Media.PhyBlock=ReadBlock;
            Media.Sector=sect;

            return(ERROR);
        }
    }

    Media.PhyBlock=WriteBlock;
    Media.Sector=sect;
    return(SMSUCCESS);
}

int Copy_D_BlockTail(PFDO_DEVICE_EXTENSION fdoExt)
{
    BYTE sect;
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    for(sect=Media.Sector; Media.Sector<Ssfdc.MaxSectors; Media.Sector++)
    {
        if (Copy_D_PhyOneSect(fdoExt))
        {
            if (ErrCode==ERR_HwError)
                return(ERROR);

            Media.PhyBlock=WriteBlock;
            Media.Sector=sect;

            return(ERROR);
        }
    }

    if (Release_D_ReadBlock(fdoExt))
        return(ERROR);

    Media.PhyBlock=WriteBlock;
    Media.Sector=sect;
    return(SMSUCCESS);
}

int Reassign_D_BlockHead(PFDO_DEVICE_EXTENSION fdoExt)
{
    DWORD  mode;
    WORD   block;
    BYTE   sect;
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    mode=SectCopyMode;
    block=ReadBlock;
    sect=Media.Sector;

    if (Assign_D_WriteBlock())
        return(ERROR);

    SectCopyMode=REQ_FAIL;

    for(Media.Sector=0; Media.Sector<sect; Media.Sector++)
    {
        if (Copy_D_PhyOneSect(fdoExt))
        {
            if (ErrCode==ERR_HwError)
                return(ERROR);
            if (Release_D_WriteBlock(fdoExt))
                return(ERROR);

            ErrCode = ERR_WriteFault;
            SectCopyMode=mode;
            WriteBlock=ReadBlock;
            ReadBlock=block;
            Media.Sector=sect;
            Media.PhyBlock=WriteBlock;

            return(ERROR);
        }
    }

    if (Release_D_ReadBlock(fdoExt))
        return(ERROR);

    SectCopyMode=mode;
    ReadBlock=block;
    Media.Sector=sect;
    Media.PhyBlock=WriteBlock;
    return(SMSUCCESS);
}
*/
int Assign_D_WriteBlock(void)
{
	ReadBlock=Media.PhyBlock;

	for(WriteBlock=AssignStart[Media.Zone]; WriteBlock<Ssfdc.MaxBlocks; WriteBlock++)
	{
		if (!Chk_D_Bit(Assign[Media.Zone],WriteBlock))
		{
			Set_D_Bit(Assign[Media.Zone],WriteBlock);
			AssignStart[Media.Zone]=WriteBlock+1;
			Media.PhyBlock=WriteBlock;
			SectCopyMode=REQ_ERASE;
			return(SMSUCCESS);
		}
	}

	for(WriteBlock=0; WriteBlock<AssignStart[Media.Zone]; WriteBlock++)
	{
		if (!Chk_D_Bit(Assign[Media.Zone],WriteBlock))
		{
			Set_D_Bit(Assign[Media.Zone],WriteBlock);
			AssignStart[Media.Zone]=WriteBlock+1;
			Media.PhyBlock=WriteBlock;
			SectCopyMode=REQ_ERASE;
			return(SMSUCCESS);
		}
	}

	WriteBlock=NO_ASSIGN;
	ErrCode = ERR_WriteFault;
	return(ERROR);
}

int Release_D_ReadBlock(struct us_data *us)
{
	DWORD mode;

	mode=SectCopyMode;
	SectCopyMode=COMPLETED;

	if (mode==COMPLETED)
		return(SMSUCCESS);

	Log2Phy[Media.Zone][Media.LogBlock]=WriteBlock;
	Media.PhyBlock=ReadBlock;

	if (Media.PhyBlock==NO_ASSIGN)
	{
		Media.PhyBlock=WriteBlock;
		return(SMSUCCESS);
	}

	if (mode==REQ_ERASE)
	{
		if (Erase_D_PhyOneBlock(us))
		{
			if (ErrCode==ERR_HwError) return(ERROR);
			if (MarkFail_D_PhyOneBlock(us)) return(ERROR);
		}
		else
			Clr_D_Bit(Assign[Media.Zone],Media.PhyBlock);
	}
	else if (MarkFail_D_PhyOneBlock(us))
		return(ERROR);

	Media.PhyBlock=WriteBlock;
	return(SMSUCCESS);
}

int Release_D_WriteBlock(struct us_data *us)
{
	SectCopyMode=COMPLETED;
	Media.PhyBlock=WriteBlock;

	if (MarkFail_D_PhyOneBlock(us))
		return(ERROR);

	Media.PhyBlock=ReadBlock;
	return(SMSUCCESS);
}

int Copy_D_PhyOneSect(struct us_data *us)
{
	int           i;
	DWORD  err, retry;

	/* pr_info("Copy_D_PhyOneSect --- Secotr = %x\n", Media.Sector); */
	if (ReadBlock!=NO_ASSIGN)
	{
		Media.PhyBlock=ReadBlock;
		for(retry=0; retry<2; retry++)
		{
			if (retry!=0)
			{
				Ssfdc_D_Reset(us);
				if (Ssfdc_D_ReadCisSect(us,WorkBuf,WorkRedund))
				{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }

				if (Check_D_CISdata(WorkBuf,WorkRedund))
				{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
			}

			if (Ssfdc_D_ReadSect(us,WorkBuf,WorkRedund))
			{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
			if (Check_D_DataStatus(WorkRedund))
			{ err=ERROR; break; }
			if (!Check_D_ReadError(WorkRedund))
			{ err=SMSUCCESS; break; }
			if (!Check_D_Correct(WorkBuf,WorkRedund))
			{ err=SMSUCCESS; break; }

			err=ERROR;
			SectCopyMode=REQ_FAIL;
		}
	}
	else
	{
		err=SMSUCCESS;
		for(i=0; i<SECTSIZE; i++)
			WorkBuf[i]=DUMMY_DATA;
		Clr_D_RedundantData(WorkRedund);
	}

	Set_D_LogBlockAddr(WorkRedund);
	if (err==ERROR)
	{
		Set_D_RightECC(WorkRedund);
		Set_D_DataStaus(WorkRedund);
	}

	Media.PhyBlock=WriteBlock;

	if (Ssfdc_D_WriteSectForCopy(us, WorkBuf, WorkRedund))
	{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
	if (Ssfdc_D_CheckStatus())
	{ ErrCode = ERR_WriteFault; return(ERROR); }

	Media.PhyBlock=ReadBlock;
	return(SMSUCCESS);
}

int Read_D_PhyOneSect(struct us_data *us, WORD count, BYTE *buf)
{
	int           i;
	DWORD  retry;

	if (Media.PhyBlock==NO_ASSIGN)
	{
		for(i=0; i<SECTSIZE; i++)
			*buf++=DUMMY_DATA;
		return(SMSUCCESS);
	}

	for(retry=0; retry<2; retry++)
	{
		if (retry!=0)
		{
			Ssfdc_D_Reset(us);

			if (Ssfdc_D_ReadCisSect(us,WorkBuf,WorkRedund))
			{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
			if (Check_D_CISdata(WorkBuf,WorkRedund))
			{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
		}

		if (Ssfdc_D_ReadBlock(us,count,buf,Redundant))
		{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
		if (Check_D_DataStatus(Redundant))
		{ ErrCode = ERR_DataStatus; return(ERROR); }

		if (!Check_D_ReadError(Redundant))
			return(SMSUCCESS);

		if (!Check_D_Correct(buf,Redundant))
		{ ErrCode = ERR_CorReadErr; return(ERROR); }
	}

	ErrCode = ERR_EccReadErr;
	return(ERROR);
}
/*
int Write_D_PhyOneSect(PFDO_DEVICE_EXTENSION fdoExt, WORD count, BYTE *buf)
{
    SSFDCTYPE_T aa = (SSFDCTYPE_T ) &Ssfdc;
    ADDRESS_T   bb = (ADDRESS_T) &Media;

    if (Ssfdc_D_WriteBlock(fdoExt,count,buf,Redundant))
    { ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
    if (Ssfdc_D_CheckStatus())
    { ErrCode = ERR_WriteFault; return(ERROR); }

    return(SMSUCCESS);
}
*/
int Erase_D_PhyOneBlock(struct us_data *us)
{

	if (Ssfdc_D_EraseBlock(us))
	{ ErrCode = ERR_HwError; MediaChange=ERROR; return(ERROR); }
	if (Ssfdc_D_CheckStatus())
	{ ErrCode = ERR_WriteFault; return(ERROR); }

	return(SMSUCCESS);
}

int Set_D_PhyFmtValue(struct us_data *us)
{
    if (Set_D_SsfdcModel(us->SM_DeviceID))
        return(ERROR);


    return(SMSUCCESS);
}

int Search_D_CIS(struct us_data *us)
{

	Media.Zone=0; Media.Sector=0;

	for (Media.PhyBlock=0; Media.PhyBlock<(Ssfdc.MaxBlocks-Ssfdc.MaxLogBlocks-1); Media.PhyBlock++)
	{
		if (Ssfdc_D_ReadRedtData(us, Redundant))
		{
			Ssfdc_D_Reset(us);
			return(ERROR);
		}

		if (!Check_D_FailBlock(Redundant))
			break;
	}

	if (Media.PhyBlock==(Ssfdc.MaxBlocks-Ssfdc.MaxLogBlocks-1))
	{
		Ssfdc_D_Reset(us);
		return(ERROR);
	}

	while (Media.Sector<CIS_SEARCH_SECT)
	{
		if (Media.Sector)
		{
			if (Ssfdc_D_ReadRedtData(us, Redundant))
			{
				Ssfdc_D_Reset(us);
				return(ERROR);
			}
		}
		if (!Check_D_DataStatus(Redundant))
		{
			if (Ssfdc_D_ReadSect(us,WorkBuf,Redundant))
			{
				Ssfdc_D_Reset(us);
				return(ERROR);
			}

			if (Check_D_CISdata(WorkBuf,Redundant))
			{
				Ssfdc_D_Reset(us);
				return(ERROR);
			}

			CisArea.PhyBlock=Media.PhyBlock;
			CisArea.Sector=Media.Sector;
			Ssfdc_D_Reset(us);
			return(SMSUCCESS);
		}

		Media.Sector++;
	}

	Ssfdc_D_Reset(us);
	return(ERROR);
}

int Make_D_LogTable(struct us_data *us)
{
	WORD  phyblock,logblock;

	if (Log2Phy[Media.Zone]==NULL)
	{
		Log2Phy[Media.Zone] = kmalloc(MAX_LOGBLOCK*sizeof(WORD), GFP_KERNEL);
		/* pr_info("ExAllocatePool Zone = %x, Addr = %x\n",
				Media.Zone, Log2Phy[Media.Zone]); */
		if (Log2Phy[Media.Zone]==NULL)
			return(ERROR);
	}

	Media.Sector=0;

	{
		/* pr_info("Make_D_LogTable --- MediaZone = 0x%x\n",
							Media.Zone); */
		for(Media.LogBlock=0; Media.LogBlock<Ssfdc.MaxLogBlocks; Media.LogBlock++)
			Log2Phy[Media.Zone][Media.LogBlock]=NO_ASSIGN;

		for(Media.PhyBlock=0; Media.PhyBlock<(MAX_BLOCKNUM/8); Media.PhyBlock++)
			Assign[Media.Zone][Media.PhyBlock]=0x00;

		for(Media.PhyBlock=0; Media.PhyBlock<Ssfdc.MaxBlocks; Media.PhyBlock++)
		{
			if ((!Media.Zone) && (Media.PhyBlock<=CisArea.PhyBlock))
			{
				Set_D_Bit(Assign[Media.Zone],Media.PhyBlock);
				continue;
			}

			if (Ssfdc_D_ReadRedtData(us, Redundant))
			{ Ssfdc_D_Reset(us); return(ERROR); }

			if (!Check_D_DataBlank(Redundant))
				continue;

			Set_D_Bit(Assign[Media.Zone],Media.PhyBlock);

			if (Check_D_FailBlock(Redundant))
				continue;


			if (Load_D_LogBlockAddr(Redundant))
				continue;

			if (Media.LogBlock>=Ssfdc.MaxLogBlocks)
				continue;

			if (Log2Phy[Media.Zone][Media.LogBlock]==NO_ASSIGN)
			{
				Log2Phy[Media.Zone][Media.LogBlock]=Media.PhyBlock;
				continue;
			}

			phyblock     = Media.PhyBlock;
			logblock     = Media.LogBlock;
			Media.Sector = (BYTE)(Ssfdc.MaxSectors-1);

			if (Ssfdc_D_ReadRedtData(us, Redundant))
			{ Ssfdc_D_Reset(us); return(ERROR); }

			if (!Load_D_LogBlockAddr(Redundant))
			{
				if (Media.LogBlock==logblock)
				{
					Media.PhyBlock=Log2Phy[Media.Zone][logblock];

					if (Ssfdc_D_ReadRedtData(us, Redundant))
					{ Ssfdc_D_Reset(us); return(ERROR); }

					Media.PhyBlock=phyblock;

					if (!Load_D_LogBlockAddr(Redundant))
					{
						if (Media.LogBlock!=logblock)
						{
							Media.PhyBlock=Log2Phy[Media.Zone][logblock];
							Log2Phy[Media.Zone][logblock]=phyblock;
						}
					}
					else
					{
						Media.PhyBlock=Log2Phy[Media.Zone][logblock];
						Log2Phy[Media.Zone][logblock]=phyblock;
					}
				}
			}

			Media.Sector=0;

			Media.PhyBlock=phyblock;

		}

		AssignStart[Media.Zone]=0;

	}

	Ssfdc_D_Reset(us);
	return(SMSUCCESS);
}

int MarkFail_D_PhyOneBlock(struct us_data *us)
{
	BYTE sect;

	sect=Media.Sector;
	Set_D_FailBlock(WorkRedund);

	for(Media.Sector=0; Media.Sector<Ssfdc.MaxSectors; Media.Sector++)
	{
		if (Ssfdc_D_WriteRedtData(us, WorkRedund))
		{
			Ssfdc_D_Reset(us);
			Media.Sector   = sect;
			ErrCode        = ERR_HwError;
			MediaChange = ERROR;
			return(ERROR);
		}
	}

	Ssfdc_D_Reset(us);
	Media.Sector=sect;
	return(SMSUCCESS);
}
/*
