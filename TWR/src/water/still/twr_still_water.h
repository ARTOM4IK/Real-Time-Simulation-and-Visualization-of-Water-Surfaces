/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_still_water_targets.h
 * PURPOSE     : Tough Water Rendering project.
 *               Still water system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 14.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_still_water_targets_h_
#define __twr_still_water_targets_h_

/* Still water system structure */
typedef struct tagtwrSYSTEM_STILL_WATER
{
  /* Refraction and reflection targets */
  twrTARGET *RefractionTarget, *ReflectionTarget;

  /* Refraction target test function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*StillWaterTargetTest)( VOID );
} twrSYSTEM_STILL_WATER;

/* Still water system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemStillWaterInit( VOID );

/* Still water system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemStillWaterClose( VOID );

#endif /* __twr_still_water_targets_h_ */

/* END OF 'twr_still_water_targets.h' FILE */
