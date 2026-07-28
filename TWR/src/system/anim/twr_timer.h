/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_timer.h
 * PURPOSE     : Tough Water Rendering project.
 *               Timer function declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_timer_h_
#define __twr_timer_h_

#include "twr_def.h"
#include "system/twr_system.h"

/* Struct of timer datas */
typedef struct tagtwrSYSTEM_TIMER
{
  /* The timer initialisation program function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*TimerInit)( VOID );
  /* The rendering timer respponsing program function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*TimerResponse)( VOID );

  /* Timer data */
  UINT64
    StartTime,    /* Start program time */
    OldTime,      /* Previous frame time */
    OldTimeFPS,   /* Old time FPS measurement */
    PauseTime,    /* Time during pause period */
    TimePerSec,   /* Timer resolution */
    FrameCounter; /* Frames counter */
  DBL
    GlobalTime, GlobalDeltaTime, /* Global time and interframe interval */
    Time, DeltaTime,             /* Time with pause and interframe interval */
    FPS;                         /* Frames per second value */
  BOOL
    IsPause;                     /* Pause flag */
} twrSYSTEM_TIMER;

/* The timer system initialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTimerInit( VOID );

/* The timer system deinitialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTimerClose( VOID );

#endif /* __twr_timer_h_ */

/* END OF 'twr_timer.h' FILE */
