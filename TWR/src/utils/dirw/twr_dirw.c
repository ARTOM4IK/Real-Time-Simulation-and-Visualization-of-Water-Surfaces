/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_dirw.c
 * PURPOSE     : Tough Water Rendering project.
 *               Directories control module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 09.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Stop directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 * RETURNS: None.
 */
VOID TWR_DirWatcher_StopWatch( twrDIR_WATCHER *Wtch )
{
  if (Wtch->Ovr.hEvent != NULL)
    CloseHandle(Wtch->Ovr.hEvent);
  if (Wtch->hDir != NULL)
    CloseHandle(Wtch->hDir);
 
  ZeroMemory(&Wtch->Ovr, sizeof(OVERLAPPED));
  Wtch->hDir = NULL;
  Wtch->Path[0] = 0;
  Wtch->Buffer[0] = 0;
} /* End of 'TWR_DirWatcher_StopWatch' function */ 

/* Start directory watching function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 *   - directory path:
 *       CHAR *DirPath; 
 * RETURNS: None. 
 */ 
VOID TWR_DirWatcher_StartWatch( twrDIR_WATCHER *Wtch, CHAR *DirPath ) 
{ 
  TWR_DirWatcher_StopWatch(Wtch);
  strncpy(Wtch->Path, DirPath, _MAX_PATH); 
 
  /* Open directory */
  if (DirPath != NULL)
    Wtch->hDir = 
      CreateFile(DirPath, 
        FILE_LIST_DIRECTORY, 
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL, OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL);

  /* Start monitoring */
  Wtch->Ovr.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
  ReadDirectoryChangesW(Wtch->hDir,
    Wtch->Buffer, sizeof(Wtch->Buffer),
    TRUE,
    FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
      FILE_NOTIFY_CHANGE_FILE_NAME,
    NULL,
    &Wtch->Ovr,
    NULL);
} /* End of 'TWR_DirWatcher_StartWatch' function */
 
/* Check if directory is changed function.
 * ARGUMENTS:
 *   - directory watcher data:
 *       twrDIR_WATCHER *Wtch;
 * RETURNS:
 *   (BOOL) TRUE if directory contents is changed (and subfolders). 
 */ 
BOOL TWR_DirWatcher_IsChanged( twrDIR_WATCHER *Wtch )
{
  INT id;
 
  if (Wtch->Ovr.hEvent == NULL || Wtch->hDir == NULL)
    return FALSE;
 
  id = WaitForSingleObject(Wtch->Ovr.hEvent, 0);
  if (id == WAIT_OBJECT_0)
  {
    /* Restart monitoring */
    ResetEvent(Wtch->Ovr.hEvent);
    ReadDirectoryChangesW(Wtch->hDir,
      Wtch->Buffer, sizeof(Wtch->Buffer),
      TRUE,
      FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION |
        FILE_NOTIFY_CHANGE_FILE_NAME,
      NULL,
      &Wtch->Ovr,
      NULL);
    Sleep(30 * 10);
    /*
    TWR_DirWatcher_StartWatch(Wtch, Wtch->Path);
    */
    
    return TRUE;
  }
  return FALSE;
} /* End of 'TWR_DirWatcher_IsChanged' function */

/* END OF 'twr_dirw.c' FILE */
