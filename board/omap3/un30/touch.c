//==========================================================================
//
//      touch.c
//
//      Implementation of touch detection helper functions
//
//==========================================================================
//
// Copyright (C) 2010 Exor International, Inc.
// All Rights Reserved.
// -------------------------------------------
//
// Author(s):   		Abdul Nizar, Greatin baby. Exor India
//						Giovanni Pavoni. Sitek 
// Contributors:
// Date:                2010-11-01
// Purpose:             touch detection helper functions
//==========================================================================

#include <common.h>
#include <i2c.h>
#include <asm/arch/gpio.h>
//------------------------------------------------------------------------------
// Defines
//

#define COMMAND_CNV_SELECTXY			0x00
#define COMMAND_ENABLE_READ             0x3
#define COMMAND_ENABLE_WRITE			0x2
#define COMMAND_XPOS                    0xD4
#define COMMAND_YPOS                    0xC4

#define CFG0_REG						0xC
#define CFG2_REG						0xE

// CFG2 bit definitions
//MSB
#define PINTS1							(1 << 7)
#define PINTS0							(1 << 6)
#define MAV_M0M1_FLTR_SIZE_15			(3 << 4)
#define MAV_W0W1_FLTR_SIZE_7_8			(2 << 2)
//LSB
#define MAV_FLTR_EN_X					(1<<4)
#define MAV_FLTR_EN_Y					(1<<3)

// CFG0 bit definitions
//MSB
#define CL_CFG							(3 << 3)
#define PV_CFG							(4 << 0)
#define CFPSM							0x80
//LSB
#define PR_CFG							(7 << 5)
#define SNS_CFG							(7 << 2)

#define SKIP_NUM						2

// Registers expected readback values (used for checking if init really suceeded)
// NOTE: If init parameters are changed, these values must be changed accordingly
#define CFG0_CHK1						0xFC
#define CFG2_CHK0						0x38
#define CFG2_CHK1						0x18
#define I2C_SUBADDRESS_MODE_0			0x00
#define INTR_PIN      			0xB9

#ifdef CONFIG_TOUCH

static void msleep(int count)
{
	int i;

	for (i = 0; i < count; i++)
		udelay(1000);
}

int init_touch()
{
	unsigned char ucCommand = 0;
	unsigned char ucData[4];
	int i = 0;
	int ret = 0;
	
	omap_request_gpio(INTR_PIN);
	omap_set_gpio_direction(INTR_PIN,1);
	
	// SW reset sequence + sequence for exiting from factory test mode
	ucCommand = 0x82;		// MSB D7 is always set to 1; SW reset
	i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, &(ucCommand), 1);
	msleep(20);				// Just in case
	ucCommand = 0x82;		// MSB D7 is always set to 1; SW reset
	i2c_write (CONFIG_SYS_I2C_TSC2004ADDR & 0xfe, 0, I2C_SUBADDRESS_MODE_0, &(ucCommand), 1);
	msleep(20);				// Just in case
	
	// Init loop with retry
	for(i=0; i<2; i++)
	{
	  ret = 0;
	  // setup clock, settling times and operatin modes (controller working in free running)
	  ucData[2] = (SNS_CFG | PR_CFG);
	  ucData[1] = (PV_CFG | CL_CFG | CFPSM);

	  // write to CFG0 register
	  ucCommand = 0x80;		// MSB D7 is always set to 1
	  if(!ret) ret = i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, &(ucCommand), 1);
	  // enable read after conversion
	  ucData[0] = (CFG0_REG << 3)| COMMAND_ENABLE_WRITE;
	  if(!ret) ret = i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, ucData, 3);
	  msleep(200);

	  // INT output related only on physical pen touch (we will have a squrewave when touch pressed)
	  ucData[2] = (MAV_FLTR_EN_X | MAV_FLTR_EN_Y);
	  ucData[1] = (PINTS1 | MAV_M0M1_FLTR_SIZE_15 | MAV_W0W1_FLTR_SIZE_7_8);

	  // write to CFG2 register
	  ucCommand = 0x80;		// MSB D7 is always set to 1
	  if(!ret) ret = i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, &(ucCommand), 1);
	  // enable read after conversion
	  ucData[0] = (CFG2_REG << 3)| COMMAND_ENABLE_WRITE;
	  if(!ret) ret = i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, ucData, 3);
	  
	  // If we succeded in touch init ... exit loop
	  if(!ret) break;
	  
	  // Failed initializing touch ...
	  // Reinit I2C and retry
	  msleep(50);
	  i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE); 
	}
	return 1;
}


// SW reset for touch controller. 
// NOTE: This is required in order to avoid starting WCE6
//       with touch controller still free running
int deinit_touch()
{
	unsigned char ucCommand = 0;
	int i = 0;
	int ret = 0;
	
	// Init loop with retry
	for(i=0; i<2; i++)
	{
	  ret = 0;
	  ucCommand = 0x82;		// MSB D7 is always set to 1; SW reset
	  ret = i2c_write (CONFIG_SYS_I2C_TSC2004ADDR, 0, I2C_SUBADDRESS_MODE_0, &(ucCommand), 1);
	  if(!ret) break;
	  // Failed resetting touch ...
	  // Reinit I2C and retry
	  msleep(50);
	  i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE); 
	}
	
	// This is required in order to enter WCE with a well known I2C interface status
	i2c_init (CONFIG_SYS_I2C_SPEED, CONFIG_SYS_I2C_SLAVE); 
	return 1;
}


int get_touch_state()
{
	int val = 1;
	int i;
	
	//NOTE: INT pin is a squarewave when touch pressed, so we need a loop for detecting status
	if (!omap_request_gpio(INTR_PIN)) 
	{
		omap_set_gpio_direction(INTR_PIN, 1);
		for(i=0; i < 10; i++)
		{
		   if(!omap_get_gpio_datain(INTR_PIN)) val = 0;
		   msleep(1);
		}
		omap_free_gpio(INTR_PIN);
	}
	return val;
}


//Checks for touch changes since last call. Returns total number of transitions
int touch_get_tap_tap()
{
  static int transitions = 0;
  static int prev_stat = 1;
  int stat;
  
  stat = get_touch_state();
  if(stat != prev_stat)
  {
	prev_stat=stat;
	transitions ++;
  }
  
  return transitions;
}
#endif //CONFIG_TOUCH

