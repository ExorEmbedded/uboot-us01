/*
 * (C) Copyright 2010
 * Texas Instruments, <www.ti.com>
 * Syed Mohammed Khasim <kha...@ti.com>
 *
 * Referred to Linux DSS driver files for OMAP3
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation's version 2 of
 * the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef DSS_H
#define DSS_H

/* GFX registers */
#define DISPC_GFX_BA0							0x48050480
#define DISPC_GFX_POSITION						0x48050488
#define DISPC_GFX_SIZE							0x4805048c
#define DISPC_GFX_ATTRIBUTES					0x480504a0
#define DISPC_GFX_FIFO_THRESHOLD				0x480504a4
#define DISPC_GFX_ROW_INC				 		0x480504ac
#define DISPC_GFX_PIXEL_INC				 		0x480504b0
#define DISPC_GFX_WINDOW_SKIP			 		0x480504b4

/* DSS register addresses */
#define DSS_SYSCONFIG                          0x48050000
#define DSS_CONTROL                            0x48050040

/* CM DSS registers */
#define CM_FCLKEN_DSS						   0x48004e10
#define CM_CLKSEL_DSS						   0x48004e40

/* DISPC register addresses */
#define DISPC_SYSCONFIG                        0x48050410
#define DISPC_SYSSTATUS                        0x48050414
#define DISPC_CONTROL                          0x48050440
#define DISPC_CONFIG                           0x48050444
#define DISPC_DEFAULT_COLOR0                   0x4805044c
#define DISPC_DEFAULT_COLOR1                   0x48050450
#define DISPC_TRANS_COLOR0                     0x48050454
#define DISPC_TRANS_COLOR1                     0x48050458
#define DISPC_TIMING_H                         0x48050464
#define DISPC_TIMING_V                         0x48050468
#define DISPC_POL_FREQ                         0x4805046c
#define DISPC_DIVISOR                          0x48050470
#define DISPC_SIZE_DIG                         0x48050478
#define DISPC_SIZE_LCD                         0x4805047c

/* Few Register Offsets */
#define FRAME_MODE_OFFSET                      1
#define TFTSTN_OFFSET                          3
#define DATALINES_OFFSET                       8

/* Enabling Display controller */
#define LCD_ENABLE                             1
#define DIG_ENABLE                             (1 << 1)
#define GO_LCD                                 (1 << 5)
#define GO_DIG                                 (1 << 6)
#define GP_OUT0                                        (1 << 15)
#define GP_OUT1                                        (1 << 16)

#define DISPC_ENABLE                           (LCD_ENABLE | \
                                                DIG_ENABLE | \
                                                GO_LCD | \
                                                GO_DIG | \
                                                GP_OUT0| \
                                                GP_OUT1)
/* DSS clock manager division ratios */
/* DPLL4 frequency should be 864Mhz */
#define BSP_DSS_CLKSEL_TV           (16 << 8)    // DPLL4/16=54Mhz
#define BSP_DSS_CLKSEL_DSS1        	(6 << 0)     // DSS1 = DPLL4/6 = 144Mhz
#define BSP_CM_CLKSEL_DSS           (BSP_DSS_CLKSEL_TV | BSP_DSS_CLKSEL_DSS1)

struct panel_config {
       u32 xres;
       u32 yres;
       u32 bpp;
       u32 hsy_fp;
       u32 hsy_bp;
       u32 hsy_width;
       u32 hsy_inv;
       u32 vsy_fp;
       u32 vsy_bp;
       u32 vsy_width;
       u32 vsy_inv;
	   u32 blank_inv;
	   u32 pclk_freq;
	   u32 pclk_inv;
};

static inline void dss_write_reg(int reg, u32 val)
{
       __raw_writel(val, reg);
}

static inline u32 dss_read_reg(int reg)
{
       u32 l = __raw_readl(reg);
       return l;
}

#endif /* DSS_H */
