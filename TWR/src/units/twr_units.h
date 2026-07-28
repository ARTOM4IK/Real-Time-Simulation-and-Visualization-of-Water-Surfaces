/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_units.h
 * PURPOSE     : Tough Water Rendering project.
 *               Unit create functions declaration file.
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
 * LAST UPDATE : 25.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_units_h_
#define __twr_units_h_

#include "twr_def.h"

/* Get unit number in dictionary function.
 * ARGUMENTS:
 *   - unit identification name:
 *       CHAR *Name;
 * RETURNS: 
     (INT) number of unit in units dictionary (-1 if no unit).
 */
INT TWR_GetUnitNum( CHAR *Name );

/* Create unit by its number in dictionary function.
 * ARGUMENTS:
 *   - unit number in dictionary:
 *       INT Num;
 * RETURNS: None.
 */
VOID TWR_UnitAddByNum( INT Num );

/* Create unit by dictionary name function.
 * ARGUMENTS:
 *   - unit identification name:
 *       CHAR *Name;
 * RETURNS: None.
 */
VOID TWR_UnitAddByName( CHAR *Name );

#endif /* __twr_units_h_ */

/* END OF 'twr_units.h' FILE */
