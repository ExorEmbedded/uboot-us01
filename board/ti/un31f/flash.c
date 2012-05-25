/*
 * (C) Copyright 2001
 * Kyle Harris, Nexus Technologies, Inc. kharris@nexus-tech.net
 *
 * (C) Copyright 2001
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
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
 */

#include <common.h>
#include <linux/byteorder/swab.h>

flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];	/* info for FLASH chips    */

#define mb() __asm__ __volatile__ ("" : : : "memory")
#define WriteAddr(b, a) (*((volatile u32 *)(b)) = ((volatile u32)a))
#define WriteAddr16(b, a) (*((volatile u16 *)(b)) = ((volatile u16)a))
#define ReadAddr(a) (*((volatile u32 *)(a)))
#define ReadAddr16(a) (*((volatile u16 *)(a)))
#define sector_startaddr(x) ((x) & ~(PHYS_FLASH_SECT_SIZE-1))
#define page_startaddr(x) ((x) & ~(FLASH_PAGE_SIZE-1))

#define FLASH_PAGE_SIZE 64
#define FLASH_BANK_RANGE (0x3FFFFF)
#define FLASH_BANK_OFF (0x600000)
#define FLASH_POLL_PROGRAMMING 0
#define FLASH_POLL_ERASE 1

/*-----------------------------------------------------------------------
 * Functions
 */
unsigned long flash_init (void);
static ulong flash_get_size (flash_info_t * info);
static void flash_get_offsets (ulong base, flash_info_t * info);
void inline spin_wheel (void);
void flash_print_info (flash_info_t * info);
int flash_erase (flash_info_t * info, int s_first, int s_last);
static void sector_erase(ulong secaddr);
static char WaitUntilFinished(ulong addr, uchar expected_data, int type);
static void WriteBufferBurst(ulong addr, uchar *dat, uchar len);
int write_buff (flash_info_t * info, uchar * dat, ulong addr, ulong len);
void flash_read(ulong addr, ulong *buf, ulong len);
void flash_setpage(ulong addr);

/*----------------------------------------------------------------------------------
 * Sets the flash page contained the address given
 * NOTE: S29GL01GP on AMBA version
 */
void flash_setpage(ulong addr)
{
	addr -= PHYS_FLASH_1;
	WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, addr); // write bank address
}


/*----------------------------------------------------------------------------------
 * Reads 'len' ulong of data from flash address 'addr' to buffer 'buf'
 * Data must be aligned to 4-bytes boundary
 * NOTE: S29GL01GP on AMBA version
 */
void flash_read(ulong addr, ulong *buf, ulong len)
{
	ulong bank_addr;
	ulong i;

	addr -= PHYS_FLASH_1;
	bank_addr = addr & (~(FLASH_BANK_RANGE));

    WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, addr); // write bank address

	if(addr % 4 != 0) // if not aligned to 4-byte boundary
	{
	  printf ("Not 4-byte boundary aligned FLASH address\n");
	  return;
	}

	// read data until last 4-byte boundary
	for(i=0; i<len; i++)
	{
		if (((addr+4*i) & (0xFFFFFFFF - FLASH_BANK_RANGE)) != bank_addr)
		{ // if another bank is reached
			bank_addr = (addr+4*i) & (~ (FLASH_BANK_RANGE));
			WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, bank_addr); // write bank address
		}
		buf[i]=ReadAddr(PHYS_FLASH_1 + ((addr+4*i) & FLASH_BANK_RANGE)); // read data
	}
}


/*----------------------------------------------------------------------------------
 * Helper function which writes flash in burst mode, inside a single sector
 * NOTE: S29GL01GP on AMBA version
 */
static void WriteBufferBurst(ulong addr, uchar *dat, uchar len)
  {
	int i;
	ulong sect_addr = sector_startaddr(addr);
	ulong eff_len;
	ulong off;
//	ushort eff_dat;
	ushort tmp_data = 0xFFFF;
	ulong eff_addr = sect_addr & (~1);
	off = sect_addr % 2;

	if(len > 64 || len<1)
		return;

	eff_len = (((addr%2)+len)/2)+(((addr%2)+len)%2);

	// Write flash unlock sequence
	WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF,0x00000000);
	WriteAddr16(PHYS_FLASH_1+0xAAA, 0x00AA);
	WriteAddr16(PHYS_FLASH_1+0x554, 0x0055);
	
	// Write buffer load command
	WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, eff_addr);
	WriteAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE), 0x0025);

	// Write data length - 1
	WriteAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE), eff_len-1);
	// Write data

	ushort* tmppnt = (ushort*)dat;
    for(i=0; i<eff_len; i++)
	{
	  tmp_data = *tmppnt;
      tmppnt++;
	  WriteAddr16(PHYS_FLASH_1 + ((addr+(i*2)) & FLASH_BANK_RANGE), tmp_data);
	}

	//confirm write
	WriteAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE), 0x0029);

	// wait until operation is finished
	if((addr+len-1)%2==0) // if last address is even
		WaitUntilFinished(addr+len-1, dat[len-1], FLASH_POLL_PROGRAMMING);
	else // if last address is odd
		WaitUntilFinished(addr+len-2, dat[len-2], FLASH_POLL_PROGRAMMING);
	//reset flash unconditionally just in case
	WriteAddr16(PHYS_FLASH_1, 0x00F0);
}


/*----------------------------------------------------------------------------------
 * Waits until current operation is finished - polls by reading from address 'addr'
 * NOTE: S29GL01GP on AMBA version
 */
static char WaitUntilFinished(ulong addr, uchar expected_data, int type)
{
	ulong eff_addr = addr & (~1);
	ulong eff_data;
	if(type==FLASH_POLL_ERASE)
		eff_data = 0xFFFF;
	else
		eff_data = expected_data;

	ushort read_data1, read_data2, read_data3;

	while(1){
		read_data1 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
		read_data1 &= 0xFFFF;
		if(!((read_data1 ^ eff_data) & 0x0080)){ // polling finished
			read_data2 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
			read_data3 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
			read_data2 &= 0xFFFF;
			read_data3 &= 0xFFFF;

			if(type == FLASH_POLL_PROGRAMMING){
				if (!((read_data3  ^ eff_data) & 0x00FF))
					return 1;       // Programming operation completed OK
				else
					return 0;       // Programming operation error
			}else if(type == FLASH_POLL_ERASE){
				// Check for DQ6 toggling.
				if ((read_data2 ^ read_data3) & 0x0040)
				{
					// Device error
					return 0;
				}
				// Check for DQ2 toggling.
				if ((read_data2 ^ read_data3) & 0x0002)
				{
					// Device in Erase suspend mode
					return 0;
				}
				return 1;
			}else
				return 0; // unknown operation
		}else{
			// DQ7 - not valid data
			/* Check for DQ5 */
			if (read_data1 & 0x0020)
			{
			  read_data2 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
			  read_data3 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
			  read_data2 &= 0xFFFF;
			  read_data3 &= 0xFFFF;
			  // Check for DQ6 toggling.
			  if ((read_data3 ^ read_data2) & 0x0040)
				{
					break;
				}
			// else: Device busy, Re-poll
			}
			else // DQ5 = 0
			{
				// Check if Write buffer programming operation
				if (type == FLASH_POLL_PROGRAMMING)
				{
					/* Check for DQ1 */
					if (read_data1 & 0x0002)
					{
						read_data2 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
						read_data3 = ReadAddr16(PHYS_FLASH_1 + (eff_addr & FLASH_BANK_RANGE));
						read_data2 &= 0xFFFF;
						read_data3 &= 0xFFFF;
						/* Check for DQ1 and DQ7 NOT valid data */
						if ((read_data3 & 0x0002) & ((read_data3 ^ expected_data)& 0x0080))
						{
							 // Reset bank to 0
							WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, 0x00000000);
							/* Issue Write Buffer Abort Reset Command Sequence */
							WriteAddr16(PHYS_FLASH_1+0xAAA,0x00AA);
							WriteAddr16(PHYS_FLASH_1+0x554,0x0055);
							return 0;
						}
					// else: Device busy, Re-poll
					}
				// else: Device busy, Re-poll
				}
			// else: Device busy, Re-poll
			}
		}
	}
	return 0;
}


/*-----------------------------------------------------------------------
 * Erase sector starting from secaddr
 * NOTE: S29GL01GP on AMBA version
 */
static void sector_erase(ulong secaddr)
{
	printf ("Sector erase\n");

	secaddr = secaddr - PHYS_FLASH_1;	//Get address offset

	// Write flash unlock sequence
	WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, 0x00000000);
	WriteAddr16(PHYS_FLASH_1+0xAAA, 0x00AA);
	WriteAddr16(PHYS_FLASH_1+0x554, 0x0055);
	// Write setup command
	WriteAddr16(PHYS_FLASH_1+0xAAA, 0x0080);
	// Write flash unlock sequence
	WriteAddr16(PHYS_FLASH_1+0xAAA, 0x00AA);
	WriteAddr16(PHYS_FLASH_1+0x554, 0x0055);
	// Write sector address
	WriteAddr(PHYS_FLASH_1+FLASH_BANK_OFF, secaddr);
	// Write sector erase command
	WriteAddr16(PHYS_FLASH_1 + (secaddr & FLASH_BANK_RANGE), 0x0030);
	// wait until operation is finished
	WaitUntilFinished(secaddr, 0, FLASH_POLL_ERASE);
	//reset flash unconditionally just in case
	WriteAddr16(PHYS_FLASH_1, 0x00F0);
};


/*-----------------------------------------------------------------------
 * Init FLASH data structures
 * NOTE: S29GL01GP on AMBA version
 */

unsigned long flash_init (void)
{
	int i;
	ulong size = 0;
	for (i = 0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++) {
		switch (i) {
		case 0:
			flash_get_size (&flash_info[i]);
			flash_get_offsets (PHYS_FLASH_1, &flash_info[i]);
			break;
		default:
			panic ("configured too many flash banks!\n");
			break;
		}
		size += flash_info[i].size;
	}

	/* Protect monitor, loader and environment sectors
	 */
	flash_protect (FLAG_PROTECT_SET,
			CONFIG_SYS_MONITOR_BASE,
			CONFIG_SYS_MONITOR_BASE + CONFIG_SYS_MONITOR_LEN - 1, &flash_info[0]);

	flash_protect (FLAG_PROTECT_SET,
			CONFIG_ENV_ADDR,
			CONFIG_ENV_ADDR + CONFIG_SYS_ENV_SECT_SIZE - 1, &flash_info[0]);

	flash_protect (FLAG_PROTECT_SET,
			CONFIG_SYS_FLASH_BASE,
			CONFIG_SYS_FLASH_BASE + CONFIG_SYS_ENV_SECT_SIZE - 1, &flash_info[0]);

	return size;
}

/*-----------------------------------------------------------------------
 * Write sector offsets
 * NOTE: S29GL01GP on AMBA version
 */
static void flash_get_offsets (ulong base, flash_info_t * info)
{
	int i;

	if (info->flash_id == FLASH_UNKNOWN) 
	  return;
	
	if (info->flash_id == SPANSION_ID_S29GL01GP)
	  for (i = 0; i < info->sector_count; i++) 
	    {
		info->start[i] = base + (i * PHYS_FLASH_SECT_SIZE);
		info->protect[i] = 0;
	    }
}

/*-----------------------------------------------------------------------
 * 	Prints infos about FLASH chip.
 *	NOTE: Hardcoded values: S29GL01GP on AMBA
 */
void flash_print_info (flash_info_t * info)
{
	int i;

	if (info->flash_id == FLASH_UNKNOWN) 
	{
		printf ("missing or unknown FLASH type\n");
		return;
	}

	if(info->flash_id == SPANSION_ID_S29GL01GP) 
	{
	  printf ("SPANSION ");
	  printf ("FLASH S29GL01GP\n");
	}
	else
	{
	  printf ("Unknown Vendor ");
	  printf ("Unknown Chip Type\n");
	}

	printf ("  Size: %ld MB in %d Sectors\n",
			info->size >> 20, info->sector_count);

	printf ("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; ++i) {
		if ((i % 5) == 0)
			printf ("\n   ");
		printf (" %08lX%s",
			info->start[i], info->protect[i] ? " (RO)" : "     ");
	}
	printf ("\n");
	return;
}

/*
 * 	Return the size of the FLASH device.
 *	NOTE: Hardcoded values: S29GL01GP on AMBA
 */
static ulong flash_get_size (flash_info_t * info)
{

	info->flash_id = SPANSION_ID_S29GL01GP;
	info->sector_count = 1024;
	info->size = SZ_128M;

	if (info->sector_count > CONFIG_SYS_MAX_FLASH_SECT) {
		printf ("** ERROR: sector count %d > max (%d) **\n",
				info->sector_count, CONFIG_SYS_MAX_FLASH_SECT);
		info->sector_count = CONFIG_SYS_MAX_FLASH_SECT;
	}

	return (info->size);
}


/*-----------------------------------------------------------------------
 * Erase the sectors contained into the range
 * NOTE: S29GL01GP on AMBA version
 */
int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int flag, prot, sect;

	if ((s_first < 0) || (s_first > s_last)) {
		if (info->flash_id == FLASH_UNKNOWN) {
			printf ("- missing\n");
		} else {
			printf ("- no sectors to erase\n");
		}
		return 1;
	}

	if (info->flash_id != SPANSION_ID_S29GL01GP) 
	{
		printf ("Can't erase unknown flash type %08lx - aborted\n",
				info->flash_id);
		return 1;
	}

	prot = 0;
	for (sect = s_first; sect <= s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}

	if (prot) {
		printf ("- Warning: %d protected sectors will not be erased!\n",
				prot);
	} else {
		printf ("\n");
	}

	/* Disable interrupts which might cause a timeout here */
	flag = disable_interrupts ();

	/* Start erase on unprotected sectors */
	for (sect = s_first; sect <= s_last; sect++) {
		if (info->protect[sect] == 0) 
		{	/* not protected */
			ulong addr = info->start[sect];

			printf ("Erasing sector %2d ... ", sect);
			sector_erase(addr);
			printf (" done\n");
		}
	}
	return 0;
}

/*-----------------------------------------------------------------------
 * Copy memory to flash, returns:
 * 0 - OK
 * 1 - write timeout
 * 2 - Flash not erased
 * 4 - Flash not identified
 * NOTE: S29GL01GP on AMBA version
 */
int write_buff (flash_info_t * info, uchar * dat, ulong addr, ulong len)
{
	ulong curr_addr;
	ulong curr_len;
	ulong max_len;
	ulong i = 0;
	
	curr_addr = addr - PHYS_FLASH_1;

	if (info->flash_id == FLASH_UNKNOWN) 
	{
		return 4;
	}

	if(addr%2)
	{
		printf ("- no 2 bytes aligned address\n");
		return 1;
	}

	if(len%2)
	{
		printf ("- no 2 bytes aligned len\n");
		return 1;
	}

	while(len > 0)
	{
		max_len = (len > FLASH_PAGE_SIZE) ? FLASH_PAGE_SIZE : len; // length to write
		if(page_startaddr(curr_addr) != page_startaddr(curr_addr + max_len-1)) // if not all data are in the same page
			curr_len = (page_startaddr(curr_addr)+FLASH_PAGE_SIZE) - curr_addr; // reduce amount of data, to not exceed page boundary
		else
			curr_len = max_len;
		
		WriteBufferBurst(curr_addr,dat,(uchar)curr_len); // write data to flash page
		curr_addr += curr_len;
		dat += curr_len;
		len -= curr_len;
		if((i%1000)==0)spin_wheel();
		i++;
	}
  return 0;
}


void inline spin_wheel (void)
{
	static int p = 0;
	static char w[] = "\\/-";

	printf ("\010%c", w[p]);
	(++p == 3) ? (p = 0) : 0;
}
