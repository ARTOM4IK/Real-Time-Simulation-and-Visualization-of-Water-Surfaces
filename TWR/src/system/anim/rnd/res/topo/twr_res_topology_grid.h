/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_topology_grid.h
 * PURPOSE     : Tough Water Rendering project.
 *               Topology declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 21.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_topology_grid_h_
#define __twr_res_topology_grid_h_

#include "twr_def.h"

/* Grid representation type */
typedef struct tagtwrGRID
{
  INT W, H;            /* grid size (in vertexes) */
  twrVERTEX_STD4 *V;   /* array of vertexes */
} twrGRID;

/* Topology grid structure */
typedef struct tagtwrSYSTEM_TOPOLOGY_GRID
{
  /* Create grid data function.
   * ARGUMENTS:
   *   - grid:
   *       twrGRID *G;
   *   - amount of points in width and height:
   *       INT W, INT H;
   * RETURNS:
   *   (twrGRID *) pointer to created grid.
   */
  twrGRID * (*GridCreate)( INT W, INT H );

  /* Free grid data function.
   * ARGUMENTS:
   *   - grid to be deleted:
   *       twrGRID *Gr;
   * RETURNS: None.
   */
  VOID (*GridFree)( twrGRID *Gr );

  /* Create primitive from grid function.
   * ARGUMENTS:
   *   - pointer to material to create primitive from:
   *       twrMATERIAL *Mtl;
   *   - grid to be turned into primitive:
   *       twrGRID *Gr;
   * RETURNS:
   *   (twrPRIM *) pointer to created primitive.
   */
  twrPRIM * (*PrimFromGrid)( twrMATERIAL *Mtl, twrGRID *Gr );

  /* Evaluate grid normals function.
   * ARGUMENTS:
   *   - grid to evaluate normals of:
   *       twrGRID *Gr;
   * RETURNS: None.
   */
  VOID (*GridAutoNormals)( twrGRID *Gr );
} twrSYSTEM_TOPOLOGY_GRID;

/* Initialize topology grid system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyGridInit( VOID );

/* Delete topology grid system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyGridClose( VOID );

#endif /* __twr_res_topology_grid_h_ */

/* END OF 'twr_res_topology_grid.h' FILE */
