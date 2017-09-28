/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#ifndef __RTL8192D_HAL_H__
#define __RTL8192D_HAL_H__

#include "rtl8192d_spec.h"
#include "Hal8192DPhyReg.h"
#include "Hal8192DPhyCfg.h"
#include "rtl8192d_rf.h"
#include "rtl8192d_dm.h"
#include "rtl8192d_recv.h"
#include "rtl8192d_xmit.h"
#include "rtl8192d_cmd.h"
#include "rtw_efuse.h"

#include "../hal/OUTSRC/odm_precomp.h"

#ifdef CONFIG_PCI_HCI
	#include <pci_ops.h>	

	#define RTL819X_DEFAULT_RF_TYPE			RF_2T2R

	#define	RTL8192D_FW_IMG			  	"rtl8192DE\\rtl8192dfw.bin"

	#define RTL8192D_PHY_REG					"rtl8192DE\\PHY_REG.txt"
	#define RTL8192D_PHY_REG_PG				"rtl8192DE\\PHY_REG_PG.txt"
	#define RTL8192D_PHY_REG_MP				"rtl8192DE\\PHY_REG_MP.txt"

	#define RTL8192D_AGC_TAB					"rtl8192DE\\AGC_TAB.txt"
	#define RTL8192D_AGC_TAB_2G				"rtl8192DE\\AGC_TAB_2G.txt"
	#define RTL8192D_AGC_TAB_5G				"rtl8192DE\\AGC_TAB_5G.txt"
	#define RTL8192D_PHY_RADIO_A				"rtl8192DE\\radio_a.txt"
	#define RTL8192D_PHY_RADIO_B				"rtl8192DE\\radio_b.txt"	
	#define RTL8192D_PHY_RADIO_A_intPA			"rtl8192DE\\radio_a_intPA.txt"
	#define RTL8192D_PHY_RADIO_B_intPA			"rtl8192DE\\radio_b_intPA.txt"
	#define RTL8192D_PHY_MACREG				"rtl8192DE\\MAC_REG.txt"

	#define Rtl8192D_FwImageArray 				Rtl8192DEFwImgArray

	#define Rtl8192D_MAC_Array					Rtl8192DEMAC_2T_Array
	#define Rtl8192D_AGCTAB_Array				Rtl8192DEAGCTAB_Array
	#define Rtl8192D_AGCTAB_5GArray			Rtl8192DEAGCTAB_5GArray
	#define Rtl8192D_AGCTAB_2GArray			Rtl8192DEAGCTAB_2GArray
	#define Rtl8192D_AGCTAB_2TArray 			Rtl8192DEAGCTAB_2TArray
	#define Rtl8192D_AGCTAB_1TArray 			Rtl8192DEAGCTAB_1TArray
	#define Rtl8192D_PHY_REG_2TArray			Rtl8192DEPHY_REG_2TArray		
	#define Rtl8192D_PHY_REG_1TArray			Rtl8192DEPHY_REG_1TArray
	#define Rtl8192D_PHY_REG_Array_PG			Rtl8192DEPHY_REG_Array_PG
	#define Rtl8192D_PHY_REG_Array_MP			Rtl8192DEPHY_REG_Array_MP
	#define Rtl8192D_RadioA_2TArray				Rtl8192DERadioA_2TArray
	#define Rtl8192D_RadioA_1TArray				Rtl8192DERadioA_1TArray
	#define Rtl8192D_RadioB_2TArray				Rtl8192DERadioB_2TArray
	#define Rtl8192D_RadioB_1TArray				Rtl8192DERadioB_1TArray
	#define Rtl8192D_RadioA_2T_intPAArray		Rtl8192DERadioA_2T_intPAArray
	#define Rtl8192D_RadioB_2T_intPAArray 		Rtl8192DERadioB_2T_intPAArray

	#define Rtl8192D_FwImageArrayLength			Rtl8192DEImgArrayLength
	#define Rtl8192D_MAC_ArrayLength				Rtl8192DEMAC_2T_ArrayLength
	#define Rtl8192D_AGCTAB_5GArrayLength			Rtl8192DEAGCTAB_5GArrayLength
	#define Rtl8192D_AGCTAB_2GArrayLength			Rtl8192DEAGCTAB_2GArrayLength
	#define Rtl8192D_AGCTAB_2TArrayLength			Rtl8192DEAGCTAB_2TArrayLength
	#define Rtl8192D_AGCTAB_1TArrayLength			Rtl8192DEAGCTAB_1TArrayLength
	#define Rtl8192D_AGCTAB_ArrayLength 			Rtl8192DEAGCTAB_ArrayLength
	#define Rtl8192D_PHY_REG_2TArrayLength			Rtl8192DEPHY_REG_2TArrayLength
	#define Rtl8192D_PHY_REG_1TArrayLength			Rtl8192DEPHY_REG_1TArrayLength
	#define Rtl8192D_PHY_REG_Array_PGLength		Rtl8192DEPHY_REG_Array_PGLength
	#define Rtl8192D_PHY_REG_Array_MPLength		Rtl8192DEPHY_REG_Array_MPLength
	#define Rtl8192D_RadioA_2TArrayLength			Rtl8192DERadioA_2TArrayLength
	#define Rtl8192D_RadioB_2TArrayLength			Rtl8192DERadioB_2TArrayLength
	#define Rtl8192D_RadioA_2T_intPAArrayLength		Rtl8192DERadioA_2T_intPAArrayLength
	#define Rtl8192D_RadioB_2T_intPAArrayLength		Rtl8192DERadioB_2T_intPAArrayLength

#elif defined(CONFIG_USB_HCI)

	
	#define RTL819X_DEFAULT_RF_TYPE		RF_1T2R

	#define	RTL8192D_FW_IMG					"rtl8192DU\\rtl8192dfw.bin"

	#define RTL8192D_PHY_REG					"rtl8192DU\\PHY_REG.txt"
	#define RTL8192D_PHY_REG_PG				"rtl8192DU\\PHY_REG_PG.txt"
	#define RTL8192D_PHY_REG_MP				"rtl8192DU\\PHY_REG_MP.txt"			
	
	#define RTL8192D_AGC_TAB					"rtl8192DU\\AGC_TAB.txt"
	#define RTL8192D_AGC_TAB_2G				"rtl8192DU\\AGC_TAB_2G.txt"
	#define RTL8192D_AGC_TAB_5G				"rtl8192DU\\AGC_TAB_5G.txt"
	#define RTL8192D_PHY_RADIO_A				"rtl8192DU\\radio_a.txt"
	#define RTL8192D_PHY_RADIO_B				"rtl8192DU\\radio_b.txt"
	#define RTL8192D_PHY_RADIO_A_intPA		"rtl8192DU\\radio_a_intPA.txt"
	#define RTL8192D_PHY_RADIO_B_intPA		"rtl8192DU\\radio_b_intPA.txt"
	#define RTL8192D_PHY_MACREG				"rtl8192DU\\MAC_REG.txt"


	#define Rtl8192D_FwImageArray 					Rtl8192DUFwImgArray
	
	#define Rtl8192D_MAC_Array						Rtl8192DUMAC_2T_Array
	#define Rtl8192D_AGCTAB_Array					Rtl8192DUAGCTAB_Array
	#define Rtl8192D_AGCTAB_5GArray				Rtl8192DUAGCTAB_5GArray
	#define Rtl8192D_AGCTAB_2GArray				Rtl8192DUAGCTAB_2GArray
	#define Rtl8192D_AGCTAB_2TArray 				Rtl8192DUAGCTAB_2TArray
	#define Rtl8192D_AGCTAB_1TArray 				Rtl8192DUAGCTAB_1TArray
	#define Rtl8192D_PHY_REG_2TArray				Rtl8192DUPHY_REG_2TArray			
	#define Rtl8192D_PHY_REG_1TArray				Rtl8192DUPHY_REG_1TArray
	#define Rtl8192D_PHY_REG_Array_PG				Rtl8192DUPHY_REG_Array_PG
	#define Rtl8192D_PHY_REG_Array_MP				Rtl8192DUPHY_REG_Array_MP
	#define Rtl8192D_RadioA_2TArray					Rtl8192DURadioA_2TArray
	#define Rtl8192D_RadioA_1TArray					Rtl8192DURadioA_1TArray
	#define Rtl8192D_RadioB_2TArray					Rtl8192DURadioB_2TArray
	#define Rtl8192D_RadioB_1TArray					Rtl8192DURadioB_1TArray
	#define Rtl8192D_RadioA_2T_intPAArray			Rtl8192DURadioA_2T_intPAArray
	#define Rtl8192D_RadioB_2T_intPAArray 			Rtl8192DURadioB_2T_intPAArray
	
	#define Rtl8192D_FwImageArrayLength			Rtl8192DUImgArrayLength
	#define Rtl8192D_MAC_ArrayLength				Rtl8192DUMAC_2T_ArrayLength
	#define Rtl8192D_AGCTAB_5GArrayLength			Rtl8192DUAGCTAB_5GArrayLength
	#define Rtl8192D_AGCTAB_2GArrayLength			Rtl8192DUAGCTAB_2GArrayLength
	#define Rtl8192D_AGCTAB_2TArrayLength			Rtl8192DUAGCTAB_2TArrayLength
	#define Rtl8192D_AGCTAB_1TArrayLength			Rtl8192DUAGCTAB_1TArrayLength
	#define Rtl8192D_AGCTAB_ArrayLength 			Rtl8192DUAGCTAB_ArrayLength
	#define Rtl8192D_PHY_REG_2TArrayLength			Rtl8192DUPHY_REG_2TArrayLength
	#define Rtl8192D_PHY_REG_1TArrayLength			Rtl8192DUPHY_REG_1TArrayLength
	#define Rtl8192D_PHY_REG_Array_PGLength		Rtl8192DUPHY_REG_Array_PGLength
	#define Rtl8192D_PHY_REG_Array_MPLength		Rtl8192DUPHY_REG_Array_MPLength
	#define Rtl8192D_RadioA_2TArrayLength			Rtl8192DURadioA_2TArrayLength
	#define Rtl8192D_RadioB_2TArrayLength			Rtl8192DURadioB_2TArrayLength
	#define Rtl8192D_RadioA_2T_intPAArrayLength		Rtl8192DURadioA_2T_intPAArrayLength			
	#define Rtl8192D_RadioB_2T_intPAArrayLength		Rtl8192DURadioB_2T_intPAArrayLength

/*	#define Rtl819XFwImageArray					Rtl8192DUFwImgArray
	#define Rtl819XMAC_Array					Rtl8192DUMAC_2TArray
	#define Rtl819XAGCTAB_Array					Rtl8192DUAGCTAB_Array
	#define Rtl819XAGCTAB_5GArray				Rtl8192DUAGCTAB_5GArray
	#define Rtl819XAGCTAB_2GArray				Rtl8192DUAGCTAB_2GArray
	#define Rtl819XPHY_REG_2TArray				Rtl8192DUPHY_REG_2TArray
	#define Rtl819XPHY_REG_1TArray				Rtl8192DUPHY_REG_1TArray
	#define Rtl819XRadioA_2TArray				Rtl8192DURadioA_2TArray
	#define Rtl819XRadioA_1TArray				Rtl8192DURadioA_1TArray
	#define Rtl819XRadioA_2T_intPAArray 			Rtl8192DURadioA_2T_intPAArray
	#define Rtl819XRadioB_2TArray				Rtl8192DURadioB_2TArray
	#define Rtl819XRadioB_1TArray				Rtl8192DURadioB_1TArray
	#define Rtl819XRadioB_2T_intPAArray 			Rtl8192DURadioB_2T_intPAArray
	#define Rtl819XPHY_REG_Array_PG 			Rtl8192DUPHY_REG_Array_PG
	#define Rtl819XPHY_REG_Array_MP 			Rtl8192DUPHY_REG_Array_MP

	#define Rtl819XAGCTAB_2TArray				Rtl8192DUAGCTAB_2TArray
	#define Rtl819XAGCTAB_1TArray				Rtl8192DUAGCTAB_1TArray*/

#endif

#define DRVINFO_SZ	4
#define PageNum_128(_Len)		(u32)(((_Len)>>7) + ((_Len)&0x7F ? 1:0))

#define IS_FW_HEADER_EXIST(_pFwHdr)	((le16_to_cpu(_pFwHdr->Signature)&0xFFF0) == 0x92C0 ||\
									(le16_to_cpu(_pFwHdr->Signature)&0xFFF0) == 0x88C0 ||\
									(le16_to_cpu(_pFwHdr->Signature)&0xFFFF) == 0x92D0 ||\
									(le16_to_cpu(_pFwHdr->Signature)&0xFFFF) == 0x92D1 ||\
									(le16_to_cpu(_pFwHdr->Signature)&0xFFFF) == 0x92D2 ||\
									(le16_to_cpu(_pFwHdr->Signature)&0xFFFF) == 0x92D3 )

#define FW_8192D_SIZE				0x8020
#define FW_8192D_START_ADDRESS	0x1000
#define FW_8192D_END_ADDRESS		0x1FFF

#define MAX_PAGE_SIZE				4096

typedef enum _FIRMWARE_SOURCE{
	FW_SOURCE_IMG_FILE = 0,
	FW_SOURCE_HEADER_FILE = 1,	
}FIRMWARE_SOURCE, *PFIRMWARE_SOURCE;

typedef struct _RT_FIRMWARE{
	FIRMWARE_SOURCE	eFWSource;
	u8*			szFwBuffer;
	u32			ulFwLength;
}RT_FIRMWARE, *PRT_FIRMWARE, RT_FIRMWARE_92D, *PRT_FIRMWARE_92D;

typedef struct _RT_8192D_FIRMWARE_HDR {//8-byte alinment required

	u16		Signature;
	u8		Category;
	u8		Function;
	u16		Version;	
	u8		Subversion;
	u8		Rsvd1;


	u8		Month;
	u8		Date;
	u8		Hour;
	u8		Minute;
	u16		RamCodeSize;
	u16		Rsvd2;

	u32		SvnIdx;
	u32		Rsvd3;

	u32		Rsvd4;
	u32		Rsvd5;

}RT_8192D_FIRMWARE_HDR, *PRT_8192D_FIRMWARE_HDR;

#define DRIVER_EARLY_INT_TIME		0x05
#define BCN_DMA_ATIME_INT_TIME		0x02

typedef	enum _BT_CoType{
	BT_2Wire			= 0,		
	BT_ISSC_3Wire	= 1,
	BT_Accel			= 2,
	BT_CSR			= 3,
	BT_CSR_ENHAN	= 4,
	BT_RTL8756		= 5,
} BT_CoType, *PBT_CoType;

typedef	enum _BT_CurState{
	BT_OFF		= 0,	
	BT_ON		= 1,
} BT_CurState, *PBT_CurState;

typedef	enum _BT_ServiceType{
	BT_SCO			= 0,	
	BT_A2DP			= 1,
	BT_HID			= 2,
	BT_HID_Idle		= 3,
	BT_Scan			= 4,
	BT_Idle			= 5,
	BT_OtherAction	= 6,
	BT_Busy			= 7,
	BT_OtherBusy		= 8,
} BT_ServiceType, *PBT_ServiceType;

typedef	enum _BT_RadioShared{
	BT_Radio_Shared 	= 0,	
	BT_Radio_Individual	= 1,
} BT_RadioShared, *PBT_RadioShared;

typedef struct _BT_COEXIST_STR{
	u8					BluetoothCoexist;
	u8					BT_Ant_Num;
	u8					BT_CoexistType;
	u8					BT_State;
	u8					BT_CUR_State;	
	u8					BT_Ant_isolation;
	u8					BT_PapeCtrl;	
	u8					BT_Service;			
	u8					BT_RadioSharedType;
	u8					Ratio_Tx;
	u8					Ratio_PRI;
}BT_COEXIST_STR, *PBT_COEXIST_STR;


#ifdef CONFIG_USB_RX_AGGREGATION

typedef enum _USB_RX_AGG_MODE{
	USB_RX_AGG_DISABLE,
	USB_RX_AGG_DMA,
	USB_RX_AGG_USB,
	USB_RX_AGG_DMA_USB
}USB_RX_AGG_MODE;

#define MAX_RX_DMA_BUFFER_SIZE	10240	

#endif


#define TX_SELE_HQ			BIT(0)	
#define TX_SELE_LQ			BIT(1)	
#define TX_SELE_NQ			BIT(2)	



#define TX_TOTAL_PAGE_NUMBER		0xF8
#define TX_PAGE_BOUNDARY			(TX_TOTAL_PAGE_NUMBER + 1)

#define NORMAL_PAGE_NUM_PUBQ		0x56


#define TEST_PAGE_NUM_PUBQ			0x89
#define TX_TOTAL_PAGE_NUMBER_92D_DUAL_MAC		0x7A
#define NORMAL_PAGE_NUM_PUBQ_92D_DUAL_MAC			0x5A
#define NORMAL_PAGE_NUM_HPQ_92D_DUAL_MAC			0x10
#define NORMAL_PAGE_NUM_LPQ_92D_DUAL_MAC			0x10
#define NORMAL_PAGE_NUM_NORMALQ_92D_DUAL_MAC		0

#define TX_PAGE_BOUNDARY_DUAL_MAC			(TX_TOTAL_PAGE_NUMBER_92D_DUAL_MAC + 1)

#define WMM_TEST_TX_TOTAL_PAGE_NUMBER	0xF5
#define WMM_TEST_TX_PAGE_BOUNDARY	(WMM_TEST_TX_TOTAL_PAGE_NUMBER + 1)

#define WMM_TEST_PAGE_NUM_PUBQ		0xA3
#define WMM_TEST_PAGE_NUM_HPQ		0x29
#define WMM_TEST_PAGE_NUM_LPQ		0x29


#define WMM_NORMAL_TX_TOTAL_PAGE_NUMBER	0xF5
#define WMM_NORMAL_TX_PAGE_BOUNDARY	(WMM_TEST_TX_TOTAL_PAGE_NUMBER + 1)

#define WMM_NORMAL_PAGE_NUM_PUBQ		0xB0
#define WMM_NORMAL_PAGE_NUM_HPQ		0x29
#define WMM_NORMAL_PAGE_NUM_LPQ			0x1C
#define WMM_NORMAL_PAGE_NUM_NPQ		0x1C

#define WMM_NORMAL_PAGE_NUM_PUBQ_92D		0X65//0x82
#define WMM_NORMAL_PAGE_NUM_HPQ_92D		0X30//0x29
#define WMM_NORMAL_PAGE_NUM_LPQ_92D		0X30
#define WMM_NORMAL_PAGE_NUM_NPQ_92D		0X30


#define CHIP_BONDING_IDENTIFIER(_value)	(((_value)>>22)&0x3)
#define CHIP_BONDING_92C_1T2R	0x1
#define CHIP_BONDING_88C_USB_MCARD	0x2
#define CHIP_BONDING_88C_USB_HP	0x1

#include "HalVerDef.h"
#include "hal_com.h"

enum ChannelPlan{
	CHPL_FCC	= 0,
	CHPL_IC		= 1,
	CHPL_ETSI	= 2,
	CHPL_SPAIN	= 3,
	CHPL_FRANCE	= 4,
	CHPL_MKK	= 5,
	CHPL_MKK1	= 6,
	CHPL_ISRAEL	= 7,
	CHPL_TELEC	= 8,
	CHPL_GLOBAL	= 9,
	CHPL_WORLD	= 10,
};

typedef struct _TxPowerInfo{
	u8 CCKIndex[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 HT40_1SIndex[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 HT40_2SIndexDiff[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	s8 HT20IndexDiff[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 OFDMIndexDiff[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 HT40MaxOffset[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 HT20MaxOffset[RF_PATH_MAX][CHANNEL_GROUP_MAX];
	u8 TSSI_A[3];
	u8 TSSI_B[3];
	u8 TSSI_A_5G[3];	
	u8 TSSI_B_5G[3];
}TxPowerInfo, *PTxPowerInfo;

#define EFUSE_REAL_CONTENT_LEN	1024
#define EFUSE_MAP_LEN				256
#define EFUSE_MAX_SECTION			32
#define EFUSE_MAX_SECTION_BASE	16
#define EFUSE_OOB_PROTECT_BYTES 	18

typedef enum _PA_MODE {
	PA_MODE_EXTERNAL = 0x00,
	PA_MODE_INTERNAL_SP3T = 0x01,
	PA_MODE_INTERNAL_SPDT = 0x02	
} PA_MODE;

/* Copy from rtl8192c */
enum c2h_id_8192d {
	C2H_DBG = 0,
	C2H_TSF = 1,
	C2H_AP_RPT_RSP = 2,
	C2H_CCX_TX_RPT = 3,
	C2H_BT_RSSI = 4,
	C2H_BT_OP_MODE = 5,
	C2H_EXT_RA_RPT = 6,
	C2H_HW_INFO_EXCH = 10,
	C2H_C2H_H2C_TEST = 11,
	C2H_BT_INFO = 12,
	C2H_BT_MP_INFO = 15,
	MAX_C2HEVENT
};

#ifdef CONFIG_PCI_HCI
struct hal_data_8192de
{
	HAL_VERSION 	VersionID;
	MACPHY_MODE_8192D	MacPhyMode92D;
	BAND_TYPE	CurrentBandType92D;
	BAND_TYPE	BandSet92D;
	BOOLEAN		bIsVS;
	BOOLEAN		bSupportRemoteWakeUp;
	u8	AutoLoadStatusFor8192D;

	BOOLEAN		bNOPG;

	BOOLEAN       bMasterOfDMSP;
	BOOLEAN       bSlaveOfDMSP;

	u16	CustomerID;

	u16	FirmwareVersion;
	u16	FirmwareVersionRev;
	u16	FirmwareSubVersion;

	u32	IntrMask[2];
	u32	IntrMaskToSet[2];

	u32	DisabledFunctions;

	u32	ReceiveConfig;
	u32	TransmitConfig;
	WIRELESS_MODE	CurrentWirelessMode;
	HT_CHANNEL_WIDTH	CurrentChannelBW;
	u8	CurrentChannel;
	u8	nCur40MhzPrimeSC;// Control channel sub-carrier
	u16	BasicRateSet;

	u8	rf_chip;
	u8	rf_type;
	u8	NumTotalRFPath;

	u16	EEPROMVID;
	u16	EEPROMDID;
	u16	EEPROMSVID;
	u16	EEPROMSMID;
	u16	EEPROMChannelPlan;
	u16	EEPROMVersion;

	u8	EEPROMCustomerID;
	u8	EEPROMBoardType;
	u8	EEPROMRegulatory;

	u8	EEPROMThermalMeter;

	u8	EEPROMC9;
	u8	EEPROMCC;
	u8	PAMode;

	u8	TxPwrLevelCck[RF_PATH_MAX][CHANNEL_MAX_NUMBER_2G];
	u8	TxPwrLevelHT40_1S[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	u8	TxPwrLevelHT40_2S[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	s8	TxPwrHt20Diff[RF_PATH_MAX][CHANNEL_MAX_NUMBER];// HT 20<->40 Pwr diff
	u8	TxPwrLegacyHtDiff[RF_PATH_MAX][CHANNEL_MAX_NUMBER];// For HT<->legacy pwr diff
	u8	PwrGroupHT20[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	u8	PwrGroupHT40[RF_PATH_MAX][CHANNEL_MAX_NUMBER];

	u8	LegacyHTTxPowerDiff;// Legacy to HT rate power diff

	u8	CrystalCap;

#ifdef CONFIG_BT_COEXIST
	struct btcoexist_priv	bt_coexist;
#endif

	u8	framesync;
	u32	framesyncC34;
	u8	framesyncMonitor;
	u8	DefaultInitialGain[4];
	u8	pwrGroupCnt;
	u32	MCSTxPowerLevelOriginalOffset[MAX_PG_GROUP][16];
	u32	CCKTxPowerLevelOriginalOffset;

	u32	AntennaTxPath;				
	u32	AntennaRxPath;				
	u8	BluetoothCoexist;
	u8	ExternalPA;
	u8	InternalPA5G[2];

	u8	b1x1RecvCombine;

	u32	AcParam_BE;

	u8	CurrentCckTxPwrIdx;
	u8	CurrentOfdm24GTxPwrIdx;

	BB_REGISTER_DEFINITION_T	PHYRegDef[4];
	
	u32	RfRegChnlVal[2];


	BOOLEAN		bPhyValueInitReady;

	BOOLEAN		bTXPowerDataReadFromEEPORM;

	BOOLEAN		bInSetPower;

	BOOLEAN		bRDGEnable;

	BOOLEAN		bLoadIMRandIQKSettingFor2G;// True if IMR or IQK  have done  for 2.4G in scan progress
	BOOLEAN		bNeedIQK;

	BOOLEAN		bLCKInProgress;

	BOOLEAN		bEarlyModeEnable;

#if 1
	IQK_MATRIX_REGS_SETTING IQKMatrixRegSetting[IQK_Matrix_Settings_NUM];
#else
	u4Byte				IQKMatrixReg[IQK_Matrix_REG_NUM];
	IQK_MATRIX_REGS_SETTING			   IQKMatrixRegSetting[IQK_Matrix_Settings_NUM];
#endif

	u8	LastHMEBoxNum;

	u8	fw_ractrl;
	u32	RegBcnCtrlVal;
	u8	RegTxPause;
	u8	RegFwHwTxQCtrl;
	u8	RegReg542;
	u8	RegCR_1;
	
	struct dm_priv	dmpriv;
	DM_ODM_T 		odmpriv;
	u8	bInterruptMigration;

	u8	FwRsvdPageStartOffset;

	u32	interfaceIndex;

	u16	RegRRSR;

	u16	EfuseUsedBytes;
	
	BOOLEAN 			EepromOrEfuse;
	u8				EfuseMap[2][HWSET_MAX_SIZE_512];
	u8				EfuseUsedPercentage;
	EFUSE_HAL			EfuseHal;

	u8	RTSInitRate;	
#ifdef CONFIG_P2P
	struct P2P_PS_Offload_t	p2p_ps_offload;
#endif
};

typedef struct hal_data_8192de HAL_DATA_TYPE, *PHAL_DATA_TYPE;

#define DF_TX_BIT		BIT0
#define DF_RX_BIT		BIT1
#define DF_IO_BIT		BIT2
#define DF_IO_D3_BIT			BIT3

#define RT_DF_TYPE		u32
#define RT_DISABLE_FUNC(__pAdapter, __FuncBits) ((__pAdapter)->DisabledFunctions |= ((RT_DF_TYPE)(__FuncBits)))
#define RT_ENABLE_FUNC(__pAdapter, __FuncBits) ((__pAdapter)->DisabledFunctions &= (~((RT_DF_TYPE)(__FuncBits))))
#define RT_IS_FUNC_DISABLED(__pAdapter, __FuncBits) ( (__pAdapter)->DisabledFunctions & (__FuncBits) )

void InterruptRecognized8192DE(PADAPTER Adapter, PRT_ISR_CONTENT pIsrContent);
VOID UpdateInterruptMask8192DE(PADAPTER Adapter, u32 AddMSR, u32 RemoveMSR);
#endif

#ifdef CONFIG_USB_HCI

typedef	enum _INTERFACE_SELECT_8192DUSB{
	INTF_SEL0_USB 			= 0,	
	INTF_SEL1_MINICARD	= 1,	
	INTF_SEL2_EKB_PRO		= 2,	
	INTF_SEL3_PRO			= 3,	
} INTERFACE_SELECT_8192DUSB, *PINTERFACE_SELECT_8192DUSB;

typedef INTERFACE_SELECT_8192DUSB INTERFACE_SELECT_USB;

struct hal_data_8192du
{
	HAL_VERSION 	VersionID;

	MACPHY_MODE_8192D	MacPhyMode92D;
	BAND_TYPE	CurrentBandType92D;
	BAND_TYPE	BandSet92D;
	BOOLEAN		bIsVS;

	BOOLEAN		bNOPG;

	BOOLEAN		bSupportRemoteWakeUp;
	BOOLEAN       bMasterOfDMSP;
	BOOLEAN       bSlaveOfDMSP;
#ifdef CONFIG_DUALMAC_CONCURRENT
	BOOLEAN		bInModeSwitchProcess;
#endif

	u16	CustomerID;

	u16	FirmwareVersion;
	u16	FirmwareVersionRev;
	u16	FirmwareSubVersion;

	u32	ReceiveConfig;
	WIRELESS_MODE	CurrentWirelessMode;
	HT_CHANNEL_WIDTH	CurrentChannelBW;
	u8	CurrentChannel;
	u8	nCur40MhzPrimeSC;// Control channel sub-carrier
	u16	BasicRateSet;

	INTERFACE_SELECT_8192DUSB	InterfaceSel;

	u8	rf_chip;
	u8	rf_type;
	u8	NumTotalRFPath;

	u8	EEPROMVersion;
	u16	EEPROMVID;
	u16	EEPROMPID;
	u16	EEPROMSVID;
	u16	EEPROMSDID;
	u8	EEPROMCustomerID;
	u8	EEPROMSubCustomerID;	
	u8	EEPROMRegulatory;

	u8	EEPROMThermalMeter;

	u8	EEPROMC9;
	u8	EEPROMCC;
	u8	PAMode;

	u8	TxPwrLevelCck[RF_PATH_MAX][CHANNEL_MAX_NUMBER_2G];
	u8	TxPwrLevelHT40_1S[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	u8	TxPwrLevelHT40_2S[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	s8	TxPwrHt20Diff[RF_PATH_MAX][CHANNEL_MAX_NUMBER];// HT 20<->40 Pwr diff
	u8	TxPwrLegacyHtDiff[RF_PATH_MAX][CHANNEL_MAX_NUMBER];// For HT<->legacy pwr diff
	u8	PwrGroupHT20[RF_PATH_MAX][CHANNEL_MAX_NUMBER];
	u8	PwrGroupHT40[RF_PATH_MAX][CHANNEL_MAX_NUMBER];

	u8	LegacyHTTxPowerDiff;// Legacy to HT rate power diff

	u8	CrystalCap;

#ifdef CONFIG_BT_COEXIST
	struct btcoexist_priv	bt_coexist;
#endif

	u8	framesync;
	u32	framesyncC34;
	u8	framesyncMonitor;
	u8	DefaultInitialGain[4];
	u8	pwrGroupCnt;
	u32	MCSTxPowerLevelOriginalOffset[MAX_PG_GROUP][16];
	u32	CCKTxPowerLevelOriginalOffset;

	u32	AntennaTxPath;				
	u32	AntennaRxPath;				
	u8	BluetoothCoexist;
	u8	ExternalPA;
	u8	InternalPA5G[2];

	u8	b1x1RecvCombine;

	u32	AcParam_BE;

	u8	CurrentCckTxPwrIdx;
	u8	CurrentOfdm24GTxPwrIdx;

	BB_REGISTER_DEFINITION_T	PHYRegDef[4];
	

	u32	RfRegChnlVal[2];


	BOOLEAN		bPhyValueInitReady;

	BOOLEAN		bTXPowerDataReadFromEEPORM;

	BOOLEAN		bInSetPower;

	BOOLEAN		bRDGEnable;

	BOOLEAN		bLoadIMRandIQKSettingFor2G;// True if IMR or IQK  have done  for 2.4G in scan progress
	BOOLEAN		bNeedIQK;

	BOOLEAN		bLCKInProgress;

	BOOLEAN		bEarlyModeEnable;

#if 1
	IQK_MATRIX_REGS_SETTING IQKMatrixRegSetting[IQK_Matrix_Settings_NUM];
#else
	u4Byte				IQKMatrixReg[IQK_Matrix_REG_NUM];
	IQK_MATRIX_REGS_SETTING			   IQKMatrixRegSetting[IQK_Matrix_Settings_NUM];
#endif

	u8	LastHMEBoxNum;

	u8	fw_ractrl;
	u32	RegBcnCtrlVal;
	u8	RegTxPause;
	u8	RegFwHwTxQCtrl;
	u8	RegReg542;	
	u8	RegCR_1;
	
	struct dm_priv	dmpriv;
	DM_ODM_T 		odmpriv;
	u8	FwRsvdPageStartOffset;

	BOOLEAN		bReadRFbyFW;


	u32	UsbBulkOutSize;

	u32	interfaceIndex;

	u8	OutEpQueueSel;
	u8	OutEpNumber;

#ifdef CONFIG_USB_TX_AGGREGATION
	u8	UsbTxAggMode;
	u8	UsbTxAggDescNum;
#endif
#ifdef CONFIG_USB_RX_AGGREGATION
	u16	HwRxPageSize;			
	u32	MaxUsbRxAggBlock;

	USB_RX_AGG_MODE	UsbRxAggMode;
	u8	UsbRxAggBlockCount;		
	u8	UsbRxAggBlockTimeout;
	u8	UsbRxAggPageCount;		
	u8	UsbRxAggPageTimeout;
#endif

	u16	RegRRSR;

	u16	EfuseUsedBytes;
	
	BOOLEAN 			EepromOrEfuse;
	u8				EfuseMap[2][HWSET_MAX_SIZE_512];
	u8				EfuseUsedPercentage;
	EFUSE_HAL			EfuseHal;

	u8	RTSInitRate;	
#ifdef CONFIG_P2P
	struct P2P_PS_Offload_t	p2p_ps_offload;
#endif
};

typedef struct hal_data_8192du HAL_DATA_TYPE, *PHAL_DATA_TYPE;
#endif

#define GET_HAL_DATA(__pAdapter)	((HAL_DATA_TYPE *)((__pAdapter)->HalData))
#define GET_RF_TYPE(priv)	(GET_HAL_DATA(priv)->rf_type)

int FirmwareDownload92D(IN PADAPTER Adapter);
VOID rtl8192d_FirmwareSelfReset(IN PADAPTER Adapter);
void rtl8192d_ReadChipVersion(IN PADAPTER Adapter);
VOID rtl8192d_EfuseParseChnlPlan(PADAPTER Adapter, u8 *hwinfo, BOOLEAN AutoLoadFail);
VOID rtl8192d_ReadTxPowerInfo(PADAPTER Adapter, u8* PROMContent, BOOLEAN AutoLoadFail);
VOID rtl8192d_ResetDualMacSwitchVariables(IN PADAPTER Adapter);
u8 GetEEPROMSize8192D(PADAPTER Adapter);
BOOLEAN PHY_CheckPowerOffFor8192D(PADAPTER Adapter);
VOID PHY_SetPowerOnFor8192D(PADAPTER Adapter);
void rtl8192d_free_hal_data(_adapter * padapter);
void rtl8192d_set_hal_ops(struct hal_ops *pHalFunc);
void	rtl8192d_clone_haldata(_adapter* dst_adapter, _adapter* src_adapter);
#endif

