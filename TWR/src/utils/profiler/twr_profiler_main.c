/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler_main.c
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler handle main implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include <stdarg.h>
#include <stdio.h>
#include "twr_profiler_local.h"
#include "twr_profiler_font.h"

/* Timer variables */
UINT64
  TWR_ProfilerStartTime,    /* Start profiler time */
  TWR_ProfilerTimePerSec;   /* Timer resolution */

/* Profile tread info data */

/* Last profile time stamp value */
static DBL TWR_ProfilerLastTimeStamp = 0;

/* Last profile average time stamp */
static DBL TWR_ProfilerLastAverageTime = 0;

/* Profile registered values array */
static terPROFILER_STAMP TWR_ProfilerStamps[TWR_PROFILER_MAX_STAMPS];

/* Profile per frame updated stamps */
INT TWR_ProfilerStampsCount = 0;

/* Profile per frame filled stamps */
INT TWR_ProfilerStampsFilled = 0;

/* Profile visibility flag */
BOOL TWR_ProfilerIsVisible = TRUE;

/* Profile collection flag */
BOOL TWR_ProfilerIsCollection = TRUE;

/* Profiles strings and size */
CHAR TWR_ProfilerStings[TWR_PROFILER_MAX_STRINGS][TWR_PROFILER_MAX_STRING_SIZE];
INT TWR_ProfilerStingsCount;

/* Profiler FPS */
DBL TWR_ProfilerFPS;

/* Start/stop profiler collection function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerStartStop( VOID )
{
  TWR_ProfilerIsCollection = !TWR_ProfilerIsCollection;
} /* End of 'TWR_ProfilerStartStop' function */

/* Profiling restart (should be called before frame snap start) function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerReset( VOID )
{
  TWR_ProfilerStampsCount = 0;
  if (TWR_ProfilerIsCollection)
    TWR_ProfilerStingsCount = 0;
} /* End of 'TWR_ProfilerReset' function */

/* Collect profiling information (should be called before mesure event) function.
 * ARGUMENTS:
 *   - profile sample name:
 *       CHAR *ProfName;
 * RETURNS: None.
 */
VOID TWR_ProfilerCollect( CHAR *ProfName )
{
  LARGE_INTEGER ts;
  DBL t;
  terPROFILER_STAMP *St;

  QueryPerformanceCounter(&ts);
  t = (DBL)(ts.QuadPart - TWR_ProfilerStartTime) / TWR_ProfilerTimePerSec;

  if (TWR_ProfilerStampsCount < 0 || !TWR_ProfilerIsCollection)
  {
    TWR_ProfilerLastTimeStamp = t;
    return;
  }

  if (TWR_ProfilerStampsCount >= TWR_PROFILER_MAX_STAMPS)
    return;

  St = &TWR_ProfilerStamps[TWR_ProfilerStampsCount];
  St->Name = ProfName;
  if (TWR_ProfilerStampsCount == 0)
    St->Accum += t - St->LastStamp;
  else
    St->Accum += t - TWR_ProfilerLastTimeStamp;
  St->Count++;
  St->LastStamp = t;
  if (St->Count > 1 && t - St->LastMeasure > 3)
  {
    St->Average = St->Accum / St->Count;
    St->Accum = 0;
    St->Count = 0;
    St->LastMeasure = t;
  }
  TWR_ProfilerLastTimeStamp = t;
  TWR_ProfilerStampsCount++;
  if (TWR_ProfilerStampsCount > TWR_ProfilerStampsFilled)
    TWR_ProfilerStampsFilled = TWR_ProfilerStampsCount;
} /* End of 'TWR_ProfilerCollect' function */

/* Add text to profiler function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - print parameters:
 *       . . .
 * RETURNS: None.
 */
VOID TWR_ProfilerAddText( CHAR *Fmt, ... )
{
  va_list ap;
  static CHAR Buf[10000];

  if (TWR_ProfilerStingsCount >= TWR_PROFILER_MAX_STRINGS)
    return;
  if (!TWR_ProfilerIsCollection)
    return;

  va_start(ap, Fmt);
  vsprintf(Buf, Fmt, ap);
  va_end(ap);

  strncpy(TWR_ProfilerStings[TWR_ProfilerStingsCount++], Buf, TWR_PROFILER_MAX_STRING_SIZE - 1);
} /* End of 'TWR_ProfilerAddText' function */

/* NVidia OpenGL extension defines */
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049

/* Display all profiler data function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerDisplay( VOID )
{
  INT i, len = 0, cnt = 0, hn = 1, tn = 0, x = 0, y = 47, tr_w = 370;
  INT total_mem_kb = 0, cur_avail_mem_kb = 0;
  static CHAR Buf[100000];
  static DBL LastRedrawTime;

  if (hn < TWR_ProfilerStampsFilled)
    hn = (INT)TWR_ProfilerStampsFilled;

  /*
  if (!Tr.IsVisible)
    continue;
  */

  /* Update texture */
  glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &total_mem_kb);
  glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &cur_avail_mem_kb);
  len += sprintf(Buf + len, "GPU Mem usage: %.2f MiB / %.2f MiB\n",
    (total_mem_kb - cur_avail_mem_kb) / 1024.0,
    total_mem_kb / 1024.0);

  TWR_ProfilerPutStr(x + 8, y + 8, Buf, 0xFFCCFFCC);
  Buf[0] = 0;
  len = 0;
  for (i = 0; i < TWR_ProfilerStampsFilled; i++)
    if (TWR_ProfilerStamps[i].Name != NULL)
    {
      DBL pc = TWR_ProfilerStamps[0].Average != 0 ? 100 * TWR_ProfilerStamps[i].Average / TWR_ProfilerStamps[0].Average : i == 0 ? 100 : 0;

      len += sprintf(Buf + len, "%22.22s:%9.5fms [%6.2f%%]\n",
        TWR_ProfilerStamps[i].Name, TWR_ProfilerStamps[i].Average * 1000, pc);
      if (i == 0 && TWR_ProfilerStamps[i].Average != 0)
        TWR_ProfilerFPS = 1.0 / TWR_ProfilerStamps[i].Average;
    }
  TWR_ProfilerPutStr(x + 8, y + 8 + TWR_ProfilerFontH, Buf, 0xFFFFFFAA);

  TWR_ProfilerPutBar(x + 8 - 8, y + 8 + 4, 8, 8, TWR_ProfilerIsCollection ? 0xFF00FF00 : 0xFFFF0000);

  for (i = 0; i < TWR_ProfilerStingsCount; i++)
  {
    sprintf(Buf, "%02i:", i);
    TWR_ProfilerPutStr(x + tr_w, y + 8 + i * TWR_ProfilerFontH, Buf, 0xFFFFAAFF);
    TWR_ProfilerPutStr(x + tr_w + TWR_ProfilerFontW * 3, y + 8 + i * TWR_ProfilerFontH, TWR_ProfilerStings[i], 0xFFAAFFFF);
  }

  x += tr_w;
  if (x + tr_w >= TWR_ProfilerFrameW)
  {
    x = 0;
    y += (hn + 3) * TWR_ProfilerFontH;
    hn = 1;
  }
} /* End of 'TWR_ProfilerDisplay' function */

/* END OF 'twr_profiler_main.c' FILE */
