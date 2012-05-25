/*
 * am3517evm.c - board file for TI's AM3517 family of devices.
 *
 * Author: Vaibhav Hiremath <hvaibhav@ti.com>
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

#include <common.h>
#include <asm/io.h>
#include <asm/arch/mem.h>
#include <asm/arch/mux.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/emac_defs.h>
#include <i2c.h>
#include <asm/mach-types.h>
#include <asm/arch/gpio.h>
#include <asm/mach-types.h>
#include "un31f.h"


#define AM3517_IP_SW_RESET	0x48002598
#define CPGMACSS_SW_RST		(1 << 1)

/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	/* board id for Linux */
	gd->bd->bi_arch_number = MACH_TYPE_OMAP3517EVM;
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}

/*
 * Routine: misc_init_r
 * Description: Init ethernet (done here so udelay works)
 */
int misc_init_r(void)
{
	unsigned char byte;
	volatile unsigned int ctr;
	u32 reset;

#ifdef CONFIG_DRIVER_OMAP34XX_I2C
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
#endif

#if defined(CONFIG_DRIVER_TI_EMAC)

	omap_request_gpio(30);
	omap_set_gpio_direction(30,0);
	omap_set_gpio_dataout(30,0);
	ctr  = 0;
	do{
		udelay(1000);
		ctr++;
		}while (ctr <300);
	omap_set_gpio_dataout(30,1);
	ctr =0;
	/* allow the PHY to stabilize and settle down */
	do{
		udelay(1000);
		ctr++;
		}while (ctr <300);

	/*ensure that the module is out of reset*/
	reset = readl(AM3517_IP_SW_RESET);
	reset &= (~CPGMACSS_SW_RST);
	writel(reset,AM3517_IP_SW_RESET);

#endif

#if defined(CONFIG_CMD_I2C)
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
	/* Set i2C exapander u20 for HECC signal CAN_STB to low */
	byte = 0xFF;
	i2c_read(0x21, 0, 0, &byte, 1);
	printf("HECC U20: port before = %08X\n", byte);
	byte = 0xBF;
	i2c_write(0x21, 6, 0, &byte, 1);
	byte = 0x0;
	i2c_write(0x21, 2, 0, &byte, 1);
	printf("HECC U20: programmed CAN_STB low\n");
	byte = 0xFF;
	i2c_read(0x21, 0, 0, &byte, 1);
	printf("HECC U20: port after = %08X\n", byte);
#endif
	dieid_num_r();

	return 0;
}

/*
 * Initializes on-chip ethernet controllers.
 * to override, implement board_eth_init()
 */
int cpu_eth_init(bd_t *bis)
{
#if defined(CONFIG_DRIVER_TI_EMAC)
	printf("davinci_emac_initialize\n");
	davinci_emac_initialize();
#endif
	return 0;
}


/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
	MUX_UN31F();
}


/******************************************************************************
 * Routine: setup_ambabridge
 * Description: Setting up the configuration GPMC registers specific to the
 *		AMBA bridge. And init the peripherals.
 *****************************************************************************/
void setup_ambabridge(void)
{
	unsigned int *gpmc_cs1_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(1 * GPMC_CONFIG_WIDTH));
	unsigned int *gpmc_cs2_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(2 * GPMC_CONFIG_WIDTH));
	unsigned int *ledreg =(unsigned int*)ADDR_LEDREG;

	/* Configure GPMC registers for APB bus (base = 0x1a000000)*/
	writel(0, gpmc_cs1_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(APB_GPMC_CONFIG1, gpmc_cs1_base + OFFS(GPMC_CONFIG1));
	writel(APB_GPMC_CONFIG2, gpmc_cs1_base + OFFS(GPMC_CONFIG2));
	writel(APB_GPMC_CONFIG3, gpmc_cs1_base + OFFS(GPMC_CONFIG3));
	writel(APB_GPMC_CONFIG4, gpmc_cs1_base + OFFS(GPMC_CONFIG4));
	writel(APB_GPMC_CONFIG5, gpmc_cs1_base + OFFS(GPMC_CONFIG5));
	writel(APB_GPMC_CONFIG6, gpmc_cs1_base + OFFS(GPMC_CONFIG6));
	writel(APB_GPMC_CONFIG7, gpmc_cs1_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);

	/* Configure GPMC registers for AXI bus (base = 0x10000000)*/
	writel(0, gpmc_cs2_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(AXI_GPMC_CONFIG1, gpmc_cs2_base + OFFS(GPMC_CONFIG1));
	writel(AXI_GPMC_CONFIG2, gpmc_cs2_base + OFFS(GPMC_CONFIG2));
	writel(AXI_GPMC_CONFIG3, gpmc_cs2_base + OFFS(GPMC_CONFIG3));
	writel(AXI_GPMC_CONFIG4, gpmc_cs2_base + OFFS(GPMC_CONFIG4));
	writel(AXI_GPMC_CONFIG5, gpmc_cs2_base + OFFS(GPMC_CONFIG5));
	writel(AXI_GPMC_CONFIG6, gpmc_cs2_base + OFFS(GPMC_CONFIG6));
	writel(AXI_GPMC_CONFIG7, gpmc_cs2_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);

	/* On LED */
	writel(0x00000001, ledreg);
}


/******************************************************************************
 * Routine: Enable_RS232PHY
 * Description: Enables the programmable serial PHY in RS232 mode, TX enabled
 *****************************************************************************/
void Enable_RS232PHY(void)
{
  if (!omap_request_gpio(95)) 		/* MODE RS232 */
	{
		omap_set_gpio_direction(95, 0);
		omap_set_gpio_dataout(95, 0);
	}

  if (!omap_request_gpio(76)) 		/* RXEN enabled */
	{
		omap_set_gpio_direction(76, 0);
		omap_set_gpio_dataout(76, 1);
	}

  if (!omap_request_gpio(71)) 		/* DXEN enabled */
	{
		omap_set_gpio_direction(71, 0);
		omap_set_gpio_dataout(71, 1);
	}
}

