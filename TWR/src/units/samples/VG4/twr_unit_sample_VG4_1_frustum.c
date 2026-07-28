/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_VG4_1_frustum.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by VG4 (frustum test) implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_VG4_FRUSTUM
{
  TWR_UNIT_BASE_FIELDS;
  twrPRIM *Floor;      /* Floor primitive */
  twrFONT *Fnt;        /* Test fonts */
  twrLIGHT *Sun;       /* Sun light source */
  twrPRIM *LightDebug; /* Light primitive */
  CAMERA Cam;          /* Test camera */
} twrUNIT_SAMPLE_VG4_FRUSTUM;

/* Draw AABB function.
 * ARGUMENTS:
 *   - corner points:
 *       VEC3 MinBB, MaxBB;
 *   - cylinders radius:
 *       FLT R;
 *   - color:
 *       VEC4 Color;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS: None.
 */
static VOID TWR_DrawBB( VEC3 MinBB, VEC3 MaxBB, FLT R, VEC4 Color, MATR M )
{
  VEC3 ps[] =
  {
    {MinBB.X, MinBB.Y, MinBB.Z},
    {MaxBB.X, MinBB.Y, MinBB.Z},
    {MaxBB.X, MaxBB.Y, MinBB.Z},
    {MinBB.X, MaxBB.Y, MinBB.Z},
    {MinBB.X, MinBB.Y, MaxBB.Z},
    {MaxBB.X, MinBB.Y, MaxBB.Z},
    {MaxBB.X, MaxBB.Y, MaxBB.Z},
    {MinBB.X, MaxBB.Y, MaxBB.Z},
  };
  INT i, e[] =
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

  for (i = 0; i < 8; i++)
    Twr->MrkDrawSphere(ps[i], Vec3Set1(R * 1.2), Color, M);
  for (i = 0; i < 24; i += 2)
    Twr->MrkDrawCylinder(ps[e[i]], R, ps[e[i + 1]], R, Color, M);
} /* End of 'TWR_DrawBB' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4_FRUSTUM *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_VG4_FRUSTUM *Uni )
{
  twrMATERIAL_PATTERN *MtlPatLightDebug = Twr->MtlPatCreate("LightDebug", "lights/debug", 0, 1, 0, NULL, 0);
  twrMATERIAL *MtlLightDebug = Twr->MtlCreate("LightDebug", MtlPatLightDebug, TWR_MTL_OPAQUE);
  twrVERTEX_STD6 V[] = 
  {
    {{-300, 0, -300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{-300, 0, 300}, {0, 50}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{300, 0, -300}, {50, 0}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
    {{300, 0, 300}, {50, 50}, {0, 1, 0}, {1, 1, 1, 1}, {1, 0, 0}, {0, 0, -1}},
  };
  twrVERTEX_ATTRS attrs_plane[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
    {"InTangent",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Tng)},
    {"InBitangent", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Btn)},
  };
  twrMATERIAL_PATTERN *MtlPatPlane;
  twrMATERIAL *MtlPlane;

  /* Plane */
  MtlPatPlane = Twr->MtlPatCreate("plain", "normal_mapping", 4, 0, sizeof(twrVERTEX_STD6), attrs_plane, 6);
  MtlPlane = Twr->MtlCreate("MyMtl", MtlPatPlane, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(MtlPlane, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  /* Twr->MtlBindTex(MtlPlane, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1))); */
  Twr->MtlBindTex(MtlPlane, 1, Twr->TexLoad("bin/textures/NormalMapsTextures/156/156.jpg"));
  Twr->MtlBindTex(MtlPlane, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(1), 102)));
  /* Twr->MtlBindTex(MtlPlane, 3, Twr->TexCreateFromVec4("NMap", Vec4Set(0.5, 0.5, 1, 0))); */
  Twr->MtlBindTex(MtlPlane, 3, Twr->TexLoad("bin/textures/NormalMapsTextures/156/156_norm.jpg"));
  Uni->Floor = Twr->PrimCreate(MtlPlane, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);

  /* Sun */
  Uni->LightDebug = Twr->PrimCreate(MtlLightDebug, TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  Uni->Sun = Twr->LightAddDir(Vec3Normalize(Vec3Set(1, 3, 1)), Vec3Set(1, 1, 1));
  Twr->MtlBindBuf(MtlLightDebug, Uni->Sun->Buffer);
  Twr->LightEnableShadow(Uni->Sun);

  Uni->Fnt = Twr->FntCreate("ConsolasBold", Vec4Set(1, 1, 1, 1));

  CamSet(&Uni->Cam, Vec3Set(-18, 18, 18), Vec3Set(0, 0, 0), Vec3Set(0, 1, 0));
  CamSetProj(&Uni->Cam, 1, 2, 100);
  CamSetSize(&Uni->Cam, 1920, 1080);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4_FRUSTUM *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_VG4_FRUSTUM *Uni )
{
  Twr->FntFree(Uni->Fnt);
  Twr->PrimFree(Uni->Floor);
  Twr->PrimFree(Uni->LightDebug);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4_FRUSTUM *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_VG4_FRUSTUM *Uni )
{
  if (Uni->Sun != NULL)
  {
    if (Twr->KeysClick['H'])
      Uni->Sun->Dir = Vec3Normalize(Vec3Set(1, 1, 1));
    Uni->Sun->Dir = VectorTransform(Uni->Sun->Dir, MatrRotateX(30 * Twr->GlobalDeltaTime * (Twr->Keys['K'] - Twr->Keys['J'])));
    Uni->Sun->Dir = VectorTransform(Uni->Sun->Dir, MatrRotateY(30 * Twr->GlobalDeltaTime * (Twr->Keys['I'] - Twr->Keys['U'])));
    Uni->Sun->Dir = VectorTransform(Uni->Sun->Dir, MatrRotateZ(30 * Twr->GlobalDeltaTime * (Twr->Keys['M'] - Twr->Keys['N'])));
    Twr->LightUpdate(Uni->Sun);
  }
  Uni->Cam.Loc = Vec3Set(Uni->Cam.Loc.X + 30 * Twr->GlobalDeltaTime * (Twr->Keys['R'] - Twr->Keys['T']),
                         Uni->Cam.Loc.Y + 30 * Twr->GlobalDeltaTime * (Twr->Keys['F'] - Twr->Keys['G']),
                         Uni->Cam.Loc.Z + 30 * Twr->GlobalDeltaTime * (Twr->Keys['V'] - Twr->Keys['B']));
  CamSet(&Uni->Cam, Uni->Cam.Loc, Vec3Set(0, 0, 0), Vec3Set(0, 1, 0));

} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VG4_FRUSTUM *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_VG4_FRUSTUM *Uni )
{
  CHAR Buf[102];

  sprintf(Buf, "FPS: %.3f\n<%d,%d>", Twr->FPS, Twr->Mx, Twr->My);
  Twr->FntDraw(Uni->Fnt, Buf, Vec3Set(300, 0, 0), 0);
  Twr->PrimDraw(Uni->Floor, MatrIdentity());
  Twr->PrimDraw(Uni->LightDebug, MatrIdentity());

  if (1)
  {
    INT i;
    MATR InvVP = MatrInverse(Uni->Cam.MatrVP);
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
    VEC3 Corners[8], Corners1[8], NDC_Corners[8] =
    {
      /* {-1, -1, Uni->Cam.ProjDist}, {1, -1, Uni->Cam.ProjDist}, {1, 1, Uni->Cam.ProjDist}, {-1, 1, Uni->Cam.ProjDist}, */
      /* {-1, -1, Uni->Cam.ProjFarClip}, {1, -1, Uni->Cam.ProjFarClip}, {1, 1, Uni->Cam.ProjFarClip}, {-1, 1, Uni->Cam.ProjFarClip} */
      {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
      {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}
    };
    FLT ShadowDistance = 0, fr_left, fr_right, fr_bottom, fr_top, fr_near, fr_far;
    MATR LightView = MatrView(Vec3AddVec3(Uni->Cam.At, Uni->Sun->Dir), Uni->Cam.At, Vec3Set(0, 1, 0));
    VEC3 MinBB = {0}, MaxBB = {0}, MinBB1 = {0}, MaxBB1 = {0}, V;
    MATR ShadowMatr;
    CAMERA Cam;

    for (i = 0; i < 8; i++)
      Corners[i] = Vec3MulMatr(NDC_Corners[i], InvVP);

    for (i = 0; i < 8; i++)
    {
      VEC3 p = PointTransform(Corners[i], LightView); //Corners[i];//

      Corners1[i] = p;
      if (i == 0)
        MinBB = MaxBB = p;
      else
      {
        MinBB = Vec3MinVec3(MinBB, p);
        MaxBB = Vec3MaxVec3(MaxBB, p);
      }
      if (i == 0)
        MinBB1 = MaxBB1 = Corners[i];
      else
      {
        MinBB1 = Vec3MinVec3(MinBB1, Corners[i]);
        MaxBB1 = Vec3MaxVec3(MaxBB1, Corners[i]);
      }
    }
    fr_left = MinBB.X;
    fr_right = MaxBB.X;
    fr_bottom = MinBB.Y;
    fr_top = MaxBB.Y;
    fr_near = MinBB.Z - ShadowDistance;
    fr_far = MaxBB.Z + ShadowDistance;

    TWR_DrawBB(MinBB, MaxBB, 0.50, Vec4Set(0.8, 0.8, 1, 1), MatrInverse(LightView));
    TWR_DrawBB(MinBB1, MaxBB1, 0.18, Vec4Set(0.18, 0.18, 1, 1), MatrIdentity());
    V = PointTransform(Vec3DivNum(Vec3AddVec3(MinBB, MaxBB), 2), MatrInverse(LightView));
    Twr->MrkDrawSphere(V, Vec3Set1(2), Vec4Set(0.47, 1, 1, 1), MatrIdentity());
    Twr->MrkDrawCylinder(V, 0.8, Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 30)), 0.8, Vec4Set(0.47, 1, 1, 1), MatrIdentity());
    Twr->MrkDrawCylinder(Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 30)), 2, Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 36)), 0, Vec4Set(0.47, 1, 1, 1), MatrIdentity());


    Twr->MrkDrawSphere(Uni->Cam.Loc, Vec3Set1(0.30), Vec4Set(1, 0, 0, 1), MatrIdentity());
    for (i = 0; i < 8; i++)
      Twr->MrkDrawSphere(Corners[i], Vec3Set1(0.30), Vec4Set(1, 0, 0, 1), MatrIdentity());
    for (i = 0; i < 24; i += 2)
      Twr->MrkDrawCylinder(Corners[e[i]], 0.22, Corners[e[i + 1]], 0.22, Vec4Set(1, 0.50, 0.50, 1), MatrIdentity());
    Twr->MrkDrawSphere(MinBB, Vec3Set1(0.8), Vec4Set(1, 1, 0, 1), MatrIdentity());
    Twr->MrkDrawSphere(MaxBB, Vec3Set1(0.8), Vec4Set(0, 1, 1, 1), MatrIdentity());

    Cam.MatrProj = MatrOrtho(fr_left - 2, fr_right + 2, fr_bottom - 2, fr_top + 2, -fr_far - 2, -fr_near + 2);
    CamSet(&Cam, Vec3AddVec3(Uni->Cam.At, Uni->Sun->Dir), Uni->Cam.At, Vec3Set(0, 1, 0));
    ShadowMatr = Cam.MatrVP;
    InvVP = MatrInverse(Cam.MatrVP);

    V = Vec3Set1(0);
    for (i = 0; i < 8; i++)
    {
      Corners[i] = Vec3MulMatr(NDC_Corners[i], InvVP);
      V = Vec3AddVec3(V, Corners[i]);
    }
    V = Vec3DivNum(V, 8);
    Twr->MrkDrawSphere(V, Vec3Set1(2), Vec4Set(1, 1, 1, 1), MatrIdentity());
    Twr->MrkDrawCylinder(V, 0.8, Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 30)), 0.8, Vec4Set(1, 1, 1, 1), MatrIdentity());
    Twr->MrkDrawCylinder(Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 30)), 2, Vec3AddVec3(V, Vec3MulNum(Uni->Sun->Dir, 36)), 0, Vec4Set(1, 1, 1, 1), MatrIdentity());
    for (i = 0; i < 8; i++)
      Twr->MrkDrawSphere(Corners[i], Vec3Set1(0.30), Vec4Set(0, 1, 0, 1), MatrIdentity());
    for (i = 0; i < 24; i += 2)
      Twr->MrkDrawCylinder(Corners[e[i]], 0.22, Corners[e[i + 1]], 0.22, Vec4Set(0.50, 1, 0.50, 1), MatrIdentity());

    /*
    for (i = 0; i < 8; i++)
      Corners1[i] = PointTransform(Corners[i], Cam.MatrView);
    for (i = 0; i < 8; i++)
      Twr->MrkDrawSphere(Corners1[i], Vec3Set1(0.30), Vec4Set(0, 0, 1, 1), MatrIdentity());
    for (i = 0; i < 24; i += 2)
      Twr->MrkDrawCylinder(Corners1[e[i]], 0.22, Corners1[e[i + 1]], 0.22, Vec4Set(0.50, 0.50, 1, 1), MatrIdentity());
    */
  }
} /* End of 'TWR_UnitRender' function */

/* VG4 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleVG4Frustum( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_VG4_FRUSTUM))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleVG4' function */

/* END OF 'twr_unit_sample_VG4_1_frustum.c' FILE */
