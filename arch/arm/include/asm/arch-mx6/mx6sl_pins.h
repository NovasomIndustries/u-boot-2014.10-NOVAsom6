/*
 * Copyright (C) 2013 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASM_ARCH_MX6_MX6SL_PINS_H__
#define __ASM_ARCH_MX6_MX6SL_PINS_H__

#include <asm/imx-common/iomux-v3.h>

enum {
	MX6_PAD_ECSPI1_MISO__ECSPI_MISO				= IOMUX_PAD(0x0358, 0x0068, 0, 0x0684, 0, 0),
	MX6_PAD_ECSPI1_MOSI__ECSPI_MOSI				= IOMUX_PAD(0x035C, 0x006C, 0, 0x0688, 0, 0),
	MX6_PAD_ECSPI1_SCLK__ECSPI_SCLK				= IOMUX_PAD(0x0360, 0x0070, 0, 0x067C, 0, 0),
	MX6_PAD_ECSPI1_SS0__GPIO4_IO11				= IOMUX_PAD(0x0364, 0x0074, 5, 0x0000, 0, 0),
	MX6_PAD_SD2_CLK__USDHC2_CLK				= IOMUX_PAD(0x055C, 0x0254, 0, 0x0000, 0, 0),
	MX6_PAD_SD2_CMD__USDHC2_CMD				= IOMUX_PAD(0x0560, 0x0258, 0, 0x0000, 0, 0),
	MX6_PAD_SD2_DAT0__USDHC2_DAT0				= IOMUX_PAD(0x0564, 0x025C, 0, 0x0000, 0, 0),
	MX6_PAD_SD2_DAT1__USDHC2_DAT1				= IOMUX_PAD(0x0568, 0x0260, 0, 0x0000, 0, 0),
	MX6_PAD_SD2_DAT2__USDHC2_DAT2				= IOMUX_PAD(0x056C, 0x0264, 0, 0x0000, 0, 0),
	MX6_PAD_SD2_DAT3__USDHC2_DAT3				= IOMUX_PAD(0x0570, 0x0268, 0, 0x0000, 0, 0),
	MX6_PAD_UART1_RXD__UART1_RXD				= IOMUX_PAD(0x05A0, 0x0298, 0, 0x07FC, 0, 0),
	MX6_PAD_UART1_TXD__UART1_TXD				= IOMUX_PAD(0x05A4, 0x029C, 0, 0x0000, 0, 0),

	MX6_PAD_FEC_MDC__FEC_MDC				= IOMUX_PAD(0x41c, 0x12c, 0, 0x000, 0, 0),
	MX6_PAD_FEC_MDIO__FEC_MDIO				= IOMUX_PAD(0x420, 0x130, 0, 0x6f4, 1, 0),
	MX6_PAD_FEC_CRS_DV__FEC_RX_DV				= IOMUX_PAD(0x418, 0x128, 0, 0x704, 1, 0),
	MX6_PAD_FEC_RXD0__FEC_RX_DATA0				= IOMUX_PAD(0x42c, 0x13c, 0, 0x6f8, 0, 0),
	MX6_PAD_FEC_RXD1__FEC_RX_DATA1				= IOMUX_PAD(0x430, 0x140, 0, 0x6fc, 1, 0),
	MX6_PAD_FEC_TX_EN__FEC_TX_EN				= IOMUX_PAD(0x438, 0x148, 0, 0x000, 0, 0),
	MX6_PAD_FEC_TXD0__FEC_TX_DATA0				= IOMUX_PAD(0x43c, 0x14c, 0, 0x000, 0, 0),
	MX6_PAD_FEC_TXD1__FEC_TX_DATA1				= IOMUX_PAD(0x440, 0x150, 0, 0x000, 0, 0),
	MX6_PAD_FEC_REF_CLK__FEC_REF_OUT			= IOMUX_PAD(0x424, 0x134, 0x10, 0x000, 0, 0),
	MX6_PAD_FEC_RX_ER__GPIO_4_19				= IOMUX_PAD(0x0428, 0x0138, 5, 0x0000, 0, 0),
	MX6_PAD_FEC_TX_CLK__GPIO_4_21				= IOMUX_PAD(0x0434, 0x0144, 5, 0x0000, 0, 0),
};
#endif	/* __ASM_ARCH_MX6_MX6SL_PINS_H__ */
