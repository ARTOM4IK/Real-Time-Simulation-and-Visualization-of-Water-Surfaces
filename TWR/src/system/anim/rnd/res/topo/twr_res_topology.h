/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_topology.h
 * PURPOSE     : Tough Water Rendering project.
 *               Topology declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_topology_h_
#define __twr_res_topology_h_

#include "twr_def.h"

/* Topology structure */
typedef struct tagtwrSYSTEM_TOPOLOGY
{
  /* Triangle's tangent space evaluation function.
   * ARGUMENTS:
   *   - pointer to triangle's vertices:
   *       twrVERTEX_STD6 *V0, *V1, *V2;
   * RETURNS: None.
   */
  VOID (*EvalTriangleTS)( twrVERTEX_STD6 *V0, twrVERTEX_STD6 *V1, twrVERTEX_STD6 *V2 );
} twrSYSTEM_TOPOLOGY;

/* Initialize topology system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyInit( VOID );

/* Delete topology system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyClose( VOID );

#endif /* __twr_res_topology_h_ */

/* END OF 'twr_res_topology.h' FILE */
