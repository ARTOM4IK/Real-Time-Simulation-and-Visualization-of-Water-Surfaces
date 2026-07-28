/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_def.h
 * PURPOSE     : Tough Water Rendering project.
 *               Project common declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Belyh Andrey (AB7),
 *               Novozhilova Maryana (MN7),
 *               Porotikov Valery (VP7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 24.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_def_h_
#define __twr_def_h_

#ifdef _DEBUG
#  define _CRTDBG_MAP_ALLOC
#  include <crtdbg.h>
#  define SetDbgMemHooks() \
     _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
       _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
#else /* _DEBUG */
#  define SetDbgMemHooks() ((VOID)0)
#endif /* _DEBUG */
#include <stdlib.h>

#include "mth/twr_mth.h"

#define TWR_STR_MAX 300

/* Large integer types */
typedef long long INT64;
typedef unsigned long long UINT64;

/***
 * Future usage types
 ***/

/* Structured parameters representation type */
typedef struct tagtwrPARAMS
{
  BYTE Dummy;
} twrPARAMS;

/***
 * Types forward declarations.
 ***/

/* Forward declaration of primitive structure */
typedef struct tagtwrPRIM twrPRIM;

typedef struct tagtwrMATERIAL twrMATERIAL;
typedef struct tagtwrMATERIAL_PATTERN twrMATERIAL_PATTERN;

typedef struct tagtwrSHADER twrSHADER;
typedef struct tagtwrTEXTURE twrTEXTURE;
typedef struct tagtwrBUFFER twrBUFFER;

/***
 * Helpful functions
 ***/

/* Check is directory exists function.
 * ARGUMENTS:
 *   - directory path name:
 *       CHAR *DirName;
 * RETURNS:
 *   (BOOL) TRUE if directory exists, FALSE otherwise.
 */
__inline BOOL TWR_IsDirExists( CHAR *DirName )
{
  DWORD file_type = GetFileAttributes(DirName);

  if (file_type == INVALID_FILE_ATTRIBUTES)
    return FALSE;
  if (file_type & FILE_ATTRIBUTE_DIRECTORY)
    return TRUE;
  return FALSE;
} /* End of 'TWR_IsDirExists' function */

#endif /* __twr_def_h_ */

/* END OF 'twr_def.h' FILE */
