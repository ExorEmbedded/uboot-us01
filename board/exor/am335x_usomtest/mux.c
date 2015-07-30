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


static struct module_pin_mux rgmii1_pin_mux[] = {
	{OFFSET(mii1_txen), MODE(2)},                                   /* RGMII1_TCTL */
	{OFFSET(mii1_rxdv), MODE(2) | RXACTIVE},                        /* RGMII1_RCTL */
	{OFFSET(mii1_txd3), MODE(2)},                                   /* RGMII1_TD3 */
	{OFFSET(mii1_txd2), MODE(2)},                                   /* RGMII1_TD2 */
	{OFFSET(mii1_txd1), MODE(2)},                                   /* RGMII1_TD1 */
	{OFFSET(mii1_txd0), MODE(2)},                                   /* RGMII1_TD0 */
	{OFFSET(mii1_txclk), MODE(2)},                                  /* RGMII1_TCLK */
	{OFFSET(mii1_rxclk), MODE(2) | RXACTIVE},                       /* RGMII1_RCLK */
	{OFFSET(mii1_rxd3), MODE(2) | RXACTIVE},                        /* RGMII1_RD3 */
	{OFFSET(mii1_rxd2), MODE(2) | RXACTIVE},                        /* RGMII1_RD2 */
	{OFFSET(mii1_rxd1), MODE(2) | RXACTIVE},                        /* RGMII1_RD1 */
	{OFFSET(mii1_rxd0), MODE(2) | RXACTIVE},                        /* RGMII1_RD0 */
	{OFFSET(mdio_data), MODE(0) | RXACTIVE | PULLUP_EN},            /* MDIO_DATA */
	{OFFSET(mdio_clk), MODE(0) | PULLUP_EN},                        /* MDIO_CLK */
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
  {OFFSET(spi0_sclk)        , MODE(7)}, /* gpio 0_2 */
  {OFFSET(spi0_cs0)         , MODE(7)}, /* gpio 0_5 */
  {OFFSET(spi0_cs1)         , MODE(7)}, /* gpio 0_6 */
  {OFFSET(spi0_d0)          , MODE(7)}, /* gpio 0_3 */
  {OFFSET(spi0_d1)          , MODE(7)}, /* gpio 0_4 */
  {OFFSET(gpmc_ad4)         , MODE(7)}, /* gpio 1_4 */
  {OFFSET(xdma_event_intr0) , MODE(7)}, /* gpio 0_19 */

  {OFFSET(gpmc_csn0)        , MODE(7)}, /* gpio 1_29 */
  {OFFSET(ecap0_in_pwm0_out), MODE(7)}, /* gpio 0_7 */
  {OFFSET(gpmc_csn3)        , MODE(7)}, /* gpio 2_0 */
  {OFFSET(lcd_ac_bias_en)   , MODE(7)}, /* gpio 2_25 */
  {OFFSET(lcd_hsync)        , MODE(7)}, /* gpio 2_23 */
  {OFFSET(lcd_vsync)        , MODE(7)}, /* gpio 2_22 */
  {OFFSET(lcd_pclk)         , MODE(7)}, /* gpio 2_24 */
  {OFFSET(gpmc_ad15)        , MODE(7)}, /* gpio 1_15 */
  {OFFSET(gpmc_ad14)        , MODE(7)}, /* gpio 1_14 */
  {OFFSET(gpmc_ad13)        , MODE(7)}, /* gpio 1_13 */
  {OFFSET(gpmc_ad12)        , MODE(7)}, /* gpio 1_12 */
  {OFFSET(gpmc_ad11)        , MODE(7)}, /* gpio 0_27 */
  {OFFSET(gpmc_ad10)        , MODE(7)}, /* gpio 0_26 */
  {OFFSET(gpmc_ad9)         , MODE(7)}, /* gpio 0_23 */
  {OFFSET(gpmc_ad8)         , MODE(7)}, /* gpio 0_22 */
  {OFFSET(lcd_data0)        , MODE(7)}, /* gpio 2_6 */
  {OFFSET(lcd_data1)        , MODE(7)}, /* gpio 2_7 */
  {OFFSET(lcd_data2)        , MODE(7)}, /* gpio 2_8 */
  {OFFSET(lcd_data3)        , MODE(7)}, /* gpio 2_9 */
  {OFFSET(lcd_data4)        , MODE(7)}, /* gpio 2_10 */
  {OFFSET(lcd_data5)        , MODE(7)}, /* gpio 2_11 */
  {OFFSET(lcd_data6)        , MODE(7)}, /* gpio 2_12 */
  {OFFSET(lcd_data7)        , MODE(7)}, /* gpio 2_13 */
  {OFFSET(lcd_data8)        , MODE(7)}, /* gpio 2_14 */
  {OFFSET(lcd_data9)        , MODE(7)}, /* gpio 2_15 */
  {OFFSET(lcd_data10)       , MODE(7)}, /* gpio 2_16 */
  {OFFSET(lcd_data11)       , MODE(7)}, /* gpio 2_17 */
  {OFFSET(lcd_data12)       , MODE(7)}, /* gpio 0_8  */
  {OFFSET(lcd_data13)       , MODE(7)}, /* gpio 0_9  */
  {OFFSET(lcd_data14)       , MODE(7)}, /* gpio 0_10  */
  {OFFSET(lcd_data15)       , MODE(7)}, /* gpio 0_11  */
  
  {OFFSET(uart0_ctsn)       , MODE(7)}, /* gpio 1_8  */
  {OFFSET(uart0_rtsn)       , MODE(7)}, /* gpio 1_9  */
  {OFFSET(mcasp0_fsr)       , MODE(7)}, /* gpio 3_19 */
  {OFFSET(mcasp0_aclkx)     , MODE(7)}, /* gpio 3_18 */
  {OFFSET(mii1_rxclk)       , MODE(7)}, /* gpio 3_10 */
  {OFFSET(uart1_rxd)        , MODE(7)}, /* gpio 0_14 */
  {OFFSET(uart1_ctsn)       , MODE(7)}, /* gpio 0_12 */
  {OFFSET(uart1_rtsn)       , MODE(7)}, /* gpio 0_13 */
  {OFFSET(uart1_txd)        , MODE(7)}, /* gpio 0_15 */
  {OFFSET(mii1_txd2)        , MODE(7)}, /* gpio 0_17 */
  {OFFSET(mii1_txd3)        , MODE(7)}, /* gpio 0_16 */
  {OFFSET(mii1_rxd2)        , MODE(7)}, /* gpio 2_19 */
  {OFFSET(mii1_rxd3)        , MODE(7)}, /* gpio 2_18 */
  {OFFSET(gpmc_a11)         , MODE(7)}, /* gpio 1_27 */
  {OFFSET(gpmc_a10)         , MODE(7)}, /* gpio 1_26 */
  {OFFSET(gpmc_a9)          , MODE(7)}, /* gpio 1_25 */
  {OFFSET(gpmc_a8)          , MODE(7)}, /* gpio 1_24 */
  {OFFSET(mii1_col)         , MODE(7)}, /* gpio 3_0  */
  
  {OFFSET(gpmc_wen)         , MODE(7)}, /* gpio 2_4  */
  {OFFSET(emu0)             , MODE(7)}, /* gpio 3_7  */
  {OFFSET(gpmc_clk)         , MODE(7)}, /* gpio 2_1  */
  {OFFSET(emu1)             , MODE(7)}, /* gpio 3_8  */
  {OFFSET(gpmc_advn_ale)    , MODE(7)}, /* gpio 2_2  */
  {OFFSET(gpmc_be0n_cle)    , MODE(7)}, /* gpio 2_5  */
  {OFFSET(mcasp0_ahclkx)    , MODE(7)}, /* gpio 3_21 */
  {OFFSET(mcasp0_axr0)      , MODE(7)}, /* gpio 3_16 */
  {OFFSET(mcasp0_fsx)       , MODE(7)}, /* gpio 3_15 */
  {OFFSET(mcasp0_ahclkr)    , MODE(7)}, /* gpio 3_17 */
  {OFFSET(mcasp0_ahclkx)    , MODE(7)}, /* gpio 3_14 */
  {OFFSET(gpmc_be1n)        , MODE(7)}, /* gpio 1_28  */
  {OFFSET(gpmc_a0)          , MODE(7)}, /* gpio 1_16  */
  {OFFSET(gpmc_wpn)         , MODE(7)}, /* gpio 0_31  */
  {OFFSET(gpmc_a2)          , MODE(7)}, /* gpio 1_18  */
  {OFFSET(gpmc_a3)          , MODE(7)}, /* gpio 1_19  */
  {OFFSET(gpmc_a4)          , MODE(7)}, /* gpio 1_20  */
  {OFFSET(gpmc_a5)          , MODE(7)}, /* gpio 1_21  */
  {OFFSET(gpmc_wait0)       , MODE(7)}, /* gpio 0_30  */
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
}
