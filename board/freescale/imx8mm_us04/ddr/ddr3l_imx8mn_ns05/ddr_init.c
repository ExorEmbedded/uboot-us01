/*
 * Copyright 2019 NXP
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Generated code from MX8M_DDR_tool
 * Align with uboot-imx_v2017.03_4.9.51_imx8m_ga
 */

#include <common.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/ddr.h>
#include <asm/arch/clock.h>
#include "../ddr.h"

#ifdef CONFIG_ENABLE_DDR_TRAINING_DEBUG
#define ddr_printf(args...) printf(args)
#else
#define ddr_printf(args...)
#endif

#include "../wait_ddrphy_training_complete.c"

#ifndef SRC_DDRC_RCR_ADDR
#define SRC_DDRC_RCR_ADDR SRC_IPS_BASE_ADDR +0x1000
#endif
#ifndef DDR_CSD1_BASE_ADDR
#define DDR_CSD1_BASE_ADDR 0x40000000
#endif
#define SILICON_TRAIN

void ddr_cfg_phy(void);
volatile unsigned int tmp, tmp_t, i;
void ddr_init(void)
{
	/** Initialize DDR clock and DDRC registers **/
	reg32_write(0x30330244,0x0);
	reg32_write(0x30330248,0x0);
	reg32_write(0x303304ac,0xe);
	reg32_write(0x303304b0,0xe);
	reg32_write(0x30330504,0x2);
	reg32_write(0x30391000,0x8f00003f);
	reg32_write(0x30391000,0x8f00000f);
	reg32_write(0x3038a088,0x7070000);
	reg32_write(0x3038a084,0x4030000);
	reg32_write(0x303a00ec,0xffff);
	tmp=reg32_read(0x303a00f8);
	tmp |= 0x20;
	reg32_write(0x303a00f8,tmp);
	reg32_write(0x30391004,0x8f000000);
	reg32_write(0x30360054,0x12c091);
	reg32_write(0x30360058,0x0);
	tmp=reg32_read(0x30360050);
	tmp |= 0x200;
	reg32_write(0x30360050,tmp);
	tmp=reg32_read(0x30360050);
	tmp &= ~0x10;
	reg32_write(0x30360050,tmp);
	do{
		tmp=reg32_read(0x30360050);
		if(tmp&0x80000000) break;
	}while(1);
	reg32_write(0x30391000,0x8f000006);
	reg32_write(0x3d400304,0x1);
	reg32_write(0x3d400030,0x20);
	reg32_write(0x3d400000,0x81040001);
	reg32_write(0x3d400064,0x61008c);
	reg32_write(0x3d4000d0,0xc00200c5);
	reg32_write(0x3d4000d4,0x1000b);
	reg32_write(0x3d4000dc,0x1d700004);
	reg32_write(0x3d4000e0,0x180000);
	reg32_write(0x3d4000e4,0x90000);
	reg32_write(0x3d4000f0,0x0);
	reg32_write(0x3d4000f4,0xee5);
	reg32_write(0x3d400100,0xc101b0e);
	reg32_write(0x3d400104,0x30314);
	reg32_write(0x3d400108,0x4060509);
	reg32_write(0x3d40010c,0x2006);
	reg32_write(0x3d400110,0x6020306);
	reg32_write(0x3d400114,0x4040302);
	reg32_write(0x3d400120,0x909);
	reg32_write(0x3d400180,0x40800020);
	reg32_write(0x3d400184,0xc350);
	reg32_write(0x3d400190,0x3868203);
	reg32_write(0x3d400194,0x20303);
	reg32_write(0x3d4001b4,0x603);
	reg32_write(0x3d400198,0x7000000);
	reg32_write(0x3d4001b0,0x11);
	reg32_write(0x3d4001a0,0x400018);
	reg32_write(0x3d4001a4,0x5003c);
	reg32_write(0x3d4001a8,0x80000000);
	reg32_write(0x3d4001c4,0x0);
	reg32_write(0x3d400200,0x1f);
	reg32_write(0x3d400204,0x80808);
	reg32_write(0x3d400208,0x0);
	reg32_write(0x3d40020c,0x0);
	reg32_write(0x3d400210,0x1f1f);
	reg32_write(0x3d400214,0x7070707);
	reg32_write(0x3d400218,0x7070707);
	reg32_write(0x3d400240,0x600060c);
	reg32_write(0x3d400244,0x1323);
	reg32_write(0x3d400400,0x100);
	reg32_write(0x3d400250,0x7ab50b07);
	reg32_write(0x3d400254,0x22);
	reg32_write(0x3d40025c,0x7b00665e);
	reg32_write(0x3d400264,0xb0000040);
	reg32_write(0x3d40026c,0x50000a0c);
	reg32_write(0x3d400300,0x17);
	reg32_write(0x3d40036c,0x10000);
	reg32_write(0x3d400404,0x3051);
	reg32_write(0x3d400408,0x61d2);
	reg32_write(0x3d400494,0xe00);
	reg32_write(0x3d400498,0x7ff);
	reg32_write(0x3d40049c,0xe00);
	reg32_write(0x3d4004a0,0x7ff);
	reg32_write(0x3d402064,0x28003b);
	reg32_write(0x3d4020dc,0x12200004);
	reg32_write(0x3d4020e0,0x0);
	reg32_write(0x3d402100,0x7090b07);
	reg32_write(0x3d402104,0x20209);
	reg32_write(0x3d402108,0x3030407);
	reg32_write(0x3d40210c,0x2006);
	reg32_write(0x3d402110,0x3020203);
	reg32_write(0x3d402114,0x3030202);
	reg32_write(0x3d402120,0x909);
	reg32_write(0x3d402180,0x40800020);
	reg32_write(0x3d402190,0x3818200);
	reg32_write(0x3d402194,0x20303);
	reg32_write(0x3d4021b4,0x100);
	reg32_write(0x3d402240,0x6000604);
	reg32_write(0x3d4020f4,0xee5);
	reg32_write(0x3d400028,0x0);
	reg32_write(0x30391000,0x8f000004);
	reg32_write(0x30391000,0x8f000000);
	reg32_write(0x3d400304,0x0);
	reg32_write(0x3d400060,0x1);
	reg32_write(0x3d400320,0x0);
	reg32_write(0x3d4001b0,0x10);
	reg32_write(0x3d400320,0x1);

	/* Configure DDR PHY's registers */
	ddr_cfg_phy();

	reg32_write(DDRC_RFSHCTL3(0), 0x00000000);
	reg32_write(DDRC_SWCTL(0), 0x0000);
	/*
	 * ------------------- 9 -------------------
	 * Set DFIMISC.dfi_init_start to 1 
	 *  -----------------------------------------
	 */
	reg32_write(DDRC_DFIMISC(0), 0x00000030);
	reg32_write(DDRC_SWCTL(0), 0x0001);

	/* wait DFISTAT.dfi_init_complete to 1 */
	tmp_t = 0;
	while(tmp_t==0){
		tmp  = reg32_read(DDRC_DFISTAT(0));
		tmp_t = tmp & 0x01;
		tmp  = reg32_read(DDRC_MRSTAT(0));
	}

	reg32_write(DDRC_SWCTL(0), 0x0000);

	/* clear DFIMISC.dfi_init_complete_en */
	reg32_write(DDRC_DFIMISC(0), 0x00000010);
	reg32_write(DDRC_DFIMISC(0), 0x00000011);
	reg32_write(DDRC_PWRCTL(0), 0x00000088);

	tmp = reg32_read(DDRC_CRCPARSTAT(0));
	/*
	 * set SWCTL.sw_done to enable quasi-dynamic register
	 * programming outside reset.
	 */
	reg32_write(DDRC_SWCTL(0), 0x00000001);

	/* wait SWSTAT.sw_done_ack to 1 */
	while((reg32_read(DDRC_SWSTAT(0)) & 0x1) == 0)
		;

	/* wait STAT.operating_mode([1:0] for ddr3) to normal state */
	while ((reg32_read(DDRC_STAT(0)) & 0x3) != 0x1)
		;

	reg32_write(DDRC_PWRCTL(0), 0x00000088);
	/* reg32_write(DDRC_PWRCTL(0), 0x018a); */
	tmp = reg32_read(DDRC_CRCPARSTAT(0));

	/* enable port 0 */
	reg32_write(DDRC_PCTRL_0(0), 0x00000001);
	/* enable DDR auto-refresh mode */
	tmp = reg32_read(DDRC_RFSHCTL3(0)) & ~0x1;
	reg32_write(DDRC_RFSHCTL3(0), tmp);
}
