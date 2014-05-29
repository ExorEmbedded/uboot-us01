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
void enable_uart0_pin_mux(void);
void enable_uart1_pin_mux(void);
void enable_i2c0_pin_mux(void);
void enable_board_pin_mux(void);
#endif
