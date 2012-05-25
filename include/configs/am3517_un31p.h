/*
 * am3517_un31p.h - Configuration settings for the un31p AM3517 EVM board.
 *
 * Modified by: Giovanni P. Sitek S.p.a.
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

#ifndef __CONFIG_H
#define __CONFIG_H

#include <asm/sizes.h>

#define CONFIG_ARCH_CPU_INIT	1

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMCORTEXA8	1		/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1			/* in a TI OMAP core */
#define CONFIG_OMAP34XX		1		/* which is a 34XX */
#define CONFIG_OMAP3_AM3517EVM	1	/* working with AM3517 */
#define CONFIG_UN31	 1				/* This is the target platform family*/

//#define CONFIG_UN31P_CAREL	1	/* define this to understand if it is a Carel or a Sitek board*/
//when Carel board the message used to interrupt the booting sequence and get verbose mode is x0Dx03x0Dx03
//when Sitek board the message is x03 only
#define CAREL_BOARD 1
#define SITEK_BOARD 0

#define CONFIG_BOOTX	 1				/* Enable bootx sequence*/
#define CONFIG_TOUCH	 1				/* Enable touch*/

#include <asm/arch/cpu.h>	/* get chip and board defs */
#include <asm/arch/omap3.h>


/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#undef CONFIG_USE_IRQ			/* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/*
 * Size of malloc() pool
 */
#define CONFIG_ENV_SIZE			(128 << 10)	/* 128 KiB */
						/* Sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + (128 << 10))
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */
						
/*
 * HW drivers
 */
#define CONFIG_VIDEO_OMAP3 1
#define CONFIG_VIDEO_MEM_ADDR 0x87000000
						
/*
 * Support for EMIF4
 */
#define CONFIG_EMIF4			1

/*
 * DDR size interfaced
 */
#define CONFIG_SYS_CS0_SIZE             (256 * 1024 * 1024)

/*
 * Support for AMBA paged NorFLASH
 * NOTE 
 * Even if un31p platform is not provided of FPGA/AMBA bus and the NorFlash
 * is directly connected to GPMC (linear addressing layout) the CONFIG_AMBA_FLASH
 * is needed anymore in order to perform NORFlash reading through a specific helper
 * function (to prevent 8 bit accesses to Flash which is not allowed.)
 */
#define CONFIG_AMBA_FLASH		1

/* Various define
 */
#define CONFIG_SKIP_RELOCATE_UBOOT 1
#define CONFIG_CMD_DATA_SIZE 1
#define CONFIG_SKIP_PRCMINIT 1

/*
 * NS16550 Configuration
 */
#define V_NS16550_CLK			48000000	/* 48MHz (APLL96/2) */

#define CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE	(-4)
#define CONFIG_SYS_NS16550_CLK		V_NS16550_CLK

/*
 * select serial console configuration
 */
#define CONFIG_CONS_INDEX		3
#define CONFIG_SYS_NS16550_COM3		OMAP34XX_UART3
#define CONFIG_SERIAL3			3	/* UART3 on AM3517 EVM */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}

#define CONFIG_MMC			1
#define CONFIG_OMAP3_MMC		1
#define CONFIG_DOS_PARTITION		1

/* USB
 * Enable CONFIG_MUSB_HCD for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_UDD for Device functionalities.
 */
#define CONFIG_USB_AM3517		1
#define CONFIG_MUSB_HCD			1
/*#define CONFIG_MUSB_UDC		1*/

#ifdef CONFIG_USB_AM3517

#ifdef CONFIG_MUSB_HCD
#define CONFIG_CMD_USB

#define CONFIG_USB_STORAGE
#define CONFIG_CMD_STORAGE
#define CONFIG_CMD_FAT

#ifdef CONFIG_USB_KEYBOARD
#define CONFIG_SYS_USB_EVENT_POLL
#define CONFIG_PREBOOT "usb start"
#endif /* CONFIG_USB_KEYBOARD */

#endif /* CONFIG_MUSB_HCD */

#ifdef CONFIG_MUSB_UDC
/* USB device configuration */
#define CONFIG_USB_DEVICE		1
#define CONFIG_USB_TTY			1
#define CONFIG_SYS_CONSOLE_IS_IN_ENV	1
/* Change these to suit your needs */
#define CONFIG_USBD_VENDORID		0x0451
#define CONFIG_USBD_PRODUCTID		0x5678
#define CONFIG_USBD_MANUFACTURER	"Sitek-Carel"
#define CONFIG_USBD_PRODUCT_NAME	"UN31P"
#endif /* CONFIG_MUSB_UDC */

#endif /* CONFIG_USB_AM3517 */


/* -------------- commands to include-------------- */
/* ------------------------------------------------ */
#include <config_cmd_default.h>
#define CONFIG_CMD_DISPLAYCONFIGXML	/* Enables displaycongig.xml parsing support and lcd database handling */
#define CONFIG_CMD_I2CHWCFG			/* Enables HW cfg support into I2C SEEPROM */

#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/

#define CONFIG_CMD_I2C		/* I2C serial bus support	*/
#define CONFIG_CMD_MMC		/* MMC support			*/
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_CMD_BIN		/* Support for WCE6 native nk.bin image */

#undef CONFIG_CMD_FPGA		/* FPGA configuration Support	*/
#undef CONFIG_CMD_IMI		/* iminfo			*/
#undef CONFIG_CMD_IMLS		/* List all found images	*/

#define CONFIG_HARD_I2C			1
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_SYS_I2C_BUS		0
#define CONFIG_SYS_I2C_BUS_SELECT	1
#define CONFIG_DRIVER_OMAP34XX_I2C	1
#define CONFIG_SYS_I2C_HWCFGADD		0x54
#define CONFIG_SYS_I2C_TSC2004ADDR	0x4B
#define CONFIG_SYS_I2C_ADPADD		0x56
#define CONFIG_CMD_NET

#define CONFIG_NETMASK			255.255.0.0
#define CONFIG_BOOTFILE			"nk.bin"
#define CONFIG_AUTO_COMPLETE		1

/* Environment information */
#define CONFIG_BOOTDELAY	1

/* Default bootargs for booting Linux on UN31 from ubifs NORFLASH (only the "static part", which does not depend on the specific board, the rest is appended by bootx)
 */
#define CONFIG_BOOTARGS "ubi.mtd=7 root=ubi0:rootfs rootfstype=ubifs vram=12M omapfb.vram=0:12M"

/* Default environment items */
#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x88000000\0" \
	"wce_tftp=tftp ${loadaddr} nk.bin\0" \
	"wce_dhcp=dhcp ${loadaddr} nk.bin\0" \
	"wce_ramboot=echo Booting from RAM/USB...; bootwce ${loadaddr}\0" \
	"wce_usb=fatload usb 0:1 ${loadaddr} nk.bin\0" \
	"wce_mmc=fatload mmc 1 ${loadaddr} nk.bin\0" \
	"lcd_hsy=40 40 48 1\0" \
	"lcd_vsy=26 16 10 1\0" \
	"lcd_blank=0\0" \
	"lcd_size=800 480 16\0" \
	"lcd_pclk=32 0\0" \

/*
 * Miscellaneous configurable options
 */
#define V_PROMPT		"UN31P > "

#define CONFIG_SYS_LONGHELP			/* undef to save memory */
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_PROMPT			V_PROMPT
#define CONFIG_SYS_CBSIZE			512	/* Console I/O Buffer Size */

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE			(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS			32	/* max number of command */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE			(CONFIG_SYS_CBSIZE)
/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + 0xFFFFFFF) /* 256MB */
#define CONFIG_SYS_LOAD_ADDR		(OMAP34XX_SDRC_CS0+0x08000000)	/* Start from 128MB offset */

/*
 * AM3517 has 12 GP timers, they can be driven by the system clock
 * (12/13/16.8/19.2/38.4MHz) or by 32KHz clock. We use 13MHz (V_SCLK).
 * This rate is divided by a local divisor.
 */
#define CONFIG_SYS_TIMERBASE		OMAP34XX_GPT2
#define CONFIG_SYS_PTV			2	/* Divisor: 2^(PTV+1) => 8 */
#define CONFIG_SYS_HZ			1000

/*-----------------------------------------------------------------------
 * Stack sizes
 *
 * The stack sizes are set up in start.S using the settings below
 */
#define CONFIG_STACKSIZE	(128 << 10)	/* regular stack 128 KiB */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	(4 << 10)	/* IRQ stack 4 KiB */
#define CONFIG_STACKSIZE_FIQ	(4 << 10)	/* FIQ stack 4 KiB */
#endif

/*-----------------------------------------------------------------------
 * Physical Memory Map
 */
#define CONFIG_NR_DRAM_BANKS	2	/* CS1 may or may not be populated */
#define PHYS_SDRAM_1		OMAP34XX_SDRC_CS0
#define PHYS_SDRAM_1_SIZE	SZ_256M	/* 256 meg on board */
#define PHYS_SDRAM_2		OMAP34XX_SDRC_CS1

/* SDRAM Bank Allocation method */
#define SDRC_R_B_C		1


/*-----------------------------------------------------------------------
 * FLASH and environment organization
 */
#define CONFIG_SYS_MAX_FLASH_SECT	1024		//Number of FLASH sectors on one chip
#define CONFIG_SYS_MAX_FLASH_BANKS	1		//One FLASH chip only
#define CONFIG_SYS_MONITOR_LEN		SZ_256K		// Reserve 2 sectors for uBoot (256KB)
#define PHYS_FLASH_SIZE				SZ_128M 	//Flash size = 128MB
#define CONFIG_SYS_FLASH_BASE		0x8000000	//Flash base address
#define PHYS_FLASH_1		   		0x8000000
#define PHYS_FLASH_SECT_SIZE		SZ_128K		//Flash sector size
/* Dummy declaration of flash banks to get compilation right */
#define CONFIG_SYS_FLASH_BANKS_LIST	{0, 0}
/* Monitor uBoot at start of flash */
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_FLASH_BASE + SZ_128K)

#define ENV_IS_VARIABLE			1		//Environment is variable and is into Flash
#define CONFIG_ENV_IS_IN_FLASH		1

#define CONFIG_SYS_ENV_SECT_SIZE	SZ_128K		//The 3rd sector is reserved for the ENVIRONMENT
#define CONFIG_ENV_ADDR				(CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN)
#define CONFIG_SPLASH_ADDR          (CONFIG_ENV_ADDR + SZ_256K)
#define CONFIG_XFIS_ADDR 			(CONFIG_ENV_ADDR + SZ_128K)
#define CONFIG_CONFIGOSM_SIZE  	    0x2000000
#define CONFIG_SYS_FLASH_DMA		1 //Use Omap DMA for reading from NorFlash

/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

#ifndef __ASSEMBLY__
extern struct gpmc *gpmc_cfg;
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

/*-----------------------------------------------------
 * ethernet support for AM3517 EVM
 *------------------------------------------------
 */
#if defined(CONFIG_CMD_NET)
#define CONFIG_DRIVER_TI_EMAC
#define CONFIG_DRIVER_TI_EMAC_USE_RMII
#define CONFIG_MII
#define	CONFIG_BOOTP_DEFAULT
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_DNS2
#define CONFIG_BOOTP_SEND_HOSTNAME
#define CONFIG_NET_RETRY_COUNT 10
#define CONFIG_NET_MULTI
#endif

#define	CONFIG_HOSTNAME		"un31p"
#define	CONFIG_IPADDR		192.168.11.44
#define	CONFIG_SERVERIP		192.168.5.97
#define	CONFIG_GATEWAYIP	192.168.20.1
#define	CONFIG_ETHADDR		00:00:00:00:00:00

/* Actual u-boot subversion */
#define CONFIG_IDENT_STRING "UN31p 1.00.03 Alpha"

/* NAND support */
#define CONFIG_SYS_NAND_ADDR		NAND_BASE	/* physical address */
							/* to access nand */
#define CONFIG_SYS_NAND_BASE		NAND_BASE	/* physical address */
							/* to access */
/*
 * Board NAND Info.
 */
#define CONFIG_SYS_NAND_ADDR		NAND_BASE	/* physical address */
							/* to access nand */
#define CONFIG_SYS_NAND_BASE		NAND_BASE	/* physical address */
							/* to access */
							/* nand at CS0 */
#define CONFIG_SYS_MAX_NAND_DEVICE	1		/* Max number of */
							/* NAND devices */
#define CONFIG_SYS_64BIT_VSPRINTF		/* needed for nand_util.c */

#define CONFIG_JFFS2_NAND
/* nand device jffs2 lives on */
#define CONFIG_JFFS2_DEV		"nand0"
/* start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x680000
#define CONFIG_JFFS2_PART_SIZE		0xf980000	/* sz of jffs2 part */

#endif /* __CONFIG_H */







