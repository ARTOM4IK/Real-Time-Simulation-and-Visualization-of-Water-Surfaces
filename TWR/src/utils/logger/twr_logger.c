/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_logger.c
 * PURPOSE     : Tough Water Rendering project.
 *               System logger implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* Output attributes */
#define TWR_ATTR_INFO 0x1F
#define TWR_ATTR_WARNING 0x6F
#define TWR_ATTR_ERROR 0x4F
#define TWR_ATTR_SYS 0x2F
#define TWR_ATTR_TEXT 0x0F
#define TWR_ATTR_TIME 0x0A

/* Output buffer */
static CHAR TWR_LoggerBuffer[100000];

/* Set output attribute function.
 * ARGUMENTS:
 *   - color attribute (low 4 bits forecolor, high 4 bits - backcolor, IRGB system):
 *       INT Attr;
 * RETURNS: None.
 */
static VOID TWR_SetAttr( INT Attr )
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attr);
} /* End of 'TWR_SetAttr' function */

/* Log variable vars message function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       va_list ArgPtr;
 * RETURNS: None.
 */
static VOID TWR_LoggerLogV( CHAR *Fmt, va_list ArgPtr )
{
  if (Fmt[0] == 0)
    return;

  vsprintf(TWR_LoggerBuffer, Fmt, ArgPtr);

  /* Output text */
  printf("%s", TWR_LoggerBuffer);
} /* End of 'TWR_LoggerLogV' function */

/* Log ordinary message (without prefix) output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       ...;
 * RETURNS: None.
 */
static VOID TWR_LoggerLog( CHAR *Fmt, ... )
{
  va_list ap;

  va_start(ap, Fmt);
  TWR_LoggerLogV(Fmt, ap);
  va_end(ap);
} /* End of 'TWR_LoggerLog' function */

/* Output current time function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_OutTime( VOID )
{
  struct tm *tim;
  time_t t;
  static CHAR Buf[300];

  time(&t);
  tim = localtime(&t);
  strftime(Buf, sizeof(Buf) - 1, "%Y-%m-%d %H:%M:%S ", tim);

  TWR_SetAttr(TWR_ATTR_TIME);
  TWR_LoggerLog("%s", Buf);
  TWR_SetAttr(TWR_ATTR_TEXT);
} /* End of 'TWR_SetAttr' function */

/* Inform message output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       ...;
 * RETURNS: None.
 */
static VOID TWR_LoggerInfo( CHAR *Fmt, ... )
{
  INT len = strlen(TWR_LoggerBuffer);
  va_list ap;

  if (len > 0 && TWR_LoggerBuffer[len - 1] != '\n')
    TWR_LoggerLog("\n");

  TWR_OutTime();
  TWR_SetAttr(TWR_ATTR_INFO);
  TWR_LoggerLog("[INFO]");
  TWR_SetAttr(TWR_ATTR_TEXT);
  TWR_LoggerLog(" ");

  va_start(ap, Fmt);
  TWR_LoggerLogV(Fmt, ap);
  va_end(ap);
} /* End of 'TWR_LoggerInfo' function */

/* Warning message output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       ...;
 * RETURNS: None.
 */
static VOID TWR_LoggerWarning( CHAR *Fmt, ... )
{
  INT len = strlen(TWR_LoggerBuffer);
  va_list ap;

  if (len > 0 && TWR_LoggerBuffer[len - 1] != '\n')
    TWR_LoggerLog("\n");

  TWR_OutTime();
  TWR_SetAttr(TWR_ATTR_WARNING);
  TWR_LoggerLog("[WARNING]");
  TWR_SetAttr(TWR_ATTR_TEXT);
  TWR_LoggerLog(" ");

  va_start(ap, Fmt);
  TWR_LoggerLogV(Fmt, ap);
  va_end(ap);
} /* End of 'TWR_LoggerWarning' function */

/* Error message output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       ...;
 * RETURNS: None.
 */
static VOID TWR_LoggerError( CHAR *Fmt, ... )
{
  INT len = strlen(TWR_LoggerBuffer);
  va_list ap;

  if (len > 0 && TWR_LoggerBuffer[len - 1] != '\n')
    TWR_LoggerLog("\n");

  TWR_OutTime();
  TWR_SetAttr(TWR_ATTR_ERROR);
  TWR_LoggerLog("[ERROR]");
  TWR_SetAttr(TWR_ATTR_TEXT);
  TWR_LoggerLog(" ");

  va_start(ap, Fmt);
  TWR_LoggerLogV(Fmt, ap);
  va_end(ap);
} /* End of 'TWR_LoggerError' function */

/* System message output function.
 * ARGUMENTS:
 *   - format string:
 *       CHAR *Fmt;
 *   - arbitrary arguments list:
 *       ...;
 * RETURNS: None.
 */
static VOID TWR_LoggerSys( CHAR *Fmt, ... )
{
  INT len = strlen(TWR_LoggerBuffer);
  va_list ap;

  if (len > 0 && TWR_LoggerBuffer[len - 1] != '\n')
    TWR_LoggerLog("\n");

  TWR_OutTime();
  TWR_SetAttr(TWR_ATTR_SYS);
  TWR_LoggerLog("[SYS]");
  TWR_SetAttr(TWR_ATTR_TEXT);
  TWR_LoggerLog(" ");

  va_start(ap, Fmt);
  TWR_LoggerLogV(Fmt, ap);
  va_end(ap);
} /* End of 'TWR_LoggerSys' function */

/* Change output color function.
 * ARGUMENTS:
 *   - foreground and background colors (RGB in DWORD pack):
 *       DWORD ForeColor, BackColor;
 * RETURNS: None.
 */
static VOID TWR_LoggerSetColor( DWORD ForeColor, DWORD BackColor )
{
  printf("\x1b[38;2;%d;%d;%dm\x1b[48;2;%d;%d;%dm",
    COM_LONGBYTE2(ForeColor), COM_LONGBYTE1(ForeColor), COM_LONGBYTE0(ForeColor),
    COM_LONGBYTE2(BackColor), COM_LONGBYTE1(BackColor), COM_LONGBYTE0(BackColor));
} /* End of 'TWR_LoggerSetColor' function */

/* Logger system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLoggerInit( VOID )
{
  HMONITOR hMon;
  MONITORINFOEX moninfo;
  HRSRC hrsrc;
  HGLOBAL hm;
  RECT rc;
  INT ConWidth, ConHeight, ConPosX, ConPosY;
  UINT size;
  CONSOLE_FONT_INFOEX cfi = {0};
  HINSTANCE hInstance = GetModuleHandle(NULL);

  /* Console initialization */
  AllocConsole();
  cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
  GetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
  cfi.dwFontSize.Y = 18;
  cfi.FontWeight = FW_BOLD;
  SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

  Twr->hConWnd = GetConsoleWindow();

  /* Set console window in the center */
  hMon = MonitorFromWindow(Twr->hConWnd, MONITOR_DEFAULTTONEAREST);
  moninfo.cbSize = sizeof(moninfo);
  GetMonitorInfo(hMon, (MONITORINFO *)&moninfo);
  rc = moninfo.rcMonitor;
  AdjustWindowRect(&rc, GetWindowLong(Twr->hConWnd, GWL_STYLE), FALSE);

  ConWidth = (rc.right - rc.left) * 0.8;
  ConHeight = (rc.bottom - rc.top) * 0.8;
  ConPosX = ConWidth / 14;
  ConPosY = ConHeight / 14;
  /* Move-resize console to specified position */
  SetWindowPos(Twr->hConWnd, 0, ConPosX, ConPosY, ConWidth, ConHeight, 0);

  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");
  if ((hrsrc = FindResource(hInstance, "CGSG.LOGO.ASCII", "RC_DATA")) != NULL &&
      (size = SizeofResource(hInstance, hrsrc)) != 0 &&
      (hm = LoadResource(hInstance, hrsrc)) != NULL)
  {
    printf("%s", (CHAR *)LockResource(hm));
    UnlockResource(hm);
  }

  SetConsoleTitle("CGSG Tough Water Rendering 2025 Project. CGSG forever!!!");

  /* Set console language */
  freopen("CONOUT$", "w", stdout);
  system("@chcp 1251 > nul");

  /* Set project logger */
  SetConsoleOutputCP(1251);
  TWR_OutTime();
  TWR_SetAttr(0x0E);
  TWR_LoggerLog("[TWR START]");
  TWR_SetAttr(TWR_ATTR_TEXT);
  TWR_LoggerSetColor(0xFFFF00, 0x880000);
  TWR_LoggerLog(" Start TWR system");
  TWR_SetAttr(TWR_ATTR_TEXT);


  /* Setup function pointers */
  Twr->Log = TWR_LoggerLog;
  Twr->Info = TWR_LoggerInfo;
  Twr->Warning = TWR_LoggerWarning;
  Twr->Error = TWR_LoggerError;
  Twr->Sys = TWR_LoggerSys;
  Twr->SetColor = TWR_LoggerSetColor;

  SetForegroundWindow(Twr->hConWnd);
  Twr->Sys("Logger system is initialized");
} /* End of 'TWR_SystemLoggerInit' function */

/* Logger system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLoggerClose( VOID )
{
  Twr->Sys("Logger system is deinitialized\n\n");
  FreeConsole();
} /* End of 'TWR_SystemLoggerClose' function */

/* END OF 'twr_logger.c' FILE */
