//==========================================================================
//
//      bootx.c
//
//      Implementation of bootx sequence
//
//==========================================================================
//
// Copyright (C) 2010 Exor International, Inc.
// All Rights Reserved.
// -------------------------------------------
//
// Author(s):          Abdul Nizar
// Contributors:
// Date:                2010-09-01
// Purpose:             UNxx Boot sequence
// Description:         This is an implementation of boot system contain two os images (main and config).
//                      the images are stored in flash binary partition.
//
//==========================================================================

/* #define DEBUG */

#include <common.h>
#include <flash.h>
#include <bootx.h>
#include <touch.h>
#include <display_print.h>
#include <mmc.h>
#include <fat.h>

#define XFIS_MAX_ENTRIES  4
#define XFIS_ENTRY_SIZE   32
#define IMAGE_HDR_SIZE    32
#define MAX_BOOTARG_LEN   512
#define LINUXOSIMAGETYPE  2

char xfis_table[XFIS_MAX_ENTRIES*XFIS_ENTRY_SIZE];
struct xfis_entry  xfis_entry;

extern int gunzip (void *dst, int dstlen, unsigned char *src, unsigned long *lenp);

#ifdef CONFIG_UN31P_CAREL
extern int breakbootstring();
#endif

#ifdef CONFIG_BOOTX
/*-----------------------------------------------------------------------
 * Functions
 */

/*-----------------------------------------------------------------------
 * Check if the OS to boot is a WCE6 or Linux image, based on the OS
 * header.
 * The first byte of the header indicates the OS type (0=WCE5, 1=WCE6, 2=Linux)
 *
 * Input: char* os_header = pointer to OS header
 * Output: 1= Linux Image, 0=WCE image
 */
static int IsLinuxImage(char* os_header)
{
  if(*os_header != LINUXOSIMAGETYPE) return 0;
  
  /* In this case this is a Linux image */
  return 1;
}

/*-----------------------------------------------------------------------
 * This function is responsible of booting the Linux image, which has
 * been previously loaded into RAM, at address CONFIG_SYS_LOAD_ADDR
 *
 * It performs the folllowing sequence:
 * 1) Completes the bootargs, based on environment variables, which are board specific (MAC address, carrier id, display id)
 * 2) Boots Linux (bootm) at address CONFIG_SYS_LOAD_ADDR
 *
 * NOTE: Should not return if Linux image is valid (CRC32 is Ok)
 */
static void DoStartLinux()
{
  char  bootarguments[MAX_BOOTARG_LEN] = "";	/* Contains the complete booargs string */
  char  tmpstr[30];				/* Tmp string using for conversions */
  char* envval;					/* Pointer to the retriven environment variable value */
  unsigned long i;				/* Tmp long int, used for conversions */
    
  /* ------------Building the bootargs---------------
  /* 1) Append the CONFIG_BOOTARGS string, if defined */
#ifdef CONFIG_BOOTARGS
  strcpy(bootarguments,CONFIG_BOOTARGS); 
#endif

  /* 2) Append the "hw_dispid=hexval" entry */
  envval = getenv("hw_dispid");
  if(envval)
  {
    i = (simple_strtoul (envval, NULL, 10)) & 0xff;
    sprintf(tmpstr," hw_dispid=%x",i);
    strcat(bootarguments, tmpstr);
  }
  
  /* 3) Append the "ethaddr=00:30:d8:01:c1:ab" entry */
  envval = getenv("ethaddr");
  if(envval)
  {
    sprintf(tmpstr," ethaddr=%s",envval);
    strcat(bootarguments, tmpstr);
  }
  
  /* 4) Append the "board_version=decval,0" entry */
  envval = getenv("boardhwcode");
  if(envval)
  {
    sprintf(tmpstr," board_version=%s,0",envval);
    strcat(bootarguments, tmpstr);
  }
  
  /* 5) Set the updated bootargs env. variable */
  setenv("bootargs", bootarguments);
  //printf("bootx: bootargs=%s\n",bootarguments);
  
  /* And now try to boot Linux at CONFIG_SYS_LOAD_ADDR*/
  sprintf(tmpstr,"bootm %x",CONFIG_SYS_LOAD_ADDR);
  //printf("bootx: bootcmd=%s\n",tmpstr);
  run_command(tmpstr, 0);
}


/*-----------------------------------------------------------------------
 * Check if the MMC card contains the $0030D8$.bin file (1=success 0=failure)
 */
static int mmc_strt_configos(void)
{
#ifdef CONFIG_MMC
  char buf [4];
  block_dev_desc_t *dev_desc=NULL;
	
  // 1) Init the MMC subsystem ... if failure exit
  if(mmc_legacy_init(1))
    return 0;
	
  // 2) Get the MMC device descriptor... if failure exit
  dev_desc = mmc_get_dev(1);
  if (dev_desc==NULL)
    return 0;
	
  // 3) Register the device... if failure exit
  if (fat_register_device(dev_desc,1)!=0)
    return 0;
	
  // 4) Try to read the $0030D8$.bin file to check if exists (we read just max 1 byte)
  if( do_fat_read("$0030D8$.bin", buf, 1, LS_NO) == -1)
    return 0;
  else
    return 1;
#else
    return 0;
#endif
}

int xflash_init(void)
{
  int stat,len;
  unsigned long *flash_addr;
  unsigned long *data_addr = (unsigned long *)xfis_table;

  if ((stat = flash_init()) == 0) 
  {
    return -1;
  }
  if (addr2info(CONFIG_XFIS_ADDR) != NULL)
  {
    flash_read(CONFIG_XFIS_ADDR,data_addr,XFIS_MAX_ENTRIES*XFIS_ENTRY_SIZE/4);
  }
  return 0;
}

int xfis_entry_valid(image_type type) 
{
  unsigned long checksum = 0;
  int i;
  struct xfis_entry *tEntry = ((struct xfis_entry *)xfis_table) + type;
  if(!tEntry->boot_flag) 
  {
    return 0;
  }
  
  for(i=0;i<7;i++) 
  {
    checksum += ((unsigned long*)tEntry)[i];
  }
  if(checksum !=  tEntry->entry_cksum) 
  {
    printf("xfis_entry_valid wrong checksum \n");
    return 0;
  }
  return 1;
}

/*-----------------------------------------------------------------------
 * load main wince image from flash to ram and load
 *
 */
 
int load_main_image(void)
{
    int res = 0;
    static char line[2];
    struct xfis_entry *tEntry;
    unsigned long srcAddr,destAddr,flash_addr;
    unsigned long to_xfer,to_dma, crc,size =0;
    unsigned long buf[IMAGE_HDR_SIZE/4];
    char fLinux;

    // If the MMC card contains the $0030D8$.bin file ... force loading the configOS
    if(mmc_strt_configos() == 1)
    {
      printf ("$0030D8$.bin file found on MMC \n");
      initialize_display(); //clear screen for display print and initialize x/y resolutions
      print_str((screenx/32 - 10)>0? screenx/32 - 10: 0, 0, (unsigned char*)"$0030D8$.bin found", 0xffff,0x0000,1);
      print_str((screenx/32 - 10)>0? screenx/32 - 10: 0, 2, (unsigned char*)"Going to Config mode", 0xffff,0x0000,1);
      return -2;
    }
	
    if(!xfis_entry_valid(MAIN_IMAGE))
      return -1;

    tEntry = ((struct xfis_entry *)xfis_table)+ MAIN_IMAGE;
    srcAddr = tEntry->flash_base;

    if(tEntry->size)
    {
      //check for valid header 
      if (addr2info(srcAddr) != NULL)
      {
	flash_read(srcAddr,buf,IMAGE_HDR_SIZE/4);
      }
      if((buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6]) == buf[7])
      {
	size = tEntry->size - IMAGE_HDR_SIZE;
	to_xfer = size;
	srcAddr += IMAGE_HDR_SIZE;
	tEntry->file_cksum = buf[6];		
      }
      else
      {
	return -1;
      }
    }
    else
      to_xfer = 0;

    destAddr = tEntry->mem_base;
#ifdef CONFIG_TOUCH	
    init_touch();
#endif	

    /* Check if it is a Linux image */
    fLinux = IsLinuxImage((char*)buf);
    
    /* If Linux, need to load on a tmp are, from which the kernel will be uncompressed */
    if(fLinux)
      destAddr = CONFIG_SYS_LOAD_ADDR;
	
    if(to_xfer) 
    {
      if (addr2info(srcAddr) != NULL)
      {
	flash_read(srcAddr,destAddr,to_xfer/4);
      }
		
#ifdef CONFIG_TOUCH
      // TAP-TAP detect
      // 1) Check if at least a couple of TAP-TAP was done during DMA transfer
      res = touch_get_tap_tap();
      // 2) If so, loops for some about 2 secs in order to univocally detect a "secure" TAP-TAP sequence
      //in case of small os (size less than 8 mb )wait of tap tap detection
      if( to_xfer < 0x800000 || res > 5) 
      {
	u32 now;
	u32 timeout = 2000; //2 second
	u32 start = get_timer(0);
	//loops for some about 2 secs			
	while(get_timer(0) - start < timeout) 
	{
	  res = touch_get_tap_tap();
	  //if got enough tap exit
	  if(res > 15)
	    break;
	}
		    
	res = touch_get_tap_tap();			
	if(res > 15)
	{
	  printf ("TAP-TAP detected %d\n", res);
	  initialize_display(); //clear screen for display print and initialize x/y resolutions
	  print_str((screenx/32 - 10)>0? screenx/32 - 10: 0, 0, (unsigned char*)"Tap Tap is detected.", 0xffff,0x0000,1);
	  print_str((screenx/32 - 10)>0? screenx/32 - 10: 0, 2, (unsigned char*)"Going to Config mode", 0xffff,0x0000,1);
	  deinit_touch();
	  return -2;
	}
	else
	{
	  //printf ("TAP-TAP not detected %d\n", res);
	}
      }
      deinit_touch();
#endif		
      for(res = 0; res < 10; res ++)

#ifdef CONFIG_UN31P_CAREL
      if( check_sicar()==1)
      {
	if(breakbootstring()==1)
	{
	  return -2;
	}
      }
      else
      {
	if(ctrlc())
	{
	  return -2;
	}
      }
#else
      if(ctrlc())
      {
	return -2;
      }
#endif

      /* Start Linux if this is the case */
      if(fLinux)
      {
	DoStartLinux();
	return -1;		// Should return if and only if the image is not valid (ie CRC32 check failed)
      }
      
      /* Jump to WinCE entrypoint */
      ((void (*)(void)) tEntry->mem_base) ();
    }
    return 0;
}

/*-----------------------------------------------------------------------
 * load config wince image from flash to ram and load
 *
 */
int load_config_image(void)
{
  int res = 0;
  static char line[2];
  struct xfis_entry *tEntry;
  unsigned long srcAddr,destAddr,flash_addr;
  unsigned long to_xfer,to_dma, crc,size =0;
  unsigned long buf[IMAGE_HDR_SIZE/4];
  char fLinux;

  puts ("Loading Config OS\n");
  if(!xfis_entry_valid(CONFIG_IMAGE))
    return -1;
  tEntry = ((struct xfis_entry *)xfis_table)+ CONFIG_IMAGE;
  srcAddr = tEntry->flash_base;

  if(tEntry->size)
  {
    //check for valid header 
    if (addr2info(srcAddr) != NULL)
    {
      flash_read(srcAddr,buf,IMAGE_HDR_SIZE/4);
    }
    if((buf[0] + buf[1] + buf[2] + buf[3] + buf[4] + buf[5] + buf[6] == buf[7]))
    {
      size = tEntry->size - IMAGE_HDR_SIZE;
      to_xfer = size;
      srcAddr += IMAGE_HDR_SIZE;
      tEntry->file_cksum = buf[6];   			
    }
    else
    {
      return -1;
    }
  }
  else
    to_xfer = 0;

  destAddr = tEntry->mem_base;

  /* Check if it is a Linux image */
  fLinux = IsLinuxImage((char*)buf);
    
  /* If Linux, need to load on a tmp are, from which the kernel will be uncompressed */
  if(fLinux)
    destAddr = CONFIG_SYS_LOAD_ADDR;
  
  if(to_xfer) 
  {
    ulong temp = CONFIG_SYS_LOAD_ADDR;
    if (addr2info(srcAddr) != NULL)
    {
      flash_read(srcAddr,temp,to_xfer/4);
    }

#ifdef CONFIG_UN31P_CAREL

    if( check_sicar()==1)
    {
      if(breakbootstring()==1)
      {
	return -2;
      }
    }
    else
    {
      if(ctrlc())
      {
	return -2;
      }

    }
#else
    if(ctrlc())
    {
      return -2;
    }
#endif

    if(!fLinux)
    {
      if (gunzip ((void *)destAddr, CONFIG_CONFIGOSM_SIZE, (uchar *)temp, &to_xfer) != 0) 
      {
	puts ("GUNZIP: uncompress, out-of-mem or overwrite error "
	"- must RESET board to recover\n");
      }
    }

#ifdef CONFIG_UN31P_CAREL

    if( check_sicar()==1)
    {
      if(breakbootstring()==1)
      {
	return -2;
      }
    }
    else
    {
      if(ctrlc())
      {
	return -2;
      }
    }
#else
    if(ctrlc())
    {
      return -2;
    }
#endif

    /* Start Linux if this is the case */
    if(fLinux)
    {
      DoStartLinux();
      return -1;		// Should return if and only if the image is not valid (ie CRC32 check failed)
    }

    /* Jump to WinCE entrypoint */
    ((void (*)(void)) tEntry->mem_base) ();
  }
  return 0;
}

#endif /* CONFIG_BOOTX */

void do_bootx(void)
{
#ifdef CONFIG_BOOTX
   if(xflash_init() == 0)
   {
#ifdef CONFIG_UN31P_CAREL
      enable_voltagesun31();
#endif
      load_main_image();   /* no way to return is loaded successfully.So no need of checking error */
      load_config_image(); /* no way to return is loaded successfully.So no need of checking error */
   }
#endif /* CONFIG_BOOTX */
}

