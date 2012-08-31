/*
 * un31p.c - board file for UN31 AM3517 family of devices.
 *
 * Author: Giovanni Pavoni
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

#include "un31p.h"

#define AM3517_IP_SW_RESET	0x48002598
#define CPGMACSS_SW_RST		(1 << 1)

/*
 * This routine starts the PWM backlight dimming gptimer in such a way a 100% duty cycle is achieved; this to make things independent from pwm frequency
 * required by the specific backlight controller. Hardcoded values are used, no need to configure anything, since a 100% duty is used.
 */
static void StartPWMBacklight(void)
{
  static struct gptimer *timer_base = (struct gptimer *)OMAP34XX_GPT9;
  
  writel(0xfffff800, &timer_base->tldr);
  writel(0xfffffff8, &timer_base->tmar);
  writel(0x00000215, &timer_base->tiocp_cfg);
  writel(0x00000000, &timer_base->tisr);
  writel(0x00000000, &timer_base->tier);
  writel(0x00000000, &timer_base->twer);
  writel(0x00001843, &timer_base->tclr);
  writel(0xfffff800, &timer_base->tcrr);
}

/*
 * This routine configures the AM35xx MPU clock frequency at the optimal/nominal value of 600Mhz
 * Such a function is required for Linux OS, since Linux is not actually capable of reconfiguring the AM35xx MPU
 * clock frequency (while Wince6 is).
 */
static void configure_optimal_mpu_clock(void)
{
  #define AM35XX_CM_CLKSEL1_PLL_MPU_ADDR	0x48004940
  #define AM35XX_CM_CLKEN_PLL_MPU_ADDR		0x48004904
  #define AM35XX_CM_IDLEST_PLL_ADDR		0x48004924
  
  #define UN31_PLL_CFG_VAL			0x0012580C
  #define UN31_PLL_UNLOCK_VAL			0x00000035
  #define UN31_PLL_LOCK_VAL			0x00000037
  
  /* Unlock the PLL and go to bypass mode */
  writel(UN31_PLL_UNLOCK_VAL,AM35XX_CM_CLKEN_PLL_MPU_ADDR);
  
  /* Wait for PLL to be unlocked */
  while(1 == (0x01 & readl(AM35XX_CM_IDLEST_PLL_ADDR)));
  
  /* Configure the m:n PLL ratio */
  writel(UN31_PLL_CFG_VAL,AM35XX_CM_CLKSEL1_PLL_MPU_ADDR);
  
  /* Lock the PLL and go to bypass mode */
  writel(UN31_PLL_LOCK_VAL,AM35XX_CM_CLKEN_PLL_MPU_ADDR);
  
  /* Wait for PLL to be locked */
  while(0 == (0x01 & readl(AM35XX_CM_IDLEST_PLL_ADDR)));
}


/*
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
	DECLARE_GLOBAL_DATA_PTR;

	/* board id for Linux */
	gd->bd->bi_arch_number = 2777; /* This is the board id which is assumed for UN31 boards */
	/* boot param addr */
	gd->bd->bi_boot_params = (OMAP34XX_SDRC_CS0 + 0x100);

	return 0;
}


/******************************************************************************
 * Routine: lcd_pwron
 * Description: power on the LCD circuitry
 *****************************************************************************/
int lcd_pwron(void)
{
	omap_request_gpio(6);
	omap_set_gpio_direction(6,0);
	omap_set_gpio_dataout(6,1);
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
	int ret = 0;
	int i;
	
	/* Start with LCD power ctl pins LOW */
	omap_request_gpio(6);
	omap_set_gpio_direction(6,0);
	omap_set_gpio_dataout(6,0);
	
	omap_request_gpio(7);
	omap_set_gpio_direction(7,0);
	omap_set_gpio_dataout(7,0);

	omap_request_gpio(55);
	omap_set_gpio_direction(55,0);
	omap_set_gpio_dataout(55,0);

	/* Enable +5VCC and 3V3s on carrier board, battery check is low */
	omap_request_gpio(159);
	omap_set_gpio_direction(159,0);
	omap_set_gpio_dataout(159,1);

	omap_request_gpio(157);
	omap_set_gpio_direction(157,0);
	omap_set_gpio_dataout(157,1);

	omap_request_gpio(156);
	omap_set_gpio_direction(156,0);
	omap_set_gpio_dataout(156,0);

#ifdef CONFIG_DRIVER_OMAP34XX_I2C
	i2c_init(CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE);
#ifdef CONFIG_CMD_I2CHWCFG
	ret = i2cgethwcfg ();
	if(ret)
	  ret = i2cgethwcfg (); /* Added one retry ... just in case (never experienced the need for that */
#endif
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
	dieid_num_r();
	
#ifdef CONFIG_VIDEO_OMAP3
	omap3_dss_panel_getenv(&display_cfg);
	omap3_dss_panel_config(&display_cfg);
	omap3_dss_set_background_col(BACKGROUND_COLOR);
	omap3_dss_panel_splash(&display_cfg);
	lcd_pwron();
	omap3_dss_enable();
	/* Blight on */
	for(i=0;i<200;i++)udelay(1000);
	omap_request_gpio(7);
	omap_set_gpio_direction(7,0);
	omap_set_gpio_dataout(7,1);
	omap_request_gpio(55);
	//!!!omap_set_gpio_direction(55,0);
	//!!!omap_set_gpio_dataout(55,1);
	StartPWMBacklight();
#endif
	
	configure_optimal_mpu_clock();
	return ret;
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
	MUX_UN31P();
}


/******************************************************************************
 * Routine: setup_ambabridge
 * Description: Setting up the configuration GPMC registers for the board
 * NOTE: Even if this platform does not provide FPGA/AMBA, this function is
 *       required anymore for setting up the GPMC bus.
 *****************************************************************************/
void setup_ambabridge(void)
{
	unsigned int *gpmc_cs0_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(0 * GPMC_CONFIG_WIDTH));
	unsigned int *gpmc_cs1_base = (unsigned int *)(GPMC_CONFIG_CS0_BASE +(1 * GPMC_CONFIG_WIDTH));
	
	/* Configure GPMC registers for NORFlash (base = 0x08000000)*/
	/* Muxmode, asynch, single access, 16 bit , CS0             */
	writel(0, gpmc_cs0_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(BSP_GPMC_NOR_CONFIG1, gpmc_cs0_base + OFFS(GPMC_CONFIG1));
	writel(BSP_GPMC_NOR_CONFIG2, gpmc_cs0_base + OFFS(GPMC_CONFIG2));
	writel(BSP_GPMC_NOR_CONFIG3, gpmc_cs0_base + OFFS(GPMC_CONFIG3));
	writel(BSP_GPMC_NOR_CONFIG4, gpmc_cs0_base + OFFS(GPMC_CONFIG4));
	writel(BSP_GPMC_NOR_CONFIG5, gpmc_cs0_base + OFFS(GPMC_CONFIG5));
	writel(BSP_GPMC_NOR_CONFIG6, gpmc_cs0_base + OFFS(GPMC_CONFIG6));
	writel(BSP_GPMC_NOR_CONFIG7, gpmc_cs0_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);

	/* Configure GPMC registers for NAND memory (base = 0x30000000)*/
	/* CS1														   */
	writel(0, gpmc_cs1_base + OFFS(GPMC_CONFIG7));
	sdelay(1000);

	writel(BSP_GPMC_NAND_CONFIG1, gpmc_cs1_base + OFFS(GPMC_CONFIG1));
	writel(BSP_GPMC_NAND_CONFIG2, gpmc_cs1_base + OFFS(GPMC_CONFIG2));
	writel(BSP_GPMC_NAND_CONFIG3, gpmc_cs1_base + OFFS(GPMC_CONFIG3));
	writel(BSP_GPMC_NAND_CONFIG4, gpmc_cs1_base + OFFS(GPMC_CONFIG4));
	writel(BSP_GPMC_NAND_CONFIG5, gpmc_cs1_base + OFFS(GPMC_CONFIG5));
	writel(BSP_GPMC_NAND_CONFIG6, gpmc_cs1_base + OFFS(GPMC_CONFIG6));
	writel(BSP_GPMC_NAND_CONFIG7, gpmc_cs1_base + OFFS(GPMC_CONFIG7));
	sdelay(2000);
}


/******************************************************************************
 * Routine: Enable_RS232PHY
 * Description: Enables the programmable serial PHY in RS232 mode, TX enabled
 *****************************************************************************/
void Enable_RS232PHY(void)
{
  if (!omap_request_gpio(63)) 		/* MODE RS232 */
	{
		omap_set_gpio_direction(63, 0);
		omap_set_gpio_dataout(63, 0);
	}

  if (!omap_request_gpio(185)) 		/* MODE RS232 for ETOP507 */
	{
		omap_set_gpio_direction(185, 0);
		omap_set_gpio_dataout(185, 0);
	}

  if (!omap_request_gpio(148)) 		/* RXEN enabled */
	{
		omap_set_gpio_direction(148, 0);
		omap_set_gpio_dataout(148, 1);
	}

  if (!omap_request_gpio(170)) 		/* RXEN enabled for ETOP507 */
	{
		omap_set_gpio_direction(170, 0);
		omap_set_gpio_dataout(170, 1);
	}

  if (!omap_request_gpio(149)) 		/* DXEN enabled */
	{
		omap_set_gpio_direction(149, 0);
		omap_set_gpio_dataout(149, 1);
	}

if (!omap_request_gpio(184)) 		/* DXEN enabled for ETOP507*/
	{
		omap_set_gpio_direction(184, 0);
		omap_set_gpio_dataout(184, 1);
	}

}

//if it-s a Carel board it returns 1 else if it a sitek board it returns 0
unsigned char check_sicar()
{
 int val;

 	MUX_VAL(CP(UART1_RTS),(IEN  | DIS | M4)) 
	//Understanding method to known board type (PGD5 or PGD7)
	//is valid only for Carel board so it cannot be used
	//by a function that try to understand if board is Carel or
	//Sitek. 
	//So, for now, this function will return always Carel bord type.
	//Sitek UBoot have not CONFIG_UN31P_CAREL defined so this function
	//is never called
	return 1;
	//Understand if it is a PGD5 or PGD7 board
	if(!omap_request_gpio(4))
	{
		omap_set_gpio_direction(4, 1);
	        val = omap_get_gpio_datain(4);
	}
	if(val==0)
	{

		//PGD5
		if(!omap_request_gpio(149))
		{
			omap_set_gpio_direction(149, 1);
			val = omap_get_gpio_datain(149);
		}
	}
	else
	{
		if(!omap_request_gpio(184))
		{
			omap_set_gpio_direction(184, 1);
			val = omap_get_gpio_datain(184);
		}
	}
	if (val == 1) 
		return 1;
	else
		{
 			MUX_VAL(CP(UART1_RTS),(IDIS | PTD | EN  | M4))
			return 0;
		}

}

void enable_voltagesun31()
{
	/* Enable +5VCC and 3V3s on carrier board, battery check is low */
	omap_request_gpio(159);
	omap_set_gpio_direction(159,0);
	omap_set_gpio_dataout(159,1);

	omap_request_gpio(157);
	omap_set_gpio_direction(157,0);
	omap_set_gpio_dataout(157,1);

	omap_request_gpio(156);
	omap_set_gpio_direction(156,0);
	omap_set_gpio_dataout(156,0);
}
