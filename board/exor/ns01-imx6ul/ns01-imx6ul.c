/*
 * Copyright (C) 2017 Exor Int. S.p.a.
 *
 * Author: Giovanni Pavoni
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/clock.h>
#include <asm/arch/iomux.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/mxc_i2c.h>
#include <asm/io.h>
#include <common.h>
#include <miiphy.h>
#include <netdev.h>
#include <fsl_esdhc.h>
#include <i2c.h>
#include <linux/sizes.h>
#include <usb.h>
#include <power/pmic.h>
#include <power/pfuze3000_pmic.h>
#include "../../freescale/common/pfuze.h"

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL  (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |		\
	PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define USDHC_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_22K_UP  | PAD_CTL_SPEED_LOW |		\
	PAD_CTL_DSE_80ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define I2C_PAD_CTRL	(PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED |		\
	PAD_CTL_DSE_40ohm | PAD_CTL_HYS |			\
	PAD_CTL_ODE)

#define OTG_ID_PAD_CTRL (PAD_CTL_PKE | PAD_CTL_PUE |		\
	PAD_CTL_PUS_47K_UP  | PAD_CTL_SPEED_LOW |		\
	PAD_CTL_DSE_80ohm   | PAD_CTL_SRE_FAST  | PAD_CTL_HYS)

#define MDIO_PAD_CTRL  (PAD_CTL_PUS_100K_UP | PAD_CTL_PUE |     \
	PAD_CTL_DSE_48ohm   | PAD_CTL_SRE_FAST | PAD_CTL_ODE)

#define ENET_PAD_CTRL  (PAD_CTL_PUS_100K_UP | PAD_CTL_PUE |     \
	PAD_CTL_SPEED_HIGH   |                                   \
	PAD_CTL_DSE_48ohm   | PAD_CTL_SRE_FAST)

#define ENET_CLK_PAD_CTRL  (PAD_CTL_DSE_40ohm   | PAD_CTL_SRE_FAST)

#define RMII_PHY_RESET IMX_GPIO_NR(1, 28)
#define TX1_EN_GPIO IMX_GPIO_NR(4, 24)

#ifdef CONFIG_SYS_I2C_MXC
#define PC MUX_PAD_CTRL(I2C_PAD_CTRL)

#define NS01EVK_VAL    129
#define NS01EK435_VAL  130
#define NS01PA18_VAL   132

void ena_rs232phy(void);

/*
 * Read I2C SEEPROM infos and set env. variables accordingly
 */
static int read_eeprom(void)
{
#if (defined(CONFIG_CMD_I2CHWCFG))  
  extern int i2cgethwcfg (void);
  return i2cgethwcfg();
#endif
  return 0;
}

/*
 * Reads the hwcfg.txt file from USB stick (root of FATFS partition) if any, parses it
 * and updates the environment variable accordingly.
 * 
 * NOTE: This function is used in case the I2C SEEPROM contents are not valid, in order to get
 *       a temporary and volatile HW configuration from USB to boot properly Linux (even if the I2C SEEPROM is not programmed) 
 */
static int USBgethwcfg(void)
{
  
  printf("Trying to get the HW cfg from USB stick...\n");
  
  run_command("usb stop", 0);
  run_command("usb reset", 0);
  run_command("setenv filesize 0", 0);
  run_command("fatload usb 0 ${loadaddr} hwcfg.txt", 0);
  run_command("env import -t ${loadaddr} ${filesize}", 0);
  run_command("usb stop", 0);
  
  return 0;
}

/* I2C2 for PMIC */
struct i2c_pads_info i2c_pad_info1 = {
	.scl = {
		.i2c_mode =  MX6_PAD_UART2_TX_DATA__I2C4_SCL | PC,
		.gpio_mode = MX6_PAD_UART2_TX_DATA__GPIO1_IO20 | PC,
		.gp = IMX_GPIO_NR(1, 20),
	},
	.sda = {
		.i2c_mode = MX6_PAD_UART2_RX_DATA__I2C4_SDA | PC,
		.gpio_mode = MX6_PAD_UART2_RX_DATA__GPIO1_IO21 | PC,
		.gp = IMX_GPIO_NR(1, 21),
	},
};
#endif

static iomux_v3_cfg_t const fec_pads[] = {
	MX6_PAD_ENET1_TX_EN__ENET2_MDC		| MUX_PAD_CTRL(MDIO_PAD_CTRL),
	MX6_PAD_ENET1_TX_DATA1__ENET2_MDIO	| MUX_PAD_CTRL(MDIO_PAD_CTRL),
	MX6_PAD_ENET2_TX_DATA0__ENET2_TDATA00	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_TX_DATA1__ENET2_TDATA01	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_TX_CLK__ENET2_REF_CLK2	| MUX_PAD_CTRL(ENET_CLK_PAD_CTRL),
	MX6_PAD_ENET2_TX_EN__ENET2_TX_EN	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_RX_DATA0__ENET2_RDATA00	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_RX_DATA1__ENET2_RDATA01	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_RX_EN__ENET2_RX_EN	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_ENET2_RX_ER__ENET2_RX_ER	| MUX_PAD_CTRL(ENET_PAD_CTRL),
	MX6_PAD_UART4_TX_DATA__GPIO1_IO28	| MUX_PAD_CTRL(NO_PAD_CTRL),
};

static void setup_iomux_fec(void)
{
	imx_iomux_v3_setup_multiple_pads(fec_pads, ARRAY_SIZE(fec_pads));
}

int board_eth_init(bd_t *bis)
{
	setup_iomux_fec();

	gpio_direction_output(RMII_PHY_RESET, 0);
	/*
	 * According to KSZ8081MNX-RNB manual:
	 * For warm reset, the reset (RST#) pin should be asserted low for a
	 * minimum of 500μs.  The strap-in pin values are read and updated
	 * at the de-assertion of reset.
	 */
	udelay(500);

	gpio_direction_output(RMII_PHY_RESET, 1);
	/*
	 * According to KSZ8081MNX-RNB manual:
	 * After the de-assertion of reset, wait a minimum of 100μs before
	 * starting programming on the MIIM (MDC/MDIO) interface.
	 */
	udelay(100);

	return fecmxc_initialize(bis);
}

static int setup_fec(void)
{
	struct iomuxc *const iomuxc_regs = (struct iomuxc *)IOMUXC_BASE_ADDR;
	int ret;

	clrsetbits_le32(&iomuxc_regs->gpr[1], IOMUX_GPR1_FEC2_MASK,
			IOMUX_GPR1_FEC2_CLOCK_MUX1_SEL_MASK);

	ret = enable_fec_anatop_clock(1, ENET_50MHZ);
	if (ret)
		return ret;

	enable_enet_clk(1);

	return 0;
}

int board_phy_config(struct phy_device *phydev)
{
	phy_write(phydev, MDIO_DEVAD_NONE, 0x1f, 0x8190);

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}

int dram_init(void)
{
	gd->ram_size = imx_ddr_size();

	return 0;
}

static iomux_v3_cfg_t const uart1_pads[] = {
	MX6_PAD_UART1_TX_DATA__UART1_DCE_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_UART1_RX_DATA__UART1_DCE_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	MX6_PAD_CSI_DATA03__GPIO4_IO24 | MUX_PAD_CTRL(NO_PAD_CTRL),
};



static iomux_v3_cfg_t const usdhc1_pads[] = {
	MX6_PAD_SD1_CLK__USDHC1_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_CMD__USDHC1_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DATA0__USDHC1_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DATA1__USDHC1_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DATA2__USDHC1_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DATA3__USDHC1_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	MX6_PAD_NAND_RE_B__USDHC2_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NAND_WE_B__USDHC2_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NAND_DATA00__USDHC2_DATA0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NAND_DATA01__USDHC2_DATA1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NAND_DATA02__USDHC2_DATA2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_NAND_DATA03__USDHC2_DATA3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

#define USB_OTHERREGS_OFFSET	0x800
#define UCTRL_PWR_POL		(1 << 9)

static iomux_v3_cfg_t const usb_otg_pad[] = {
	MX6_PAD_GPIO1_IO00__ANATOP_OTG1_ID | MUX_PAD_CTRL(OTG_ID_PAD_CTRL),
};

static void setup_iomux_uart(void)
{
	imx_iomux_v3_setup_multiple_pads(uart1_pads, ARRAY_SIZE(uart1_pads));
}

static void setup_usb(void)
{
	imx_iomux_v3_setup_multiple_pads(usb_otg_pad, ARRAY_SIZE(usb_otg_pad));
}

static struct fsl_esdhc_cfg usdhc_cfg[2] = {
	{USDHC1_BASE_ADDR, 0, 4},
	{USDHC2_BASE_ADDR, 0, 4},
};

int board_mmc_getcd(struct mmc *mmc)
{
	return 1;
}

/*
 * According to the board_mmc_init() the following map is done:
 * (U-Boot device node)    (Physical Port)
 * mmc0                    USDHC1 (SDcard)
 * mmc1                    USDHC2 (EMMC)
 */
int board_mmc_init(bd_t *bis)
{
  int ret;
  
  imx_iomux_v3_setup_multiple_pads(usdhc1_pads, ARRAY_SIZE(usdhc1_pads));
  usdhc_cfg[0].sdhc_clk = mxc_get_clock(MXC_ESDHC_CLK);
  ret = fsl_esdhc_initialize(bis, &usdhc_cfg[0]);
  if(ret)
    printf("Failed to initialize SDcard (mmc0)\n");
  
  imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
  usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
  ret = fsl_esdhc_initialize(bis, &usdhc_cfg[1]);
  if(ret)
    printf("Failed to initialize EMMC (mmc1)\n");
  
  return 0;
}

int board_early_init_f(void)
{
	setup_iomux_uart();

	return 0;
}

#ifdef CONFIG_POWER
#define I2C_PMIC       0
static struct pmic *pfuze;
int power_init_board(void)
{
	int ret;
	unsigned int reg, rev_id;

	ret = power_pfuze3000_init(I2C_PMIC);
	if (ret)
		return ret;

	pfuze = pmic_get("PFUZE3000");
	ret = pmic_probe(pfuze);
	if (ret)
		return ret;

	pmic_reg_read(pfuze, PFUZE3000_DEVICEID, &reg);
	pmic_reg_read(pfuze, PFUZE3000_REVID, &rev_id);
	printf("PMIC: PFUZE3000 DEV_ID=0x%x REV_ID=0x%x\n", reg, rev_id);

	/* disable Low Power Mode during standby mode */
	pmic_reg_write(pfuze, PFUZE3000_LDOGCTL, 0x1);

	/* SW1B step ramp up time from 2us to 4us/25mV */
	pmic_reg_write(pfuze, PFUZE3000_SW1BCONF, 0x40);

	/* SW1B mode to APS/PFM */
	pmic_reg_write(pfuze, PFUZE3000_SW1BMODE, 0xc);

	/* SW1B standby voltage set to 0.975V */
	pmic_reg_write(pfuze, PFUZE3000_SW1BSTBY, 0xb);
	
	/* 5.0V on SWBST for USB */
	pmic_reg_write(pfuze, PFUZE3000_SWBSTCTL, 0x48);

	return 0;
}
#endif

int board_usb_phy_mode(int port)
{
	if (port == 1)
		return USB_INIT_HOST;
	else
		return USB_INIT_DEVICE;
}

int board_ehci_hcd_init(int port)
{
	u32 *usbnc_usb_ctrl;

	if (port > 1)
		return -EINVAL;

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
				 port * 4);

	/* Set Power polarity */
	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}

int board_init(void)
{
	/* Address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	#ifdef CONFIG_SYS_I2C_MXC
		setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x7f, &i2c_pad_info1);
	#endif

	setup_fec();
	setup_usb();

	return 0;
}

int checkboard(void)
{
	puts("Board: NS01-IMX6UL-EMMC\n");

	return 0;
}

int board_late_init(void)
{
  char* tmp;
  unsigned long hwcode = 0;
  unsigned long rs232phyena = 0;
		
  /* Enable the rs232 phy based on "rs232_txen" environment variable */
  tmp = getenv("rs232_txen");
  if(tmp)
  {
    rs232phyena = (simple_strtoul (tmp, NULL, 10))&0xff;
    if(rs232phyena != 0)
    {
      ena_rs232phy();
    }
  }

  /* Get the system configuration from the I2C SEEPROM */
  if(read_eeprom())
  {
    ena_rs232phy();
    printf("Failed to read the HW cfg from the I2C SEEPROM: trying to load it from USB ...\n");
    USBgethwcfg();
  }
 
  /* Set the "board_name" env. variable according with the "hw_code" */
  tmp = getenv("hw_code");
  if(!tmp)
  {
    puts ("WARNING: 'hw_code' environment var not found!\n");
  }
  else
    hwcode = (simple_strtoul (tmp, NULL, 10))&0xff;
  
  if(hwcode==NS01EVK_VAL)
    setenv("board_name", "ns01-evk"); 
  else if(hwcode==NS01EK435_VAL)
    setenv("board_name", "ns01-ek435"); 
  else if(hwcode==NS01PA18_VAL)
    setenv("board_name", "ns01-pa18");
  else
  {
    puts ("WARNING: unknowm carrier hw code; using 'usom_undefined' board name. \n");
    setenv("board_name", "usom_undefined");
  }
  
  /* Check if file $0030d8$.bin exists on the 1st partition of the SD-card and, if so, skips booting the mainOS */
  run_command("setenv skipbsp1 0", 0);
  run_command("mmc dev 0", 0);
  run_command("mmc rescan", 0);
  run_command("if test -e mmc 0:1 /$0030d8$.bin; then setenv skipbsp1 1; fi", 0);
    
  return 0;
}

#ifdef CONFIG_HAVEPRGUART
void ena_rs232phy(void)
{
  gpio_request(TX1_EN_GPIO, "");
  gpio_direction_output(TX1_EN_GPIO, 1);
  
  udelay(1000);
}
#else
void ena_rs232phy(void){}
#endif
