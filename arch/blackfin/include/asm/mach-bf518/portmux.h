/*
 * Copyright 2008-2009 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later
 */

#ifndef _MACH_PORTMUX_H_
#define _MACH_PORTMUX_H_

#define MAX_RESOURCES	MAX_BLACKFIN_GPIOS

/* EMAC MII/RMII Port Mux */
#define P_MII0_ETxD2	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(0))
#define P_MII0_ERxD2	(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(0))
#define P_MII0_ETxD3	(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(0))
#define P_MII0_ERxD3	(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(0))
#define P_MII0_ERxCLK	(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(0))
#define P_MII0_ERxDV	(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(0))
#define P_MII0_COL	(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(0))

#define P_MII0_MDC	(P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(0))
#define P_MII0_MDIO	(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(0))
#define P_MII0_ETxD0	(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(0))
#define P_MII0_ERxD0	(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(0))
#define P_MII0_ETxD1	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(0))
#define P_MII0_ERxD1	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(0))
#define P_MII0_ETxEN	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(0))
#define P_MII0_PHYINT	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(0))
#define P_MII0_CRS	(P_DEFINED | P_IDENT(GPIO_PG0) | P_FUNCT(0))
#define P_MII0_ERxER	(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(0))
#define P_MII0_TxCLK	(P_DEFINED | P_IDENT(GPIO_PG2) | P_FUNCT(0))

#define P_MII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxD2, \
	P_MII0_ETxD3, \
	P_MII0_ETxEN, \
	P_MII0_TxCLK, \
	P_MII0_PHYINT, \
	P_MII0_COL, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_ERxD2, \
	P_MII0_ERxD3, \
	P_MII0_ERxDV, \
	P_MII0_ERxCLK, \
	P_MII0_ERxER, \
	P_MII0_CRS, \
	P_MII0_MDC, \
	P_MII0_MDIO, 0}

#define P_RMII0 {\
	P_MII0_ETxD0, \
	P_MII0_ETxD1, \
	P_MII0_ETxEN, \
	P_MII0_ERxD0, \
	P_MII0_ERxD1, \
	P_MII0_ERxER, \
	P_MII0_TxCLK, \
	P_MII0_PHYINT, \
	P_MII0_CRS, \
	P_MII0_MDC, \
	P_MII0_MDIO, 0}

/* PPI Port Mux */
#define P_PPI0_D0	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(1))
#define P_PPI0_D1	(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(1))
#define P_PPI0_D2	(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(1))
#define P_PPI0_D3	(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(1))
#define P_PPI0_D4	(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(1))
#define P_PPI0_D5	(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(1))
#define P_PPI0_D6	(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(1))
#define P_PPI0_D7	(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(1))
#define P_PPI0_D8	(P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(1))
#define P_PPI0_D9	(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(1))
#define P_PPI0_D10	(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(1))
#define P_PPI0_D11	(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(1))
#define P_PPI0_D12	(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(1))
#define P_PPI0_D13	(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(1))
#define P_PPI0_D14	(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(1))
#define P_PPI0_D15	(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(1))

#define P_PPI0_CLK	(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(1))
#define P_PPI0_FS1	(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(1))
#define P_PPI0_FS2	(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(1))
#define P_PPI0_FS3	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(1))

/* SPI Port Mux */
#define P_SPI0_SS	(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(0))
#define P_SPI0_SCK	(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(0))
#define P_SPI0_MISO	(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(0))
#define P_SPI0_MOSI	(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(0))

#define P_SPI0_SSEL1	(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(0))
#define P_SPI0_SSEL2	(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(0))
#define P_SPI0_SSEL3	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(2))
#define P_SPI0_SSEL4	(P_DEFINED | P_IDENT(GPIO_PH8) | P_FUNCT(2))
#define P_SPI0_SSEL5	(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(2))

#define P_SPI1_SS	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(1))
#define P_SPI1_SCK	(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(1))
#define P_SPI1_MISO	(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(1))
#define P_SPI1_MOSI	(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(1))

#define P_SPI1_SSEL1	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(2))
#define P_SPI1_SSEL2	(P_DEFINED | P_IDENT(GPIO_PF0) | P_FUNCT(2))
#define P_SPI1_SSEL3	(P_DEFINED | P_IDENT(GPIO_PG0) | P_FUNCT(2))
#define P_SPI1_SSEL4	(P_DEFINED | P_IDENT(GPIO_PF8) | P_FUNCT(2))
#define P_SPI1_SSEL5	(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(2))

#define GPIO_DEFAULT_BOOT_SPI_CS GPIO_PG15
#define P_DEFAULT_BOOT_SPI_CS P_SPI0_SSEL2

/* SPORT Port Mux */
#define P_SPORT0_DRPRI	(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(0))
#define P_SPORT0_RSCLK	(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(0))
#define P_SPORT0_RFS	(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(0))
#define P_SPORT0_TFS	(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(0))
#define P_SPORT0_DTPRI	(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(0))
#define P_SPORT0_TSCLK	(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(0))
#define P_SPORT0_DTSEC	(P_DEFINED | P_IDENT(GPIO_PG9) | P_FUNCT(0))
#define P_SPORT0_DRSEC	(P_DEFINED | P_IDENT(GPIO_PG10) | P_FUNCT(0))

#define P_SPORT1_DRPRI	(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(0))
#define P_SPORT1_RFS	(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(0))
#define P_SPORT1_RSCLK	(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(0))
#define P_SPORT1_DTPRI	(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(0))
#define P_SPORT1_TFS	(P_DEFINED | P_IDENT(GPIO_PH4) | P_FUNCT(0))
#define P_SPORT1_TSCLK	(P_DEFINED | P_IDENT(GPIO_PH5) | P_FUNCT(0))
#define P_SPORT1_DTSEC	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(0))
#define P_SPORT1_DRSEC	(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(0))

/* UART Port Mux */
#define P_UART0_TX	(P_DEFINED | P_IDENT(GPIO_PG9) | P_FUNCT(1))
#define P_UART0_RX	(P_DEFINED | P_IDENT(GPIO_PG10) | P_FUNCT(1))

#define P_UART1_TX	(P_DEFINED | P_IDENT(GPIO_PH6) | P_FUNCT(1))
#define P_UART1_RX	(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(1))

/* Timer */
#define P_TMRCLK	(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(2))
#define P_TMR0		(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(2))
#define P_TMR1		(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(2))
#define P_TMR2		(P_DEFINED | P_IDENT(GPIO_PF9) | P_FUNCT(2))
#define P_TMR3		(P_DEFINED | P_IDENT(GPIO_PF10) | P_FUNCT(2))
#define P_TMR4		(P_DEFINED | P_IDENT(GPIO_PG9) | P_FUNCT(2))
#define P_TMR5		(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(2))
#define P_TMR6		(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(2))
#define P_TMR7		(P_DEFINED | P_IDENT(GPIO_PH7) | P_FUNCT(2))

/* DMA */
#define P_DMAR1		(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(1))
#define P_DMAR0		(P_DEFINED | P_IDENT(GPIO_PG2) | P_FUNCT(1))

/* TWI */
#define P_TWI0_SCL	(P_DONTCARE)
#define P_TWI0_SDA	(P_DONTCARE)

/* PWM */
#define P_PWM0_AH		(P_DEFINED | P_IDENT(GPIO_PF1) | P_FUNCT(2))
#define P_PWM0_AL		(P_DEFINED | P_IDENT(GPIO_PF2) | P_FUNCT(2))
#define P_PWM0_BH		(P_DEFINED | P_IDENT(GPIO_PF3) | P_FUNCT(2))
#define P_PWM0_BL		(P_DEFINED | P_IDENT(GPIO_PF4) | P_FUNCT(2))
#define P_PWM0_CH		(P_DEFINED | P_IDENT(GPIO_PF5) | P_FUNCT(2))
#define P_PWM0_CL		(P_DEFINED | P_IDENT(GPIO_PF6) | P_FUNCT(2))
#define P_PWM0_SYNC		(P_DEFINED | P_IDENT(GPIO_PF7) | P_FUNCT(2))

#define P_PWM1_AH		(P_DEFINED | P_IDENT(GPIO_PF11) | P_FUNCT(2))
#define P_PWM1_AL		(P_DEFINED | P_IDENT(GPIO_PF12) | P_FUNCT(2))
#define P_PWM1_BH		(P_DEFINED | P_IDENT(GPIO_PF13) | P_FUNCT(2))
#define P_PWM1_BL		(P_DEFINED | P_IDENT(GPIO_PF14) | P_FUNCT(2))
#define P_PWM1_CH		(P_DEFINED | P_IDENT(GPIO_PG1) | P_FUNCT(2))
#define P_PWM1_CL		(P_DEFINED | P_IDENT(GPIO_PG2) | P_FUNCT(2))
#define P_PWM1_SYNC		(P_DEFINED | P_IDENT(GPIO_PF15) | P_FUNCT(2))

#define P_PWM_TRIPB		(P_DEFINED | P_IDENT(GPIO_PG14) | P_FUNCT(2))

/* RSI */
#define P_RSI_DATA0		(P_DEFINED | P_IDENT(GPIO_PG3) | P_FUNCT(1))
#define P_RSI_DATA1		(P_DEFINED | P_IDENT(GPIO_PG4) | P_FUNCT(1))
#define P_RSI_DATA2		(P_DEFINED | P_IDENT(GPIO_PG5) | P_FUNCT(1))
#define P_RSI_DATA3		(P_DEFINED | P_IDENT(GPIO_PG6) | P_FUNCT(1))
#define P_RSI_DATA4		(P_DEFINED | P_IDENT(GPIO_PH0) | P_FUNCT(2))
#define P_RSI_DATA5		(P_DEFINED | P_IDENT(GPIO_PH1) | P_FUNCT(2))
#define P_RSI_DATA6		(P_DEFINED | P_IDENT(GPIO_PH2) | P_FUNCT(2))
#define P_RSI_DATA7		(P_DEFINED | P_IDENT(GPIO_PH3) | P_FUNCT(2))
#define P_RSI_CMD		(P_DEFINED | P_IDENT(GPIO_PG7) | P_FUNCT(1))
#define P_RSI_CLK		(P_DEFINED | P_IDENT(GPIO_PG8) | P_FUNCT(1))

/* PTP */
#define P_PTP_PPS		(P_DEFINED | P_IDENT(GPIO_PG12) | P_FUNCT(2))
#define P_PTP_CLKOUT		(P_DEFINED | P_IDENT(GPIO_PG13) | P_FUNCT(2))

/* AMS */
#define P_AMS2			(P_DEFINED | P_IDENT(GPIO_PG11) | P_FUNCT(1))
#define P_AMS3			(P_DEFINED | P_IDENT(GPIO_PG15) | P_FUNCT(2))

#define P_HWAIT			(P_DEFINED | P_IDENT(GPIO_PG0) | P_FUNCT(1))

#endif				/* _MACH_PORTMUX_H_ */
