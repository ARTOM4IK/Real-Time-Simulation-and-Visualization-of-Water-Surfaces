/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_profiler_msg.c
 * PURPOSE     : Tough Water Rendering project.
 *               Profiler window messages handle implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 26.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include <stdio.h>
#include <psapi.h>
#include "twr_profiler_local.h"

#pragma comment(lib, "psapi")

/***
 * Cracked message handle functions
 ***/

/* WM_CREATE window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - structure with creation data:
 *       CREATESTRUCT *CS;
 * RETURNS:
 *   (BOOL) TRUE to continue creation window, FALSE to terminate.
 */
static BOOL OnCreate( HWND hWnd, CREATESTRUCT* CS )
{
  HDC hDC = GetDC(hWnd);

  TWR_hProfilerDCFrame = CreateCompatibleDC(hDC);
  ReleaseDC(hWnd, hDC);
  TWR_hProfilerBmFrame = NULL;

  SetTimer(hWnd, 102, 1000 + 102 + 0 * 28 * 102 /* 1000 + 102 */, NULL);

  return TRUE;
} /* End of 'OnCreate' function */

/* WM_DESTROY window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 * RETURNS: None.
 */
static VOID OnDestroy( HWND hWnd )
{
  DeleteObject(TWR_hProfilerBmFrame);
  DeleteDC(TWR_hProfilerDCFrame);
} /* End of 'OnDestroy' function */

/* WM_SIZE window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - sizing flag (see SIZE_***, like SIZE_MAXIMIZED)
 *       UINT State;
 *   - new width and height of client area:
 *       INT W, H;
 * RETURNS: None.
 */
static VOID OnSize( HWND hWnd, UINT State, INT W, INT H )
{
  BITMAPINFOHEADER bmih;

  if (State == SIZE_MINIMIZED)
    /* Block window collapsing */
    ShowWindow(hWnd, SW_SHOWNORMAL);

  if (TWR_hProfilerBmFrame != NULL)
    DeleteObject(TWR_hProfilerBmFrame);

  /* Create new frame */
  bmih.biSize = sizeof(BITMAPINFOHEADER);
  bmih.biBitCount = 32;                  /* BGRx */
  bmih.biPlanes = 1;                     /* Always 1 */
  bmih.biCompression = BI_RGB;           /* 0 - no compression */
  bmih.biWidth = W;                      /* width */
  bmih.biHeight = -H;                    /* height, order from top to bottom */
  bmih.biSizeImage = W * H * 4;          /* image size in bytes */
  bmih.biClrUsed = 0;                    /* no color table */
  bmih.biClrImportant = 0;
  bmih.biXPelsPerMeter = bmih.biYPelsPerMeter = 0; /* resolution */
  TWR_hProfilerBmFrame = CreateDIBSection(NULL, (BITMAPINFO *)&bmih, DIB_RGB_COLORS, (VOID **)&TWR_ProfilerFrameBits, NULL, 0);

  TWR_ProfilerFrameW = W;
  TWR_ProfilerFrameH = H;
  SelectObject(TWR_hProfilerDCFrame, TWR_hProfilerBmFrame);
  SendMessage(hWnd, WM_TIMER, 0, 0);
} /* End of 'OnSize' function */

/* WM_TIMER window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - specified the timer identifier.
 *       INT Id;
 * RETURNS: None.
 */
static VOID OnTimer( HWND hWnd, INT Id )
{
  HDC hDC;
  const CHAR *str;
  SYSTEMTIME st;
  PROCESS_MEMORY_COUNTERS mi;
  DWORD *Ptr = TWR_ProfilerFrameBits, counter = TWR_ProfilerFrameW * TWR_ProfilerFrameH;
  static CHAR Buf[102];

  /* Redraw profiler screen */
  /* memset(TWR_ProfilerFrameBits, 0x47, TWR_ProfilerFrameW * TWR_ProfilerFrameH * 4); */
  while (counter-- > 0)
    *Ptr++ = 0x47304780;

  GetLocalTime(&st);
  TWR_ProfilerPutStr(8, 8, "TWR Profiler Status", 0xFFFFFFFF);
  sprintf(Buf, "FPS: %.3f (%.3f)", TWR_ProfilerFPS, Twr->FPS);
  TWR_ProfilerPutStr(-8 + 102 + 90, 27, Buf, 0xFFAAFFFF);
  sprintf(Buf, "%02d:%02d:%02d %02d.%02d.%04d",
    st.wHour, st.wMinute, st.wSecond, st.wDay, st.wMonth, st.wYear);
  TWR_ProfilerPutStr(8, 27, Buf, 0xFFFFCCCC);
  sprintf(Buf, "Frame: %dx%d (time: %.3f sec)",
    Twr->W, Twr->H, 0 /* Twr->GlobalTime */);
  TWR_ProfilerPutStr(8, 42, Buf, 0xFF88FF88);

  mi.cb = sizeof(mi);
  GetProcessMemoryInfo(GetCurrentProcess(), &mi, sizeof(mi));
  sprintf(Buf, "Mem: %.1f MiB", mi.WorkingSetSize / (1024 * 1024.0));
  TWR_ProfilerPutStr(8 + 102 + 50 + 18, 8, Buf, 0xFFCCFFCC);

  str = glGetString(GL_VERSION);
  if (str != NULL)
    TWR_ProfilerPutStr(8 + 102 + 50 + 2 * 30 + 18 + 47 * 3, 8, str, 0xFF00FF00);
  str = glGetString(GL_RENDERER);
  if (str != NULL)
    TWR_ProfilerPutStr(8 + 102 + 50 + 2 * 30 + 18 + 47 * 3, 8 + 16, str, 0xFF00FF00);
  str = glGetString(GL_VENDOR);
  if (str != NULL)
    TWR_ProfilerPutStr(8 + 102 + 50 + 2 * 30 + 18 + 47 * 3, 8 + 16 * 2, str, 0xFF00FF00);

  TWR_ProfilerDisplay();

  /* Output to window */
  hDC = GetDC(hWnd);
  /* BitBlt(hDC, 0, 0, TWR_ProfilerFrameW, TWR_ProfilerFrameH, TWR_hProfilerDCFrame, 0, 0, SRCCOPY); */
  StretchBlt(hDC, 0, 0, TWR_ProfilerFrameW * TWR_ProfilerFrameZoom, TWR_ProfilerFrameH * TWR_ProfilerFrameZoom, TWR_hProfilerDCFrame, 0, 0, TWR_ProfilerFrameW, TWR_ProfilerFrameH, SRCCOPY);
  ReleaseDC(hWnd, hDC);
} /* End of 'OnTimer' function */

/* WM_ERASEBKGND window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS:
 *   (BOOL) TRUE if background is erased, FALSE otherwise.
 */
static BOOL OnEraseBkgnd( HWND hWnd, HDC hDC )
{
  return TRUE;
} /* End of 'OnEraseBkgnd' function */

/* WM_PAINT window message handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - device context of client area:
 *       HDC hDC;
 * RETURNS: None.
 */
static VOID OnPaint( HWND hWnd, HDC hDC )
{
  /* BitBlt(hDC, 0, 0, TWR_ProfilerFrameW, TWR_ProfilerFrameH, TWR_hProfilerDCFrame, 0, 0, SRCCOPY); */
  StretchBlt(hDC, 0, 0, TWR_ProfilerFrameW * TWR_ProfilerFrameZoom, TWR_ProfilerFrameH * TWR_ProfilerFrameZoom, TWR_hProfilerDCFrame, 0, 0, TWR_ProfilerFrameW, TWR_ProfilerFrameH, SRCCOPY);
} /* End of 'OnPaint' function */

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
                                         WPARAM wParam, LPARAM lParam )
{  
  HDC hDC;
  PAINTSTRUCT ps;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    ((MINMAXINFO *)lParam)->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) +
      GetSystemMetrics(SM_CYMENU) +
      GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    return OnCreate(hWnd, (CREATESTRUCT *)lParam) ? 0 : -1;
  case WM_DESTROY:
    OnDestroy(hWnd);
    return 0;
  case WM_SIZE:
    OnSize(hWnd, (UINT)wParam, (INT)(SHORT)LOWORD(lParam), (INT)(SHORT)HIWORD(lParam));
    return 0;
  case WM_ACTIVATE:
    /* OnActivate(hWnd, (UINT)LOWORD(wParam), (HWND)(lParam), (BOOL)HIWORD(wParam)); */
    return 0;
  case WM_CLOSE:
    /* Block window closing */
    ShowWindow(hWnd, SW_HIDE);
    return 0;
  case WM_ERASEBKGND:
    return (LRESULT)OnEraseBkgnd(hWnd, (HDC)wParam);
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    OnPaint(hWnd, hDC);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_TIMER:
    OnTimer(hWnd, (UINT)wParam);
    return 0;
  case WM_HSCROLL:
    /* OnHScroll(hWnd, (HWND)lParam, LOWORD(wParam), HIWORD(wParam)); */
    return 0;
  case WM_VSCROLL:
    /* OnVScroll(hWnd, (HWND)lParam, LOWORD(wParam), HIWORD(wParam)); */
    return 0;
  case WM_ENTERSIZEMOVE:
    /* OnEnterSizeMove(hWnd); */
    return 0;
  case WM_EXITSIZEMOVE:
    /* OnExitSizeMove(hWnd); */
    return 0;
  /*
  case WM_SETFOCUS:
    return 0;
  */
  case WM_KEYDOWN:
    /* Temporary */
    switch (wParam)
    {
    case 'P':
      if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 &&
          (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 &&
          (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0)
        TWR_ProfilerShowHide();
      return 0;
    case 'C':
      if ((GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 &&
          (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 &&
          (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0)
        TWR_ProfilerStartStop();
      return 0;
    case VK_ADD:
      if (TWR_ProfilerFrameZoom < 8)
        TWR_ProfilerFrameZoom++;
      break;
    case VK_SUBTRACT:
      if (TWR_ProfilerFrameZoom > 1)
        TWR_ProfilerFrameZoom--;
      break;
    case '1':
      SetLayeredWindowAttributes(hWnd, 0, 90, LWA_ALPHA);
      break;
    case '2':
      SetLayeredWindowAttributes(hWnd, 0, 130, LWA_ALPHA);
      break;
    case '3':
      SetLayeredWindowAttributes(hWnd, 0, 190, LWA_ALPHA);
      break;
    case '4':
      SetLayeredWindowAttributes(hWnd, 0, 220, LWA_ALPHA);
      break;
    case '5':
      SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);
      break;
    }
    OnTimer(hWnd, 0);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'TWR_ProfilerWindowFunc' function */

/* END OF 'twr_profiler_msg.c' FILE */
