/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_input.h
 * PURPOSE     : Tough Water Rendering project.
 *               Input system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_input_h_
#define __twr_input_h_

#include "twr_def.h"

/* Struct of input system (mouse and keyboard) */
typedef struct tagtwrSYSTEM_INPUT
{
  VOID (*InputInit)( VOID );
  VOID (*InputResponse)( VOID );

  /* Input data */
  BYTE Keys[256];      /* Current keyboard statement buffer */
  BYTE KeysOld[256];   /* Previous frame keyboard statement buffer */
  BYTE KeysClick[256]; /* Click array */

  INT Mx, My, Mz,      /* Mouse absolute values */
    Mdx, Mdy, Mdz;     /* Mouse delta values */
} twrSYSTEM_INPUT;

/* Input initialization function.
 * ARGUMENTS: None
* RETURNS: None.
 */
VOID TWR_AnimInputInit( VOID );

/* Input response function.
 * ARGUMENTS: None
* RETURNS: None.
 */
VOID TWR_AnimInputResponse( VOID );

/* Input system initialization function.
 * ARGUMENTS: None
 *  RETURNS: None.
 */
VOID TWR_SystemInputInit( VOID );

/* Input system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemInputClose( VOID );

#endif /* __twr_input_h_ */

/* END OF 'twr_input.h' FILE */
