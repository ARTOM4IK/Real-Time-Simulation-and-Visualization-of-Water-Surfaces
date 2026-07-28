/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_buffers.c
 * PURPOSE     : Tough Water Rendering project.
 *               Buffers handle module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_res_buffers.h"

/* Stock for buffers storage */
static STOCK TWR_StkBuffers;

/* Buffer handle delete function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_BufStrategyDelete( twrBUFFER *Buf )
{
  Twr->Log("\nDelete buffer:");
  Twr->SetColor(0xFFFF00, 0);
  Twr->Log(Buf->Name);
  Twr->SetColor(0xFFFFFF, 0);
  Twr->Log(" [%p Id: %d Ref: %d]\n", Buf, Buf->BufId, Buf->RefCounter);
  glDeleteBuffers(1, &Buf->BufId);
  Buf->BufId = -102;
} /* End of 'TWR_BufStrategyDelete' function */

/* Buffer usage counter increase function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_BufReferenceInc( twrBUFFER *Buf )
{
  if (Buf == NULL)
    return;
  Buf->RefCounter++;
} /* End of 'TWR_BufReferenceInc' function */

/* Add Buffer to stock function.
 * ARGUMENTS:
 *   - buffer name:
 *       CHAR *Name;
 *   - buffer OpenGL bind point:
 *       UINT BindPoint;
 *   - buffer size in bytes:
 *       INT Size;
 *   - buffer data:
 *       VOID *Bits;
 * RETURNS:
 *   (twrBUFFER *) Buf.
 */
static twrBUFFER * TWR_BufCreate( CHAR *Name, UINT BindPoint, INT Size, VOID *Bits )
{
  twrBUFFER *Buf = NULL;

  if ((Buf = StockAlloc(&TWR_StkBuffers)) == NULL)
    return NULL;
  Buf->Size = Size;
  Buf->BindPoint = BindPoint;
  strncpy(Buf->Name, Name, TWR_STR_MAX - 1);
  if (Size == 0)
    Size = 1;
  glGenBuffers(1, &(Buf->BufId));
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buf->BufId);
  glBufferData(GL_SHADER_STORAGE_BUFFER, Size, NULL, GL_STATIC_COPY);
  if (Bits != NULL)
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Size, Bits);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  Buf->RefCounter = 0;
  return Buf;
} /* End of 'TWR_BufCreate' function */

/* Buffer handle deinitialization function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_BufFree( twrBUFFER *Buf )
{
  if (Buf == NULL)
    return;
  Buf->RefCounter--;
  if (Buf->RefCounter <= 0)
    StockDelete(Buf);
} /* End of 'TWR_BufFree' function */

/* Buffer data update function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 *   - offset:
 *       INT Offset;
 *   - size in bytes:
 *       INT Size;
 *   - buffer data ('Size' bytes):
 *       VOID *Bits;
 * RETURNS: None.
 */
static VOID TWR_BufUpdate( twrBUFFER *Buf, INT Offset, INT Size, VOID *Bits )
{
  if (Buf == NULL)
    return;
  if (Offset >= Buf->Size)
    return;
  if (Offset < 0)
    Offset = 0;
  if (Size == 0)
    Size = Buf->Size;
  if (Offset + Size > Buf->Size)
    Size = Buf->Size - Offset;
  /* Update buffer data */
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buf->BufId);
  glBufferSubData(GL_SHADER_STORAGE_BUFFER, Offset, Size, Bits);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
} /* End of 'TWR_BufUpdate' function */

/* Buffer data resize function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 *   - size in bytes:
 *       INT Size;
 * RETURNS: 
 *   (BOOL) TRUE if done, FALSE if failed.
 */
static BOOL TWR_BufResize( twrBUFFER *Buf, INT Size )
{
  if (Buf == NULL)
    return FALSE;
  Buf->Size = Size;
  if (Size == 0)
    Size = 1;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buf->BufId);
  glBufferData(GL_SHADER_STORAGE_BUFFER, Size, NULL, GL_STATIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  return TRUE;
} /* End of 'TWR_BufResize' function */

/* Buffer data read function.
 * ARGUMENTS:
 *   - buffer:
 *       twrBUFFER *Buf;
 * RETURNS: 
 *   (VOID *) data.
 */
static VOID * TWR_BufRead( twrBUFFER *Buf )
{
  VOID *data = NULL;

  if (Buf == NULL)
    return NULL;
  if (Buf->Size == 0)
    return NULL;
  if ((data = malloc(Buf->Size)) == NULL)
    return NULL;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, Buf->BufId);
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, Buf->Size, data);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  return data;
} /* End of 'TWR_BufRead' function */

/* Buffer apply function.
 * ARGUMENTS:
 *   - buffer pointer:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_BufApply( twrBUFFER *Buf )
{
  if (Buf == NULL)
    return;
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, Buf->BindPoint, Buf->BufId);
} /* End of 'TWR_BufApply' function */

/* Buffers system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemBuffersInit( VOID )
{
  Twr->BufCreate = TWR_BufCreate;
  Twr->BufFree = TWR_BufFree;
  Twr->BufApply = TWR_BufApply;
  Twr->BufResize = TWR_BufResize;
  Twr->BufUpdate = TWR_BufUpdate;
  Twr->BufRead = TWR_BufRead;
  Twr->BufReferenceInc = TWR_BufReferenceInc;
  StockCreate(&TWR_StkBuffers, "Buffers", sizeof(twrBUFFER), 30, TWR_BufStrategyDelete);
  TWR_StockRegisterToStrip(&TWR_StkBuffers);
  Twr->Sys("Buffers system is initialized");
} /* End of 'TWR_SystemBuffersInit' function */

/* Buffers system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemBuffersClose( VOID )
{
  StockFree(&TWR_StkBuffers);
  Twr->Sys("Buffers system is deinitialized");
} /* End of 'TWR_SystemInputClose' function */

/* END OF 'twr_res_buffers.c' FILE */
