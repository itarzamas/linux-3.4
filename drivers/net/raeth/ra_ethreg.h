#ifndef __RA_ETHREG_H__
#define __RA_ETHREG_H__

#include <asm/rt2880/rt_mmap.h>

#define u32				unsigned int
#define u16				unsigned short

#define ETHER_ADDR_LEN			6

#define PHYS_TO_K1(physaddr)		KSEG1ADDR(physaddr)
#define phys_to_bus(a)			(a & 0x1FFFFFFF)

#define sysRegRead(phys)		(*(volatile u32 *)PHYS_TO_K1(phys))
#define sysRegWrite(phys, val)		((*(volatile u32 *)PHYS_TO_K1(phys)) = (val))

/* Register Map Detail */
#define REG_SYSCFG1			(RALINK_SYSCTL_BASE + 0x14)

// Define Whole FE Reset Register
#define REG_RSTCTRL			(RALINK_SYSCTL_BASE + 0x34)

/* Register GPIO mode */
#define REG_GPIOMODE			(RALINK_SYSCTL_BASE + 0x60)

#if defined (CONFIG_RALINK_MT7621)
#define REG_CLK_CFG_0			(RALINK_SYSCTL_BASE + 0x2C)
#define REG_PAD_RGMII2_MDIO_CFG		(RALINK_SYSCTL_BASE + 0x58)
#define REG_ESW_PHY_POLLING		(RALINK_ETH_SW_BASE + 0x0000)
#elif defined (CONFIG_RALINK_MT7620)
#define REG_ESW_PHY_POLLING		(RALINK_ETH_SW_BASE + 0x7000)
#endif

/* FE_INT_STATUS */
#if defined (CONFIG_RALINK_RT5350) || defined (CONFIG_RALINK_MT7620) || \
    defined (CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)

#define RX_COHERENT			BIT(31)
#define RX_DLY_INT			BIT(30)
#define TX_COHERENT			BIT(29)
#define TX_DLY_INT			BIT(28)
#define RX_DONE_INT1			BIT(17)
#define RX_DONE_INT0			BIT(16)
#define TX_DONE_INT3			BIT(3)
#define TX_DONE_INT2			BIT(2)
#define TX_DONE_INT1			BIT(1)
#define TX_DONE_INT0			BIT(0)
#if defined (CONFIG_RALINK_MT7621)
#define RLS_COHERENT			BIT(29)
#define RLS_DLY_INT			BIT(28)
#define RLS_DONE_INT			BIT(0)
#endif

#else

#define CNT_PPE_AF			BIT(31)
#define CNT_GDM_AF			BIT(29)
#define PSE_P2_FC			BIT(26)
#define GDM_CRC_DROP			BIT(25)
#define PSE_BUF_DROP			BIT(24)
#define GDM_OTHER_DROP			BIT(23)
#define PSE_P1_FC			BIT(22)
#define PSE_P0_FC			BIT(21)
#define PSE_FQ_EMPTY			BIT(20)
#define GE1_STA_CHG			BIT(18)
#define TX_COHERENT			BIT(17)
#define RX_COHERENT			BIT(16)
#define TX_DONE_INT3			BIT(11)
#define TX_DONE_INT2			BIT(10)
#define TX_DONE_INT1			BIT(9)
#define TX_DONE_INT0			BIT(8)
#define RX_DONE_INT1			RX_DONE_INT0
#define RX_DONE_INT0			BIT(2)
#define TX_DLY_INT			BIT(1)
#define RX_DLY_INT			BIT(0)

#endif

#if defined (CONFIG_RAETH_NAPI)
#define FE_DLY_INIT_VALUE		0x00000000
#define FE_INT_INIT_VALUE		(TX_DONE_INT0 | RX_DONE_INT0 | RX_DONE_INT1)
#define FE_INT_MASK_TX_RX		(TX_DONE_INT0 | RX_DONE_INT0 | RX_DONE_INT1)
#define FE_INT_MASK_TX			(TX_DONE_INT0)
#else
#define FE_DLY_INIT_VALUE		0x84048404
#define FE_INT_INIT_VALUE		(TX_DLY_INT | RX_DLY_INT)
#endif

/* FE_INT_STATUS2 */
#if defined (CONFIG_RALINK_MT7621)
#define GE2_LINK_INT			BIT(25)
#endif

/* Register Categories Definition */
#define RAFRAMEENGINE_OFFSET		0x0000
#define RAGDMA_OFFSET			0x0020
#define RAPSE_OFFSET			0x0040
#define RAGDMA2_OFFSET			0x0060
#define RACDMA_OFFSET			0x0080
#if defined (CONFIG_RALINK_RT5350) || defined (CONFIG_RALINK_MT7620) || \
    defined (CONFIG_RALINK_MT7621) || defined (CONFIG_RALINK_MT7628)
#define RAPDMA_OFFSET			0x0800
#else
#define RAPDMA_OFFSET			0x0100
#endif
#if defined (CONFIG_RALINK_RT5350) || defined (CONFIG_RALINK_MT7628)
#define RASDM_OFFSET			0x0C00
#endif
#if defined (CONFIG_RALINK_MT7621)
#define RACMTABLE_OFFSET		0x2000
#elif defined (CONFIG_RALINK_MT7620)
#define RACMTABLE_OFFSET		0x1000
#else
#define RACMTABLE_OFFSET		0x0400
#endif

#if defined (CONFIG_RALINK_RT5350) || defined (CONFIG_RALINK_MT7628)

/* 1. PDMA */
#define TX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x000)
#define TX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x004)
#define TX_CTX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x008)
#define TX_DTX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x00C)

#define TX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x010)
#define TX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x014)
#define TX_CTX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x018)
#define TX_DTX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x01C)

#define TX_BASE_PTR2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x020)
#define TX_MAX_CNT2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x024)
#define TX_CTX_IDX2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x028)
#define TX_DTX_IDX2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x02C)

#define TX_BASE_PTR3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x030)
#define TX_MAX_CNT3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x034)
#define TX_CTX_IDX3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x038)
#define TX_DTX_IDX3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x03C)

#define RX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x100)
#define RX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x104)
#define RX_CALC_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x108)
#define RX_DRX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x10C)

#define RX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x110)
#define RX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x114)
#define RX_CALC_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x118)
#define RX_DRX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x11C)

#define PDMA_INFO			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x200)
#define PDMA_GLO_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x204)
#define PDMA_RST_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x208)
#define DLY_INT_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x20C)
#define FREEQ_THRES			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x210)
#define FE_INT_STATUS			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x220)
#define FE_INT_ENABLE			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x228)
#define PDMA_SCH_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x280)

#define SDM_CON				(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x00)  //Switch DMA configuration
#define SDM_RRING			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x04)  //Switch DMA Rx Ring
#define SDM_TRING			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x08)  //Switch DMA Tx Ring
#define SDM_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x0C)  //Switch MAC address LSB
#define SDM_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x10)  //Switch MAC Address MSB
#define SDM_TPCNT			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x100) //Switch DMA Tx packet count
#define SDM_TBCNT			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x104) //Switch DMA Tx byte count
#define SDM_RPCNT			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x108) //Switch DMA rx packet count
#define SDM_RBCNT			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x10C) //Switch DMA rx byte count
#define SDM_CS_ERR			(RALINK_FRAME_ENGINE_BASE+RASDM_OFFSET+0x110) //Switch DMA rx checksum error count

#elif defined (CONFIG_RALINK_MT7620) || defined (CONFIG_RALINK_MT7621)

/* 1. Frame Engine Global Registers */
#define FE_GLO_CFG			(RALINK_FRAME_ENGINE_BASE + 0x00)
#define FE_RST_GLO			(RALINK_FRAME_ENGINE_BASE + 0x04)
#define FE_INT_STATUS2			(RALINK_FRAME_ENGINE_BASE + 0x08)
#define FE_INT_ENABLE2			(RALINK_FRAME_ENGINE_BASE + 0x0C)
#define FOE_TS_T			(RALINK_FRAME_ENGINE_BASE + 0x10)

/* 2. PDMA */
#define TX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x000)
#define TX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x004)
#define TX_CTX_IDX0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x008)
#define TX_DTX_IDX0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x00C)

#define TX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x010)
#define TX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x014)
#define TX_CTX_IDX1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x018)
#define TX_DTX_IDX1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x01C)

#define TX_BASE_PTR2			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x020)
#define TX_MAX_CNT2			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x024)
#define TX_CTX_IDX2			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x028)
#define TX_DTX_IDX2			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x02C)

#define TX_BASE_PTR3			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x030)
#define TX_MAX_CNT3			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x034)
#define TX_CTX_IDX3			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x038)
#define TX_DTX_IDX3			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x03C)

#define RX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x100)
#define RX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x104)
#define RX_CALC_IDX0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x108)
#define RX_DRX_IDX0			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x10C)

#define RX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x110)
#define RX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x114)
#define RX_CALC_IDX1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x118)
#define RX_DRX_IDX1			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x11C)

#define PDMA_INFO			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x200)
#define PDMA_GLO_CFG			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x204)
#define PDMA_RST_CFG			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x208)
#define DLY_INT_CFG			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x20C)
#define FREEQ_THRES			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x210)
#define FE_INT_STATUS			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x220)
#define FE_INT_ENABLE			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x228)
#define SCH_Q01_CFG			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x280)
#define SCH_Q23_CFG			(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET+0x284)

#if defined (CONFIG_RALINK_MT7621)
#define PSE_RELATED			0x0040
#define PSE_FQ_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x00)
#define CDMA_FC_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x04)
#define GDMA1_FC_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x08)
#define GDMA2_FC_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x0C)
#define CDMA_OQ_STA			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x10)
#define GDMA1_OQ_STA			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x14)
#define GDMA2_OQ_STA			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x18)
#define PSE_IQ_STA			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x1C)

#define CDMA_RELATED			0x0400
#define CDMA_CSG_CFG			(RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00) //fake definition
#define CDMP_IG_CTRL			(RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00)
#define CDMP_EG_CTRL			(RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x04)

#define GDMA1_RELATED			0x0500
#define GDMA1_FWD_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x00)
#define GDMA1_SHPR_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x04)
#define GDMA1_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x08)
#define GDMA1_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x0C)

#define GDMA2_RELATED			0x1500
#define GDMA2_FWD_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x00)
#define GDMA2_SHPR_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x04)
#define GDMA2_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x08)
#define GDMA2_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x0C)

#else /* MT7620 */
#define PSE_RELATED			0x0500
#define PSE_FQFC_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x00)
#define PSE_IQ_CFG			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x04)
#define PSE_QUE_STA			(RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x08)

#define CDMA_RELATED			0x0400
#define CDMA_CSG_CFG			(RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00)
#define SMACCR0				(RALINK_ETH_SW_BASE + 0x3FE4)
#define SMACCR1				(RALINK_ETH_SW_BASE + 0x3FE8)
#define CKGCR				(RALINK_ETH_SW_BASE + 0x3FF0)

#define GDMA1_RELATED			0x0600
#define GDMA1_FWD_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x00)
#define GDMA1_SHPR_CFG			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x04)
#define GDMA1_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x08)
#define GDMA1_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x0C)
#endif

#define PDMA_FC_CFG			(RALINK_FRAME_ENGINE_BASE+0x100)

#else /* RT3052, RT3352, RT3883 */

/* 1. Frame Engine Global Registers */
#define MDIO_ACCESS			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x00)
#define MDIO_CFG			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x04)
#define FE_GLO_CFG			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x08)
#define FE_RST_GLO			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x0C)
#define FE_INT_STATUS			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x10)
#define FE_INT_ENABLE			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x14)
#define MDIO_CFG2			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x18) //Original:FC_DROP_STA
#define FOC_TS_T			(RALINK_FRAME_ENGINE_BASE+RAFRAMEENGINE_OFFSET+0x1C)

/* 2. GDMA Registers */
#define	GDMA1_FWD_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA_OFFSET+0x00)
#define GDMA1_SCH_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA_OFFSET+0x04)
#define GDMA1_SHPR_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA_OFFSET+0x08)
#define GDMA1_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE+RAGDMA_OFFSET+0x0C)
#define GDMA1_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE+RAGDMA_OFFSET+0x10)

#define	GDMA2_FWD_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA2_OFFSET+0x00)
#define GDMA2_SCH_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA2_OFFSET+0x04)
#define GDMA2_SHPR_CFG			(RALINK_FRAME_ENGINE_BASE+RAGDMA2_OFFSET+0x08)
#define GDMA2_MAC_ADRL			(RALINK_FRAME_ENGINE_BASE+RAGDMA2_OFFSET+0x0C)
#define GDMA2_MAC_ADRH			(RALINK_FRAME_ENGINE_BASE+RAGDMA2_OFFSET+0x10)

/* 3. PSE */
#define PSE_FQ_CFG			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x00)
#define CDMA_FC_CFG			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x04)
#define GDMA1_FC_CFG			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x08)
#define GDMA2_FC_CFG			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x0C)
#define PDMA_FC_CFG			(RALINK_FRAME_ENGINE_BASE+0x1f0)

/* 4. CDMA */
#define CDMA_CSG_CFG			(RALINK_FRAME_ENGINE_BASE+RACDMA_OFFSET+0x00)
#define CDMA_SCH_CFG			(RALINK_FRAME_ENGINE_BASE+RACDMA_OFFSET+0x04)
/* skip ppoe sid and vlan id definition */

/* 5. PDMA */
#define PDMA_GLO_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x00)
#define PDMA_RST_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x04)
#define PDMA_SCH_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x08)

#define DLY_INT_CFG			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x0C)

#define TX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x10)
#define TX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x14)
#define TX_CTX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x18)
#define TX_DTX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x1C)

#define TX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x20)
#define TX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x24)
#define TX_CTX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x28)
#define TX_DTX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x2C)

#define TX_BASE_PTR2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x40)
#define TX_MAX_CNT2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x44)
#define TX_CTX_IDX2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x48)
#define TX_DTX_IDX2			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x4C)

#define TX_BASE_PTR3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x50)
#define TX_MAX_CNT3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x54)
#define TX_CTX_IDX3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x58)
#define TX_DTX_IDX3			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x5C)

#define RX_BASE_PTR0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x30)
#define RX_MAX_CNT0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x34)
#define RX_CALC_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x38)
#define RX_DRX_IDX0			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x3C)

#define RX_BASE_PTR1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x40)
#define RX_MAX_CNT1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x44)
#define RX_CALC_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x48)
#define RX_DRX_IDX1			(RALINK_FRAME_ENGINE_BASE+RAPDMA_OFFSET+0x4C)

#define CDMA_OQ_STA			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x4c)
#define GDMA1_OQ_STA			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x50)
#define PPE_OQ_STA			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x54)
#define PSE_IQ_STA			(RALINK_FRAME_ENGINE_BASE+RAPSE_OFFSET+0x58)

#endif

#if defined (CONFIG_RALINK_MT7621)

#define GDMA1_RX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x400) // 64 bits
#define GDMA1_RX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x408)
#define GDMA1_RX_OERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x410)
#define GDMA1_RX_FERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x414)
#define GDMA1_RX_SERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x418)
#define GDMA1_RX_LERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x41C)
#define GDMA1_RX_CERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x420)
#define GDMA1_RX_FCCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x424)
#define GDMA1_TX_SKIPCNT		(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x428)
#define GDMA1_TX_COLCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x42C)
#define GDMA1_TX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x430) // 64 bits
#define GDMA1_TX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x438)

#define GDMA2_RX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x440) // 64 bits
#define GDMA2_RX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x448)
#define GDMA2_RX_OERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x450)
#define GDMA2_RX_FERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x454)
#define GDMA2_RX_SERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x458)
#define GDMA2_RX_LERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x45C)
#define GDMA2_RX_CERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x460)
#define GDMA2_RX_FCCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x464)
#define GDMA2_TX_SKIPCNT		(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x468)
#define GDMA2_TX_COLCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x46C)
#define GDMA2_TX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x470) // 64 bits
#define GDMA2_TX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x478)

#elif !defined (CONFIG_RALINK_RT5350) && !defined (CONFIG_RALINK_MT7628)

#define GDMA1_TX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x300)
#define GDMA1_TX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x304)
#define GDMA1_TX_SKIPCNT		(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x308)
#define GDMA1_TX_COLCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x30C)
#define GDMA1_RX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x320)
#define GDMA1_RX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x324)
#define GDMA1_RX_OERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x328)
#define GDMA1_RX_FERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x32C)
#define GDMA1_RX_SERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x330)
#define GDMA1_RX_LERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x334)
#define GDMA1_RX_CERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x338)
#define GDMA1_RX_FCCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x33C)

#define GDMA2_TX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x340)
#define GDMA2_TX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x344)
#define GDMA2_TX_SKIPCNT		(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x348)
#define GDMA2_TX_COLCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x34C)
#define GDMA2_RX_GBCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x360)
#define GDMA2_RX_GPCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x364)
#define GDMA2_RX_OERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x368)
#define GDMA2_RX_FERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x36C)
#define GDMA2_RX_SERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x370)
#define GDMA2_RX_LERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x374)
#define GDMA2_RX_CERCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x378)
#define GDMA2_RX_FCCNT			(RALINK_FRAME_ENGINE_BASE+RACMTABLE_OFFSET+0x37C)

#endif

/* ====================================== */

#define GDM1_TCI_81XX			BIT(24)
#define GDM1_ICS_EN			BIT(22)
#define GDM1_TCS_EN			BIT(21)
#define GDM1_UCS_EN			BIT(20)
#define GDM1_JMB_EN			BIT(19)
#define GDM1_DISPAD			BIT(18)
#define GDM1_DISCRC			BIT(17)
#define GDM1_STRPCRC			BIT(16)

//GDMA1 uni-cast frames destination port
#define GDM1_UFRC_P_CPU			(0 << 12)
#if defined (CONFIG_RALINK_MT7621)
#define GDM1_UFRC_P_PPE			(4 << 12)
#else
#define GDM1_UFRC_P_PPE			(6 << 12)
#endif

//GDMA1 broad-cast MAC address frames
#define GDM1_BFRC_P_CPU			(0 << 8)
#if defined (CONFIG_RALINK_MT7621)
#define GDM1_BFRC_P_PPE			(4 << 8)
#else
#define GDM1_BFRC_P_PPE			(6 << 8)
#endif

//GDMA1 multi-cast MAC address frames
#define GDM1_MFRC_P_CPU			(0 << 4)
#if defined (CONFIG_RALINK_MT7621)
#define GDM1_MFRC_P_PPE			(4 << 4)
#else
#define GDM1_MFRC_P_PPE			(6 << 4)
#endif

//GDMA1 other MAC address frames destination port
#define GDM1_OFRC_P_CPU			(0 << 0)
#if defined (CONFIG_RALINK_MT7621)
#define GDM1_OFRC_P_PPE			(4 << 0)
#else
#define GDM1_OFRC_P_PPE			(6 << 0)
#endif

#if defined (CONFIG_RALINK_MT7620) || defined (CONFIG_RALINK_MT7621)
/* checksum generator registers are removed */
#define ICS_GEN_EN			(0 << 2)
#define UCS_GEN_EN			(0 << 1)
#define TCS_GEN_EN			(0 << 0)
#else
#define ICS_GEN_EN			(1 << 2)
#define UCS_GEN_EN			(1 << 1)
#define TCS_GEN_EN			(1 << 0)
#endif

// MDIO_CFG	bit
#define MDIO_CFG_GP1_FC_TX		(1 << 11)
#define MDIO_CFG_GP1_FC_RX		(1 << 10)

/* ====================================== */
/* ====================================== */
#define GP1_LNK_DWN			BIT(9)
#define GP1_AN_FAIL			BIT(8)
/* ====================================== */
/* ====================================== */
#define PSE_RESET			BIT(0)
/* ====================================== */
#define PST_DRX_IDX1			BIT(17)
#define PST_DRX_IDX0			BIT(16)
#define PST_DTX_IDX3			BIT(3)
#define PST_DTX_IDX2			BIT(2)
#define PST_DTX_IDX1			BIT(1)
#define PST_DTX_IDX0			BIT(0)

#define RX_2B_OFFSET			BIT(31)
#define DESC_32B_EN			BIT(8)
#define TX_WB_DDONE			BIT(6)
#define RX_DMA_BUSY			BIT(3)
#define TX_DMA_BUSY			BIT(1)
#define RX_DMA_EN			BIT(2)
#define TX_DMA_EN			BIT(0)

#define PDMA_BT_SIZE_4DWORDS		(0<<4)
#define PDMA_BT_SIZE_8DWORDS		(1<<4)
#define PDMA_BT_SIZE_16DWORDS		(2<<4)
#define PDMA_BT_SIZE_32DWORDS		(3<<4)

/* Register bits.
 */

#define MACCFG_RXEN			(1<<2)
#define MACCFG_TXEN			(1<<3)
#define MACCFG_PROMISC			(1<<18)
#define MACCFG_RXMCAST			(1<<19)
#define MACCFG_FDUPLEX			(1<<20)
#define MACCFG_PORTSEL			(1<<27)
#define MACCFG_HBEATDIS			(1<<28)

#define DMACTL_SR			(1<<1)	/* Start/Stop Receive */
#define DMACTL_ST			(1<<13)	/* Start/Stop Transmission Command */

#define DMACFG_SWR			(1<<0)	/* Software Reset */
#define DMACFG_BURST32			(32<<8)

#define DMASTAT_TS			0x00700000	/* Transmit Process State */
#define DMASTAT_RS			0x000e0000	/* Receive Process State */

#define MACCFG_INIT			0 //(MACCFG_FDUPLEX) // | MACCFG_PORTSEL)


/* Descriptor bits.
 */
#define R_OWN				0x80000000	/* Own Bit */
#define RD_RER				0x02000000	/* Receive End Of Ring */
#define RD_LS				0x00000100	/* Last Descriptor */
#define RD_ES				0x00008000	/* Error Summary */
#define RD_CHAIN			0x01000000	/* Chained */

/* Word 0 */
#define T_OWN				0x80000000	/* Own Bit */
#define TD_ES				0x00008000	/* Error Summary */

/* Word 1 */
#define TD_LS				0x40000000	/* Last Segment */
#define TD_FS				0x20000000	/* First Segment */
#define TD_TER				0x08000000	/* Transmit End Of Ring */
#define TD_CHAIN			0x01000000	/* Chained */

#define TD_SET				0x08000000	/* Setup Packet */


#define RSTCTL				(0x34)
#define RSTCTL_RSTENET1			(1<<19)
#define RSTCTL_RSTENET2			(1<<20)

#define INIT_VALUE_OF_RT3883_PSE_FQ_CFG	0xff908000
#define INIT_VALUE_OF_PSE_FQFC_CFG	0x80504000
#define INIT_VALUE_OF_FORCE_100_FD	0x1001BC01
#if defined (CONFIG_RTL8367) || defined (CONFIG_RTL8367_MODULE)
#define INIT_VALUE_OF_FORCE_1000_FD	0x0001DC01
#else
#define INIT_VALUE_OF_FORCE_1000_FD	0x1F01DC01
#endif


/*=========================================
      PDMA RX Descriptor Format define
=========================================*/

struct PDMA_rxdesc {
	unsigned int rxd_info1_u32;
	unsigned int rxd_info2_u32;
	unsigned int rxd_info3_u32;
	unsigned int rxd_info4_u32;
#if defined (CONFIG_RAETH_32B_DESC)
	unsigned int rxd_info5_u32;
	unsigned int rxd_info6_u32;
	unsigned int rxd_info7_u32;
	unsigned int rxd_info8_u32;
#endif
};

#define RX2_DMA_SDL0_GET(_x)		(((_x) >> 16) & 0x3fff)
#define RX2_DMA_SDL0_SET(_x)		(((_x) & 0x3fff) << 16)
#define RX2_DMA_LS1			BIT(14)
#define RX2_DMA_TAG			BIT(15)
#define RX2_DMA_LS0			BIT(30)
#define RX2_DMA_DONE			BIT(31)

#define RX3_DMA_VID(_x)			((_x) & 0xffff)
#define RX3_DMA_TPID(_x)		(((_x) >> 16) & 0xffff)

#if defined (CONFIG_RALINK_MT7621)
#define RX4_DMA_SP(_x)			(((_x) >> 19) & 0xf)
#define RX4_DMA_ALG_SET			(0xFF800000)
#define RX4_DMA_L4F			BIT(23)
#define RX4_DMA_L4FVLD			BIT(24)
#define RX4_DMA_TACK			BIT(25)
#define RX4_DMA_IP4F			BIT(26)
#define RX4_DMA_IP4			BIT(27)
#define RX4_DMA_IP6			BIT(28)
#elif defined (CONFIG_RALINK_MT7620)
#define RX4_DMA_SP(_x)			(((_x) >> 19) & 0x7)
#define RX4_DMA_ALG_SET			(0xFFC00000)
#define RX4_DMA_L4F			BIT(22)
#define RX4_DMA_L4FVLD			BIT(23)
#define RX4_DMA_TACK			BIT(24)
#define RX4_DMA_IP4F			BIT(25)
#define RX4_DMA_IP4			BIT(26)
#define RX4_DMA_IP6			BIT(27)
#else
#define RX4_DMA_ALG_SET			BIT(15)
#define RX4_DMA_SP(_x)			(((_x) >> 24) & 0x7)
#define RX4_DMA_L4F			BIT(28)
#define RX4_DMA_IPF			BIT(29)
#define RX4_DMA_L4FVLD			BIT(30)
#define RX4_DMA_IPFVLD			BIT(31)
#endif

/*=========================================
      PDMA TX Descriptor Format define
=========================================*/

struct PDMA_txdesc {
	unsigned int txd_info1_u32;
	unsigned int txd_info2_u32;
	unsigned int txd_info3_u32;
	unsigned int txd_info4_u32;
#if defined (CONFIG_RAETH_32B_DESC)
	unsigned int txd_info5_u32;
	unsigned int txd_info6_u32;
	unsigned int txd_info7_u32;
	unsigned int txd_info8_u32;
#endif
};

#define TX2_DMA_SDL1(_x)		((_x) & 0x3fff)
#define TX2_DMA_LS1			BIT(14)
#define TX2_DMA_BURST			BIT(15)
#define TX2_DMA_SDL0(_x)		(((_x) & 0x3fff) << 16)
#define TX2_DMA_LS0			BIT(30)
#define TX2_DMA_DONE			BIT(31)

#if defined (CONFIG_RALINK_MT7621)
#define TX4_DMA_VLAN_TAG(_x)		((_x) & 0x1FFFF)
#define TX4_DMA_UDF(_x)			((_x) << 19)
#define TX4_DMA_FPORT(_x)		((_x) << 25)
#define TX4_DMA_TSO			BIT(28)
#define TX4_DMA_TUI_CO(_x)		((_x) << 29)
#elif defined (CONFIG_RALINK_MT7620)
#define TX4_DMA_VIDX(_x)		((_x) & 0xf)
#define TX4_DMA_VPRI(_x)		(((_x) & VLAN_PRIO_MASK) >> 9)
#define TX4_DMA_INSV			BIT(7)
#define TX4_DMA_UDF(_x)			((_x) << 15)
#define TX4_DMA_FP_BMAP(_x)		((_x) << 20)
#define TX4_DMA_TSO			BIT(28)
#define TX4_DMA_TUI_CO(_x)		((_x) << 29)
#else
#define TX4_DMA_VIDX(_x)		((_x) & 0xf)
#define TX4_DMA_VPRI(_x)		(((_x) & VLAN_PRIO_MASK) >> 9)
#define TX4_DMA_INSV			BIT(7)
#define TX4_DMA_QN(_x)			((_x) << 16)
#define TX4_DMA_PN(_x)			((_x) << 24)
#define TX4_DMA_TUI_CO(_x)		((_x) << 29)
#endif


#endif
