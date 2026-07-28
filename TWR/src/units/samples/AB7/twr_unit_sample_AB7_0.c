/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_AB7_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by AB7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 07.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_AB7
{
  TWR_UNIT_BASE_FIELDS;
  twrPRIM *Land;         /* Land primitive */
  INT GridW, GridH;      /* Grid size parameters */
  INT LandW, LandH;      /* Real land size parameters */
  twrVERTEX_STD4 *LandV; /* Vertex array */
} twrUNIT_SAMPLE_AB7;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_AB7 *Uni )
{
  INT *Ind, i, k, j;
  FLT min_h = 0, h, max_h = 0;
  twrMATERIAL_PATTERN *mtl_pat;
  twrMATERIAL *mtl;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /* Grid tesselation & land real size */
  Uni->GridW = Uni->GridH = 1024;
  Uni->LandW = Uni->LandH = 47;
  /* Vertex array memory allocation */
  if ((Uni->LandV = malloc(sizeof(twrVERTEX_STD4) * Uni->GridW * Uni->GridH)) == NULL)
    return;
  /* Add vertices parameters */
  for (i = 0; i < Uni->GridH; i++)
    for (j = 0; j < Uni->GridW; j++)
    {
      /* Current vertex */
      twrVERTEX_STD4 *v = &Uni->LandV[i * Uni->GridW + j];

      v->P = Vec3Set(Uni->LandH * (2 * i / (FLT)Uni->GridH - 1), 0, Uni->LandW * (2 * j / (FLT)Uni->GridW - 1));
      h = v->P.Y = 8 * NoiseTurb2D(v->P.X / 30.0, v->P.Z / 30.0, 7);
      if (min_h == 0)
        min_h = h;
      if (max_h == 0)
        max_h = h;
      if (h > max_h)
        max_h = h;
      if (h < min_h)
        min_h = h;
      v->T = Vec2Set(i / (FLT)Uni->GridH, j / (FLT)Uni->GridW);
    }
  /* Calculate color */
  for (i = 0; i < Uni->GridH; i++)
    for (j = 0; j < Uni->GridW; j++)
    {
      /* Current vertex */
      FLT t, n;
      twrVERTEX_STD4 *v = &Uni->LandV[i * Uni->GridW + j];

      h = v->P.Y;
      t = (max_h - h) / (max_h - min_h);
      n = 2 * Noise2D(v->P.X, v->P.Z) - 1;
      if (t <= 0.3 + n * 0.1)
        v->C = Vec4DivNum(Vec4Set(200, 209, 218, 255), 255); /* White */
      else if (t <= 0.4 + n * 0.1)
        v->C = Vec4DivNum(Vec4Set(139, 155, 116, 255), 255); /* Light green */
      else if (t <= 0.5 + n * 0.1)
        v->C = Vec4DivNum(Vec4Set(63, 108, 41, 255), 255);   /* Green */
      else if (t <= 0.8 + n * 0.1)
        v->C = Vec4DivNum(Vec4Set(53, 73, 82, 255), 255);    /* Light grey */
      else if (t > 0.8 + n * 0.1)
        v->C = Vec4DivNum(Vec4Set(24, 50, 39, 255), 255);    /* Grey */
    }
  /* Set base normals */
  for (i = 0; i < Uni->GridW * Uni->GridH; i++)
    Uni->LandV[i].N = Vec3Set(0, 1, 0);
  /* Correct normals */
  for (i = 0; i < Uni->GridH - 1; i++)
    for (j = 0; j < Uni->GridW - 1; j++)
    {
      twrVERTEX_STD4
        *P00 = Uni->LandV + i * Uni->GridW + j,
        *P01 = Uni->LandV + i * Uni->GridW + j + 1,
        *P10 = Uni->LandV + (i + 1) * Uni->GridW + j,
        *P11 = Uni->LandV + (i + 1) * Uni->GridW + j + 1;
      VEC3 N;

      N = Vec3Normalize(Vec3CrossVec3(Vec3SubVec3(P00->P, P10->P), Vec3SubVec3(P11->P, P10->P)));
      P00->N = Vec3AddVec3(P00->N, N);
      P10->N = Vec3AddVec3(P10->N, N);
      P11->N = Vec3AddVec3(P11->N, N);
      N = Vec3Normalize(Vec3CrossVec3(Vec3SubVec3(P11->P, P01->P), Vec3SubVec3(P00->P, P01->P)));
      P00->N = Vec3AddVec3(P00->N, N);
      P01->N = Vec3AddVec3(P01->N, N);
      P11->N = Vec3AddVec3(P11->N, N);
    }
  /* Normalize every normal */
  for (i = 0; i < Uni->GridH * Uni->GridW; i++)
    Uni->LandV[i].N = Vec3Normalize(Uni->LandV[i].N);
  /* Add indexing */
  if ((Ind = malloc(sizeof(INT) * ((Uni->GridH - 1) * (Uni->GridW * 2 + 1) - 1))) == NULL)
    return;
  for (i = 0, k = 0; i < Uni->GridH - 1; i++)
  {
    for (j = 0; j < Uni->GridW; j++)
    {
      Ind[k++] = (i + 1) * Uni->GridW + j;
      Ind[k++] = i * Uni->GridW + j;
    }
    if (i != Uni->GridH - 2)
      Ind[k++] = -1;
  }
  /* Create primitive with default material */
  mtl_pat = Twr->MtlPatCreate("Land material pattern", "default", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  mtl = Twr->MtlCreate("Land material", mtl_pat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(mtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(mtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.5), 1)));
  Twr->MtlBindTex(mtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Uni->Land = Twr->PrimCreate(mtl, TWR_RND_PRIM_TRISTRIP, Uni->LandV, Uni->GridH * Uni->GridW, Ind, (Uni->GridH - 1) * (Uni->GridW * 2 + 1) - 1);

  Twr->LightEnableShadow(Twr->LightAddDir(Vec3Set(0, 1, 0), Vec3Set(1, 1, 1)));

  free(Ind);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_AB7 *Uni )
{
  /* Free vertex array */
  Twr->PrimFree(Uni->Land);
  free(Uni->LandV);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_AB7 *Uni )
{
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_AB7 *Uni )
{
  /* Render  land */
  Twr->PrimDraw(Uni->Land, MatrTranslate(Vec3Set(0, -18, 0)));
} /* End of 'TWR_UnitRender' function */

/* AB7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleAB7( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_AB7))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleAB7' function */

/* END OF 'twr_unit_sample_AB7_0.c' FILE */
