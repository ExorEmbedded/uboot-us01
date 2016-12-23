/*
 * Copyright (C) 2012-2014 Freescale Semiconductor, Inc.
 *
 * Configuration settings for the Freescale i.MX6Q SabreAuto board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __MX6DL_USOM_CONFIG_H
#define __MX6DL_USOM_CONFIG_H

#define CONFIG_MACH_TYPE	3529
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONFIG_CONSOLE_DEV		"ttymxc0"
#define CONFIG_MMCROOT			"/dev/mmcblk2p2"  /* SDHC3 */

/* 
 * Silent console option
 */
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SILENT_U_BOOT_ONLY
#define CONFIG_SYS_DEVICE_NULLDEV

/* 
 * #include "mx6usom_common.h"
 */
#include "mx6usom_wu16_common.h"
#include <asm/imx-common/gpio.h>

#undef CONFIG_MFG_NAND_PARTITION
#define CONFIG_MFG_NAND_PARTITION ""

#undef CONFIG_SYS_USE_SPINOR
#undef CONFIG_SYS_USE_EIMNOR
#undef CONFIG_VIDEO
#undef CONFIG_SPLASH_SCREEN

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
