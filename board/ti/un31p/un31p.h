/*
 * un31p.h - board header file for UN31 family of devices.
 *
 * Author: Giovanni P.
 *
 * Derived from evm code
 *
 * Copyright (C) 2009 Texas Instruments Incorporated
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifdef CONFIG_VIDEO_OMAP3
#include <asm/arch/dss.h>
#endif

#ifndef _AM3517EVM_H_
#define _AM3517EVM_H_

#ifndef GPMC_CONFIG_WIDTH
#define OFFS(x)				((x) >> 2)
#define GPMC_CONFIG_WIDTH	0x30
#define GPMC_CONFIG1		0x00
#define GPMC_CONFIG2		0x04
#define GPMC_CONFIG3		0x08
#define GPMC_CONFIG4		0x0C
#define GPMC_CONFIG5		0x10
#define GPMC_CONFIG6		0x14
#define GPMC_CONFIG7		0x18

#define GPMC_MASKADDRESS_SHIFT	(8)
#define GPMC_MASKADDRESS_MASK	(0xF << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_128MB	(0x8 << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_64MB	(0xC << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_32MB	(0xE << GPMC_MASKADDRESS_SHIFT)
#define GPMC_MASKADDRESS_16MB	(0xF << GPMC_MASKADDRESS_SHIFT)
#define GPMC_CSVALID            (1<<6)
#endif

//  NAND settings, not optimized
#define BSP_GPMC_NAND_CONFIG1       0x00001800      // 16 bit NAND interface
#define BSP_GPMC_NAND_CONFIG2       0x00060600      // 0x00141400
#define BSP_GPMC_NAND_CONFIG3       0x00060401      // 0x00141400
#define BSP_GPMC_NAND_CONFIG4       0x05010801      // 0x0F010F01
#define BSP_GPMC_NAND_CONFIG5       0x00080909      // 0x010C1414
#define BSP_GPMC_NAND_CONFIG6       0x050001C0      // 0x00000A80
#define BSP_GPMC_NAND_CONFIG7       ((CONFIG_SYS_NAND_BASE >> 24) | GPMC_MASKADDRESS_16MB | GPMC_CSVALID)

//  NOR settings
#define BSP_GPMC_NOR_CONFIG1        0x00011210      // 16 bit NOR interface (128MB size)
#define BSP_GPMC_NOR_CONFIG2        0x00101001
#define BSP_GPMC_NOR_CONFIG3        0x00020201
#define BSP_GPMC_NOR_CONFIG4        0x0F031003
#define BSP_GPMC_NOR_CONFIG5        0x000F1111
#define BSP_GPMC_NOR_CONFIG6        0x0F030080
#define BSP_GPMC_NOR_CONFIG7        ((CONFIG_SYS_FLASH_BASE >> 24) | GPMC_MASKADDRESS_128MB | GPMC_CSVALID)

#ifdef CONFIG_VIDEO_OMAP3
#define BACKGROUND_COLOR    0x00000000

struct panel_config display_cfg = {
       .xres		= 800,
       .yres		= 480,
       .bpp 		= 16,
       .hsy_fp		= 40,
       .hsy_bp		= 40,
       .hsy_width 	= 48,
       .hsy_inv		= 1,
       .vsy_fp		= 26,
       .vsy_bp		= 16,
       .vsy_width 	= 10,
       .vsy_inv		= 1,
	   .blank_inv	= 0,
	   .pclk_freq	= 24,
	   .pclk_inv	= 0
};

#endif


const omap3_sysinfo sysinfo = {
	DDR_DISCRETE,
	"am3517 un31p board",
	"NORflash",
};

void setup_ambabridge(void);
void Enable_RS232PHY(void);
unsigned char check_sicar(void);
void enable_voltagesun31();

#define CAREL_BOARD 1
#define SITEK_BOARD 0



/*
 * IEN  - Input Enable
 * IDIS - Input Disable
 * PTD  - Pull type Down
 * PTU  - Pull type Up
 * DIS  - Pull type selection is inactive
 * EN   - Pull type selection is active
 * M0   - Mode 0
 * The commented string gives the final mux configuration for that pin
 */
#define MUX_UN31P() \
 /*SDRC*/\
 MUX_VAL(CP(SDRC_D0),		(IEN  | PTD | DIS | M0)) /*SDRC_D0*/\
 MUX_VAL(CP(SDRC_D1),		(IEN  | PTD | DIS | M0)) /*SDRC_D1*/\
 MUX_VAL(CP(SDRC_D2),		(IEN  | PTD | DIS | M0)) /*SDRC_D2*/\
 MUX_VAL(CP(SDRC_D3),		(IEN  | PTD | DIS | M0)) /*SDRC_D3*/\
 MUX_VAL(CP(SDRC_D4),		(IEN  | PTD | DIS | M0)) /*SDRC_D4*/\
 MUX_VAL(CP(SDRC_D5),		(IEN  | PTD | DIS | M0)) /*SDRC_D5*/\
 MUX_VAL(CP(SDRC_D6),		(IEN  | PTD | DIS | M0)) /*SDRC_D6*/\
 MUX_VAL(CP(SDRC_D7),		(IEN  | PTD | DIS | M0)) /*SDRC_D7*/\
 MUX_VAL(CP(SDRC_D8),		(IEN  | PTD | DIS | M0)) /*SDRC_D8*/\
 MUX_VAL(CP(SDRC_D9),		(IEN  | PTD | DIS | M0)) /*SDRC_D9*/\
 MUX_VAL(CP(SDRC_D10),		(IEN  | PTD | DIS | M0)) /*SDRC_D10*/\
 MUX_VAL(CP(SDRC_D11),		(IEN  | PTD | DIS | M0)) /*SDRC_D11*/\
 MUX_VAL(CP(SDRC_D12),		(IEN  | PTD | DIS | M0)) /*SDRC_D12*/\
 MUX_VAL(CP(SDRC_D13),		(IEN  | PTD | DIS | M0)) /*SDRC_D13*/\
 MUX_VAL(CP(SDRC_D14),		(IEN  | PTD | DIS | M0)) /*SDRC_D14*/\
 MUX_VAL(CP(SDRC_D15),		(IEN  | PTD | DIS | M0)) /*SDRC_D15*/\
 MUX_VAL(CP(SDRC_D16),		(IEN  | PTD | DIS | M0)) /*SDRC_D16*/\
 MUX_VAL(CP(SDRC_D17),		(IEN  | PTD | DIS | M0)) /*SDRC_D17*/\
 MUX_VAL(CP(SDRC_D18),		(IEN  | PTD | DIS | M0)) /*SDRC_D18*/\
 MUX_VAL(CP(SDRC_D19),		(IEN  | PTD | DIS | M0)) /*SDRC_D19*/\
 MUX_VAL(CP(SDRC_D20),		(IEN  | PTD | DIS | M0)) /*SDRC_D20*/\
 MUX_VAL(CP(SDRC_D21),		(IEN  | PTD | DIS | M0)) /*SDRC_D21*/\
 MUX_VAL(CP(SDRC_D22),		(IEN  | PTD | DIS | M0)) /*SDRC_D22*/\
 MUX_VAL(CP(SDRC_D23),		(IEN  | PTD | DIS | M0)) /*SDRC_D23*/\
 MUX_VAL(CP(SDRC_D24),		(IEN  | PTD | DIS | M0)) /*SDRC_D24*/\
 MUX_VAL(CP(SDRC_D25),		(IEN  | PTD | DIS | M0)) /*SDRC_D25*/\
 MUX_VAL(CP(SDRC_D26),		(IEN  | PTD | DIS | M0)) /*SDRC_D26*/\
 MUX_VAL(CP(SDRC_D27),		(IEN  | PTD | DIS | M0)) /*SDRC_D27*/\
 MUX_VAL(CP(SDRC_D28),		(IEN  | PTD | DIS | M0)) /*SDRC_D28*/\
 MUX_VAL(CP(SDRC_D29),		(IEN  | PTD | DIS | M0)) /*SDRC_D29*/\
 MUX_VAL(CP(SDRC_D30),		(IEN  | PTD | DIS | M0)) /*SDRC_D30*/\
 MUX_VAL(CP(SDRC_D31),		(IEN  | PTD | DIS | M0)) /*SDRC_D31*/\
 MUX_VAL(CP(SDRC_CLK),		(IEN  | PTD | DIS | M0)) /*SDRC_CLK*/\
 MUX_VAL(CP(SDRC_DQS0),		(IEN  | PTD | DIS | M0)) /*SDRC_DQS0*/\
 MUX_VAL(CP(SDRC_DQS1),		(IEN  | PTD | DIS | M0)) /*SDRC_DQS1*/\
 MUX_VAL(CP(SDRC_DQS2),		(IEN  | PTD | DIS | M0)) /*SDRC_DQS2*/\
 MUX_VAL(CP(SDRC_DQS3),		(IEN  | PTD | DIS | M0)) /*SDRC_DQS3*/\
 /*GPMC*/\
 MUX_VAL(CP(GPMC_A1),		(IDIS | PTU | EN  | M0)) /*GPMC_A1*/\
 MUX_VAL(CP(GPMC_A2),		(IDIS | PTU | EN  | M0)) /*GPMC_A2*/\
 MUX_VAL(CP(GPMC_A3),		(IDIS | PTU | EN  | M0)) /*GPMC_A3*/\
 MUX_VAL(CP(GPMC_A4),		(IDIS | PTU | EN  | M0)) /*GPMC_A4*/\
 MUX_VAL(CP(GPMC_A5),		(IDIS | PTU | EN  | M0)) /*GPMC_A5*/\
 MUX_VAL(CP(GPMC_A6),		(IDIS | PTU | EN  | M0)) /*GPMC_A6*/\
 MUX_VAL(CP(GPMC_A7),		(IDIS | PTU | EN  | M0)) /*GPMC_A7*/\
 MUX_VAL(CP(GPMC_A8),		(IDIS | PTU | EN  | M0)) /*GPMC_A8*/\
 MUX_VAL(CP(GPMC_A9),		(IDIS | PTU | EN  | M0)) /*GPMC_A9*/\
 MUX_VAL(CP(GPMC_A10),		(IDIS | PTU | EN  | M0)) /*GPMC_A10*/\
 MUX_VAL(CP(GPMC_D0),		(IEN  | PTU | EN  | M0)) /*GPMC_D0*/\
 MUX_VAL(CP(GPMC_D1),		(IEN  | PTU | EN  | M0)) /*GPMC_D1*/\
 MUX_VAL(CP(GPMC_D2),		(IEN  | PTU | EN  | M0)) /*GPMC_D2*/\
 MUX_VAL(CP(GPMC_D3),		(IEN  | PTU | EN  | M0)) /*GPMC_D3*/\
 MUX_VAL(CP(GPMC_D4),		(IEN  | PTU | EN  | M0)) /*GPMC_D4*/\
 MUX_VAL(CP(GPMC_D5),		(IEN  | PTU | EN  | M0)) /*GPMC_D5*/\
 MUX_VAL(CP(GPMC_D6),		(IEN  | PTU | EN  | M0)) /*GPMC_D6*/\
 MUX_VAL(CP(GPMC_D7),		(IEN  | PTU | EN  | M0)) /*GPMC_D7*/\
 MUX_VAL(CP(GPMC_D8),		(IEN  | PTU | EN  | M0)) /*GPMC_D8*/\
 MUX_VAL(CP(GPMC_D9),		(IEN  | PTU | EN  | M0)) /*GPMC_D9*/\
 MUX_VAL(CP(GPMC_D10),		(IEN  | PTU | EN  | M0)) /*GPMC_D10*/\
 MUX_VAL(CP(GPMC_D11),		(IEN  | PTU | EN  | M0)) /*GPMC_D11*/\
 MUX_VAL(CP(GPMC_D12),		(IEN  | PTU | EN  | M0)) /*GPMC_D12*/\
 MUX_VAL(CP(GPMC_D13),		(IEN  | PTU | EN  | M0)) /*GPMC_D13*/\
 MUX_VAL(CP(GPMC_D14),		(IEN  | PTU | EN  | M0)) /*GPMC_D14*/\
 MUX_VAL(CP(GPMC_D15),		(IEN  | PTU | EN  | M0)) /*GPMC_D15*/\
 MUX_VAL(CP(GPMC_NCS0),		(IDIS | PTU | EN  | M0)) /*GPMC_nCS0*/\
 MUX_VAL(CP(GPMC_NCS1),		(IDIS | PTU | EN  | M0)) /*GPMC_nCS1*/\
 MUX_VAL(CP(GPMC_NCS2),		(IDIS | PTU | EN  | M0)) /*GPMC_nCS2*/\
 MUX_VAL(CP(GPMC_NCS3),		(IEN  | PTU | EN  | M4)) /*GPIO_54*/\
 MUX_VAL(CP(GPMC_NCS4),		(IDIS | PTU | EN  | M3)) /*GPIO*/\
 MUX_VAL(CP(GPMC_NCS5),		(IDIS | PTU | EN  | M4)) /*GPIO*/\
 MUX_VAL(CP(GPMC_NCS6),		(IEN  | PTU | EN  | M4)) /*GPIO*/\
 MUX_VAL(CP(GPMC_NCS7),		(IEN  | PTU | EN  | M4)) /*GPIO*/\
 MUX_VAL(CP(GPMC_CLK),		(IEN  | PTU | EN  | M0)) /*GPMC_CLK*/\
 MUX_VAL(CP(GPMC_NADV_ALE),	(IDIS | PTD | DIS | M0)) /*GPMC_nADV_ALE*/\
 MUX_VAL(CP(GPMC_NOE),		(IDIS | PTD | DIS | M0)) /*GPMC_nOE*/\
 MUX_VAL(CP(GPMC_NWE),		(IDIS | PTD | DIS | M0)) /*GPMC_nWE*/\
 MUX_VAL(CP(GPMC_NBE0_CLE),	(IDIS | PTU | EN  | M0)) /*GPMC_nBE0_CLE*/\
 MUX_VAL(CP(GPMC_NBE1),		(IEN  | PTU | EN  | M0)) /*GPMC_nBE1*/\
 MUX_VAL(CP(GPMC_NWP),		(IEN  | PTD | DIS | M0)) /*GPMC_nWP*/\
 MUX_VAL(CP(GPMC_WAIT0),	(IEN  | PTU | EN  | M0)) /*GPMC_WAIT0*/\
 MUX_VAL(CP(GPMC_WAIT1),	(IEN  | PTU | EN  | M4)) /*GPIO*/\
 MUX_VAL(CP(GPMC_WAIT2),	(IEN  | PTU | EN  | M4)) /*GPIO_64*/\
							 /* - ETH_nRESET*/\
 MUX_VAL(CP(GPMC_WAIT3),	(IEN  | PTU | EN  | M4)) /*GPIO*/\
 /*DSS*/\
 MUX_VAL(CP(DSS_PCLK),		(IDIS | PTD | DIS | M0)) /*DSS_PCLK*/\
 MUX_VAL(CP(DSS_HSYNC),		(IDIS | PTD | DIS | M0)) /*DSS_HSYNC*/\
 MUX_VAL(CP(DSS_VSYNC),		(IDIS | PTD | DIS | M0)) /*DSS_VSYNC*/\
 MUX_VAL(CP(DSS_ACBIAS),		(IDIS | PTD | DIS | M0)) /*DSS_ACBIAS*/\
 MUX_VAL(CP(DSS_DATA0),		(IDIS | PTD | DIS | M0)) /*DSS_DATA0*/\
 MUX_VAL(CP(DSS_DATA1),		(IDIS | PTD | DIS | M0)) /*DSS_DATA1*/\
 MUX_VAL(CP(DSS_DATA2),		(IDIS | PTD | DIS | M0)) /*DSS_DATA2*/\
 MUX_VAL(CP(DSS_DATA3),		(IDIS | PTD | DIS | M0)) /*DSS_DATA3*/\
 MUX_VAL(CP(DSS_DATA4),		(IDIS | PTD | DIS | M0)) /*DSS_DATA4*/\
 MUX_VAL(CP(DSS_DATA5),		(IDIS | PTD | DIS | M0)) /*DSS_DATA5*/\
 MUX_VAL(CP(DSS_DATA6),		(IDIS | PTD | DIS | M0)) /*DSS_DATA6*/\
 MUX_VAL(CP(DSS_DATA7),		(IDIS | PTD | DIS | M0)) /*DSS_DATA7*/\
 MUX_VAL(CP(DSS_DATA8),		(IDIS | PTD | DIS | M0)) /*DSS_DATA8*/\
 MUX_VAL(CP(DSS_DATA9),		(IDIS | PTD | DIS | M0)) /*DSS_DATA9*/\
 MUX_VAL(CP(DSS_DATA10),	(IDIS | PTD | DIS | M0)) /*DSS_DATA10*/\
 MUX_VAL(CP(DSS_DATA11),	(IDIS | PTD | DIS | M0)) /*DSS_DATA11*/\
 MUX_VAL(CP(DSS_DATA12),	(IDIS | PTD | DIS | M0)) /*DSS_DATA12*/\
 MUX_VAL(CP(DSS_DATA13),	(IDIS | PTD | DIS | M0)) /*DSS_DATA13*/\
 MUX_VAL(CP(DSS_DATA14),	(IDIS | PTD | DIS | M0)) /*DSS_DATA14*/\
 MUX_VAL(CP(DSS_DATA15),	(IDIS | PTD | DIS | M0)) /*DSS_DATA15*/\
 MUX_VAL(CP(DSS_DATA16),	(IDIS | PTD | DIS | M0)) /*DSS_DATA16*/\
 MUX_VAL(CP(DSS_DATA17),	(IDIS | PTD | DIS | M0)) /*DSS_DATA17*/\
 MUX_VAL(CP(DSS_DATA18),	(IDIS | PTD | DIS | M0)) /*DSS_DATA18*/\
 MUX_VAL(CP(DSS_DATA19),	(IDIS | PTD | DIS | M0)) /*DSS_DATA19*/\
 MUX_VAL(CP(DSS_DATA20),	(IDIS | PTD | DIS | M0)) /*DSS_DATA20*/\
 MUX_VAL(CP(DSS_DATA21),	(IDIS | PTD | DIS | M0)) /*DSS_DATA21*/\
 MUX_VAL(CP(DSS_DATA22),	(IDIS | PTD | DIS | M0)) /*DSS_DATA22*/\
 MUX_VAL(CP(DSS_DATA23),	(IDIS | PTD | DIS | M0)) /*DSS_DATA23*/\
 /*CAMERA*/\
 MUX_VAL(CP(CAM_HS),		(IEN  | PTU | EN  | M0)) /*CAM_HS */\
 MUX_VAL(CP(CAM_VS),		(IEN  | PTU | EN  | M0)) /*CAM_VS */\
 MUX_VAL(CP(CAM_XCLKA),		(IDIS | PTD | DIS | M0)) /*CAM_XCLKA*/\
 MUX_VAL(CP(CAM_PCLK),		(IEN  | PTU | EN  | M0)) /*CAM_PCLK*/\
 MUX_VAL(CP(CAM_FLD),		(IDIS | PTD | DIS | M4)) /*GPIO_98*/\
							 /* - CAM_RESET*/\
 MUX_VAL(CP(CAM_D0),		(IEN  | PTD | DIS | M0)) /*CAM_D0*/\
 MUX_VAL(CP(CAM_D1),		(IEN  | PTD | DIS | M0)) /*CAM_D1*/\
 MUX_VAL(CP(CAM_D2),		(IEN  | PTD | DIS | M0)) /*CAM_D2*/\
 MUX_VAL(CP(CAM_D3),		(IEN  | PTD | DIS | M0)) /*CAM_D3*/\
 MUX_VAL(CP(CAM_D4),		(IEN  | PTD | DIS | M0)) /*CAM_D4*/\
 MUX_VAL(CP(CAM_D5),		(IEN  | PTD | DIS | M0)) /*CAM_D5*/\
 MUX_VAL(CP(CAM_D6),		(IEN  | PTD | DIS | M0)) /*CAM_D6*/\
 MUX_VAL(CP(CAM_D7),		(IEN  | PTD | DIS | M0)) /*CAM_D7*/\
 MUX_VAL(CP(CAM_D8),		(IEN  | PTD | DIS | M0)) /*CAM_D8*/\
 MUX_VAL(CP(CAM_D9),		(IEN  | PTD | DIS | M0)) /*CAM_D9*/\
 MUX_VAL(CP(CAM_D10),		(IEN  | PTD | DIS | M0)) /*CAM_D10*/\
 MUX_VAL(CP(CAM_D11),		(IEN  | PTD | DIS | M0)) /*CAM_D11*/\
 MUX_VAL(CP(CAM_XCLKB),		(IDIS | PTD | DIS | M0)) /*CAM_XCLKB*/\
 MUX_VAL(CP(CAM_WEN),		(IEN  | PTD | DIS | M4)) /*GPIO_167*/\
 MUX_VAL(CP(CAM_STROBE),	(IDIS | PTD | DIS | M0)) /*CAM_STROBE*/\
 MUX_VAL(CP(CSI2_DX0),		(IEN  | PTD | DIS | M0)) /*CSI2_DX0*/\
 MUX_VAL(CP(CSI2_DY0),		(IEN  | PTD | DIS | M0)) /*CSI2_DY0*/\
 MUX_VAL(CP(CSI2_DX1),		(IEN  | PTD | DIS | M0)) /*CSI2_DX1*/\
 MUX_VAL(CP(CSI2_DY1),		(IEN  | PTD | DIS | M0)) /*CSI2_DY1*/\
 /*Audio Interface */\
 MUX_VAL(CP(MCBSP2_FSX),	(IEN  | PTD | DIS | M0)) /*McBSP2_FSX*/\
 MUX_VAL(CP(MCBSP2_CLKX),	(IEN  | PTD | DIS | M0)) /*McBSP2_CLKX*/\
 MUX_VAL(CP(MCBSP2_DR),		(IEN  | PTD | DIS | M0)) /*McBSP2_DR*/\
 MUX_VAL(CP(MCBSP2_DX),		(IDIS | PTD | DIS | M0)) /*McBSP2_DX*/\
 /*Expansion card  */\
 MUX_VAL(CP(MMC1_CLK),		(IEN  | PTU | EN  | M0)) /*MMC1_CLK*/\
 MUX_VAL(CP(MMC1_CMD),		(IEN  | PTU | DIS | M0)) /*MMC1_CMD*/\
 MUX_VAL(CP(MMC1_DAT0),		(IEN  | PTU | DIS | M0)) /*MMC1_DAT0*/\
 MUX_VAL(CP(MMC1_DAT1),		(IEN  | PTU | DIS | M0)) /*MMC1_DAT1*/\
 MUX_VAL(CP(MMC1_DAT2),		(IEN  | PTU | DIS | M0)) /*MMC1_DAT2*/\
 MUX_VAL(CP(MMC1_DAT3),		(IEN  | PTU | DIS | M0)) /*MMC1_DAT3*/\
 MUX_VAL(CP(MMC1_DAT4),		(IEN  | PTU | EN  | M4)) /*MMC1_DAT4,\
							   WriteProtect*/\
 MUX_VAL(CP(MMC1_DAT5),		(IEN  | PTU | EN  | M4)) /*MMC1_DAT5,\
							   CardDetect*/\
 MUX_VAL(CP(MMC1_DAT6),		(IEN  | PTU | EN | M4)) /*MMC1_DAT6*/\
 MUX_VAL(CP(MMC1_DAT7),		(IEN  | PTU | EN | M4)) /*MMC1_DAT7*/\
 /*Wireless LAN */\
 MUX_VAL(CP(MMC2_CLK),		(IEN  | PTD | EN | M0)) /*MMC2_CLK*/\
 MUX_VAL(CP(MMC2_CMD),		(IEN  | PTD | DIS | M0)) /*MMC2_CMD*/\
 MUX_VAL(CP(MMC2_DAT0),		(IEN  | PTD | DIS | M0)) /*MMC2_DAT0*/\
 MUX_VAL(CP(MMC2_DAT1),		(IEN  | PTD | DIS | M0)) /*MMC2_DAT1*/\
 MUX_VAL(CP(MMC2_DAT2),		(IEN  | PTU | EN  | M4)) /*MMC2_DAT2*/\
 MUX_VAL(CP(MMC2_DAT3),		(IEN  | PTU | EN  | M4)) /*MMC2_DAT3*/\
 /*Bluetooth*/\
 MUX_VAL(CP(MCBSP3_DX),		(IDIS | PTD | DIS | M0)) /*McBSP3_DX*/\
 MUX_VAL(CP(MCBSP3_DR),		(IEN  | PTD | DIS | M0)) /*McBSP3_DR*/\
 MUX_VAL(CP(MCBSP3_CLKX),	(IEN  | PTD | DIS | M0)) /*McBSP3_CLKX  */\
 MUX_VAL(CP(MCBSP3_FSX),	(IEN  | PTD | DIS | M0)) /*McBSP3_FSX*/\
 MUX_VAL(CP(UART2_CTS),		(IEN  | PTU | EN  | M0)) /*UART2_CTS*/\
 MUX_VAL(CP(UART2_RTS),		(IDIS | PTD | DIS | M0)) /*UART2_RTS*/\
 MUX_VAL(CP(UART2_TX),		(IDIS | PTD | DIS | M0)) /*UART2_TX*/\
 MUX_VAL(CP(UART2_RX),		(IEN  | PTD | DIS | M0)) /*UART2_RX*/\
 /*Modem Interface */\
 MUX_VAL(CP(UART1_TX),		(IDIS | PTD | EN  | M4)) /*GPIO    */\
 MUX_VAL(CP(UART1_RTS),		(IDIS | PTD | EN  | M4)) /*GPIO     */\
 MUX_VAL(CP(UART1_CTS),		(IDIS | PTD | EN  | M4)) /*GPIO     */\
 MUX_VAL(CP(UART1_RX),		(IDIS | PTD | EN  | M4)) /*GPIO    */\
 MUX_VAL(CP(MCBSP4_CLKX),	(IDIS | PTD | DIS | M4)) /*GPIO_152*/\
							 /* - LCD_INI*/\
 MUX_VAL(CP(MCBSP4_DR),		(IDIS | PTD | DIS | M4)) /*GPIO_153*/\
							 /* - LCD_ENVDD */\
 MUX_VAL(CP(MCBSP4_DX),		(IDIS | PTD | DIS | M4)) /*GPIO_154*/\
							 /* - LCD_QVGA/nVGA */\
 MUX_VAL(CP(MCBSP4_FSX),	(IDIS | PTD | DIS | M4)) /*GPIO_155*/\
							 /* - LCD_RESB */\
 MUX_VAL(CP(MCBSP1_CLKR),	(IDIS | PTD | DIS | M4)) /*GPIO156*/\
 MUX_VAL(CP(MCBSP1_FSR),	(IDIS | PTU | DIS | M4)) /*GPIO157*/\
 MUX_VAL(CP(MCBSP1_DX),		(IDIS | PTD | DIS | M0)) /*MCBSP1_DX*/\
 MUX_VAL(CP(MCBSP1_DR),		(IDIS | PTD | DIS | M4)) /*GPIO159*/\
 MUX_VAL(CP(MCBSP_CLKS),	(IEN  | PTU | DIS | M0)) /*MCBSP_CLKS  */\
 MUX_VAL(CP(MCBSP1_FSX),	(IEN  | PTD | DIS | M0)) /*MCBSP1_FSX*/\
 MUX_VAL(CP(MCBSP1_CLKX),	(IEN  | PTD | DIS | M0)) /*MCBSP1_CLKX  */\
 /*Serial Interface*/\
 MUX_VAL(CP(UART3_CTS_RCTX),	(IEN  | PTU | DIS  | M0)) /*UART3_CTS_*/\
							 /* RCTX*/\
 MUX_VAL(CP(UART3_RTS_SD),	(IDIS | PTD | DIS | M0)) /*UART3_RTS_SD */\
 MUX_VAL(CP(UART3_RX_IRRX),	(IEN  | PTD | DIS | M0)) /*UART3_RX_IRRX*/\
 MUX_VAL(CP(UART3_TX_IRTX),	(IDIS | PTD | DIS | M0)) /*UART3_TX_IRTX*/\
 MUX_VAL(CP(HSUSB0_CLK),	(IEN  | PTD | DIS | M0)) /*HSUSB0_CLK*/\
 MUX_VAL(CP(HSUSB0_STP),	(IDIS | PTU | EN  | M0)) /*HSUSB0_STP*/\
 MUX_VAL(CP(HSUSB0_DIR),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DIR*/\
 MUX_VAL(CP(HSUSB0_NXT),	(IEN  | PTD | DIS | M0)) /*HSUSB0_NXT*/\
 MUX_VAL(CP(HSUSB0_DATA0),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA0*/\
 MUX_VAL(CP(HSUSB0_DATA1),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA1*/\
 MUX_VAL(CP(HSUSB0_DATA2),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA2*/\
 MUX_VAL(CP(HSUSB0_DATA3),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA3*/\
 MUX_VAL(CP(HSUSB0_DATA4),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA4*/\
 MUX_VAL(CP(HSUSB0_DATA5),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA5*/\
 MUX_VAL(CP(HSUSB0_DATA6),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA6*/\
 MUX_VAL(CP(HSUSB0_DATA7),	(IEN  | PTD | DIS | M0)) /*HSUSB0_DATA7*/\
 MUX_VAL(CP(I2C1_SCL),		(IEN  | PTU | EN  | M0)) /*I2C1_SCL*/\
 MUX_VAL(CP(I2C1_SDA),		(IEN  | PTU | EN  | M0)) /*I2C1_SDA*/\
 MUX_VAL(CP(I2C2_SCL),		(IEN  | PTU | EN  | M0)) /*I2C2_SCL*/\
 MUX_VAL(CP(I2C2_SDA),		(IEN  | PTU | EN  | M0)) /*I2C2_SDA*/\
 MUX_VAL(CP(I2C3_SCL),		(IEN  | PTD | EN  | M4)) /*GPIO_184*/\
 MUX_VAL(CP(I2C3_SDA),		(IDIS | PTD | DIS | M4)) /*GPIO_185*/\
 MUX_VAL(CP(I2C4_SCL),		(IEN  | PTU | EN  | M0)) /*I2C4_SCL*/\
 MUX_VAL(CP(I2C4_SDA),		(IEN  | PTU | EN  | M0)) /*I2C4_SDA*/\
 MUX_VAL(CP(HDQ_SIO),		(IDIS | PTD | DIS | M4)) /*GPIO_170*/\
 MUX_VAL(CP(MCSPI1_CLK),	(IEN  | PTU | EN  | M4)) /*McSPI1_CLK*/\
 MUX_VAL(CP(MCSPI1_SIMO),	(IEN  | PTU | EN  | M4)) /*McSPI1_SIMO  */\
 MUX_VAL(CP(MCSPI1_SOMI),	(IEN  | PTU | EN  | M4)) /*McSPI1_SOMI  */\
 MUX_VAL(CP(MCSPI1_CS0),	(IEN  | PTU | EN  | M4)) /*McSPI1_CS0*/\
 MUX_VAL(CP(MCSPI1_CS1),	(IEN  | PTU | EN  | M4)) /*McSPI1_CS1*/\
							 /* TS_PEN_IRQ */\
 MUX_VAL(CP(MCSPI1_CS2),	(IEN  | PTU | EN  | M4)) /*McSPI1_CS2*/\
							 /* - LAN_INTR*/\
 MUX_VAL(CP(MCSPI1_CS3),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 MUX_VAL(CP(MCSPI2_CLK),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 MUX_VAL(CP(MCSPI2_SIMO),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 MUX_VAL(CP(MCSPI2_SOMI),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 MUX_VAL(CP(MCSPI2_CS0),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 MUX_VAL(CP(MCSPI2_CS1),	(IEN  | PTD | EN  | M3)) /*HsUSB2*/\
 /*Control and debug */\
 MUX_VAL(CP(SYS_32K),		(IEN  | PTD | DIS | M0)) /*SYS_32K*/\
 MUX_VAL(CP(SYS_CLKREQ),	(IEN  | PTD | DIS | M0)) /*SYS_CLKREQ*/\
 MUX_VAL(CP(SYS_NIRQ),		(IEN  | PTU | EN  | M4)) /*GPIO_0*/\
 MUX_VAL(CP(SYS_NRESWARM), 	(IDIS | PTU | DIS | M4)) /*SYS_nRESWARM */\
							 /* - GPIO30 */\
 MUX_VAL(CP(SYS_BOOT0),		(IEN  | PTD | DIS | M4)) /*GPIO_2*/\
							 /* - PEN_IRQ */\
 MUX_VAL(CP(SYS_BOOT1),		(IEN  | PTD | DIS | M4)) /*GPIO_3 */\
 MUX_VAL(CP(SYS_BOOT2),		(IEN  | PTD | EN  | M4)) /*GPIO_4*/\
 MUX_VAL(CP(SYS_BOOT3),		(IEN  | PTD | DIS | M4)) /*GPIO_5*/\
 MUX_VAL(CP(SYS_BOOT4),		(IEN  | PTD | DIS | M4)) /*GPIO_6*/\
 MUX_VAL(CP(SYS_BOOT5),		(IEN  | PTD | DIS | M4)) /*GPIO_7*/\
 MUX_VAL(CP(SYS_BOOT6),		(IDIS | PTD | DIS | M4)) /*GPIO_8*/\
							 /* - VIO_1V8*/\
 MUX_VAL(CP(SYS_OFF_MODE),	(IEN  | PTD | DIS | M0)) /*SYS_OFF_MODE*/\
 MUX_VAL(CP(SYS_CLKOUT1),	(IEN  | PTD | DIS | M0)) /*SYS_CLKOUT1*/\
 MUX_VAL(CP(SYS_CLKOUT2),	(IEN  | PTU | EN  | M0)) /*SYS_CLKOUT2*/\
 MUX_VAL(CP(JTAG_nTRST),	(IEN  | PTD | DIS | M0)) /*JTAG_nTRST*/\
 MUX_VAL(CP(JTAG_TCK),		(IEN  | PTD | DIS | M0)) /*JTAG_TCK*/\
 MUX_VAL(CP(JTAG_TMS),		(IEN  | PTD | DIS | M0)) /*JTAG_TMS*/\
 MUX_VAL(CP(JTAG_TDI),		(IEN  | PTD | DIS | M0)) /*JTAG_TDI*/\
 MUX_VAL(CP(JTAG_EMU0),		(IEN  | PTD | DIS | M0)) /*JTAG_EMU0*/\
 MUX_VAL(CP(JTAG_EMU1),		(IEN  | PTD | DIS | M0)) /*JTAG_EMU1*/\
 MUX_VAL(CP(ETK_CLK_ES2),	(IDIS | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_CTL_ES2),	(IDIS | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D0_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D1_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D2_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D3_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D4_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D5_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D6_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D7_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D8_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D9_ES2 ),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D10_ES2),	(IDIS | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D11_ES2),	(IDIS | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D12_ES2),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D13_ES2),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D14_ES2),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 MUX_VAL(CP(ETK_D15_ES2),	(IEN  | PTD | EN  | M3)) /*HsUSB1*/\
 /*Die to Die */\
 MUX_VAL(CP(CCDC_PCLK),		(IEN  | PTU | EN  | M0)) /*ccdc_pclk*/\
 MUX_VAL(CP(CCDC_FIELD),	(IEN  | PTD | DIS  | M1)) /*ccdc_field*/\
 MUX_VAL(CP(CCDC_HD),		(IEN  | PTU | EN  | M0)) /*ccdc_hd*/\
 MUX_VAL(CP(CCDC_VD),		(IEN  | PTU | EN  | M0)) /*ccdc_vd*/\
 MUX_VAL(CP(CCDC_WEN),		(IEN  | PTD | DIS  | M1)) /*ccdc_wen*/\
 MUX_VAL(CP(CCDC_DATA0),	(IEN  | PTD | DIS | M0)) /*ccdc_data0*/\
 MUX_VAL(CP(CCDC_DATA1),    (IEN  | PTD | DIS | M0)) /*ccdc_data1*/\
 MUX_VAL(CP(CCDC_DATA2),	(IEN  | PTD | DIS | M0)) /*ccdc_data2*/\
 MUX_VAL(CP(CCDC_DATA3),	(IEN  | PTD | DIS | M0)) /*ccdc_data3*/\
 MUX_VAL(CP(CCDC_DATA4),	(IEN  | PTD | DIS | M0)) /*ccdc_data4*/\
 MUX_VAL(CP(CCDC_DATA5),	(IEN  | PTD | DIS | M0)) /*ccdc_data5*/\
 MUX_VAL(CP(CCDC_DATA6),	(IEN  | PTD | DIS | M0)) /*ccdc_data6*/\
 MUX_VAL(CP(CCDC_DATA7),	(IEN  | PTD | DIS | M0)) /*ccdc_data7*/\
 MUX_VAL(CP(RMII_MDIO_DATA),(IEN  |  M0)) /*rmii_mdio_data*/\
 MUX_VAL(CP(RMII_MDIO_CLK),	(M0)) /*rmii_mdio_clk*/\
 MUX_VAL(CP(RMII_RXD0)	,	(IEN  | PTD | M0)) /*rmii_rxd0*/\
 MUX_VAL(CP(RMII_RXD1),		(IEN  | PTD | M0)) /*rmii_rxd1*/\
 MUX_VAL(CP(RMII_CRS_DV),	(IEN  | PTD | M0)) /*rmii_crs_dv*/\
 MUX_VAL(CP(RMII_RXER),		(PTD | M0)) /*rmii_rxer*/\
 MUX_VAL(CP(RMII_TXD0),		(PTD | M0)) /*rmii_txd0*/\
 MUX_VAL(CP(RMII_TXD1),		(PTD | M0)) /*rmii_txd1*/\
 MUX_VAL(CP(RMII_TXEN),		(PTD | M0)) /*rmii_txen*/\
 MUX_VAL(CP(RMII_50MHZ_CLK),(IEN  | PTD | EN  | M0)) /*rmii_50mhz_clk*/\
 MUX_VAL(CP(USB0_DRVBUS),	(IEN  | PTD | EN  | M0)) /*usb0_drvbus*/\
 MUX_VAL(CP(HECC1_TXD),		(IEN  | PTU | EN  | M0)) /*hecc1_txd*/\
 MUX_VAL(CP(HECC1_RXD),		(IEN  | PTU | EN  | M0)) /*hecc1_rxd*/\
 MUX_VAL(CP(SYS_BOOT7),		(IEN  | PTD | EN  | M0)) /*sys_boot7*/\
 MUX_VAL(CP(SDRC_DQS0N),	(IEN  | PTD | EN  | M0)) /*sdrc_dqs0n*/\
 MUX_VAL(CP(SDRC_DQS1N),	(IEN  | PTD | EN  | M0)) /*sdrc_dqs1n*/\
 MUX_VAL(CP(SDRC_DQS2N),	(IEN  | PTD | EN  | M0)) /*sdrc_dqs2n*/\
 MUX_VAL(CP(SDRC_DQS3N),	(IEN  | PTD | EN  | M0)) /*sdrc_dqs3n*/\
 MUX_VAL(CP(STRBEN_DLY0),	(IEN  | PTD | EN  | M0)) /*sdrc_strben_dly0*/\
 MUX_VAL(CP(STRBEN_DLY1),	(IEN  | PTD | EN  | M0)) /*sdrc_strben_dly1*/\
 MUX_VAL(CP(SYS_BOOT8),		(IEN  | PTD | EN  | M0)) /*sys_boot8*/\
 MUX_VAL(CP(D2D_MCAD34),	(IEN  | PTD | EN  | M0)) /*d2d_mcad34*/\
 MUX_VAL(CP(D2D_MCAD35),	(IEN  | PTD | EN  | M0)) /*d2d_mcad35*/\
 MUX_VAL(CP(D2D_MCAD36),	(IEN  | PTD | EN  | M0)) /*d2d_mcad36*/\
 MUX_VAL(CP(D2D_CLK26MI),	(IEN  | PTD | DIS | M0)) /*d2d_clk26mi*/\
 MUX_VAL(CP(D2D_NRESPWRON),	(IEN  | PTD | EN  | M0)) /*d2d_nrespwron*/\
 MUX_VAL(CP(D2D_NRESWARM),	(IEN  | PTU | EN  | M0)) /*d2d_nreswarm */\
 MUX_VAL(CP(D2D_ARM9NIRQ),	(IEN  | PTD | DIS | M0)) /*d2d_arm9nirq */\
 MUX_VAL(CP(D2D_UMA2P6FIQ),	(IEN  | PTD | DIS | M0)) /*d2d_uma2p6fiq*/\
 MUX_VAL(CP(D2D_SPINT),		(IEN  | PTD | EN  | M0)) /*d2d_spint*/\
 MUX_VAL(CP(D2D_FRINT),		(IEN  | PTD | EN  | M0)) /*d2d_frint*/\
 MUX_VAL(CP(D2D_DMAREQ0),	(IEN  | PTD | DIS | M0)) /*d2d_dmareq0*/\
 MUX_VAL(CP(D2D_DMAREQ1),	(IEN  | PTD | DIS | M0)) /*d2d_dmareq1*/\
 MUX_VAL(CP(D2D_DMAREQ2),	(IEN  | PTD | DIS | M0)) /*d2d_dmareq2*/\
 MUX_VAL(CP(D2D_DMAREQ3),	(IEN  | PTD | DIS | M0)) /*d2d_dmareq3*/\
 MUX_VAL(CP(D2D_N3GTRST),	(IEN  | PTD | DIS | M0)) /*d2d_n3gtrst*/\
 MUX_VAL(CP(D2D_N3GTDI),	(IEN  | PTD | DIS | M0)) /*d2d_n3gtdi*/\
 MUX_VAL(CP(D2D_N3GTDO),	(IEN  | PTD | DIS | M0)) /*d2d_n3gtdo*/\
 MUX_VAL(CP(D2D_N3GTMS),	(IEN  | PTD | DIS | M0)) /*d2d_n3gtms*/\
 MUX_VAL(CP(D2D_N3GTCK),	(IEN  | PTD | DIS | M0)) /*d2d_n3gtck*/\
 MUX_VAL(CP(D2D_N3GRTCK),	(IEN  | PTD | DIS | M0)) /*d2d_n3grtck*/\
 MUX_VAL(CP(D2D_MSTDBY),	(IEN  | PTU | EN  | M0)) /*d2d_mstdby*/\
 MUX_VAL(CP(D2D_SWAKEUP),	(IEN  | PTD | EN  | M0)) /*d2d_swakeup*/\
 MUX_VAL(CP(D2D_IDLEREQ),	(IEN  | PTD | DIS | M0)) /*d2d_idlereq*/\
 MUX_VAL(CP(D2D_IDLEACK),	(IEN  | PTU | EN  | M0)) /*d2d_idleack*/\
 MUX_VAL(CP(D2D_MWRITE),	(IEN  | PTD | DIS | M0)) /*d2d_mwrite*/\
 MUX_VAL(CP(D2D_SWRITE),	(IEN  | PTD | DIS | M0)) /*d2d_swrite*/\
 MUX_VAL(CP(D2D_MREAD),		(IEN  | PTD | DIS | M0)) /*d2d_mread*/\
 MUX_VAL(CP(D2D_SREAD),		(IEN  | PTD | DIS | M0)) /*d2d_sread*/\
 MUX_VAL(CP(D2D_MBUSFLAG),	(IEN  | PTD | DIS | M0)) /*d2d_mbusflag*/\
 MUX_VAL(CP(D2D_SBUSFLAG),	(IEN  | PTD | DIS | M0)) /*d2d_sbusflag*/\
 MUX_VAL(CP(SDRC_CKE0),		(M0)) /*sdrc_cke0*/\
 MUX_VAL(CP(SDRC_CKE1),		(M0)) /*sdrc_cke1*/\

#endif
