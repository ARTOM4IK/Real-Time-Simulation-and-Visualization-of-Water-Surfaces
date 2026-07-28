/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_buffers.h
 * PURPOSE     : Tough Water Rendering project.
 *               Buffers declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 26.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_buffers_h_
#define __twr_res_buffers_h_

#include "twr_def.h"

/* Buffer type */
typedef struct tagtwrBUFFER
{
  CHAR Name[TWR_STR_MAX];   /* Buffer name */
  INT Size;                 /* Buffer size */
  UINT BufId;               /* Buffer identificator (for Open_GL) */
  UINT BindPoint;           /* Buffer binding point on shaders */
  INT RefCounter;           /* Reference counter */
} twrBUFFER;

/* Buffers binding points on shaders */
#define TWR_BUF_BIND_MATRIX_IN 0
#define TWR_BUF_BIND_MATRIX_OUT 1
#define TWR_BUF_BIND_CAMERA 2
#define TWR_BUF_BIND_SYNC 3
#define TWR_BUF_BIND_LIGHTS 4
#define TWR_BUF_BIND_SHADOW 5
#define TWR_BUF_BIND_MARKER 6
#define TWR_BUF_BIND_MATERIAL 7
#define TWR_BUF_BIND_FIRST_FREE 8

/* buffers system handle type */
typedef struct tagtwrSYSTEM_BUFFERS
{
  /* Add Buffer to stock function.
   * ARGUMENTS:
   *   - Buffer name:
   *       CHAR *Name;
   *   - OpenGL Buffer bind point:
   *       UINT BindPoint;
   *   - Buffer size in bytes:
   *       INT Size;
   *   - Buffer data:
   *       VOID *Bits;
   * RETURNS:
   *    twrBUFFER *Buf.
   */
  twrBUFFER * (*BufCreate)( CHAR *Name, UINT BindPoint, INT Size, VOID *Bits );
  
  /* Buffer data update function.
   * ARGUMENTS:
   *   - Buffer:
   *       twrBUFFER *Buf;
   *   - Offset:
   *       INT Offset.
   *   - Size in bytes:
   *       INT Size.
   *   - Buffer data ('Size' bytes):
   *       VOID *Bits;
   * RETURNS: None.
   */
  VOID (*BufUpdate)( twrBUFFER *Buf, INT Offset, INT Size, VOID *Bits );

  /* Buffer usage counter increase function.
   * ARGUMENTS:
   *   - buffer:
   *       twrBUFFER *Buf;
   * RETURNS: None.
   */
  VOID (*BufReferenceInc)( twrBUFFER *Buf );

  /* Buffer handle deinitialization function.
   * ARGUMENTS: twrBUFFER *Buf.
   * RETURNS: None.
   */
  VOID (*BufFree)( twrBUFFER *Buf );

  /* Buffer data resize function.
   * ARGUMENTS:
   *   - Buffer:
   *       twrBUFFER *Buf;
   *   - Size in bytes:
   *       INT Size.
   * RETURNS: TRUE if done, FALSE if failed.
   */
  BOOL (*BufResize)( twrBUFFER *Buf, INT Size );
 
  /* Buffer data read function.
   * ARGUMENTS:
   *   - Buffer:
   *       twrBUFFER *Buf;
   * RETURNS: 
   *   - (VOID *) data.
   */
  VOID * (*BufRead)( twrBUFFER *Buf );

  /* Buffer apply function.
   * ARGUMENTS:
   *   - Buffer pointer:
   *       twrBUFFER *Buf;
   * RETURNS: None.
   */
  VOID (*BufApply)( twrBUFFER *Buf );
} twrSYSTEM_BUFFERS;

/* Buffers system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemBuffersInit( VOID );

/* Buffers system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemBuffersClose( VOID );


#endif /* __twr_res_buffers_h_ */

/* END OF 'twr_res_buffers.h' FILE */
