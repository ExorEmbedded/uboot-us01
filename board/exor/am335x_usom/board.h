/*
 * board.h
 *
 * uS01 AM335x boards information header
 *
 * Copyright (C) 2014 Exor S.p.a.
 *
 * Based on:
 * u-boot:/board/ti/am335x/board.h
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/* Handled HW codes */
#define ETOP507_VAL  104
#define ETOP507G_VAL 105
#define ECO_VAL      110
#define PLCM07_VAL   111
#define ETOP705_VAL  120
#define EXWARE_VAL   127
#define NA16_VAL     128
#define AB19_VAL     140

/* System GPIOs */
#define RXEN0_GPIO 37
#define DXEN0_GPIO 38
#define MODE0_GPIO 39
#define GNLED_GPIO 66
#define RDLED_GPIO 69
#define ESDBY_GPIO 65
#define E3W3S_GPIO 103

void enable_uart0_pin_mux(void);
void enable_uart1_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_board_pin_mux(void);
void enable_rmii2_pin_mux(void);
#endif
