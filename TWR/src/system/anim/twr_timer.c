/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_timer.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with timer functions.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 26.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_timer.h"
#include "system/twr_system.h"

/* The timer initialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_TimerInit( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceFrequency(&t);
  Twr->Timer.TimePerSec = t.QuadPart;
  QueryPerformanceCounter(&t);
  Twr->Timer.StartTime = Twr->Timer.OldTime = Twr->Timer.OldTimeFPS = t.QuadPart;
  Twr->Timer.FrameCounter = 0;
  Twr->Timer.IsPause = FALSE;
  Twr->Timer.FPS = 30.0;
  Twr->Timer.PauseTime = 0;
} /* End of 'TWR_TimerInit' function */ 

/* The rendering timer respponsing program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_TimerResponse( VOID )
{
  LARGE_INTEGER t;

  QueryPerformanceCounter(&t);
  /* Global time */
  Twr->Timer.GlobalTime = (DBL)(t.QuadPart - Twr->Timer.StartTime) / Twr->Timer.TimePerSec;
  Twr->Timer.GlobalDeltaTime = (DBL)(t.QuadPart - Twr->Timer.OldTime) / Twr->Timer.TimePerSec;
  /* Time with pause */
  if (Twr->Timer.IsPause)
  {
    Twr->Timer.DeltaTime = 0;
    Twr->Timer.PauseTime += t.QuadPart - Twr->Timer.OldTime;
  }
  else
  {
    Twr->Timer.DeltaTime = Twr->Timer.GlobalDeltaTime;
    Twr->Timer.Time = (DBL)(t.QuadPart - Twr->Timer.PauseTime - Twr->Timer.StartTime) / Twr->Timer.TimePerSec;
  }
  /* FPS */
  Twr->Timer.FrameCounter++;
  if (t.QuadPart - Twr->Timer.OldTimeFPS > Twr->Timer.TimePerSec)
  {
    Twr->Timer.FPS = Twr->Timer.FrameCounter * Twr->Timer.TimePerSec / (DBL)(t.QuadPart - Twr->Timer.OldTimeFPS);
    Twr->Timer.OldTimeFPS = t.QuadPart;
    Twr->Timer.FrameCounter = 0;
  }
  Twr->Timer.OldTime = t.QuadPart;
} /* End of 'TWR_TimerResponse' function */ 

/* The timer system initialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTimerInit( VOID )
{
  Twr->Timer.TimerInit = TWR_TimerInit;
  Twr->Timer.TimerResponse = TWR_TimerResponse;
  TWR_TimerInit();
  Twr->Info("Timer system is initialized");
} /* End of 'TWR_SystemTimerInit' function */ 

/* The timer system deinitialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTimerClose( VOID )
{
} /* End of 'TWR_SystemTimerClose' function */ 

/* END OF 'twr_timer.c' FILE */
