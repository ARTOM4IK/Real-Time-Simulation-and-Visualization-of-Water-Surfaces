/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_system.h
 * PURPOSE     : Tough Water Rendering project.
 *               Main system declaration file.
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
 * LAST UPDATE : 28.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_system_h_
#define __twr_system_h_

#include "anim/twr_anim.h"
#include "utils/logger/twr_logger.h"

/* System context representation type */
typedef struct tagtwrSYSTEM
{
  /* Initialization flag */
  BOOL IsInit;

  /* Logger system */
  union
  {
    twrSYSTEM_LOGGER;
    twrSYSTEM_LOGGER Logger;
  };

  /* Animation system */
  union
  {
    twrSYSTEM_ANIMATION;
    twrSYSTEM_ANIMATION Animation;
  };
} twrSYSTEM;

/* Global system context */
extern twrSYSTEM *Twr;

/* Main system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_Init( VOID );

/* Register stock to frame-end strip function.
 * ARGUMENTS:
 *   - stock pointer:
 *       STOCK *Stk;
 * RETURNS: None.
 */
VOID TWR_StockRegisterToStrip( STOCK *Stk );

/* Main system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_Close( VOID );

#endif /* __twr_system_h_ */

/* END OF 'twr_system.h' FILE */
