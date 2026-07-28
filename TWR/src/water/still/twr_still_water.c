/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_still_water_targets.c
 * PURPOSE     : Tough Water Rendering project.
 *               Still water system inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

#define WATER_SIZE 300

static twrMATERIAL_PATTERN *PlaneMtlPat;
static twrMATERIAL *PlaneMtl;
static twrPRIM *PrimPlane;
static twrGRID *GridPlane;
static twrTEXTURE *NMTexture;
static twrTEXTURE *DuDvTexture;
static INT W = 256, H = 256;
static CAMERA RefractionCam, ReflectionCam;

/* Refraction target test function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_StillWaterTargetTest( VOID )
{
  /* twrVERTEX_STD4 V1[] =
    {
      {{0, 5, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{0, -5, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
      {{10, 5, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{10, -5, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    };
  INT Ind[] =
    {
      0, 1, 2,
      1, 2, 3,
    }; */

  /* MtlPat = Twr->MtlPatCreate("Refraction target test material pattern", "default1", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(MtlPat);
  Mtl = Twr->MtlCreate("Refraction target test material", MtlPat, TWR_MTL_TRANSPARENT);
  Twr->MtlReferenceInc(Mtl); */

  RefractionCam = Twr->Cam;
  ReflectionCam = Twr->Cam;

  /* Set refraction camera */
  CamSet(&RefractionCam, Vec3Set(50, 100, 50), Vec3Set(10, 1, 10), Vec3Set(0, 1, 0));
  CamSetProj(&RefractionCam, RefractionCam.ProjSize, RefractionCam.ProjDist, 100);
  /* CamSetSize(&RefractionCam, 1000, 1000); */

  /* Set reflection camera */
  CamSet(&ReflectionCam, Vec3Set(50, 30 - Twr->Cam.Loc.Y, 50), Vec3Set(0, 1, 0), Vec3Set(0, 1, 0));
  CamSetProj(&ReflectionCam, ReflectionCam.ProjSize, ReflectionCam.ProjDist, 100);
  /* CamSetSize(&ReflectionCam, 1000, 1000); */

  Twr->TrgPassDraw(Twr->RefractionTarget, RefractionCam);
  Twr->TrgPassDraw(Twr->ReflectionTarget, ReflectionCam);
  /* Twr->MrkDrawSphere(RefractionCam.Loc, Vec3Set1(1), Vec4Set1(1), MatrIdentity()); */

  if (1)
  {
    INT i;
    MATR InvVP = MatrInverse(RefractionCam.MatrVP);
    INT e[] =
    {
      0, 1,
      1, 2,
      2, 3,
      3, 0,

      4, 5,
      5, 6,
      6, 7,
      7, 4,

      0, 4,
      1, 5,
      2, 6,
      3, 7,
    };
    VEC3 Corners[8], NDC_Corners[8] =
    {
      {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
      {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };

    for (i = 0; i < 8; i++)
      Corners[i] = Vec3MulMatr(NDC_Corners[i], InvVP);

    /*
    Twr->MrkDrawSphere(RefractionCam.Loc, Vec3Set1(0.30), Vec4Set(1, 0, 0, 1), MatrIdentity());
    for (i = 0; i < 8; i++)
      Twr->MrkDrawSphere(Corners[i], Vec3Set1(0.30), Vec4Set(1, 0, 0, 1), MatrIdentity());
    for (i = 0; i < 24; i += 2)
      Twr->MrkDrawCylinder(Corners[e[i]], 0.22, Corners[e[i + 1]], 0.22, Vec4Set(1, 0.50, 0.50, 1), MatrIdentity());
    */
  }

  /* Twr->MtlBindTex(PlaneMtl, 0, Twr->TexCreateFromVec4("Ka refraction target", Vec4Set(0.5, 0.30, 0.47, 0.23))); */
  Twr->MtlBindTex(PlaneMtl, 5, Twr->RefractionTarget->Attachments[0]);
  Twr->MtlBindTex(PlaneMtl, 6, Twr->ReflectionTarget->Attachments[0]);
  /* Pr = Twr->PrimCreate(PlaneMtl, TWR_RND_PRIM_TRISTRIP, V1, 4, Ind, 6); */

  Twr->PrimDraw(PrimPlane, MatrIdentity());
} /* End of 'TWR_RefractionTargetTest' function */

/* Still water system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemStillWaterInit( VOID )
{
  INT x, y;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /* Create water material and material pattern */
  PlaneMtlPat = Twr->MtlPatCreate("Plane material pattern", "samples/mn7/water", 8, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(PlaneMtlPat);
  PlaneMtl = Twr->MtlCreate("Plane material", PlaneMtlPat, TWR_MTL_TRANSPARENT);
  PlaneMtl->Trans = 0.5;
  Twr->MtlReferenceInc(PlaneMtl);

  GridPlane = Twr->GridCreate(W, H);

  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
    {
      GridPlane->V[y * W + x].P = Vec3Set(WATER_SIZE * x / (W - 1.0),
                                          30,
                                          WATER_SIZE * y / (H - 1.0));
      /* GridPlane->V[y * W + x].T.X *= WATER_SIZE;
      GridPlane->V[y * W + x].T.Y *= WATER_SIZE; */
    }

  /* Calculate normals for water and create primitive from grid */
  Twr->GridAutoNormals(GridPlane);
  PrimPlane = Twr->PrimFromGrid(PlaneMtl, GridPlane);

  /* Load normal mapping and dudv textures */
  NMTexture = Twr->TexLoad("bin/textures/water/water_NM.bmp");
  DuDvTexture = Twr->TexLoad("bin/textures/water/water_dudv.bmp");

  /* Bind textures to water material */
  Twr->MtlBindTex(PlaneMtl, 0, Twr->TexCreateFromVec4("Ka water texture", Vec4Set(0.5, 0.5, 0.5, 1)));
  Twr->MtlBindTex(PlaneMtl, 1, Twr->TexCreateFromVec4("Kd water texture", Vec4Set(0, 0, 1, 0.5)));
  Twr->MtlBindTex(PlaneMtl, 2, Twr->TexCreateFromVec4("Ks water texture", Vec4Set(1, 1, 1, 1)));
  Twr->MtlBindTex(PlaneMtl, 3, NMTexture);
  Twr->MtlBindTex(PlaneMtl, 4, DuDvTexture);

  Twr->RefractionTarget = Twr->RndTargetCreate("Refraction target", 1, TRUE, Twr->W, Twr->H);
  Twr->ReflectionTarget = Twr->RndTargetCreate("Reflection target", 1, TRUE, Twr->W, Twr->H);

  Twr->StillWaterTargetTest = TWR_StillWaterTargetTest;

  Twr->Sys("Still water system is initialized");
} /* End of 'TWR_SystemTargetsInit' function */

/* Still water system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemStillWaterClose( VOID )
{
  Twr->MtlPatFree(PlaneMtlPat);
  Twr->MtlFree(PlaneMtl);
  Twr->TexFree(NMTexture);
  Twr->TexFree(DuDvTexture);
  Twr->PrimFree(PrimPlane);
  Twr->GridFree(GridPlane);

  Twr->RndTargetClear(Twr->RefractionTarget);
  Twr->RndTargetClear(Twr->ReflectionTarget);

  Twr->Sys("Still water system is deinitialized");
} /* End of 'TWR_SystemTargetsClose' function */

/* END OF 'twr_still_water_targets.c' FILE */
