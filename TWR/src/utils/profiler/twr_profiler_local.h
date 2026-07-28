/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler_local.h
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler handle local declaration module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_profiler_local_h_
#define __twr_profiler_local_h_

#include "twr_profiler.h"

/* Stamps maximum counts */
#define TWR_PROFILER_MAX_STAMPS 47

/* Strings maximum counts */
#define TWR_PROFILER_MAX_STRINGS 102
/* Strings maximum size */
#define TWR_PROFILER_MAX_STRING_SIZE 300

/* Profile info collect data type */
typedef struct tagterPROFILER_STAMP
{
  CHAR *Name;      /* Profile stamp name */
  DBL Average;     /* Profile last average value */
  INT Count;       /* Profile average stored counter */
  DBL Accum;       /* Profile delta time stamp accumulator */
  DBL LastMeasure; /* Last average measure time stamp */
  DBL LastStamp;   /* Last time stamp value */
} terPROFILER_STAMP;

/***
 * Profiler semiglobal data
 ***/

/* Timer variables */
extern UINT64
  TWR_ProfilerStartTime,    /* Start profiler time */
  TWR_ProfilerTimePerSec;   /* Timer resolution */
/* Profiler window handle */
extern HWND TWR_hProfilerWnd;
/* Memory device context */
extern HDC TWR_hProfilerDCFrame;
/* Frame buffer bitmap */
extern HBITMAP TWR_hProfilerBmFrame;
/* Frame buffer pixel color data */
extern DWORD *TWR_ProfilerFrameBits;
/* Frame size */
extern INT TWR_ProfilerFrameW, TWR_ProfilerFrameH;
/* Frame zoom */
extern INT TWR_ProfilerFrameZoom;
/* Profiles strings and size */
extern CHAR TWR_ProfilerStings[TWR_PROFILER_MAX_STRINGS][TWR_PROFILER_MAX_STRING_SIZE];
extern INT TWR_ProfilerStingsCount;
/* Profiler FPS */
extern DBL TWR_ProfilerFPS;

/* Profiler window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message identifier (see WM_***):
 *       UINT Msg;
 *   - message first parameter ('word' param):
 *       WPARAM wParam;
 *   - message second parameter ('long' param):
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message return code (depended to Msg type).
 */
LRESULT CALLBACK TWR_ProfilerWindowFunc( HWND hWnd, UINT Msg,
                                         WPARAM wParam, LPARAM lParam );

/* Display all profiler data function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerDisplay( VOID );

/***
 * GFX functions.
 ***/

/* Draw one character function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - character code:
 *       CHAR Ch;
 *   - character color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID TWR_ProfilerPutChar( INT X, INT Y, CHAR Ch, DWORD Color );

/* Draw string function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - string pointer:
 *       const CHAR *Str;
 *   - character color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID TWR_ProfilerPutStr( INT X, INT Y, const CHAR *Str, DWORD Color );

/* Draw filled bar function.
 * ARGUMENTS:
 *   - frame position:
 *       INT X, Y;
 *   - bar size:
 *       INT W, H;
 *   - bar color:
 *       DWORD Color;
 * RETURNS: None.
 */
VOID TWR_ProfilerPutBar( INT X, INT Y, INT W, INT H, DWORD Color );

#endif /* __twr_profiler_local_h_ */

/* END OF 'twr_profiler_local.h' FILE */
