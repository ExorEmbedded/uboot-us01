#ifndef _BOOTX_H_
#define _BOOTX_H_
//==========================================================================
//
//      bootx.h
//
//      header file for bootx sequence
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


struct xfis_entry {
   unsigned long   flash_base;    // Address within FLASH of image
   unsigned long   mem_base;      // Address in memory where it executes
   unsigned long size;          // Length of image
   unsigned long boot_flag;     // charecteristic flag of image
   unsigned long res1;          // Reserved
   unsigned long res2;          // Reserved
   unsigned long file_cksum;    // Checksum over image data
   unsigned long entry_cksum;   // Checksum over image table entry
};

typedef enum _image_type {
   SPLASH_IMAGE,
   CONFIG_IMAGE,
   MAIN_IMAGE,
   FILE_SYSTEM
}image_type;

void do_bootx(void);      

#endif // _BOOTX_H_
