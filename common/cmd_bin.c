 /* This file allows booting nk.bin WCE6 images from RAM
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
#ifdef CONFIG_HAS_DATAFLASH
#include <dataflash.h>
#endif
#include <linux/ctype.h>


#ifdef CONFIG_CMD_WCE

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#undef	CMD_BIN_DEBUG

#ifdef	CMD_BIN_DEBUG
#define	PRINTF(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTF(fmt,args...)
#endif

/* ======================================================================
 * BIN file type definition
 * ====================================================================== */

#define NUM_SYNCBYTES 7
#define SYNC_MAGIC "B000FF\n"
#define BASE_BIN_RECORD_SIZE 12

struct bin_sync 
{
	char syncbytes[NUM_SYNCBYTES];
};

struct bin_header 
{
	unsigned int img_start;
	unsigned int img_length;
};

struct bin_record 
{
	unsigned int address;
	unsigned int length;
	unsigned int checksum;
};

int valid_bin_image (unsigned long addr);
unsigned long load_bin_image (unsigned long addr);


/* ======================================================================
 * Interpreter command to boot WindowsCE from a memory image.  The image 
 * must be a .bin image.  
 * ====================================================================== */
int do_bootwince ( cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	unsigned long addr;			/* Address of image            */
	char *tmp;					/* Temporary char pointer      */

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
	 * If the data at the load address is an elf image, then
	 * treat it like an elf image. Otherwise, assume that it is a
	 * binary image
	 */
	printf ("## Loading WindowsCE binary image from 0x%08x, please wait...\n", (unsigned int)addr);

	if (valid_bin_image (addr))
	{
		addr = load_bin_image (addr);
	} 
	else 
	{
		puts ("## Not a WinCE BIN image!\n");
		return 1;
	}

	if (addr==(unsigned long)0xFFFFFFFF) 
	{
		printf ("## ABORTING: wrong CRC\n");
		return 1;
	}

	/* Jump to WinCE entrypoint
	 */
	
	cleanup_before_linux();
	
	printf ("## Starting WindowsCE at 0x%08x ...\n", (unsigned int)addr);
	((void (*)(void)) addr) ();

	/* we should NEVER get here! */
	puts ("## WindowsCE terminated !!!\n");
	return 1;
}

/* ======================================================================
 * Determine if a valid BIN image exists at the given memory location.
 * Looks at the BIN header magic field.
 * ====================================================================== */
int valid_bin_image (unsigned long addr)
{
	struct bin_sync* sync;
	
	sync = (struct bin_sync*) addr;

	if (strncmp(sync->syncbytes, SYNC_MAGIC, NUM_SYNCBYTES) == 0)
		return 1;
	else return 0;
}


/* ======================================================================
 * A simple WinCE BIN loader, assumes the image is valid, returns the
 * entry point address.
 * ====================================================================== */
unsigned long load_bin_image (unsigned long addr)
{
	int i;
	unsigned long current_addr;
	unsigned long current_dest_addr;
	struct bin_header header;
	struct bin_record record;
	int record_num;
	long int record_checksum;

	/* read bin header */
	memcpy((char*)&header, (char*)(addr + NUM_SYNCBYTES), sizeof(header));
	header.img_start &= 0x03FFFFFF;
	
	PRINTF("#### .bin image header data:\n");
	PRINTF("#### start 0x%08x, size 0x%08x\n", header.img_start,
		header.img_length);
	
	current_addr = addr + sizeof(struct bin_header) + NUM_SYNCBYTES;
	current_dest_addr = header.img_start;

	record_num = 0; 
	while (1) {
		/* read record */
		memcpy((char*)&record, (char*)current_addr, sizeof(record));
		record.address &= 0x07FFFFFF;

		PRINTF("#### .bin record #%d:\n", record_num);
		PRINTF("#### start 0x%08x, size 0x%08x, chksum 0x%08x\n",
	               record.address, record.length, record.checksum);

		if (record.address==0) {
			PRINTF("#### address==0->quitting!\n");
			break;
		}

		current_addr += BASE_BIN_RECORD_SIZE;

		/* checksum verification */
		record_checksum = 0;
		for (i=0; i<record.length; i++) {
			record_checksum += ((char*)(current_addr))[i];
		}

		if (record_checksum!=record.checksum) {
			printf("wrong checksum (having 0x%08X!=0x%08X)\n", (unsigned int)record_checksum,
				record.checksum);
			return 0xFFFFFFFF;
		}


		/* clear uninitialized memory */
		memset((char*)current_dest_addr+CONFIG_WCE_BASE_RAM_ADDR, 0, record.address-current_dest_addr);
		memcpy((char *)record.address+CONFIG_WCE_BASE_RAM_ADDR, (char *)current_addr, record.length);

		current_addr += record.length;

		current_dest_addr = record.address + record.length;

		record_num++;

		putc ('.');
	}

	printf("\n");

	return header.img_start + CONFIG_WCE_BASE_RAM_ADDR;
}

/* ====================================================================== */

U_BOOT_CMD(
	bootwce,      2,      0,      do_bootwince,
	"bootwce  - Boot Windows CE from a binary (.bin) image\n",
	" [address] - load address of Windows CE BIN image.\n"
);
#endif	/* CONFIG_CMD_WCE */

 