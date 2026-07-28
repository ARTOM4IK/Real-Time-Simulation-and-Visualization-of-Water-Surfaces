/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_system.c
 * PURPOSE     : Tough Water Rendering project.
 *               Main system inplementation module.
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
 * LAST UPDATE : 04.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <string.h>

/* Global system context */
static twrSYSTEM TWR_System;
twrSYSTEM *Twr = &TWR_System;

/* Main system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_Init( VOID )
{
  memset(Twr, 0, sizeof(twrSYSTEM));

  TWR_SystemLoggerInit();
  StocksInit();
  NoiseInit();
  TWR_SystemAnimationInit();
  TWR_ProfilerInit();
  Twr->Sys("TWR system is initialized");

  Twr->IsInit = TRUE;
} /* End of 'TWR_Init' function */

/* Register stock to frame-end strip function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID TWR_StockRegisterToStrip( STOCK *Stk )
{
  if (Stk == NULL)
    return;
  if (Stk != &StockStocks && Stk != &StockToStrip)
  {
    STOCK **s = StockAlloc(&StockToStrip);

    *s = Stk;
  }
  Twr->Info("Stock registered to strip: %s", Stk->Name);
} /* End of 'TWR_StockRegisterToStrip' function */

/* Main system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_Close( VOID )
{
  Twr->IsInit = FALSE;

  TWR_SystemAnimationClose();
  StocksClose();
  TWR_SystemLoggerClose();
  memset(Twr, 0, sizeof(twrSYSTEM));
} /* End of 'TWR_Close' function */

/* END OF 'twr_system.c' FILE */
