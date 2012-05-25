/*
 * (C) Copyright 2004-2008
 * Texas Instruments, <www.ti.com>
 *
 * Derived from Beagle Board and 3430 SDP code by
 *	Richard Woodruff <r-woodruff2@ti.com>
 *	Syed Mohammed Khasim <khasim@ti.com>
 *
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <asm/io.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <asm/mach-types.h>
#include "un30.h"
#include <asm/arch/mem.h>

/******************************************************************************
 * Routine: Enable_RS232PHY
 * Description: Enables the programmable serial PHY in RS232 mode, TX enabled
 *****************************************************************************/
void Enable_RS232PHY(void)
{
  u32 tmp;
  tmp = readl(ADR_GENREG_OUT);
  tmp |= 0x02;
  writel(tmp,ADR_GENREG_OUT);
}

/******************************************************************************
 * Routine: board_init
 * Description: Early hardware init.
 *****************************************************************************/
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3_BEAGLE;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/******************************************************************************
 * Routine: lcd_pwron
 * Description: power on the LCD circuitry
 *****************************************************************************/
int lcd_pwron(struct panel_config* panel_cfg)
{
	int i;
	
	//Configure the LightEVC FPGA core
	if(panel_cfg-> pclk_inv)
	  writew(EVC_CLKINV_MASK,EVC_CLKCTRL_ADDRESS);
	else
	  writew(0,EVC_CLKCTRL_ADDRESS);

	writew(0x00ff,EVC_SCTRL_ADDRESS);

	//LCD power up sequence
	writew(EVC_ENVOLTAGE_MASK,EVC_PWRCTRL_ADDRESS);
	for(i=0;i<50;i++)udelay(1000);
	writew(EVC_ENVOLTAGE_MASK|EVC_ENVIDEO_MASK,EVC_PWRCTRL_ADDRESS);
	for(i=0;i<300;i++)udelay(1000);
	writew(EVC_ENVOLTAGE_MASK|EVC_ENVIDEO_MASK|EVC_ENBLIGHT_MASK,EVC_PWRCTRL_ADDRESS);

	omap_request_gpio(56);
	omap_set_gpio_direction(56,0);
	omap_set_gpio_dataout(56,1);
}

/******************************************************************************
 * Routine: misc_init_r
 * Description: Configure power supply
 *****************************************************************************/
int misc_init_r(void)
{
  int ret = 0;
#ifdef CONFIG_CMD_I2CHWCFG
	extern int i2cgethwcfg (void);
#endif

/*
 * I2C seeprom HW cfg handling
 */
#ifdef CONFIG_CMD_I2CHWCFG
	ret = i2cgethwcfg ();
	if(ret)
	  ret = i2cgethwcfg (); /* Added one retry ... just in case (never experienced the need for that */
#endif

/*
 * LCD support
 */
#ifdef CONFIG_VIDEO_OMAP3
	omap3_dss_panel_getenv(&display_cfg);
    omap3_dss_panel_config(&display_cfg);
    omap3_dss_set_background_col(BACKGROUND_COLOR);
	omap3_dss_panel_splash(&display_cfg);
    omap3_dss_enable();
	lcd_pwron(&display_cfg);
#endif
	return ret;
}

/******************************************************************************
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 *****************************************************************************/
void set_muxconf_regs(void)
{
	MUX_UN30();
}

/******************************************************************************
 * Routine: setup_ambabridge
 * Description: Setting up the configuration GPMC registers specific to the
 *		AMBA bridge. And init the peripherals.
 *****************************************************************************/
void setup_ambabridge(void)
{
	unsigned int *gpmc_cs3_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(3 * GPMC_CONFIG_WIDTH));
	unsigned int *gpmc_cs4_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(4 * GPMC_CONFIG_WIDTH));
	unsigned int *ledreg =(unsigned int*)ADDR_LEDREG;
	unsigned int addr[]={CONFIG_ULTIADDA_BASE,CONFIG_ULTIADDA_BASE+0x04,CONFIG_ULTIADDA_BASE+0x08,CONFIG_ULTIADDA_BASE+0x0c,CONFIG_ULTIADDA_BASE+0x10,CONFIG_ULTIADDA_BASE+0x14,CONFIG_ULTIADDA_BASE+0x18,CONFIG_ULTIADDA_BASE+0x1c,CONFIG_ULTIADDA_BASE+0xc0};
	unsigned int regv[]={0x002000l,0x004000l     ,0x006000l     ,0x008000l     ,0x00a000l     ,0x00c000l     ,0x00e000l     ,0x000000l     ,0x000000l};
	int ii;

	/* Configure GPMC registers for APB bus (base = 0x1a000000)*/
	writel(0, gpmc_cs3_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(APB_GPMC_CONFIG1, gpmc_cs3_base + OFFS(GPMC_CONFIG1));
	writel(APB_GPMC_CONFIG2, gpmc_cs3_base + OFFS(GPMC_CONFIG2));
	writel(APB_GPMC_CONFIG3, gpmc_cs3_base + OFFS(GPMC_CONFIG3));
	writel(APB_GPMC_CONFIG4, gpmc_cs3_base + OFFS(GPMC_CONFIG4));
	writel(APB_GPMC_CONFIG5, gpmc_cs3_base + OFFS(GPMC_CONFIG5));
	writel(APB_GPMC_CONFIG6, gpmc_cs3_base + OFFS(GPMC_CONFIG6));
	writel(APB_GPMC_CONFIG7, gpmc_cs3_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);

	/* Configure GPMC registers for AXI bus (base = 0x10000000)*/
	writel(0, gpmc_cs4_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(AXI_GPMC_CONFIG1, gpmc_cs4_base + OFFS(GPMC_CONFIG1));
	writel(AXI_GPMC_CONFIG2, gpmc_cs4_base + OFFS(GPMC_CONFIG2));
	writel(AXI_GPMC_CONFIG3, gpmc_cs4_base + OFFS(GPMC_CONFIG3));
	writel(AXI_GPMC_CONFIG4, gpmc_cs4_base + OFFS(GPMC_CONFIG4));
	writel(AXI_GPMC_CONFIG5, gpmc_cs4_base + OFFS(GPMC_CONFIG5));
	writel(AXI_GPMC_CONFIG6, gpmc_cs4_base + OFFS(GPMC_CONFIG6));
	writel(AXI_GPMC_CONFIG7, gpmc_cs4_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);

	// --- Initialize ADDAC/TOUCH
	for(ii=0;ii<9;ii++)
	{
	  writel(regv[ii],addr[ii]);
	}

	/* On LED */
	writel(0x00000001, ledreg);
}

