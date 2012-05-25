 /* This file allows handling display parameters based on xml descriptor file
  *
  * See file CREDITS for list of people who contributed to this
  * project.
  *
  * Author: Giovanni Pavoni Sitek S.p.a.
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


#ifdef CONFIG_CMD_DISPLAYCONFIGXML
/* ======================================================================
 * XML file definitions
 * ====================================================================== */

#define MAX_FIELDLEN	0x2000
#define MAX_DISPLAYID	99

#define DBLABEL			"DB_DispID"

struct lcd_record 
{
	unsigned int display_id;
	unsigned int resx;
	unsigned int resy;
	unsigned int bpp;
	unsigned int hs_w;
	unsigned int hs_fp;
	unsigned int hs_bp;
	unsigned int hs_inv;
	unsigned int vs_w;
	unsigned int vs_fp;
	unsigned int vs_bp;
	unsigned int vs_inv;
	unsigned int ena_inv;
	unsigned int pclk_freq;
	unsigned int pclk_inv;
};

/* Helper function for adding the current LCD description to env database
 */
static void lcd_add_database(struct lcd_record* p)
{
  char buf[100];
  char label[20];
  
  if((p->display_id) > MAX_DISPLAYID) return;
  
  /* Builds the label */
  sprintf(label,"DB_DispID%u", p->display_id);
  
  /* Builds the data */
  sprintf(buf,"%u %u %u #%u %u %u %u #%u %u %u %u #%u #%u %u", 
		   p->resx,
		   p->resy,
		   p->bpp,
		   p->hs_w,
		   p->hs_fp,
		   p->hs_bp,
		   p->hs_inv,
		   p->vs_w,
		   p->vs_fp,
		   p->vs_bp,
		   p->vs_inv,
		   p->ena_inv,
		   p->pclk_freq,
		   p->pclk_inv);
	
  /* Adds the DB item to the environment */
  setenv(label, buf);
}

/* Helper function for finding a string inside a memory area
 * addr = base address of the memory area to scan
 * len = length of the memory area to scan
 * st: The string to search for
 * NOTE: On success it returns the pointer to the last char of the
 *       string inside the searched memory area
 */
static char* findstr(const char* addr,unsigned long len,const char * st)
{
	const char *sc;
	char found = 0;
	int i;
	
	for( sc = addr; sc < (addr + len); sc++) 
	{
		found = 1;
		for(i = 0; i < strlen(st); i++)
		  if(sc[i] != st[i])
		  {
			found = 0;
			break;
		  }
		if(found) return (char*)(sc + strlen(st));
	}
	return NULL;
}

/* Helper function returning the pointer to the first numeric char
 * starting from addr, inside the len range.
 */
static char* findnum(const char* addr, unsigned long len)
{
  unsigned long i;
  
  for(i=0; i<len; i++)
	if((addr[i]>= '0') && (addr[i]<='9'))
	  return (char*)(addr+i);
	
  return NULL;
}

/* ----------------------------------------------------------------------
 * Helper function for parsing lcd datas. It parses datas of the first display found
 * addr = address in memory where to start parsing
 * plcd = pointer to struct containing lcd parsed params
 * Returns pointer to the end of parsed area if SUCCESS, NULL otherwise
 * ---------------------------------------------------------------------- */
static char* parsexml(unsigned long addr, struct lcd_record* plcd)
{
  char* pnt1, *pnt2;
 
  /* ===== Search for "Display Number" item */
  pnt1 = findstr((char*)addr, MAX_FIELDLEN, "Display Number=");
  if(!pnt1) return NULL;
  pnt1=findnum(pnt1,5);
  if(!pnt1) return NULL;
  plcd->display_id = simple_strtoul(pnt1,NULL,10);
  
  /* ===== Search for "Resolution" key */
  pnt1 = findstr(pnt1, 200, "Resolution");
  if(!pnt1) return NULL;
  /* "x=" */
  pnt2 = findstr(pnt1, 20, "x=");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->resx = simple_strtoul(pnt2,NULL,10);
  /* "y=" */
  pnt2 = findstr(pnt1, 20, "y=");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->resy = simple_strtoul(pnt2,NULL,10);
  
  /* ===== Search for "PixelClock" key */
  pnt1 = findstr(pnt1, 2000, "PixelClock");
  if(!pnt1) return NULL;
  /* "inverted=" */
  pnt2 = findstr(pnt1, 20, "inverted=");
  if(!pnt2) return NULL;
  pnt2=findstr(pnt2, 5, "true");
  if(!pnt2) 
	plcd->pclk_inv = 0;
  else
	plcd->pclk_inv = 1;
  /* "freq=" */
  pnt2 = findstr(pnt1, 40, "freq=");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->pclk_freq = simple_strtoul(pnt2,NULL,10);
  
  /* ===== Search for "Hsync" key */
  pnt1 = findstr(pnt1, 200, "Hsync");
  if(!pnt1) return NULL;
  /* "active=" */
  pnt2 = findstr(pnt1, 20, "active=");
  if(!pnt2) return NULL;
  pnt2=findstr(pnt2, 5, "lo");
  if(!pnt2) 
	plcd->hs_inv = 0;
  else
	plcd->hs_inv = 1;
  /* "Duration" */
  pnt2 = findstr(pnt1, 200, "Duration");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->hs_w = simple_strtoul(pnt2,NULL,10);
  /* "FrontPorch" */
  pnt2 = findstr(pnt1, 200, "FrontPorch");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->hs_fp = simple_strtoul(pnt2,NULL,10);
  /* "BackPorch" */
  pnt2 = findstr(pnt1, 200, "BackPorch");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->hs_bp = simple_strtoul(pnt2,NULL,10);

  /* ===== Search for "Vsync" key */
  pnt1 = findstr(pnt1, 200, "Vsync");
  if(!pnt1) return NULL;
  /* "active=" */
  pnt2 = findstr(pnt1, 20, "active=");
  if(!pnt2) return NULL;
  pnt2=findstr(pnt2, 5, "lo");
  if(!pnt2) 
	plcd->vs_inv = 0;
  else
	plcd->vs_inv = 1;
  /* "Duration" */
  pnt2 = findstr(pnt1, 200, "Duration");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->vs_w = simple_strtoul(pnt2,NULL,10);
  /* "FrontPorch" */
  pnt2 = findstr(pnt1, 200, "FrontPorch");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->vs_fp = simple_strtoul(pnt2,NULL,10);
  /* "BackPorch" */
  pnt2 = findstr(pnt1, 200, "BackPorch");
  if(!pnt2) return NULL;
  pnt2=findnum(pnt2,5);
  if(!pnt2) return NULL;
  plcd->vs_bp = simple_strtoul(pnt2,NULL,10);

  /* ===== Search for "Blank" key */
  pnt1 = findstr(pnt1, 200, "Blank");
  if(!pnt1) return NULL;
  /* "active=" */
  pnt2 = findstr(pnt1, 20, "active=");
  if(!pnt2) return NULL;
  pnt2=findstr(pnt2, 5, "lo");
  if(!pnt2) 
	plcd->ena_inv = 0;
  else
	plcd->ena_inv = 1;
 
  /* ===== Search for "Color bpp" key */
  pnt1 = findstr(pnt1, 200, "Color bpp=");
  if(!pnt1) return NULL;
  pnt2=findnum(pnt1,5);
  if(!pnt2) return NULL;
  plcd->bpp = simple_strtoul(pnt2,NULL,10);
  
  /* ===== Search for closing of display params */
  pnt1 = findstr(pnt1, 200, "/DisplayParams");
  return pnt1;
}

/* ======================================================================
 * Interpreter command to perform parsing of displayconfig.xml file
 * and generating of lcd database into environment
 * ====================================================================== */
int do_xmlread ( cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned long addr;			/* Address of loaded xml file  */
	struct lcd_record record;
	char* pnt;
	char* tmp;

	/*
	 * Check the loadaddr variable.
	 * If we don't know where the image is then we're done.
	 */
	if (argc>1) 
	{
		addr = simple_strtoul (argv[1], NULL, 16);
	} 
	else if ((tmp = getenv ("loadaddr")) != NULL) 
	{
		addr = simple_strtoul (tmp, NULL, 16);
	} 
	else 
	{
		puts ("No load address provided!\n");
		return 1;
	}

	/*
	 * Executes parsing for all contained Display identifiers
	 */
	printf ("## Parsing displayconfig.xml file ...\n");
	pnt = (char*)addr;
	while(pnt != NULL)
	{
	 pnt = parsexml(addr, &record);
	 if(pnt != NULL)
	  {
		printf ("Display Number: %d found. Adding to database.\n", record.display_id);
		lcd_add_database(&record);
		addr = (unsigned long) pnt;
	  }
	}
	printf ("Done \n");
	return 0;
}


/* ======================================================================
 * This function searches for the selected display into env database 
 * and if found sets the lcd environment parameters accordingly
 * NOTE: Returns 0 if lcdid not found
 * ====================================================================== */
int getdisplay(unsigned int lcdid)
{
  char* tmp;
  char label[20];
  char buf[100];
  char* p1, *p2;
  
  if(lcdid > MAX_DISPLAYID) return 0;
  
  /* Builds the label and searches the DB entry into environment*/
  sprintf(label,"DB_DispID%u", lcdid);
  tmp = getenv(label);
  if(tmp == NULL) return 0;
  strcpy(buf,tmp);
  
  /* lcd_size variable */
  p1 = buf;
  p2 = strstr(p1,"#");
  if(!p2)return 0;
  *p2 = '\0';
  setenv("lcd_size",p1);
  
  /* lcd_hsy variable */
  p1 = p2 + 1;
  p2 = strstr(p1,"#");
  if(!p2)return 0;
  *p2 = '\0';
  setenv("lcd_hsy",p1);
  
  /* lcd_vsy variable */
  p1 = p2 + 1;
  p2 = strstr(p1,"#");
  if(!p2)return 0;
  *p2 = '\0';
  setenv("lcd_vsy",p1);
  
  /* lcd_blank variable */
  p1 = p2 + 1;
  p2 = strstr(p1,"#");
  if(!p2)return 0;
  *p2 = '\0';
  setenv("lcd_blank",p1);

  /* lcd_pclk variable */
  p1 = p2 + 1;
  if(!p1)return 0;
  setenv("lcd_pclk",p1);
  
  return 1;
}


/* ====================================================================== */

U_BOOT_CMD(
	lcdxmlparse,      2,      0,      do_xmlread,
	" Parses a loaded displayconfig.xml file and builds lcd batabase into env.\n",
	" [address] - load address of file to parse.\n"
);


#endif	/* CONFIG_CMD_DISPLAYCONFIGXML */

