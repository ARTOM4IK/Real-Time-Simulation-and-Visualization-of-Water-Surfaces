/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_topology_grid.c
 * PURPOSE     : Tough Water Rendering project.
 *               Topology (grid) implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (TWR).
 * LAST UPDATE : 21.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_res_topology_grid.h"

/* Create grid data function.
 * ARGUMENTS:
 *   - grid:
 *       twrGRID *Gr;
 *   - amount of points in width and height:
 *       INT W, INT H;
 * RETURNS:
 *   (twrGRID *) pointer to created grid.
 */
twrGRID * TWR_GridCreate( INT W, INT H )
{
  INT i, j;
  twrGRID *Gr;

  if ((Gr = malloc(sizeof(twrGRID))) == NULL)
    return NULL;
  if ((Gr->V = malloc(sizeof(twrVERTEX_STD4) * W * H)) == NULL)
    return FALSE;
  Gr->W = W;
  Gr->H = H;

  for (i = 0; i < H; i++)
    for (j = 0; j < W; j++)
    {
      Gr->V[i * W + j].P = Vec3Set(j, 0, i);
      Gr->V[i * W + j].N = Vec3Set(0, 1, 0);
      Gr->V[i * W + j].T.X = (j / (W - 1.0));
      Gr->V[i * W + j].T.Y = (i / (H - 1.0));
      Gr->V[i * W + j].C = Vec4Set1(1);
    }

  return Gr;
} /* End of 'TWR_GridCreate' function */

/* Free grid data function.
 * ARGUMENTS:
 *   - grid to be deleted:
 *       twrGRID *Gr;
 * RETURNS: None.
 */
VOID TWR_GridFree( twrGRID *Gr )
{
  if (Gr->V != NULL)
    free(Gr->V);
  if (Gr != NULL)
    free(Gr);
} /* End of 'TWR_GridFree' function */

/* Create primitive from grid function.
 * ARGUMENTS:
 *   - pointer to material to create primitive from:
 *       twrMATERIAL *Mtl;
 *   - grid to be turned into primitive:
 *       twrGRID *Gr;
 * RETURNS:
 *   (twrPRIM *) pointer to created primitive.
 */
twrPRIM * TWR_PrimFromGrid( twrMATERIAL *Mtl, twrGRID *Gr )
{
  INT *Ind;
  INT i, j, k;
  twrPRIM *Pr;

  /* if ((Pr = malloc(sizeof(twrPRIM))) == NULL)
    return NULL; */

  /* memset(Pr, 0, sizeof(twrPRIM)); */

  if ((Ind = malloc(sizeof(INT) * ((Gr->H - 1) * (Gr->W * 2 + 1) - 1))) == NULL)
    return NULL;
  for (k = 0, i = 0; i < Gr->H - 1; i++)
  {
    for (j = 0; j < Gr->W; j++)
    {
      Ind[k++] = (i + 1) * Gr->W + j;
      Ind[k++] = i * Gr->W + j;
    }
    if (i != Gr->H - 2)
      Ind[k++] = -1;
  }
  Pr = Twr->PrimCreate(Mtl, TWR_RND_PRIM_TRISTRIP, Gr->V, Gr->W * Gr->H, Ind, (Gr->H - 1) * (Gr->W * 2 + 1) - 1);
  free(Ind);

  return Pr;
} /* End of 'TWR_PrimFromGrid' function */

/* Evaluate grid normals function.
 * ARGUMENTS:
 *   - grid to evaluate normals of:
 *       twrGRID *Gr;
 * RETURNS: None.
 */
VOID TWR_GridAutoNormals( twrGRID *Gr )
{
  INT i, j;

  for (i = 0; i < Gr->W * Gr->H; i++)
    Gr->V[i].N = Vec3Set1(0);

  for (i = 0; i < Gr->H - 1; i++)
    for (j = 0; j < Gr->W - 1; j++)
    {
      twrVERTEX_STD4
        *P00 = Gr->V + i * Gr->W + j,
        *P01 = Gr->V + i * Gr->W + j + 1,
        *P10 = Gr->V + (i + 1) * Gr->W + j,
        *P11 = Gr->V + (i + 1) * Gr->W + j + 1;

      VEC3 N = Vec3Normalize(Vec3CrossVec3(Vec3SubVec3(P00->P, P10->P),
                                           Vec3SubVec3(P11->P, P10->P)));
      P00->N = Vec3AddVec3(P00->N, N);
      P10->N = Vec3AddVec3(P10->N, N);
      P11->N = Vec3AddVec3(P11->N, N);

      N = Vec3Normalize(Vec3CrossVec3(Vec3SubVec3(P11->P, P01->P),
                                      Vec3SubVec3(P00->P, P01->P)));
      P00->N = Vec3AddVec3(P00->N, N);
      P01->N = Vec3AddVec3(P01->N, N);
      P11->N = Vec3AddVec3(P11->N, N);
    }
  for (i = 0; i < Gr->W * Gr->H; i++)
    Gr->V[i].N = Vec3Normalize(Gr->V[i].N);
} /* End of 'TWR_GridAutoNormals' function */

/* Initialize topology grid system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyGridInit( VOID )
{
  Twr->GridCreate = TWR_GridCreate;
  Twr->GridFree = TWR_GridFree;
  Twr->PrimFromGrid = TWR_PrimFromGrid;
  Twr->GridAutoNormals = TWR_GridAutoNormals;
  Twr->Sys("Topology grid system is initialized");
} /* End of 'TWR_SystemTopologyGridInit' function */

/* Delete topology grid system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyGridClose( VOID )
{
  Twr->Sys("Topology grid system is deinitialized");
} /* End of 'TWR_SystemTopologyGridClose' function */

/* END OF 'twr_res_topology_grid.c' FILE */
