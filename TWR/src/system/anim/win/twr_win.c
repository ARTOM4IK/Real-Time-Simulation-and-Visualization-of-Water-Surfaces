/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_win.c
 * PURPOSE     : Tough Water Rendering project.
 *               Window depended WinAPI handle implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Window class name */
#define TWR_WINDOW_CLASS_NAME "Tough Water Rendering project main window class name"

/* Window name */
#define TWR_WINDOW_TITLE "CGSG'Jr'2024::Tough Water Rendering system"

/* Main window handle function.
 * ARGUMENTS:
 *   - window handle:
 *       HWND hWnd;
 *   - message type (see WM_***):
 *       UINT Msg;
 *   - message 'word' parameter:
 *       WPARAM wParam;
 *   - message 'long' parameter:
 *       LPARAM lParam;
 * RETURNS:
 *   (LRESULT) message depended return value.
 */
static LRESULT CALLBACK TWR_WinMainWndFunc( HWND hWnd, UINT Msg,
                                            WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  
  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMinTrackSize.y =
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2 + 30;
    minmax->ptMinTrackSize.x =
      GetSystemMetrics(SM_CYBORDER) * 2 + 30;
    minmax->ptMaxTrackSize.y =
      GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 47, 0, NULL);
    return 0;
  case WM_SIZE:
    Twr->W = LOWORD(lParam);
    Twr->H = HIWORD(lParam);
    if (Twr->IsInit)
    {
      Twr->AnimResize(Twr->W, Twr->H);
      Twr->AnimRender();
    }
    return 0;
  case WM_TIMER:
    if (Twr->IsInit)
      Twr->AnimRender();
    return 0;
  case WM_ERASEBKGND:
    if (Twr->IsInit)
      Twr->AnimRender();
    return 1;
  case WM_KEYDOWN:
    if (wParam == VK_ESCAPE)
    {
      if (MessageBox(Twr->hWnd, "Do you want to leave from program?", "TWR", MB_YESNO | MB_ICONQUESTION) == IDYES)
        SendMessage(hWnd, WM_CLOSE, 0, 0);
    }
    else if (wParam == VK_F11)
      Twr->FlipFullScreen();
    else if (wParam == 'P' &&
             (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 &&
             (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 &&
             (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0)
      TWR_ProfilerShowHide();
    else if (wParam == 'Q' &&
             (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0 &&
             (GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0 &&
             (GetAsyncKeyState(VK_LMENU) & 0x8000) != 0)
    {
      DWORD style = GetWindowLong(Twr->hConWnd, GWL_STYLE);
      if (style & WS_VISIBLE)
        ShowWindow(Twr->hConWnd, SW_HIDE);
      else
        ShowWindow(Twr->hConWnd, SW_SHOW);
    }
    break;
  case WM_SYSKEYDOWN:
    if (wParam == VK_RETURN)
      Twr->FlipFullScreen();
    break;
  case WM_LBUTTONDOWN:
    SetCapture(hWnd);
    return 0;
  case WM_LBUTTONUP:
    ReleaseCapture();
    return 0;
  case WM_MOUSEWHEEL:
    Twr->MouseWheel += (SHORT)HIWORD(wParam);
    return 0;
  case WM_ENTERSIZEMOVE:
    Twr->IsActive = FALSE;
    return 0;
  case WM_EXITSIZEMOVE:
    Twr->IsActive = TRUE;
    return 0;
  case WM_ACTIVATE:
    Twr->IsActive = wParam != WA_INACTIVE ? TRUE : FALSE;
    return 0;
  case WM_CLOSE:
    break;
  case WM_PAINT:
    if (Twr->IsInit)
      Twr->AnimRender();
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_DESTROY:
    KillTimer(hWnd, 47);
    PostQuitMessage(30);
    Twr->hWnd = NULL;
    Twr->W = Twr->H = 0;
    Twr->IsActive = FALSE;
    Twr->IsFullScreen = FALSE;
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
} /* End of 'TWR_WinMainWndFunc' function */

/* FullScreen mode flip function.
 * ARGUMEMTS: None.
 * RETURNS: None.
 */
static VOID TWR_FlipFullScreen( VOID )
{
  static RECT SaveRect;

  if (Twr->IsFullScreen)
  {
    Twr->IsFullScreen = FALSE;
    /* Restore window rect */
    SetWindowPos(Twr->hWnd, HWND_NOTOPMOST,
      SaveRect.left, SaveRect.top, SaveRect.right - SaveRect.left, SaveRect.bottom - SaveRect.top,
      SWP_NOOWNERZORDER);
  }
  else
  {
    HMONITOR hMon;
    MONITORINFOEX moninfo;
    RECT rc;

    Twr->IsFullScreen = TRUE;

    /* Store window rect */
    GetWindowRect(Twr->hWnd, &SaveRect);

    /* Obtain monitor info */
    hMon = MonitorFromWindow(Twr->hWnd, MONITOR_DEFAULTTONEAREST);
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);
    rc = moninfo.rcMonitor;
    AdjustWindowRect(&rc, GetWindowLong(Twr->hWnd, GWL_STYLE), FALSE);
    /* Expand window */
    SetWindowPos(Twr->hWnd, HWND_TOP, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOOWNERZORDER);
  }
} /* End of 'TWR_FlipFullScreen' function */

/* Project main loop run function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_WinRun( VOID )
{
  MSG msg;

  while (TRUE)
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
        break;
      DispatchMessage(&msg);
    }
    else
      if (Twr->IsInit)
        Twr->AnimRender();
} /* End of 'TWR_WinRun' function */

/* Window subsystem initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemWindowInit( VOID )
{
  WNDCLASS wc;

  /* Store application instance */
  Twr->hInstance = GetModuleHandle(NULL);
  Twr->FlipFullScreen = TWR_FlipFullScreen;
  Twr->Run = TWR_WinRun;

  /* Fill window class description structure */
  wc.style = CS_HREDRAW | CS_VREDRAW;                    /* Window class style (see CS_***) */
  wc.cbClsExtra = 0;                                     /* Extra class memory size in bytes */
  wc.cbWndExtra = 0;                                     /* Extra window memory size in bytes */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;               /* Background brush */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);               /* Cursor handle */
  wc.hIcon = LoadIcon(Twr->hInstance, (CHAR *)IDI_LOGO);         /* Icon handle */
  wc.lpszMenuName = NULL;                                /* Menu resource name */
  wc.hInstance = Twr->hInstance;                         /* Instance handle */
  wc.lpfnWndProc = TWR_WinMainWndFunc;                   /* Window message handle function pointer */
  wc.lpszClassName = TWR_WINDOW_CLASS_NAME;              /* Window class name */

  /* Register window class */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "TWR: ERROR",
      MB_OK | MB_ICONERROR);
    Twr->hWnd = NULL;
    exit(0);
  }

  /* Window creation */
  Twr->hWnd =
    CreateWindow(TWR_WINDOW_CLASS_NAME, /* Window class name */
      TWR_WINDOW_TITLE,                 /* Window caption */
      WS_OVERLAPPEDWINDOW | WS_VISIBLE, /* Window styles */
      /* CW_USEDEFAULT, CW_USEDEFAULT,  /* Window position */
      /* CW_USEDEFAULT, CW_USEDEFAULT,  /* Window size */
      0, 0, 2000, 1200,
      NULL,                             /* Owner window handle */
      NULL,                             /* Menu handle */
      Twr->hInstance,                       /* Instance handle */
      NULL);                            /* Pointer to window extra data */
  Twr->Sys("Window system is initialized");
} /* End of 'TWR_SystemWindowInit' function */

/* Window subsystem deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemWindowClose( VOID )
{
  Twr->Sys("Window system is deinitialized");
} /* End of 'TWR_SystemWindowClose' function */

/* END OF 'twr_win.c' FILE */
