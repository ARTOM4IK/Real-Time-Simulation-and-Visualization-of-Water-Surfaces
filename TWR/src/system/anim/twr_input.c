/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_input.c
 * PURPOSE     : Tough Water Rendering project.
 *               Work with input system. 
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 02.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_input.h"
#include "system/twr_system.h"
#include <string.h>
/* The input keyboard system initialisation program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimKeyboardInit( VOID )
{
  INT i;

  for (i = 0; i < 256; i++)
  {
    Twr->Keys[i] = 0;
    Twr->KeysClick[i] = 0;
  }
  memcpy(Twr->KeysOld, Twr->Keys, 256);
} /* End of 'TWR_AnimKeyboardInit' function */

/* The input keyboard system response program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimKeyboardResponse( VOID )
{
  INT i;

  GetKeyboardState(Twr->Keys);
  for (i = 0; i < 256; i++)
  {
    Twr->Keys[i] >>= 7;
    Twr->KeysClick[i] = Twr->Keys[i] && !Twr->KeysOld[i];
  }

  if (!Twr->IsActive)
    memset(Twr->Keys, 0, sizeof(Twr->Keys));

  memcpy(Twr->KeysOld, Twr->Keys, 256);
} /* End of 'TWR_AnimKeyboardResponse' function */

/* The input mouse system initialization program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimMouseInit( VOID )
{
  POINT pt;

  GetCursorPos(&pt);
  ScreenToClient(Twr->hWnd, &pt);

  /* Absolute mouse coordinates */
  Twr->Mx = pt.x;
  Twr->My = pt.y;

  /* Screen mouse coordinates */
  Twr->Mdx = 0;
  Twr->Mdy = 0;

  Twr->Mdz = Twr->MouseWheel;
  Twr->MouseWheel = 0;
} /* End of 'TWR_AnimMouseInit' function */

/* The input mouse system response program function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_AnimMouseResponse( VOID )
{
  POINT pt;

  if (Twr->PrevInputIsActive != Twr->IsActive)
    TWR_AnimMouseInit();
  Twr->PrevInputIsActive = Twr->IsActive;
  
  GetCursorPos(&pt);
  ScreenToClient(Twr->hWnd, &pt);

  Twr->Mdx = pt.x - Twr->Mx;
  Twr->Mdy = pt.y - Twr->My;
  Twr->Mx = pt.x;
  Twr->My = pt.y;

  /* Mouse wheel */
  Twr->Mdz = Twr->MouseWheel;
  Twr->Mz += Twr->MouseWheel;

  Twr->MouseWheel = 0;

  if (!Twr->IsActive)
  {
    TWR_AnimMouseInit();
    Twr->Mx = 0;
    Twr->My = 0;
  }
} /* End of 'TWR_AnimMouseResponse' function */

/* Input initialization function.
 * ARGUMENTS: None
 *  RETURNS: None.
 */
VOID TWR_AnimInputInit( VOID )
{
  TWR_AnimKeyboardInit();
  TWR_AnimMouseInit();
} /* End of 'TWR_AnimInputInit' function */

/* Input response function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_AnimInputResponse( VOID )
{
  TWR_AnimKeyboardResponse();
  TWR_AnimMouseResponse();
} /* End of 'TWR_AnimInputResponse' function */

/* Input system initialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemInputInit( VOID )
{
  Twr->InputInit = (VOID *)TWR_AnimInputInit;
  TWR_AnimInputInit();
  Twr->InputResponse = (VOID *)TWR_AnimInputResponse;
  Twr->Info("Input system is initialized");
} /* End of 'TWR_SystemInputInit' function */

/* Input system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemInputClose( VOID )
{
} /* End of 'TWR_SystemInputClose' function */

/* END OF 'twr_input.c' FILE */
