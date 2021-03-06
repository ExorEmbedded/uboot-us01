/*
 * Copyright (C) 2012-2014 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the US03 board, when placed on the Automatic Test Equipment (UNT37)
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6DL_USOM_CONFIG_H
#define __MX6DL_USOM_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc0"
#define CONFIG_MMCROOT			"/dev/mmcblk2p2"  /* SDHC3 */

#include "mx6usom_common.h"
#include <asm/imx-common/gpio.h>

#undef CONFIG_MFG_NAND_PARTITION
#define CONFIG_MFG_NAND_PARTITION ""

#undef CONFIG_SYS_USE_SPINOR
#undef CONFIG_SYS_USE_EIMNOR
#undef CONFIG_VIDEO
#undef CONFIG_SPLASH_SCREEN

#undef CONFIG_BOOTCOUNT_LIMIT
#undef CONFIG_BOOTCOMMAND
#undef CONFIG_ANDROID_BOOTCOMMAND
#undef CONFIG_HAVEPRGUART
#undef CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_CMD_GPIO

/* USB Configs */
#define CONFIG_CMD_USB
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_MX6
#define CONFIG_USB_STORAGE
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_USB_HOST_ETHER
#define CONFIG_USB_ETHER_ASIX
#define CONFIG_MXC_USB_PORTSC  (PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS   0
#define CONFIG_USB_MAX_CONTROLLER_COUNT 2 /* Enabled USB controller number */

#define CONFIG_SYS_FSL_USDHC_NUM	2
#define CONFIG_SYS_MMC_ENV_DEV		1  /* SDHC4, EMMC */
#define CONFIG_SYS_MMC_ENV_PART         2  /* BOOT2 partition */

#ifdef CONFIG_SYS_USE_SPINOR
#define CONFIG_SF_DEFAULT_CS   (1|(IMX_GPIO_NR(3, 19)<<8))
#endif

#endif                         /* __MX6DL_USOM_CONFIG_H */
