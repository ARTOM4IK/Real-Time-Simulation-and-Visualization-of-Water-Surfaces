/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_dirw.h
 * PURPOSE     : Tough Water Rendering project.
 *               Directories control declaration module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 09.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr_def.h"

/* Directory watcher handle structure */
typedef struct tagtwrDIR_WATCHER
{
  /* Overlapped structure for changing hook */
  OVERLAPPED Ovr;
  /* Traching directory handle */
  HANDLE hDir;
  /* Read directory changes data buffer */
  BYTE Buffer[27 * 47];
  /* Store path */
  CHAR Path[_MAX_PATH];
} twrDIR_WATCHER;

/* Stop directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 * RETURNS: None.
 */
VOID TWR_DirWatcher_StopWatch( twrDIR_WATCHER *Wtch );

/* Check if directory is changed function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 * RETURNS:
 *   (BOOL) TRUE if directory contents is changed (and subfolders).
 */
BOOL TWR_DirWatcher_IsChanged( twrDIR_WATCHER *Wtch );

/* Start directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 *   - directory path:
 *       CHAR *DirPath; 
 * RETURNS: None. 
 */ 
VOID TWR_DirWatcher_StartWatch( twrDIR_WATCHER *Wtch, CHAR *DirPath );

/* END OF 'twr_dirw.h' FILE */
