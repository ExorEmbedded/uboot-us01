/*
 * board.c
 *
 * Board functions for uS01 AM335X based boards: Test ECO platforms
 *
 * Copyright (C) 2018 Exor S.p.a.
 *
 * Based on:
 * u-boot:/board/ti/am335x/board.c
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <spl.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/omap.h>
#include <asm/arch/ddr_defs.h>
#include <asm/arch/clock.h>
#include <asm/arch/gpio.h>
#include <asm/arch/mmc_host_def.h>
#include <asm/arch/sys_proto.h>
#include <asm/arch/mem.h>
#include <asm/io.h>
#include <asm/emif.h>
#include <asm/gpio.h>
#include <i2c.h>
#include <miiphy.h>
#include <cpsw.h>
#include <power/tps65217.h>
#include <power/tps65910.h>
#include <environment.h>
#include <watchdog.h>
#include "board.h"

DECLARE_GLOBAL_DATA_PTR;

void ena_rs232phy(void);
static struct ctrl_dev *cdev = (struct ctrl_dev *)CTRL_DEVICE_BASE;
static struct cpsw_slave_data cpsw_slaves[];

  #define SPLASH_HDRLEN  		56
  #define SPLASH_STRIDE_IDX		0
  #define SPLASH_SIZE_IDX		2

  #define EN_VDD_GPIO (61)
  #define EN_BL_GPIO (64)
  #define DIMM_GPIO (7)
  #define FAULT_GPIO (69)
  #define DL_GPIO (66) 
  #define USB0DRVVBUS_GPIO (18) 
  #define USB1DRVVBUS_GPIO (109) 
  

#if defined(CONFIG_SPL_BUILD)
static const struct ddr_data ddr3_data = {
  .datardsratio0 = MT41K256M16HA125E_RD_DQS,
  .datawdsratio0 = MT41K256M16HA125E_WR_DQS,
  .datafwsratio0 = MT41K256M16HA125E_PHY_FIFO_WE,
  .datawrsratio0 = MT41K256M16HA125E_PHY_WR_DATA,
};

static const struct cmd_control ddr3_cmd_ctrl_data = {
  .cmd0csratio = MT41K256M16HA125E_RATIO,
  .cmd0iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

  .cmd1csratio = MT41K256M16HA125E_RATIO,
  .cmd1iclkout = MT41K256M16HA125E_INVERT_CLKOUT,

  .cmd2csratio = MT41K256M16HA125E_RATIO,
  .cmd2iclkout = MT41K256M16HA125E_INVERT_CLKOUT,
};

static struct emif_regs ddr3_emif_reg_data = {
  .sdram_config = MT41K256M16HA125E_EMIF_SDCFG,
  .ref_ctrl = MT41K256M16HA125E_EMIF_SDREF,
  .sdram_tim1 = MT41K256M16HA125E_EMIF_TIM1,
  .sdram_tim2 = MT41K256M16HA125E_EMIF_TIM2,
  .sdram_tim3 = MT41K256M16HA125E_EMIF_TIM3,
  .zq_config = MT41K256M16HA125E_ZQ_CFG,
  .emif_ddr_phy_ctlr_1 = MT41K256M16HA125E_EMIF_READ_LATENCY,
};


#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
  /* break into full u-boot on 'c' */
  return (serial_tstc() && serial_getc() == 'c');
}
#endif

#define OSC	(V_OSCK/1000000)
const struct dpll_params dpll_ddr = {400, OSC-1, 1, -1, -1, -1, -1};

void am33xx_spl_board_init(void)
{
  int mpu_vdd;
  int sil_rev;

  printf("am33xx_spl_board_init++\n");
	
  /* Get the frequency */
  dpll_mpu_opp100.m = am335x_get_efuse_mpu_max_freq(cdev);
  printf("dpll_mpu_opp100.m = %d\n",dpll_mpu_opp100.m);

  /*
  * The uS01 systems use a TPS65910 PMIC.  For all
  * MPU frequencies we support we use a CORE voltage of
  * 1.1375V.  For MPU voltage we need to switch based on
  * the frequency we are running at.
  */
  printf("Probing TPS65910...\n");

  if (i2c_probe(TPS65910_CTRL_I2C_ADDR))
    return;
  printf("Probing TPS65910...:OK\n");
	
  /*
  * Depending on MPU clock and PG we will need a different
  * VDD to drive at that speed.
  */
  sil_rev = readl(&cdev->deviceid) >> 28;
  mpu_vdd = am335x_get_tps65910_mpu_vdd(sil_rev, dpll_mpu_opp100.m);
  printf("mpu_vdd...%d\n",mpu_vdd);

  /* Tell the TPS65910 to use i2c */
  tps65910_set_i2c_control();
  
  /* First update MPU voltage. */
  if (tps65910_voltage_update(MPU, mpu_vdd))
    return;
  printf("mpu_vdd...SET OK\n");

  /* Second, update the CORE voltage. */
  if (tps65910_voltage_update(CORE, TPS65910_OP_REG_SEL_1_1_3))
    return;
  printf("CORE voltage...SET OK\n");

  /* Set CORE Frequencies to OPP100 */
  do_setup_dpll(&dpll_core_regs, &dpll_core_opp100);

  /* Set MPU Frequency to what we detected now that voltages are set */
  do_setup_dpll(&dpll_mpu_regs, &dpll_mpu_opp100);
  printf("am33xx_spl_board_init--\n");
}

const struct dpll_params *get_dpll_ddr_params(void)
{
  enable_i2c0_pin_mux();
  i2c_init(CONFIG_SYS_OMAP24_I2C_SPEED, CONFIG_SYS_OMAP24_I2C_SLAVE);

  return &dpll_ddr;
}

void set_uart_mux_conf(void)
{
#ifdef CONFIG_SERIAL1
  enable_uart0_pin_mux();
#endif /* CONFIG_SERIAL1 */
#ifdef CONFIG_SERIAL2
  enable_uart1_pin_mux();
#endif /* CONFIG_SERIAL2 */
}

void set_mux_conf_regs(void)
{
  enable_board_pin_mux();
  
  gpio_request(ESDBY_GPIO,"");
  gpio_direction_output(ESDBY_GPIO,1);
  
  gpio_request(E3W3S_GPIO,"");
  gpio_direction_output(E3W3S_GPIO,1);
}

const struct ctrl_ioregs ioregs_uS01 = {
  .cm0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
  .cm1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
  .cm2ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
  .dt0ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
  .dt1ioctl		= MT41K256M16HA125E_IOCTRL_VALUE,
};

void sdram_init(void)
{
  config_ddr(400, &ioregs_uS01,
    &ddr3_data,
    &ddr3_cmd_ctrl_data,
    &ddr3_emif_reg_data, 0);
}

/*
 * Init the mmc (mmc0) for the dedicated/custom SPL u-boot loading sequence. 
 */
int spl_board_mmc_initialize(void)
{
  return omap_mmc_init(0, 0, 0, -1, -1);
}

/*
 * Init the EMMC (mmc1) for the dedicated/custom SPL u-boot loading sequence. 
 */
int spl_board_emmc_initialize(void)
{
  return omap_mmc_init(1, 0, 0, -1, -1);
}
#endif

void ena_rs232phy(void)
{
  gpio_request(RXEN0_GPIO,"");
  gpio_direction_output(RXEN0_GPIO,1);
  
  gpio_request(DXEN0_GPIO,"");
  gpio_direction_output(DXEN0_GPIO,1);
  
  gpio_request(MODE0_GPIO,"");
  gpio_direction_output(MODE0_GPIO,0);
  
  udelay(1000);
}

/*
 * Basic board specific setup.  Pinmux has been handled already.
 */
int board_init(void)
{
  printf("board_init++\n");
#if defined(CONFIG_HW_WATCHDOG)
  hw_watchdog_init();
#endif
  gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
  printf("board_init--\n");
  return 0;
}

#ifdef CONFIG_BOARD_LATE_INIT
#define US01_256MB_300MHZ 0x02
int board_late_init(void)
{
  //Enable display power EN_VDD
  enable_lcdc_pin_mux();
  gpio_request(EN_VDD_GPIO,"");
  gpio_direction_output(EN_VDD_GPIO,1);

  //Enable the backlight here
  gpio_request(EN_BL_GPIO,"");
  gpio_direction_output(EN_BL_GPIO,1);
  gpio_request(DIMM_GPIO,"");
  gpio_direction_output(DIMM_GPIO,1);

  //Enable USB0 DRVVBUS
  gpio_request(USB0DRVVBUS_GPIO,"");
  gpio_direction_output(USB0DRVVBUS_GPIO,1);

  //Enable USB1 DRVVBUS
  gpio_request(USB1DRVVBUS_GPIO,"");
  gpio_direction_output(USB1DRVVBUS_GPIO,1);
  
  //Green LED ON, RED OFF
  gpio_request(FAULT_GPIO ,"");
  gpio_direction_output(FAULT_GPIO,0);
  gpio_request(DL_GPIO ,"");
  gpio_direction_output(DL_GPIO,1);
  
  setenv("bootcmd", "\0");
  ena_rs232phy();
  return 0;
}
#endif

#if (defined(CONFIG_DRIVER_TI_CPSW) && !defined(CONFIG_SPL_BUILD)) 
static void cpsw_control(int enabled)
{
  /* VTP can be added here */
  return;
}

static struct cpsw_slave_data cpsw_slaves[] = {
  {
    .slave_reg_ofs	= 0x208,
    .sliver_reg_ofs	= 0xd80,
    .phy_addr 		= 1,  //NOTE: This value will be set to 2 in case of etop5xx (we have a switch in this case)
  },
  {
  .slave_reg_ofs 	= 0x308,
  .sliver_reg_ofs	= 0xdc0,
  .phy_addr		= 0,
  },
};

static struct cpsw_platform_data cpsw_data = {
  .mdio_base		= CPSW_MDIO_BASE,
  .cpsw_base		= CPSW_BASE,
  .mdio_div		= 0xff,
  .channels		= 8,
  .cpdma_reg_ofs	= 0x800,
  .slaves		= 1,
  .slave_data		= cpsw_slaves,
  .ale_reg_ofs		= 0xd00,
  .ale_entries		= 1024,
  .host_port_reg_ofs	= 0x108,
  .hw_stats_reg_ofs	= 0x900,
  .bd_ram_ofs		= 0x2000,
  .mac_control		= (1 << 5),
  .control		= cpsw_control,
  .host_port_num	= 0,
  .version		= CPSW_CTRL_VERSION_2,
};
#endif

/*
 * This function will:
 * Read the eFuse for MAC addresses, and set ethaddr/eth1addr/usbnet_devaddr
 * in the environment
 * Perform fixups to the PHY present on certain boards.  We only need this
 * function in:
 * - SPL with either CPSW or USB ethernet support
 * - Full U-Boot, with either CPSW or USB ethernet
 * Build in only these cases to avoid warnings about unused variables
 * when we build an SPL that has neither option but full U-Boot will.
 */
#if (defined(CONFIG_DRIVER_TI_CPSW) && !defined(CONFIG_SPL_BUILD))
int board_eth_init(bd_t *bis)
{
  int rv, n = 0;
  uint8_t mac_addr[6];
  uint32_t mac_hi, mac_lo;

  /* try reading mac address from efuse */
  mac_lo = readl(&cdev->macid0l);
  mac_hi = readl(&cdev->macid0h);
  mac_addr[0] = mac_hi & 0xFF;
  mac_addr[1] = (mac_hi & 0xFF00) >> 8;
  mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
  mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
  mac_addr[4] = mac_lo & 0xFF;
  mac_addr[5] = (mac_lo & 0xFF00) >> 8;

#if (defined(CONFIG_DRIVER_TI_CPSW) && !defined(CONFIG_SPL_BUILD))
  if (!getenv("ethaddr")) {
    printf("<ethaddr> not set. Validating first E-fuse MAC\n");

  if (is_valid_ether_addr(mac_addr))
    eth_setenv_enetaddr("ethaddr", mac_addr);
  }

  mac_lo = readl(&cdev->macid1l);
  mac_hi = readl(&cdev->macid1h);
  mac_addr[0] = mac_hi & 0xFF;
  mac_addr[1] = (mac_hi & 0xFF00) >> 8;
  mac_addr[2] = (mac_hi & 0xFF0000) >> 16;
  mac_addr[3] = (mac_hi & 0xFF000000) >> 24;
  mac_addr[4] = mac_lo & 0xFF;
  mac_addr[5] = (mac_lo & 0xFF00) >> 8;

  if (!getenv("eth1addr")) {
    if (is_valid_ether_addr(mac_addr))
      eth_setenv_enetaddr("eth1addr", mac_addr);
  }

  writel(RMII_MODE_ENABLE|RMII_CHIPCKL_ENABLE, &cdev->miisel);	// RMII interface, RMII clock as input
  cpsw_slaves[0].phy_if = cpsw_slaves[1].phy_if = PHY_INTERFACE_MODE_RMII;

  rv = cpsw_register(&cpsw_data);
  if (rv < 0)
    printf("Error %d registering CPSW switch\n", rv);
  else
    n += rv;

#endif
  return n;
}
#endif
