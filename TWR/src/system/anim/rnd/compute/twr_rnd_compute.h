/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_compute.h
 * PURPOSE     : Tough Water Rendering project.
 *               Compute declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_compute_h_
#define __twr_rnd_compute_h_

#include "twr_def.h"

/* Compute system representation type */
typedef struct tagtwrSYSTEM_RENDER_COMPUTE
{
  /* Buffers data */
  twrBUFFER *WorldMatrixesBuffer;    /* Buffer of matrixes for compute */
  twrBUFFER *ComputedMatrixesBuffer; /* Buffer of computed matrixes */
  UINT BuffersSize;                  /* Buffers size */

  /* Compute shader */
  twrSHADER *ComputeShd;

  /* Filling global matrixes array for buffer for compute.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*GlobalMatrixesArrayFill)( VOID );

  /* Matrixes buffers for compute shader update.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*GlobalMatrBufsUpdate)( VOID );

  /* Matrix evaluation by compute shader.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*GlobalMatrBufsCompute)( VOID );
} twrSYSTEM_RENDER_COMPUTE;

/* Compute system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemComputeInit( VOID );

/* Compute system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemComputeClose( VOID );

#endif /* __twr_rnd_compute_h_ */

/* END OF 'twr_rnd_compute.h' FILE */
