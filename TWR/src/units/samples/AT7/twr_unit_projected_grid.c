/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_projected_grid.c
 * PURPOSE     : Tough Water Rendering project.
 *               Projected grid sample implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#define TWR_GRID_SIZE (100 * 3)
#define TWR_WATER_SIZE 32

/* Sample unit description type */
typedef struct tagtwrUNIT_PROJECTED_GRID
{
  TWR_UNIT_BASE_FIELDS;
  twrMATERIAL_PATTERN *WaterMtlPat;   /* Water material pattern */
  twrMATERIAL *WaterMtl;              /* Water material */
  twrPRIM *MyWater;                   /* Water primitive */
} twrUNIT_PROJECTED_GRID;

/* Create 1D noise texture function.
 * ARGUMENTS:
 *   - width and height of texture:
 *       INT W, H;
 * RETURNS:
 *   (twrTEXTURE*) pointer to created texture.
 */
static twrTEXTURE * TWR_UnitCreateNoiseTex1D( INT W, INT H )
{
  FLT *Noise;
  INT x, y;
  twrTEXTURE *Tex;

  if ((Noise = malloc(sizeof(FLT) * W * H)) == NULL)
    return NULL;

  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
      Noise[y * W + x] = Noise2D(x * 1.847 + 1.8, y * 2.123 + 3.234);

  Tex = Twr->TexCreate("Noise texture", W, H, FALSE, GL_R32F, Noise);
  free(Noise);
  return Tex;
} /* End of 'TWR_UnitCreateNoiseTex1D' function */

/* Create 3D noise texture function.
 * ARGUMENTS:
 *   - width and height of texture:
 *       INT W, H;
 * RETURNS:
 *   (twrTEXTURE*) pointer to created texture.
 */
static twrTEXTURE * TWR_UnitCreateNoiseTex3D( INT W, INT H )
{
  VEC3 *Noise;
  INT x, y;
  twrTEXTURE *Tex;

  if ((Noise = malloc(sizeof(VEC3) * W * H)) == NULL)
    return NULL;

  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
    {
      Noise[y * W + x].X = Noise2D(x * 1.847 + 1.8, y * 2.123 + 3.234);
      Noise[y * W + x].Y = Noise2D(x * 3.436 + 8.13, y * 3.463 + 12.435);
      Noise[y * W + x].Z = Noise2D(x * 2.143 + 4.235, y * 1.83 + 30.47);
    }
  Tex = Twr->TexCreate("Noise texture", W, H, FALSE, GL_RGB32F, Noise);
  free(Noise);
  return Tex;
} /* End of 'TWR_UnitCreateNoiseTex3D' function */

/* Create water grid function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_PROJECTED_GRID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitProjectedGridCreate( twrUNIT_PROJECTED_GRID *Uni )
{
  INT i, j, k, *Ind_W;
  twrVERTEX_STD4 *V_Water;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /* Allocate memory for water data */
  if ((Ind_W = malloc(sizeof(INT) * (TWR_WATER_SIZE * 2 + 1) * (TWR_WATER_SIZE - 1) - 1)) == NULL)
    return;
  if ((V_Water = malloc(sizeof(twrVERTEX_STD4) * TWR_WATER_SIZE * TWR_WATER_SIZE)) == NULL)
    return;

  /* create indices for water grid */
  for (i = 0, k = 0; i < TWR_WATER_SIZE - 1; i++)
  {
    for (j = 0; j < TWR_WATER_SIZE; j++)
    {
      Ind_W[k++] = (i + 1) * TWR_WATER_SIZE + j;
      Ind_W[k++] = i * TWR_WATER_SIZE + j;
    }
    if (i != TWR_WATER_SIZE - 2)
      Ind_W[k++] = -1;
  }

  /* Set water params */
  for (i = 0; i < TWR_WATER_SIZE; i++)
    for (j = 0; j < TWR_WATER_SIZE; j++)
    {
      V_Water[i * TWR_WATER_SIZE + j].P = Vec3Set(TWR_GRID_SIZE * ((FLT)j / TWR_WATER_SIZE),
                                           15,
                                           TWR_GRID_SIZE * ((FLT)i / TWR_WATER_SIZE));
      V_Water[i * TWR_WATER_SIZE + j].N = Vec3Set(0, 1, 0);
      V_Water[i * TWR_WATER_SIZE + j].T.X = 47.0 * j / (TWR_WATER_SIZE - 1.0);
      V_Water[i * TWR_WATER_SIZE + j].T.Y = 47.0 * i / (TWR_WATER_SIZE - 1.0);
      V_Water[i * TWR_WATER_SIZE + j].C = Vec4Set(0.5, 0.5, 0.9, 0.9);
    }

  /* Create water material pattern, material and primitive */
  Uni->WaterMtlPat = Twr->MtlPatCreate("Water material pattern", "samples/at7/water", 7, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->WaterMtl = Twr->MtlCreate("Water material", Uni->WaterMtlPat, TWR_MTL_TRANSPARENT);
  Twr->MtlBindTex(Uni->WaterMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set(0.05, 0.05, 0.07)));
  Twr->MtlBindTex(Uni->WaterMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set(0.05, 0.15, 0.45), 0.3)));
  Twr->MtlBindTex(Uni->WaterMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set(0.3, 0.3, 0.35), 60)));
  Twr->MtlBindTex(Uni->WaterMtl, 3, TWR_UnitCreateNoiseTex1D(512, 512));
  Twr->MtlBindTex(Uni->WaterMtl, 4, TWR_UnitCreateNoiseTex3D(TWR_WATER_SIZE, TWR_WATER_SIZE));
  Twr->MtlBindTex(Uni->WaterMtl, 5, Twr->TexLoad("bin/textures/water/water_NM.bmp"));
  Twr->MtlBindTex(Uni->WaterMtl, 6, Twr->TexLoad("bin/textures/water/water_dudv.bmp"));
  Uni->MyWater = Twr->PrimCreate(Uni->WaterMtl, TWR_RND_PRIM_TRISTRIP, V_Water, TWR_WATER_SIZE * TWR_WATER_SIZE, Ind_W, (TWR_WATER_SIZE * 2 + 1) * (TWR_WATER_SIZE - 1) - 1);

  /* Clear temporary data */
  free(Ind_W);
  free(V_Water);
} /* End of 'TWR_UnitProjectedGridCreate' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_PROJECTED_GRID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_PROJECTED_GRID *Uni )
{
  TWR_UnitProjectedGridCreate(Uni);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_PROJECTED_GRID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_PROJECTED_GRID  *Uni )
{
  Twr->PrimFree(Uni->MyWater);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_PROJECTED_GRID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_PROJECTED_GRID *Uni )
{
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_PROJECTED_GRID *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_PROJECTED_GRID *Uni )
{
  Twr->PrimDraw(Uni->MyWater, MatrIdentity());
} /* End of 'TWR_UnitRender' function */

/* Projected grid water creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateProjectedGrid( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_PROJECTED_GRID))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateProjectedGrid' function */

/* END OF 'twr_unit_projected_grid.c' FILE */
