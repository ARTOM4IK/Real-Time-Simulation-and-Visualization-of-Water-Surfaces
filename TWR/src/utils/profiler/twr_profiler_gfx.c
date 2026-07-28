/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler_gfx.c
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler graphics drawing implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_profiler_local.h"
#include "twr_profiler_font.h"

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
VOID TWR_ProfilerPutChar( INT X, INT Y, CHAR Ch, DWORD Color )
{
  INT i, j;
  BYTE *ptr = TWR_ProfilerFontData + (BYTE)Ch * TWR_ProfilerFontH;

  for (i = 0; i < TWR_ProfilerFontH; i++)
    for (j = 0; j < TWR_ProfilerFontW; j++)
    {
      INT x0 = X + j, y0 = Y + i;

      if (x0 >= 0 && x0 < TWR_ProfilerFrameW && y0 >= 0 && y0 < TWR_ProfilerFrameH && 
          (ptr[i] & (0x80 >> j)) != 0)
        TWR_ProfilerFrameBits[y0 * TWR_ProfilerFrameW + x0] = Color;
    }
} /* End of 'TWR_ProfilerPutChar' function */

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
VOID TWR_ProfilerPutStr( INT X, INT Y, const CHAR *Str, DWORD Color )
{
  INT SaveX = X;

  if (X >= TWR_ProfilerFrameW || Y >= TWR_ProfilerFrameH)
    return;

  while (*Str != 0)
    if (*Str == '\n')
    {
      X = SaveX;
      Y += TWR_ProfilerFontH;
      if (Y >= TWR_ProfilerFrameH)
        return;
      Str++;
    }
    else
      TWR_ProfilerPutChar(X, Y, *Str++, Color), X += TWR_ProfilerFontW;
} /* End of 'TWR_ProfilerPutStr' function */

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
VOID TWR_ProfilerPutBar( INT X, INT Y, INT W, INT H, DWORD Color )
{
  INT X2, Y2, delta;
  DWORD addr;

  /* Trivial reject */
  if (W <= 0 || H <= 0 || X >= TWR_ProfilerFrameW || Y >= TWR_ProfilerFrameH)
    return;
  X2 = X + W - 1;
  Y2 = Y + H - 1;
  if (X2 < 0 || Y2 < 0)
    return;

  /* Clipping */
  if (X < 0)
    X = 0;
  if (Y < 0)
    Y = 0;
  if (X2 >= TWR_ProfilerFrameW)
    X2 = TWR_ProfilerFrameW - 1;
  if (Y2 >= TWR_ProfilerFrameH)
    Y2 = TWR_ProfilerFrameH - 1;

  W = X2 - X + 1;
  H = Y2 - Y + 1;

  addr = Y * TWR_ProfilerFrameW + X;
  delta = TWR_ProfilerFrameW - W;
  while (H-- > 0)
  {
    INT cnt = W;
    while (cnt-- > 0)
      TWR_ProfilerFrameBits[addr++] = Color;
    addr += delta;
  }
} /* End of 'TWR_ProfilerPutBar' function */

/* END OF 'twr_profiler_gfx.c' FILE */
