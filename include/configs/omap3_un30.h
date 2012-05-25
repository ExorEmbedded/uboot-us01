/*
 * (C) Copyright 2006-2008
 * Texas Instruments.
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Mohammed Khasim <x0khasim@ti.com>
 *
 * Configuration settings for the TI OMAP3530 Beagle board.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H
#include <asm/sizes.h>

/*
 * High Level Configuration Options
 */ 
#define CONFIG_ARMCORTEXA8	1	/* This is an ARM V7 CPU core */
#define CONFIG_OMAP		1	/* in a TI OMAP core */
#define CONFIG_OMAP34XX		1	/* which is a 34XX */
#define CONFIG_OMAP3430		1	/* which is in a 3430 */
#define CONFIG_OMAP3_UN30	1	/* working with UN30 */

#define CONFIG_BOOTX	 1				/* Enable bootx sequence*/
#define CONFIG_TOUCH	 1				/* Enable touch*/
#define CONFIG_SYS_FLASH_DMA 1			/* Enable DMA support */

#define CONFIG_ARCH_CPU_INIT 1

#include <asm/arch/cpu.h>		/* get chip and board defs */
#include <asm/arch/omap3.h>

/*
 * Display CPU and Board information
 */
#define CONFIG_DISPLAY_CPUINFO		1
#define CONFIG_DISPLAY_BOARDINFO	1

/* Clock Defines */
#define V_OSCK			26000000	/* Clock output from T2 */
#define V_SCLK			(V_OSCK >> 1)

#undef CONFIG_USE_IRQ				/* no support for IRQs */
#define CONFIG_MISC_INIT_R

#define CONFIG_CMDLINE_TAG		1	/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_REVISION_TAG		1

/*
 * Size of malloc() pool
 */
#define CONFIG_ENV_SIZE			SZ_128K	/* Total Size Environment */
						/* Sector */
#define CONFIG_SYS_MALLOC_LEN		(CONFIG_ENV_SIZE + SZ_128K)
#define CONFIG_SYS_GBL_DATA_SIZE	128	/* bytes reserved for */
						/* initial data */

/*
 * Hardware drivers
 */
#define CONFIG_VIDEO_OMAP3 1
#define CONFIG_VIDEO_MEM_ADDR 0x87000000

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
#define CONFIG_SERIAL3			3	/* UART3 on Beagle Rev 2 */

/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE
#define CONFIG_BAUDRATE			115200
#define CONFIG_SYS_BAUDRATE_TABLE	{4800, 9600, 19200, 38400, 57600,\
					115200}

  #define CONFIG_MMC			1
#define CONFIG_OMAP3_MMC		1
/* #define CONFIG_SYS_MMC_BASE		0xF0000000 */
#define CONFIG_DOS_PARTITION	1

/*
 * Support for SDRC
 */
#define CONFIG_SDRC			1

/* commands to include */

#define CONFIG_CMD_FAT		/* FAT support			*/
#define CONFIG_CMD_JFFS2	/* JFFS2 Support		*/

#define CONFIG_CMD_I2C		/* I2C serial bus support	*/

#define CONFIG_CMD_AUTOSCRIPT	/* autoscript support		*/
#define CONFIG_CMD_BDI		/* bdinfo			*/
#define CONFIG_CMD_BOOTD	/* bootd			*/
#define CONFIG_CMD_CONSOLE	/* coninfo			*/
#define CONFIG_CMD_ECHO		/* echo arguments		*/
#define CONFIG_CMD_SAVEENV	/* saveenv			*/
#define CONFIG_CMD_ITEST	/* Integer (and string) test	*/
#define CONFIG_CMD_LOADB	/* loadb			*/
#define CONFIG_CMD_MEMORY	/* md mm nm mw cp cmp crc base loop mtest */
#define CONFIG_CMD_MISC		/* misc functions like sleep etc*/
#define CONFIG_CMD_RUN		/* run command in env variable	*/
#define CONFIG_CMD_BIN		/* boot nk.bin WCE6 image */
#define CONFIG_CMD_MMC		/* MMC support			*/

#define CONFIG_CMD_NET		/* bootp, tftpboot, rarpboot	*/
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_PING
#define CONFIG_CMD_NFS
#define CONFIG_CMD_DISPLAYCONFIGXML	/* Enables displaycongig.xml parsing support and lcd database handling */
#define CONFIG_CMD_I2CHWCFG			/* Enables HW cfg support into I2C SEEPROM */
		
/* I2C settings */
#define CONFIG_DRIVER_ULTII2C		1
#define CONFIG_SYS_ULTII2CBASE		0x1ac02000		
#define CONFIG_SYS_I2C_SPEED		100000
#define CONFIG_SYS_I2C_SLAVE		1
#define CONFIG_SYS_I2C_BUS			0
#define CONFIG_SYS_I2C_BUS_SELECT	1
#define CONFIG_SYS_I2C_HWCFGADD		0x54
#define CONFIG_SYS_I2C_TSC2004ADDR	0x4B
#define CONFIG_SYS_I2C_ADPADD		0x56

#define CONFIG_JFFS2_DEV		"nor0"
/* start of jffs2 partition */
#define CONFIG_JFFS2_PART_OFFSET	0x680000
#define CONFIG_JFFS2_PART_SIZE		0xf980000	/* size of jffs2 */
							/* partition */
#define CONFIG_BOOTARGS			"setenv bootargs console=ttyS2," \
					"115200n8 noinitrd " 

#define CONFIG_NETMASK			255.255.0.0
#define CONFIG_BOOTFILE			"wce.nb0"
#define CONFIG_AUTO_COMPLETE		1
/*
 * Miscellaneous configurable options
 */
#define V_PROMPT			"UN30>"

#define CONFIG_SYS_LONGHELP		/* undef to save memory */
#define CONFIG_SYS_PROMPT		V_PROMPT
#define CONFIG_SYS_HUSH_PARSER		/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_SYS_CBSIZE		512	/* Console I/O Buffer Size */
/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + \
					sizeof(CONFIG_SYS_PROMPT)+16)
#define CONFIG_SYS_MAXARGS		16	/* max number of command args */
/* Boot Argument Buffer Size */
#define CONFIG_SYS_BARGSIZE		(CONFIG_SYS_CBSIZE)

#define CONFIG_SYS_MEMTEST_START	(OMAP34XX_SDRC_CS0)	/* memtest */
								/* works on */
#define CONFIG_SYS_MEMTEST_END		(OMAP34XX_SDRC_CS0 + \
					0x01F00000) /* 31MB */

#undef	CONFIG_SYS_CLKS_IN_HZ		/* everything, incl board info, in Hz */

#define CONFIG_SYS_LOAD_ADDR		(OMAP34XX_SDRC_CS0+0x08000000)	/* Start from 128MB offset */
							/* load address */

/*
 * OMAP3 has 12 GP timers, they can be driven by the system clock
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
#define CONFIG_STACKSIZE	SZ_128K	/* regular stack */
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ	SZ_4K	/* IRQ stack */
#define CONFIG_STACKSIZE_FIQ	SZ_4K	/* FIQ stack */
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
#define CONFIG_SYS_FLASH_BASE		0x1a000000	//Flash base address
#define PHYS_FLASH_1		   		0x1a000000
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
/*-----------------------------------------------------------------------
 * CFI FLASH driver setup
 */
/* timeout values are in ticks */
#define CONFIG_SYS_FLASH_ERASE_TOUT	(100 * CONFIG_SYS_HZ)
#define CONFIG_SYS_FLASH_WRITE_TOUT	(100 * CONFIG_SYS_HZ)

#define CONFIG_CMD_FLASH		1
#define CONFIG_AMBA_FLASH		1

/* Various define
 */
#define CONFIG_SKIP_RELOCATE_UBOOT 1
#define CONFIG_CMD_DATA_SIZE 1
#define CONFIG_SKIP_PRCMINIT 1

/* ETH ULTIEMAC configs */
#define CONFIG_ULTIEMAC			1
#define CONFIG_ULTIEMAC_BASE 0x1ac04000
#define CONFIG_ULTIEMAC_BUFFER_AMBA 0x00000000
#define CONFIG_ULTIEMAC_BUFFER_CPU 0x10000000
#define	CONFIG_HOSTNAME		"un30"

#define	CONFIG_IPADDR		192.168.11.44
#define	CONFIG_SERVERIP		192.168.5.97
#define	CONFIG_GATEWAYIP	192.168.20.1
#define	CONFIG_ETHADDR		00:00:00:00:00:00

/* Default environment items */
#define CONFIG_BOOTDELAY		1

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadaddr=0x88000000\0" \
	"wceflashaddr=0x1a160000\0" \
	"wce_store2flash=erase ${wceflashaddr} +2B80000; cp ${loadaddr} ${wceflashaddr} ADFFFF\0" \
	"wce_tftp=tftp ${loadaddr} nk.bin\0" \
	"wce_dhcp=dhcp ${loadaddr} nk.bin\0" \
	"wce_ramboot=echo Booting from RAM/USB...; bootwce ${loadaddr}\0" \
	"wce_flashboot=echo Booting from NorFlash...; bootwce ${wceflashaddr}\0" \
	"wce_usb=fatload usb 0:1 ${loadaddr} nk.bin\0" \
	"lcd_hsy=40 40 48 1\0" \
	"lcd_vsy=26 16 10 1\0" \
	"lcd_blank=0\0" \
	"lcd_size=800 480 16\0" \
	"lcd_pclk=25 0\0" \
	
#define CONFIG_BOOTCOMMAND \
	"usb start; " \
	"run wce_usb; " \
	"run wce_ramboot; " \
	"run wce_flashboot; " \

/* Actual u-boot subversion */
#define CONFIG_IDENT_STRING "UN30/DM37xx 1.00.03 Alpha"

#ifndef __ASSEMBLY__
extern struct gpmc *gpmc_cfg;
extern unsigned int boot_flash_base;
extern volatile unsigned int boot_flash_env_addr;
extern unsigned int boot_flash_off;
extern unsigned int boot_flash_sec;
extern unsigned int boot_flash_type;
#endif

/* USB
 * Enable CONFIG_MUSB_HCD for Host functionalities MSC, keyboard
 * Enable CONFIG_MUSB_UDD for Device functionalities.
 */
#define CONFIG_USB_OMAP3		1
#define CONFIG_MUSB_HCD			1
/* #define CONFIG_MUSB_UDC		1 */

#ifdef CONFIG_USB_OMAP3

#ifdef CONFIG_MUSB_HCD
#define CONFIG_CMD_USB

#define CONFIG_USB_STORAGE
#define CONGIG_CMD_STORAGE
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
#define CONFIG_USBD_MANUFACTURER	"Texas Instruments"
#define CONFIG_USBD_PRODUCT_NAME	"UN30"
#endif /* CONFIG_MUSB_UDC */

#endif /* CONFIG_USB_OMAP3 */


#endif /* __CONFIG_H */
