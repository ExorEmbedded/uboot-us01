/*
 * mux.c
 *
 * Copyright (C) 2014 Exor S.p.a.
 *
 * Based on:
 * u-boot:/board/ti/am335x/mux.c
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/hardware.h>
#include <asm/arch/mux.h>
#include <asm/io.h>
#include <i2c.h>
#include "board.h"

static struct module_pin_mux uart0_pin_mux[] = {
	{OFFSET(uart0_rxd), (MODE(0) | PULLUP_EN | RXACTIVE)},  /* UART0_RXD */
	{OFFSET(uart0_txd), (MODE(0) | PULLUDEN)},              /* UART0_TXD */
	{-1},
};

static struct module_pin_mux uart1_pin_mux[] = {
	{OFFSET(uart1_rxd), (MODE(0) | PULLUP_EN | RXACTIVE)},  /* UART1_RXD */
	{OFFSET(uart1_txd), (MODE(0) | PULLUDEN)},              /* UART1_TXD */
	{-1},
};

static struct module_pin_mux mmc0_pin_mux[] = {
	{OFFSET(mmc0_dat3), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT3 */
	{OFFSET(mmc0_dat2), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT2 */
	{OFFSET(mmc0_dat1), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT1 */
	{OFFSET(mmc0_dat0), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_DAT0 */
	{OFFSET(mmc0_clk), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CLK */
	{OFFSET(mmc0_cmd), (MODE(0) | RXACTIVE | PULLUP_EN)},	/* MMC0_CMD */
	{-1},
};

static struct module_pin_mux mmc1_pin_mux[] = {
	{OFFSET(gpmc_ad3), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT3 */
	{OFFSET(gpmc_ad2), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT2 */
	{OFFSET(gpmc_ad1), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT1 */
	{OFFSET(gpmc_ad0), (MODE(1) | RXACTIVE | PULLUP_EN)},	/* MMC1_DAT0 */
	{OFFSET(gpmc_csn1), (MODE(2) | RXACTIVE | PULLUP_EN)},	/* MMC1_CLK */
	{OFFSET(gpmc_csn2), (MODE(2) | RXACTIVE | PULLUP_EN)},	/* MMC1_CMD */
	{-1},
};

static struct module_pin_mux i2c0_pin_mux[] = {
	{OFFSET(i2c0_sda), (MODE(0) | RXACTIVE | PULLUDEN | SLEWCTRL)}, /* I2C_DATA */
	{OFFSET(i2c0_scl), (MODE(0) | RXACTIVE | PULLUDEN | SLEWCTRL)}, /* I2C_SCLK */
	{-1},
};


static struct module_pin_mux mii1_pin_mux[] = {
	{OFFSET(mii1_rxerr), MODE(0) | RXACTIVE},                       /* MII1_RXERR */
	{OFFSET(mii1_txen), MODE(0)},                                   /* MII1_TXEN */
	{OFFSET(mii1_rxdv), MODE(0) | RXACTIVE},                        /* MII1_RXDV */
	{OFFSET(mii1_txd3), MODE(0)},                                   /* MII1_TXD3 */
	{OFFSET(mii1_txd2), MODE(0)},                                   /* MII1_TXD2 */
	{OFFSET(mii1_txd1), MODE(0)},                                   /* MII1_TXD1 */
	{OFFSET(mii1_txd0), MODE(0)},                                   /* MII1_TXD0 */
	{OFFSET(mii1_txclk), MODE(0) | RXACTIVE},                       /* MII1_TXCLK */
	{OFFSET(mii1_rxclk), MODE(0) | RXACTIVE},                       /* MII1_RXCLK */
	{OFFSET(mii1_rxd3), MODE(0) | RXACTIVE},                        /* MII1_RXD3 */
	{OFFSET(mii1_rxd2), MODE(0) | RXACTIVE},                        /* MII1_RXD2 */
	{OFFSET(mii1_rxd1), MODE(0) | RXACTIVE},                        /* MII1_RXD1 */
	{OFFSET(mii1_rxd0), MODE(0) | RXACTIVE},                        /* MII1_RXD0 */
	{OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN},            /* MDIO_DATA */
	{OFFSET(mdio_clk), MODE(0) | PULLUP_EN},                        /* MDIO_CLK */
	{-1},
};

static struct module_pin_mux rmii1_pin_mux[] = {
  {OFFSET(mii1_crs), MODE(1) | RXACTIVE},	
  {OFFSET(rmii1_refclk), MODE(0) | RXACTIVE},  
  {OFFSET(mii1_rxd1), MODE(1) | RXACTIVE},	
  {OFFSET(mii1_rxd0), MODE(1) | RXACTIVE},	
  {OFFSET(mii1_rxerr), MODE(1) | RXACTIVE},	
  {OFFSET(mii1_txd1), MODE(1)},			
  {OFFSET(mii1_txd0), MODE(1)},			
  {OFFSET(mii1_txen), MODE(1)},			
  {OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN},              /* MDIO_DATA */
  {OFFSET(mdio_clk), MODE(0) | PULLUP_EN},                          /* MDIO_CLK */
  {-1},
};

static struct module_pin_mux gpio_pin_mux[] = {
  /*UART control pins */
  {OFFSET(gpmc_ad5), MODE(7)}, /* RXEN0 gpio1_5 */
  {OFFSET(gpmc_ad6), MODE(7)}, /* DXEN0 gpio1_6 */
  {OFFSET(gpmc_ad7), MODE(7)}, /* MODE0 gpio1_7 */
  /* Power enable control pins */
  {OFFSET(gpmc_clk), MODE(7)}, /* EN_STDBY gpio2_1 */
  {OFFSET(emu0), MODE(7)}, /* EN_3W3_SW gpio3_7 */
  {-1},
};

static struct module_pin_mux rmii2_pin_mux[] = {
  {OFFSET(gpmc_wait0), MODE(3) | RXACTIVE},	//mii2_crs
  {OFFSET(mii1_col), MODE(1) | RXACTIVE},       //rmii2_refclk  
  {OFFSET(gpmc_a10), MODE(3) | RXACTIVE},	//mii2_rxd1
  {OFFSET(gpmc_a11), MODE(3) | RXACTIVE},	//mii2_rxd0
  {OFFSET(gpmc_wpn), MODE(3) | RXACTIVE},	//mii2_rxerr
  {OFFSET(gpmc_a5), MODE(3)},	                //mii2_txd1		
  {OFFSET(gpmc_a4), MODE(3)},	                //mii2_txd0		
  {OFFSET(gpmc_a0), MODE(3)},	                //mii2_txen		
  {-1},
};

void enable_uart0_pin_mux(void)
{
  configure_module_pin_mux(uart0_pin_mux);
}

void enable_uart1_pin_mux(void)
{
  configure_module_pin_mux(uart1_pin_mux);
}

void enable_i2c0_pin_mux(void)
{
  configure_module_pin_mux(i2c0_pin_mux);
}


void enable_board_pin_mux(void)
{
	configure_module_pin_mux(rmii1_pin_mux);
	configure_module_pin_mux(mmc0_pin_mux);
	configure_module_pin_mux(mmc1_pin_mux);
	configure_module_pin_mux(gpio_pin_mux);
}

void enable_rmii2_pin_mux(void)
{
  configure_module_pin_mux(rmii2_pin_mux);
}

static struct module_pin_mux lcdc_pin_mux[] = {
	{OFFSET(lcd_data0), (MODE(0) | PULLUDDIS)},	/* LCD_DAT0 */
	{OFFSET(lcd_data1), (MODE(0) | PULLUDDIS)},	/* LCD_DAT1 */
	{OFFSET(lcd_data2), (MODE(0) | PULLUDDIS)},	/* LCD_DAT2 */
	{OFFSET(lcd_data3), (MODE(0) | PULLUDDIS)},	/* LCD_DAT3 */
	{OFFSET(lcd_data4), (MODE(0) | PULLUDDIS)},	/* LCD_DAT4 */
	{OFFSET(lcd_data5), (MODE(0) | PULLUDDIS)},	/* LCD_DAT5 */
	{OFFSET(lcd_data6), (MODE(0) | PULLUDDIS)},	/* LCD_DAT6 */
	{OFFSET(lcd_data7), (MODE(0) | PULLUDDIS)},	/* LCD_DAT7 */
	{OFFSET(lcd_data8), (MODE(0) | PULLUDDIS)},	/* LCD_DAT8 */
	{OFFSET(lcd_data9), (MODE(0) | PULLUDDIS)},	/* LCD_DAT9 */
	{OFFSET(lcd_data10), (MODE(0) | PULLUDDIS)},	/* LCD_DAT10 */
	{OFFSET(lcd_data11), (MODE(0) | PULLUDDIS)},	/* LCD_DAT11 */
	{OFFSET(lcd_data12), (MODE(0) | PULLUDDIS)},	/* LCD_DAT12 */
	{OFFSET(lcd_data13), (MODE(0) | PULLUDDIS)},	/* LCD_DAT13 */
	{OFFSET(lcd_data14), (MODE(0) | PULLUDDIS)},	/* LCD_DAT14 */
	{OFFSET(lcd_data15), (MODE(0) | PULLUDDIS)},	/* LCD_DAT15 */
	{OFFSET(lcd_vsync), (MODE(0))},		/* LCD_VSYNC */
	{OFFSET(lcd_hsync), (MODE(0))},		/* LCD_HSYNC */
	{OFFSET(lcd_pclk), (MODE(0))},		/* LCD_PCLK */
	{OFFSET(lcd_ac_bias_en), (MODE(0))},	/* LCD_AC_BIAS_EN */
	{OFFSET(gpmc_csn0), MODE(7)},	                //gpio1_29 en_vdd	
	{OFFSET(gpmc_csn3), MODE(7)},	                //gpio2_0 en_bl	
	{OFFSET(ecap0_in_pwm0_out), MODE(7)},	        //gpio0_7 dimm	
	{OFFSET(gpmc_be0n_cle), MODE(7)},	        //gpio2_5 FAULT
	{OFFSET(gpmc_advn_ale), MODE(7)},	        //gpio2_2 DL
	{-1},
};

void enable_lcdc_pin_mux(void)
{
  configure_module_pin_mux(lcdc_pin_mux);
}
