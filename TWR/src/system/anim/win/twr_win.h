/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_win.h
 * PURPOSE     : Tough Water Rendering project.
 *               Window depended WinAPI handle declaration module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_win_h_
#define __twr_win_h_

#include "twr_def.h"
#include "../res/twr_res.h"

/* WinAPI subsystem description type */
typedef struct tagtwrSYSTEM_WINDOW
{
  HINSTANCE hInstance;    /* Application instance handle */
  HWND hWnd;              /* Main window handle */
  INT W, H;               /* Window size */
  BOOL IsActive;          /* Window activity flag */
  BOOL PrevInputIsActive; /* Window previous input activity flag */
  BOOL IsFullScreen;      /* Full screen flag */
  INT MouseWheel;         /* Mouse wheel counter value */

  /* Flip full screen window function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*FlipFullScreen)( VOID );

  /* Run main message loop function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*Run)( VOID );
} twrSYSTEM_WINDOW;

/* Window subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemWindowInit( VOID );

/* Window subsystem deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemWindowClose( VOID );

#endif /* __twr_win_h_ */

/* END OF 'twr_win.h' FILE */
