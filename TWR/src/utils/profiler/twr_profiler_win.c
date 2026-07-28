/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler_win.c
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler window handle implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include <dwmapi.h>
#include "twr_profiler_local.h"

#pragma comment(lib, "dwmapi")

/* Profile window class name */
#define TWR_PROFILER_WIN_CLASS_NAME "TWR Profile Window Class Name"

/***
 * Double buffering data
 ***/

/* Profiler window handle */
HWND TWR_hProfilerWnd;

/* Memory device context */
HDC TWR_hProfilerDCFrame;

/* Frame buffer bitmap */
HBITMAP TWR_hProfilerBmFrame;
/* Frame buffer pixel color data */
DWORD *TWR_ProfilerFrameBits;

/* Frame size */
INT TWR_ProfilerFrameW, TWR_ProfilerFrameH;

/* Frame zoom */
INT TWR_ProfilerFrameZoom = 1;

/* Create profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerInit( VOID )
{
  HWND hWnd;
  WNDCLASSEX wc = {0};
  RECT rc;
  LARGE_INTEGER ts;

  /* Register window class */
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hIconSm = LoadIcon(NULL, IDI_EXCLAMATION);
  wc.hInstance = Twr->hInstance;
  wc.lpfnWndProc = TWR_ProfilerWindowFunc;
  wc.lpszClassName = TWR_PROFILER_WIN_CLASS_NAME;

  if (!RegisterClassEx(&wc))
  {
    MessageBox(Twr->hWnd, "Error register window class", "ERROR", MB_OK);
    return;
  }

  /* Create window */
  GetWindowRect(Twr->hWnd, &rc);
  hWnd =
    CreateWindowEx( /* WS_EX_TOOLWINDOW | WS_EX_PALETTEWINDOW |  WS_EX_TOPMOST | */ WS_EX_CLIENTEDGE,
      TWR_PROFILER_WIN_CLASS_NAME,
      "TWR Profiler",
      WS_VISIBLE | WS_OVERLAPPED | WS_MAXIMIZEBOX | /* WS_MINIMIZEB    OX | */ WS_SYSMENU | WS_THICKFRAME | WS_CAPTION,
      /* rc.right, 8, */
      CW_USEDEFAULT, CW_USEDEFAULT, 
      800, 300, /* GetSystemMetrics(SM_CYSCREEN) - 29, */
      NULL,
      NULL, Twr->hInstance, NULL);

  SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED); /*  | WS_EX_STATICEDGE); */

  /* Setup alpha layer for profiler */
  SetLayeredWindowAttributes(hWnd, 0, 220, LWA_ALPHA);

  /* Show window */
  UpdateWindow(hWnd);
  ShowWindow(hWnd, SW_SHOWNORMAL);
  TWR_hProfilerWnd = hWnd;

  /* Timer init */
  QueryPerformanceCounter(&ts);
  TWR_ProfilerStartTime = ts.QuadPart;
  /* Timer resolution */
  QueryPerformanceFrequency(&ts);
  TWR_ProfilerTimePerSec = ts.QuadPart;
  SetActiveWindow(Twr->hWnd);

  Twr->Sys("Profiler is initialized");
} /* End of 'TWR_ProfilerInit' function */

/* Deinitialize profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerClose( VOID )
{
  DestroyWindow(TWR_hProfilerWnd);
  TWR_hProfilerWnd = NULL;
  Twr->Sys("Profiler is deinitialized");
} /* End of 'TWR_ProfilerClose' function */

/* Show/hide profiler window function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ProfilerShowHide( VOID )
{
  DWORD style;

  style = GetWindowLong(TWR_hProfilerWnd, GWL_STYLE);
  if (style & WS_VISIBLE)
    ShowWindow(TWR_hProfilerWnd, SW_HIDE);
  else
    ShowWindow(TWR_hProfilerWnd, SW_SHOW);
} /* End of 'TWR_ProfilerShowHide' function */

/* END OF 'twr_profiler_win.c' FILE */
