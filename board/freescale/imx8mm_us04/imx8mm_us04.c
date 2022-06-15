/*
 * Copyright 2018 NXP
 * Copyright 2019 Exor International S.p.a.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <command.h>
#include <common.h>
#include <malloc.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm-generic/gpio.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <asm/arch/imx8mm_pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/mach-imx/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/arch/clock.h>
#include <spl.h>
#include <asm/mach-imx/dma.h>
#include <power/pmic.h>
#include <power/bd71837.h>
#include <usb.h>

DECLARE_GLOBAL_DATA_PTR;

#define UART_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_FSEL1)
#define WDOG_PAD_CTRL	(PAD_CTL_DSE6 | PAD_CTL_ODE | PAD_CTL_PUE | PAD_CTL_PE)

/* Specific code for the US04 target */
static iomux_v3_cfg_t const uart_pads[] = {
	IMX8MM_PAD_SAI2_RXC_UART1_RX | MUX_PAD_CTRL(UART_PAD_CTRL),
	IMX8MM_PAD_SAI2_RXFS_UART1_TX | MUX_PAD_CTRL(UART_PAD_CTRL),
};

static iomux_v3_cfg_t const wdog_pads[] = {
	IMX8MM_PAD_GPIO1_IO02_WDOG1_WDOG_B  | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

#define US04_RST_OUT_GPIO IMX_GPIO_NR(4, 8)
#define US04_DXEN0_GPIO   IMX_GPIO_NR(1, 1)
#define US04_RXEN0_GPIO   IMX_GPIO_NR(1, 3)
#define US04_SDCD_GPIO    IMX_GPIO_NR(2, 12)
#define US04_RST_GPIO_PAD_CTRL (PAD_CTL_PUE | PAD_CTL_DSE1)

static iomux_v3_cfg_t const us04_rst_pads[] = {
    IMX8MM_PAD_SAI1_RXD6_GPIO4_IO8 | MUX_PAD_CTRL(US04_RST_GPIO_PAD_CTRL),
    IMX8MM_PAD_GPIO1_IO01_GPIO1_IO1 | MUX_PAD_CTRL(US04_RST_GPIO_PAD_CTRL),
    IMX8MM_PAD_GPIO1_IO03_GPIO1_IO3 | MUX_PAD_CTRL(US04_RST_GPIO_PAD_CTRL),
};

#if 0
void ena_rs232phy(void)
{
  gpio_request(US04_DXEN0_GPIO, "us04_dxen0_out");
  gpio_direction_output(US04_DXEN0_GPIO, 1);
  udelay(1000);
}
#else
void ena_rs232phy(void){}
#endif

int board_early_init_f(void)
{
	struct wdog_regs *wdog = (struct wdog_regs *)WDOG1_BASE_ADDR;

	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));
	set_wdog_reset(wdog);
	imx_iomux_v3_setup_multiple_pads(uart_pads, ARRAY_SIZE(uart_pads));
	
	/* Set the US04 RESET_OUT signal to high */
	imx_iomux_v3_setup_multiple_pads(us04_rst_pads, ARRAY_SIZE(us04_rst_pads));
	gpio_request(US04_RST_OUT_GPIO, "us04_reset_out");
	gpio_direction_output(US04_RST_OUT_GPIO, 1);

	return 0;
}

#ifdef CONFIG_BOARD_POSTCLK_INIT
int board_postclk_init(void)
{
	/* TODO */
	return 0;
}
#endif

#if 0
int dram_init(void)
{
	/* rom_pointer[1] contains the size of TEE occupies */
	if (rom_pointer[1])
		gd->ram_size = PHYS_SDRAM_SIZE - rom_pointer[1];
	else
		gd->ram_size = PHYS_SDRAM_SIZE;

	return 0;
}
#endif
#ifdef CONFIG_OF_BOARD_SETUP
int ft_board_setup(void *blob, struct bd_info *bd)
{
	return 0;
}
#endif


#ifdef CONFIG_CI_UDC
int board_usb_init(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, true);

	return 0;
}

int board_usb_cleanup(int index, enum usb_init_type init)
{
	imx8m_usb_power(index, false);

	return 0;
}
#endif

int board_init(void)
{
	return 0;
}

int board_mmc_get_env_dev(int devno)
{
	return CONFIG_SYS_MMC_ENV_DEV;
}

int mmc_map_to_kernel_blk(int devno)
{
	return devno + 1;
}

int board_late_init(void)
{
	/* Unbind the UART4 core from the M4 CPU domain */
	run_command("mw.l 0x303d0518 0xff", 0);
	
	/* Default board name assignment */
	env_set("board_name", "us04_ma22");
	return 0;
}
