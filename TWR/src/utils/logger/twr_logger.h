/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_logger.h
 * PURPOSE     : Tough Water Rendering project.
 *               System logger declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 28.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_logger_h_
#define __twr_logger_h_

#include "twr_def.h"

/* Logger system representation type */
typedef struct tagtwrSYSTEM_LOGGER
{
  /* Console window handle */
  HWND hConWnd;

  /* Log ordinary message (without prefix) output function.
   * ARGUMENTS:
   *   - format string:
   *       CHAR *Fmt;
   *   - arbitrary arguments list:
   *       ...;
   * RETURNS: None.
   */
  VOID (*Log)( CHAR *Fmt, ... );

  /* Inform message output function.
   * ARGUMENTS:
   *   - format string:
   *       CHAR *Fmt;
   *   - arbitrary arguments list:
   *       ...;
   * RETURNS: None.
   */
  VOID (*Info)( CHAR *Fmt, ... );

  /* Warning message output function.
   * ARGUMENTS:
   *   - format string:
   *       CHAR *Fmt;
   *   - arbitrary arguments list:
   *       ...;
   * RETURNS: None.
   */
  VOID (*Warning)( CHAR *Fmt, ... );

  /* Error message output function.
   * ARGUMENTS:
   *   - format string:
   *       CHAR *Fmt;
   *   - arbitrary arguments list:
   *       ...;
   * RETURNS: None.
   */
  VOID (*Error)( CHAR *Fmt, ... );

  /* Systen message output function.
   * ARGUMENTS:
   *   - format string:
   *       CHAR *Fmt;
   *   - arbitrary arguments list:
   *       ...;
   * RETURNS: None.
   */
  VOID (*Sys)( CHAR *Fmt, ... );

  /* Change output color function.
   * ARGUMENTS:
   *   - foreground and background colors (RGB in DWORD pack):
   *       DWORD ForeColor, BackColor;
   * RETURNS: None.
   */
  VOID (*SetColor)( DWORD ForeColor, DWORD BackColor );
} twrSYSTEM_LOGGER;

/* Logger system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLoggerInit( VOID );

/* Logger system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLoggerClose( VOID );

#endif /* __twr_logger_h_ */

/* END OF 'twr_logger.h' FILE */
