#ifndef _TOUCH_H_
#define _TOUCH_H_
//==========================================================================
//
//      touch.h
//
//      header file for touch controller
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
// Purpose:             UNxx Touch detection
// Description:         This is an implementation for init, deinit and tap tap detection
//
//==========================================================================

#ifdef CONFIG_TOUCH
int init_touch(void); 
int deinit_touch(void); 
int get_touch_state(void);    
int touch_get_tap_tap(void);
#endif
#endif // _BOOTX_H_
