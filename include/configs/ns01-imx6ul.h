/*
 * Copyright (C) 2017 Exor Int. S.p.a.
 *
 * Configuration settings for the NS01 nanosom board.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#ifndef __NS01_IMX6UL_CONFIG_H
#define __NS01_IMX6UL_CONFIG_H


#include <asm/arch/imx-regs.h>
#include <linux/sizes.h>
#include "mx6_common.h"
#include <asm/imx-common/gpio.h>

/* 
 * Silent console option
 */
#define CONFIG_SILENT_CONSOLE
#define CONFIG_SILENT_CONSOLE_UPDATE_ON_SET
#define CONFIG_SILENT_U_BOOT_ONLY
#define CONFIG_SYS_DEVICE_NULLDEV

/* Network support */

#define CONFIG_FEC_MXC
#define CONFIG_MII
#define IMX_FEC_BASE			ENET2_BASE_ADDR
#define CONFIG_FEC_MXC_PHYADDR		0x1
#define CONFIG_FEC_XCV_TYPE		RMII
#define CONFIG_PHYLIB
#define CONFIG_PHY_MICREL

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(35 * SZ_1M) /* Increase due to DFU */

#define CONFIG_MXC_UART
#define CONFIG_MXC_UART_BASE		UART1_BASE

/* MMC Configs */
#define CONFIG_FSL_USDHC
#define CONFIG_FSL_ESDHC
#define CONFIG_SYS_FSL_ESDHC_ADDR	USDHC1_BASE_ADDR
#define CONFIG_SUPPORT_EMMC_BOOT

/* USB Configs */
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2

#define CONFIG_USBD_HS

#define CONFIG_USB_FUNCTION_MASS_STORAGE
#define CONFIG_USB_GADGET_VBUS_DRAW	2

#define CONFIG_SYS_DFU_DATA_BUF_SIZE SZ_16M
#define DFU_DEFAULT_POLL_TIMEOUT 300

#define CONFIG_SYS_MMC_IMG_LOAD_PART	1
	   
#define CONFIG_EXTRA_ENV_SETTINGS \
	"altbootcmd="CFG_SYS_ALT_BOOTCOMMAND"\0"\
	"silent=1\0" \
	"bootlimit=3\0" \
	"fdtaddr=0x83000000\0" \
	"fdt_high=0xffffffff\0" \
	"boot_fdt=yes\0" \
	"skipbsp1=0\0" \
	"bootpart=0:1\0" \
	"bootdir=/boot\0" \
	"bootfile=zImage\0" \
	"fdtfile=usom_undefined.dtb\0" \
	"fastboot=n\0" \
	"console=/dev/null\0" \
	"rs232_txen=0\0" \
	"optargs=\0" \
	"mmcdev=0\0" \
	"mmcroot=/dev/mmcblk0p2 rw\0" \
	"mmcrootfstype=ext4 rootwait\0" \
	"rootpath=/export/rootfs\0" \
	"nfsopts=nolock\0" \
	"mmcargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"hw_dispid=${hw_dispid} " \
		"hw_code=${hw_code} " \
		"fastboot=${fastboot} " \
		"board_name=${board_name} " \
		"touch_type=${touch_type} " \
		"root=${mmcroot} " \
		"rootfstype=${mmcrootfstype}\0" \
	"netargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"hw_dispid=${hw_dispid} " \
		"hw_code=${hw_code} " \
		"board_name=${board_name} " \
		"touch_type=${touch_type} " \
		"root=/dev/nfs " \
		"nfsroot=${serverip}:${rootpath},${nfsopts} rw " \
		"ip=dhcp\0" \
	"bootenv=uEnv.txt\0" \
	"loadbootenv=load mmc ${mmcdev} ${loadaddr} ${bootenv}\0" \
	"importbootenv=echo Importing environment from mmc ...; " \
		"env import -t $loadaddr $filesize\0" \
	"loadimage=load mmc ${bootpart} ${loadaddr} ${bootdir}/${bootfile}\0" \
	"loadfdt=load mmc ${bootpart} ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"mmcloados=run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"mmcboot=mmc dev ${mmcdev}; " \
		"if mmc rescan; then " \
			"echo SD/MMC found on device ${mmcdev};" \
			"if run loadbootenv; then " \
				"echo Loaded environment from ${bootenv};" \
				"run importbootenv;" \
			"fi;" \
			"if test -n $uenvcmd; then " \
				"echo Running uenvcmd ...;" \
				"run uenvcmd;" \
			"fi;" \
			"if run loadimage; then " \
				"run mmcloados;" \
			"fi;" \
		"fi;\0" \
	"usbargs=setenv bootargs console=${console} " \
		"${optargs} " \
		"hw_dispid=${hw_dispid} " \
		"hw_code=${hw_code} " \
		"board_name=${board_name} " \
		"touch_type=${touch_type} " \
		"root=${usbroot} " \
		"rootfstype=${usbrootfstype}\0" \
	"usbroot=/dev/sda2 rw\0" \
	"usbrootfstype=ext4 rootwait\0" \
	"usbloados=run usbargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run usbloadfdt; then " \
				"bootz ${loadaddr} - ${fdtaddr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"usbloadimage=load usb 0 ${loadaddr} ${bootdir}/${bootfile}\0" \
	"usbloadfdt=load usb 0 ${fdtaddr} ${bootdir}/${fdtfile}\0" \
	"usbboot=mmc dev ${mmcdev}; " \
		"if usb reset; then " \
			"if run usbloadimage; then " \
				"run usbloados;" \
			"fi;" \
			"usb stop;" \
		"fi;\0" \
	"netboot=echo Booting from network ...; " \
		"setenv autoload no; " \
		"dhcp; " \
		"tftp ${loadaddr} ${bootfile}; " \
		"tftp ${fdtaddr} ${fdtfile}; " \
		"run netargs; " \
		"bootz ${loadaddr} - ${fdtaddr}\0" \
	"findfdt="\
		"if test $board_name = ns01-evk; then " \
			"setenv fdtfile ns01-evk.dtb; fi; " \
		"if test $board_name = ns01-ek435; then " \
			"setenv fdtfile ns01-ek435.dtb; fi; " \
		"if test $board_name = ns01-pa18; then " \
			"setenv fdtfile ns01-pa18.dtb; fi; " \
		"if test $board_name = ns01-keb01; then " \
			"setenv fdtfile ns01-keb01.dtb; fi; " \
		"if test $board_name = ns01-x5; then " \
			"setenv fdtfile ns01-x5.dtb; fi; " \
		"if test $board_name = ns01-x5bs; then " \
			"setenv fdtfile ns01-x5bs.dtb; fi; " \
		"if test $board_name = usom_undefined; then " \
			"setenv fdtfile usom_undefined.dtb; fi; \0" 
	
#define CONFIG_BOOTCOMMAND \
	"setenv mmcdev 0; " \
	"run findfdt; " \
	"echo Try booting Linux from SD-card...;" \
	"run mmcboot;" \
	"if test $skipbsp1 = 0; then " \
	"echo Try booting Linux from EMMC, main BSP...;" \
	"setenv mmcdev 1; " \
	"setenv bootpart 1:3; " \
	"setenv mmcroot /dev/mmcblk1p3 ro; " \
	"run mmcboot;" \
	"fi; " \
	"echo Try booting Linux from USB stick...;" \
	"run usbboot;" \
	"echo Try booting Linux from EMMC, recovery BSP...;" \
	"setenv fastboot n; " \
	"setenv mmcdev 1; " \
	"setenv bootpart 1:2; " \
	"setenv mmcroot /dev/mmcblk1p2 ro; " \
	"run mmcboot;" 

#define CFG_SYS_ALT_BOOTCOMMAND \
	"i2c mw 68 19 0; " \
	"setenv mmcdev 0; " \
	"run findfdt; " \
	"echo Try booting Linux from SD-card...;" \
	"run mmcboot;" \
	"echo Try booting Linux from USB stick...;" \
	"run usbboot;" \
	"echo Try booting Linux from EMMC, recovery BSP...;" \
	"setenv fastboot n; " \
	"setenv mmcdev 1; " \
	"setenv bootpart 1:2; " \
	"setenv mmcroot /dev/mmcblk1p2 ro; " \
	"run mmcboot;" 
	   
#define CONFIG_SYS_MEMTEST_START	0x80000000
#define CONFIG_SYS_MEMTEST_END		CONFIG_SYS_MEMTEST_START + SZ_128M

#define CONFIG_SYS_LOAD_ADDR		CONFIG_LOADADDR
#define CONFIG_SYS_HZ			1000

#define CONFIG_CMDLINE_EDITING

/* Physical Memory Map */
#define CONFIG_NR_DRAM_BANKS		1
#define PHYS_SDRAM			MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE		PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR	IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE	IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* I2C configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C4
#define CONFIG_SYS_I2C_SPEED		100000

#define CONFIG_CMD_I2CHWCFG
#define CONFIG_SYS_I2C_EEPROM_ADDR 0x54
#define DEF_SYS_I2C_ADPADD 0x56

/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE3000
#define CONFIG_POWER_PFUZE3000_I2C_ADDR	0x08

/* environment organization */
#define CONFIG_ENV_SIZE			SZ_8K
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_OFFSET		(0)

#define CONFIG_SYS_MMC_ENV_DEV		1
#define CONFIG_SYS_MMC_ENV_PART		2
#define CONFIG_MMCROOT			"/dev/mmcblk0p2"

#define CONFIG_BOARD_LATE_INIT

/* Bootcounter using the M41T83 I2C RTC NVRAM */
#define CONFIG_BOOTCOUNT_LIMIT
#define CONFIG_BOOTCOUNT_I2C
#define CONFIG_BOOTCOUNT_ALEN 1
#define CONFIG_SYS_I2C_RTC_ADDR 0x68
#undef  CONFIG_SYS_BOOTCOUNT_ADDR
#define CONFIG_SYS_BOOTCOUNT_ADDR 0x19

/* Programmable UART on console porty*/
#define CONFIG_HAVEPRGUART

#endif /* __NS01_IMX6UL_CONFIG_H */
