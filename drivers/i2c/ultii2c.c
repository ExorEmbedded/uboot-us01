/*
 * Basic I2C functions to interface with I2C related commands.
 *
 * Copyright (c) 2010 Sitek S.p.a.
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author: Giovanni Pavoni Sitek S.p.a. 
 *
 * NOTE: Specific implementation for Ultii2c FPGA APB core.
 *
 */

#include <common.h>
#include <asm/io.h>

/* Ultii2c registers addresses */
#define ULTII2C_FIFO 	(CONFIG_SYS_ULTII2CBASE + 0x00)
#define ULTII2C_RXSTAT 	(CONFIG_SYS_ULTII2CBASE + 0x20)
#define ULTII2C_ADDR 	(CONFIG_SYS_ULTII2CBASE + 0x40)
#define ULTII2C_TNUM  	(CONFIG_SYS_ULTII2CBASE + 0x50)
#define ULTII2C_RNUM  	(CONFIG_SYS_ULTII2CBASE + 0x60)
#define ULTII2C_STRT  	(CONFIG_SYS_ULTII2CBASE + 0x70)
#define ULTII2C_STAT  	(CONFIG_SYS_ULTII2CBASE + 0x80)

/* Ultii2c register fields and constants */
#define ULTII2C_BUSY  	0x01
#define ULTII2C_WR    	0x01
#define ULTII2C_RD    	0x02
#define ULTII2C_RST   	0x04
#define ULTII2C_ERRMSK	0x1e
#define ULTII2C_RETRY	10

/* Initialization of I2C core.
 * NOTE: No action required; we keep default 100 Khz speed
 */
void i2c_init (int speed, int slaveadd)
{
  u16 tmp;
  
  /* Perform a core reset */
  writew(ULTII2C_RST, ULTII2C_STRT);

  /* wait until core busy */
  tmp=ULTII2C_BUSY;
  while(tmp)
  {
	tmp = readw(ULTII2C_STAT);
	tmp=(tmp & ULTII2C_BUSY);
  }
}

/* Helper function for reading one single byte
 */
static int i2c_read_byte (u8 devaddr, u8 * value)
{
	int i2c_error = 0;
	u16 tmp;
	
	udelay(1000);
	
	/* Issue command for reading 1 byte from the specified device */
	writew(devaddr, ULTII2C_ADDR);
	writew(1, ULTII2C_RNUM);
	writew(ULTII2C_RD, ULTII2C_STRT);

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}
	
	/* Check for errors */
	tmp = readw(ULTII2C_STAT);
	tmp=(tmp & ULTII2C_ERRMSK);
	if(tmp) i2c_error = 1;

	/* reads the buffer */
	tmp = readw(ULTII2C_RXSTAT) & 0xff;

	if
	  (!tmp) i2c_error = 1;
	else
	  tmp = readw(ULTII2C_FIFO) & 0xff;
	
	*value = (u8)(tmp);
	return i2c_error;
}

/* Helper function for writing . As of now support 30 bytes at a time
 */
static int i2c_write_byte (u8 devaddr, u8* buffer, u8 nbytes)
{
	int i2c_error = 0;
	u16 tmp;
	u8 value;
	
	if(nbytes > 30) return 1;

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}
	
	udelay(1000);	
	
	/* Issue command for writing 1 byte to the specified device */
	writew(devaddr, ULTII2C_ADDR);
	writew(nbytes, ULTII2C_TNUM);
	for(tmp=0;tmp<nbytes;tmp++)
	{
	  value = buffer[tmp];
	  writew((u16)value, ULTII2C_FIFO); 
	}
	writew(ULTII2C_WR, ULTII2C_STRT);

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}

	/* Check for errors */
	tmp = readw(ULTII2C_STAT);
	tmp=(tmp & ULTII2C_ERRMSK);
	if(tmp) i2c_error = 1;
	
	return i2c_error;
}


/* 
 * Perform a read of n bytes, from the specified i2c device, starting
 * from the specified offset
 */
int i2c_read (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	int i;
	u16 tmp;
	u8  add;

	if (alen > 1) {
		printf ("I2C read: addr len %d not supported\n", alen);
		return 1;
	}

	if (addr > 256) {
		printf ("I2C read: address out of range\n");
		return 1;
	}

	if (len > 1024) {
		printf ("I2C read: data len out of range\n");
		return 1;
	}

	/* Perform a core reset */
	writew(ULTII2C_RST, ULTII2C_STRT);

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}
	
	/* Write address */
	add = (u8)addr;
	if (i2c_write_byte (chip, &add, 1))
	{
		printf ("I2C read: I/O error (w)\n");
		return 1;
	}
	
	/* Read data */
	for (i = 0; i < len; i++)
	  if (i2c_read_byte (chip, &buffer[i])) 
	  {
		printf ("I2C read: I/O error (r)\n");
		return 1;
	  }
	return 0;
}

/* 
 * Perform a write of n bytes to the specified I2C device, starting from
 * the specified offset
 */
int i2c_write (uchar chip, uint addr, int alen, uchar * buffer, int len)
{
	int i;
	u16 tmp;
	u8  pnt[2];

	if (alen > 1) {
		printf ("I2C write: addr len %d not supported\n", alen);
		return 1;
	}

	if (addr > 256) {
		printf ("I2C write: address out of range\n");
		return 1;
	}

	if (len > 1024) {
		printf ("I2C write: data len out of range\n");
		return 1;
	}

	/* Perform a core reset */
	writew(ULTII2C_RST, ULTII2C_STRT);

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}
	if(	alen == 0)
	{
		//I2C write in subaddress mode 0
		if (i2c_write_byte (chip, buffer, len)) 
		{
			printf ("I2C write: I/O error\n");
			return 1;
		}
	}
	else
	{
		/* write data */
		for (i = 0; i < len; i++)
		{
		  pnt[0] = (u8)addr;
		  pnt[1] = buffer[i];
		  if (i2c_write_byte (chip, pnt, 2)) 
		  {
				printf ("I2C write: I/O error\n");
				return 1;
		  }
		}
	}
	
	/* Better to wait a little bit */
	udelay(30000);
	return 0;
}


/*
 * Function for checking if the specified device is responding
 */
int i2c_probe (uchar chip)
{
	int res; 
	u16 tmp;
	uchar dummy;
	
	if(chip == 0) return 1;

	/* Perform a core reset */
	writew(ULTII2C_RST, ULTII2C_STRT);

	/* wait until core busy */
	tmp=ULTII2C_BUSY;
	while(tmp)
	{
	  tmp = readw(ULTII2C_STAT);
	  tmp=(tmp & ULTII2C_BUSY);
	}
	
	/* Try a dummy write to see if chip responding */
	res = i2c_write_byte (chip, &dummy,1);

	/* Better to wait a little bit */
	udelay(30000);

	return res;
}
