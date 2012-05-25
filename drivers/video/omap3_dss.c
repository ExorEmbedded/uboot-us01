/* Written by Giovanni Pavoni Sitek S.p.a.
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

#include <common.h>
#include <asm/io.h>
#include <asm/arch/dss.h>

/*
 * Configure Panel Specific parameters
 */
void omap3_dss_panel_config(const struct panel_config *panel_cfg)
{
  u32 divisor;
  u32 timing_h;
  u32 timing_v;
  u32 pol_freq;
  u32 lcd_size;
  u32 best_ferr, freq;
  u32 i,j;
  u32 CM_CLKSEL_DSS_var;

  /* Set parameters for pixel clock */
  best_ferr =0xffffffff;
  for(i=6; i<10; i++)
	for(j=2; j<32; j++)
	{
	  freq = 864000000 / i / j;
	  if(freq >= (1000000 * panel_cfg->pclk_freq))
		if((freq - (1000000 * panel_cfg->pclk_freq)) < best_ferr)
		{
		  best_ferr = (freq - (1000000 * panel_cfg->pclk_freq));
		  divisor = j;
		  CM_CLKSEL_DSS_var = i;
		}
	}
  
  divisor |= 0x10000;

  dss_write_reg(CM_FCLKEN_DSS, 0); 				/* Set DSS1 frequency */
  dss_write_reg(CM_CLKSEL_DSS, CM_CLKSEL_DSS_var | BSP_DSS_CLKSEL_TV);
  dss_write_reg(CM_FCLKEN_DSS, 1);
    
  /* Set timings for hsync */
  timing_h = (panel_cfg-> hsy_width) - 1;
  timing_h |= (((panel_cfg-> hsy_fp) - 1) << 8);
  timing_h |= (((panel_cfg-> hsy_bp) - 1) << 20);

  /* Set timings for vsync */
  timing_v = (panel_cfg-> vsy_width) - 1;
  timing_v |= (((panel_cfg-> vsy_fp)) << 8);
  timing_v |= (((panel_cfg-> vsy_bp)) << 20);
  
  /* set polarity of signals */
  pol_freq = 0x00023000;
  if(panel_cfg-> hsy_inv)  pol_freq |= (1<<13);
  if(panel_cfg-> vsy_inv)  pol_freq |= (1<<12);
  if(panel_cfg-> pclk_inv) pol_freq |= (1<<14);
  if(panel_cfg-> blank_inv) pol_freq |= (1<<15);
  
  /* set lcd size */
  lcd_size = (panel_cfg-> xres) - 1;
  lcd_size |= (((panel_cfg-> yres) - 1) << 16);
  
  /* Write registers */
  dss_write_reg(DISPC_TIMING_H, timing_h);
  dss_write_reg(DISPC_TIMING_V, timing_v);
  dss_write_reg(DISPC_POL_FREQ, pol_freq);
  dss_write_reg(DISPC_DIVISOR,  divisor);
  dss_write_reg(DISPC_SIZE_LCD, lcd_size);
  dss_write_reg(DISPC_CONFIG,(0x02 << FRAME_MODE_OFFSET));
#ifdef CONFIG_UN31
  dss_write_reg(DISPC_CONTROL, ((0x01 << TFTSTN_OFFSET) | (0x01 << DATALINES_OFFSET)));	/* 16 bit */
#else
  dss_write_reg(DISPC_CONTROL, ((0x01 << TFTSTN_OFFSET) | (0x02 << DATALINES_OFFSET))); /* 18 bit */
#endif
}

/*
 * Enable LCD and DIGITAL OUT in DSS
 */
void omap3_dss_enable(void)
{
       u32 tmp = 0;

       tmp = dss_read_reg(DISPC_CONTROL);
       tmp |= DISPC_ENABLE;

       dss_write_reg(DISPC_CONTROL, tmp);
}

/*
 * Set Background Color in DISPC
 */
void omap3_dss_set_background_col(u32 bkcol)
{
       dss_write_reg(DISPC_DEFAULT_COLOR0, bkcol);
}

/* 
 * Gets LCD configuration vars from the environment
 */
void omap3_dss_panel_getenv(struct panel_config *panel_cfg)
{
  char *addr;
  char *end;

  /* Get lcd_size */
  addr = getenv("lcd_size");
  if(addr)
  {
	panel_cfg -> xres = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> yres = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> bpp = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }

  /* Get lcd_hsy */
  addr = getenv("lcd_hsy");
  if(addr)
  {
	panel_cfg -> hsy_width = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> hsy_fp = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> hsy_bp = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> hsy_inv = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }

  /* Get lcd_vsy */
  addr = getenv("lcd_vsy");
  if(addr)
  {
	panel_cfg -> vsy_width = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> vsy_fp = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> vsy_bp = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> vsy_inv = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }

/* Get lcd_blank */
  addr = getenv("lcd_blank");
  if(addr)
  {
	panel_cfg -> blank_inv = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  
  /* Get lcd_pclk */
  addr = getenv("lcd_pclk");
  if(addr)
  {
	panel_cfg -> pclk_freq = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
  if(addr)
  {
	panel_cfg -> pclk_inv = simple_strtoul(addr, &end, 10);
	addr = (*end) ? end + 1 : end;
  }
}  


/*
 * Places the Splash image on the LCD display
 */
void omap3_dss_panel_splash(const struct panel_config *panel_cfg)
{
  u32  splash_width;
  u32  splash_height;
  u32  splash_posx = 0;
  u32  splash_posy = 0;
  u32  framebuffer = CONFIG_VIDEO_MEM_ADDR;
  u32  splashaddr = CONFIG_SPLASH_ADDR;
  u32* header = (u32*)CONFIG_SYS_LOAD_ADDR;
  
  #define SPLASH_HDRLEN  		56
  #define SPLASH_STRIDE_IDX		0
  #define SPLASH_SIZE_IDX		2
  
  /* 
   * Gets the header of the SPLASH image and calculates the dimensions of the stored image
   */
#if defined(CONFIG_AMBA_FLASH)
  flash_read((ulong) splashaddr, (ulong *)header, (ulong)(SPLASH_HDRLEN / 4));
#else
  header = (u32*)splashaddr;
#endif
  splash_width = (header[SPLASH_STRIDE_IDX]) / 2 + 1;
  if (splash_width > (panel_cfg->xres)) return;
  if (splash_width < 10) return;
  splash_height = (((header[SPLASH_SIZE_IDX]) / 2) / splash_width);
  if (splash_height > (panel_cfg->yres)) splash_height = (panel_cfg->yres);
  if (splash_height < 10) return;

  /*
   * loads the framebuffer with the SPLASH bitmap
   */
#if defined(CONFIG_AMBA_FLASH)
  flash_read((ulong) (splashaddr + SPLASH_HDRLEN), (ulong *)framebuffer, (ulong)((splash_height * splash_width) / 2) + 1);
#else
  memcpy((char*)framebuffer, (char*)(splashaddr + SPLASH_HDRLEN), (splash_height * splash_width));
#endif
    
  /* calculates the position of the splash inside the display */
  if((panel_cfg->xres) > splash_posx) splash_posx = ((panel_cfg->xres) - splash_width) / 2;
  if((panel_cfg->yres) > splash_posy) splash_posy = ((panel_cfg->yres) - splash_height) / 2;
  
  /* Configure Graphics Window (centered on LCD, 16 bit pixel*/
  dss_write_reg(DISPC_GFX_BA0, framebuffer);
  dss_write_reg(DISPC_GFX_POSITION, (splash_posx | (splash_posy << 16)));
  dss_write_reg(DISPC_GFX_SIZE, ((splash_width - 1) | ((splash_height - 1) << 16)));
  dss_write_reg(DISPC_GFX_ATTRIBUTES, 0x0000000d);
  dss_write_reg(DISPC_GFX_FIFO_THRESHOLD, 0x00fc00c0 );
  dss_write_reg(DISPC_GFX_ROW_INC, 0x00000001 );
  dss_write_reg(DISPC_GFX_PIXEL_INC, 0x00000001 );
  dss_write_reg(DISPC_GFX_WINDOW_SKIP, 0x00000000 );
}
 
  
