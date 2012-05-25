#ifndef _DISPLAY_PRINT_H_
#define _DISPLAY_PRINT_H_

//==========================================================================
//
//      display_print.c
//
//      LCD Display print system
//
//==========================================================================
//####COPYRIGHTBEGIN####
//
// Copyright (C) 2010 Exor International, Inc.
// All Rights Reserved.
// -------------------------------------------
//#####DESCRIPTIONBEGIN####
//
// Author(s):           Greatin Baby
// Contributors:
// Date:                2010-11-11
// Purpose:             LCD Display print system
// Description:         This provides routines to display charactors in LCD display
//
//
//####DESCRIPTIONEND####
//
//==========================================================================

#ifdef CONFIG_VIDEO_OMAP3
#include <asm/arch/dss.h>
#endif

//clear the LCD display ( clears video RAM by filling 0x00)
void clear_display(void);

//print_ch : prints a charector in ascii to display on the specified x and y pos. x and y means number of 16 pixels offset
void print_ch(int x, int y, unsigned char ch, unsigned short col, unsigned short bgcol, int size);

//print_str : prints a string in ascii to display on the specified x and y pos. x and y means number of 16 pixels offset
void print_str(int x, int y, unsigned char* pch, unsigned short col, unsigned short bgcol, int size);

extern int screenx;
extern int screeny;
#endif //_DISPLAY_PRINT_H_