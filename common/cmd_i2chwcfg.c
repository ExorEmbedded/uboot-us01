 /* This file gives support for I2C SEEPROM HW parameters handling
  *
  * See file CREDITS for list of people who contributed to this
  * project.
  *
  * Author: Giovanni Pavoni Sitek S.p.a.
  *
  * --------------- Revision history -----------------------------
  * Author: Giovanni Pavoni Sitek S.p.a.
  * Reason: Added SEEPROM format 3 compatibility.
  *			The i2csavehw command generates a minimal SEEPROM format 3 
  *			data structure and allows to change just display id and 
  *			MACid if valid SEEPROM format 3 still present on SEEPROM.
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation; either version 2 of
  * the License, or (at your option) any later version.
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
  *
  */
 
 
#include <common.h>
#include <command.h>
#include <linux/ctype.h>
#include <i2c.h>

#ifdef CONFIG_CMD_I2CHWCFG

#define ENET_ADDR_LENGTH 6
/*=======================================================================
 * Layout of I2C memory
 *======================================================================= */
#define FACTORY_SECTION_SIZE_V2	128	/* First part of I2C = factory section for V2 and V1*/

/* Header */
#define SIGNATURE1_POS	0
#define SIGNATURE2_POS	1
#define VERSION_POS		2
#define CKSUM_POS		3

/* Data */
#define DISPID_POS		5

#define MACID0_POS		24
#define MACID1_POS		25
#define MACID2_POS		26
#define MACID3_POS		27
#define MACID4_POS		28
#define MACID5_POS		29

/* --------------------- Format 3 specific ------------------------------ */
#define FACTORY_SECTION_SIZE_V3	64	/* First part of I2C = factory section for V3*/
#define BL_TIME_CNT			130
#define BL_TIME_CHK			134
#define SYS_TIME_CNT		136
#define SYS_TIME_CHK		140
#define HWPICKPANELCODE_POS     4

#define ZEROCKSUM			0x57


/*=======================================================================
 * I2C pre-defined / fixed values
 *======================================================================= */
#define SIGNATURE1_VAL	0xaa
#define SIGNATURE2_VAL	0x55
#define VERSION_VAL		3
#define RFU_VAL			0xff

#ifdef CONFIG_CMD_DISPLAYCONFIGXML
  extern int getdisplay(unsigned int lcdid);
#endif


/* ======================================================================
 * Perform HW cfg load from I2C SEEPROM to env vars
 * ====================================================================== */
int i2cgethwcfg (void)
{
  unsigned char buf[FACTORY_SECTION_SIZE_V2];
  unsigned char cksum;
  int i, j;
  char label[40];
  
  /* Reads the I2C contents */
  if (i2c_read(CONFIG_SYS_I2C_HWCFGADD, 0, 1, buf, FACTORY_SECTION_SIZE_V2) != 0)
	return 1;
  
  /* Checksum check */
  if(buf[VERSION_POS] < 3) 
	j = FACTORY_SECTION_SIZE_V2;
  else
	j = FACTORY_SECTION_SIZE_V3;
  
  cksum = 1;
  for(i = CKSUM_POS + 1; i < j; i ++)
	cksum += buf[i];
  cksum -= 0xaa;
  
  if(buf[CKSUM_POS] != cksum)
  {
	puts ("I2C SEEPROM checksum error: HW cfg not valid\n");
	return 1;
  }
  
  /* Signature check */
  if( (buf[SIGNATURE1_POS] != SIGNATURE1_VAL) || (buf[SIGNATURE2_POS] != SIGNATURE2_VAL) )
  {
	puts ("I2C SEEPROM invalid signature: HW cfg not valid\n");
	return 1;
  }
	
  /* Get display number */
#ifdef CONFIG_SYS_I2C_ADPADD
  /* If display ID=0xff, get it from the ADP board */
  if(buf[DISPID_POS] == 0xff)
	  if (i2c_read(CONFIG_SYS_I2C_ADPADD, DISPID_POS, 1, &(buf[DISPID_POS]), 1) != 0)
		return 1;
#endif
  sprintf(label, "%u", buf[DISPID_POS]);
  setenv("hw_dispid", label); 
#ifdef CONFIG_CMD_DISPLAYCONFIGXML
  getdisplay(buf[DISPID_POS]);
#endif
  
  /* *** Now gets datas for version >= 2 of the SEEPROM *** */
  if(buf[VERSION_POS] < 2) 
  {
	puts ("I2C SEEPROM WARNING: old version 1 found\n");
	return 1;
  }

  /* get eth mac ID */
  eth_setenv_enetaddr("ethaddr", &(buf[MACID0_POS]));
  
  /* get boardhwcode */
  sprintf(label, "%u", buf[HWPICKPANELCODE_POS]);
  setenv("boardhwcode", label); 
  
  return 0;
}
  

/* ======================================================================
 * Performs the HW cfg store to I2C SEEPROM
 * ====================================================================== */
int do_i2csavehw ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
  unsigned char buf[FACTORY_SECTION_SIZE_V3];
  unsigned long i;
  char* tmp, * end;
  unsigned char dispid;
  u8 hw_addr[ENET_ADDR_LENGTH];
  unsigned char cksum;
  unsigned char n;

  if (argc>1) 
  {
	puts ("ERROR: Too many input parameters!\n");
	return 1;
  }
  
  /* Initializes the buffer ... uses still existing SEEPROM datas, if valid */

  /* Reads the I2C contents ...*/
  i2c_read(CONFIG_SYS_I2C_HWCFGADD, 0, 1, buf, FACTORY_SECTION_SIZE_V3);
  /* ...and verifies chksum for format 3*/
  cksum = 1;
  for(i = CKSUM_POS + 1; i < FACTORY_SECTION_SIZE_V3; i ++)
	cksum += buf[i];
  cksum -= 0xaa;
  
  if(buf[CKSUM_POS] != cksum)
  {
	puts ("WARNING: No SEEPROM format 3 found ... initializing  ...\n");
	for(i=0; i < FACTORY_SECTION_SIZE_V3; i++)
	  buf[i] = RFU_VAL;
  }
  
  /* Sets the pre-defined fields of buffer */
  buf[SIGNATURE1_POS] = SIGNATURE1_VAL;
  buf[SIGNATURE2_POS] = SIGNATURE2_VAL;
  buf[VERSION_POS] = VERSION_VAL;
  
  /* Now copy into buffer the datas from env vars */ 
  
  /* Display ID handling */
  tmp = getenv("hw_dispid");
  if(!tmp)
  {
	puts ("ERROR: 'hw_dispid' environment var not found!\n");
	return 1;
  }
  i = simple_strtoul (tmp, NULL, 10);
  dispid = (unsigned char)(i & 0xff);
  buf[DISPID_POS] = dispid;
  
  /* Eth mac address handling */
  if (eth_getenv_enetaddr("ethaddr", hw_addr))
  {
	buf[MACID0_POS] = hw_addr[0];
	buf[MACID1_POS] = hw_addr[1];
	buf[MACID2_POS] = hw_addr[2];
	buf[MACID3_POS] = hw_addr[3];
	buf[MACID4_POS] = hw_addr[4];
	buf[MACID5_POS] = hw_addr[5];
  }
  else
  {
	puts ("ERROR: 'ethaddr' environment var not found!\n");
	return 1;
  }
  
  /* Calculate and copies checksum into buffer */
  cksum = 1;
  for(i = CKSUM_POS + 1; i < FACTORY_SECTION_SIZE_V3; i ++)
	cksum += buf[i];
  cksum -= 0xaa;
  
  buf[CKSUM_POS] = cksum;
  
  /* Now write buffer into SEEPROM */
  puts ("Writing HW configuration to I2C SEEPROM ...\n");
  for(n=0; n < FACTORY_SECTION_SIZE_V3; n++)
	i2c_write (CONFIG_SYS_I2C_HWCFGADD, n, 1, &(buf[n]), 1);
    
}


/* ====================================================================== */

U_BOOT_CMD(
	i2csavehw,      2,      0,      do_i2csavehw,
	" Stores display id and MAC id into I2C SEEPROM (format v3) keeping other params unchanged.\n",
	" If no valid SEEPROM 3 datas are found, SEEPROM is initialized with defaults\n"
);


#endif	/* CONFIG_CMD_I2CHWCFG */

