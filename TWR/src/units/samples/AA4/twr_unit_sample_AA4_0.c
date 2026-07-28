/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_AA4_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by AA4 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 28.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_AA4
{
  twrMATERIAL_PATTERN *MyMtlPat[1000]; /* Test material pattern */
  twrMATERIAL *MyMtl[1000];            /* Test material */
  twrPRIM *MyPr[1000];                 /* Test primitive */
  twrMODEL *MyModel[1000];             /* Test module */
  twrPRIM *Floor;                      /* Test Floor */
  twrLIGHT *L[10];                         /* Test light */
} twrUNIT_SAMPLE_AA4;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AA4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_AA4 *Uni )
{
  twrVERTEX_STD4 V[] = 
  {
    {{-300, 0, -300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}},
    {{-300, 0, 300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}},
    {{300, 0, -300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}},
    {{300, 0, 300}, {0, 0}, {0, 1, 0}, {1, 1, 1, 1}},
  };
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  twrMATERIAL_PATTERN *MtlPat = Twr->MtlPatCreate("Phong_STD6", "samples/mn7/normal_mapping", 4, 0, sizeof(twrVERTEX_STD6), attrs, 6);

  Uni->MyMtlPat[999] = Twr->MtlPatCreate("plain", "samples/vn4/plain", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MyMtl[999] = Twr->MtlCreate("MyMtl", Uni->MyMtlPat[999], TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->MyMtl[999], 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MyMtl[999], 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->MyMtl[999], 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(1), 30)));
  Uni->Floor = Twr->PrimCreate(Uni->MyMtl[999], TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);

  Twr->ModelLoadG3DMSetTransform(MatrScale(Vec3Set1(0.01)));
  Uni->MyModel[998] = Twr->ModelLoadG3DM(Uni->MyMtlPat[998], "bin/models/sponza1.g3dm");
  /*
  for (i = 0; i < 10; i++)
    Uni->L[i] = Twr->LightAddPoint(Vec3Set(sin(i) * 14, 35, cos(i) * 14), Vec3Set(1.0 / (i + 0.4), i / 50, 1 - 1.0 / (i + 1)), Vec3Set(0, 1, 0), Vec2Set(10, 30));
  */
  /*Twr->MrkDrawTorus(Vec3Set(0, 10, 0), Vec2Set(10, 1), Vec4Set(1, 0, 1, 1), MatrIdentity());*/
  //Uni->L[0] = Twr->LightAddDir(Vec3Set(1, 1, 1), Vec3Set(1, 1, 0.8));
  Uni->L[0] = Twr->LightAddSpot(Vec3Set(70, 30, 0), Vec3Set(1, 1, 1), Vec3Set(1, 1, 0.8), Vec3Set(1, 0, 0), Vec2Set(28, 38), Vec2Set(50, 57));
  Twr->LightEnableShadow(Uni->L[0]);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AA4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_AA4 *Uni )
{
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AA4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_AA4 *Uni )
{
  if (Twr->Keys['W'] + Twr->Keys['A'] + Twr->Keys['S'] + Twr->Keys['D'] > 0)
  {
    VEC3 Dir = {0};

    Dir = Uni->L[0]->Dir;
    Dir = Vec3MulMatr(Dir, MatrRotate(30 * Twr->DeltaTime, Vec3Normalize(Vec3Set(Twr->Keys['W'] - Twr->Keys['S'], 0, Twr->Keys['A'] - Twr->Keys['D']))));
    Twr->LightSetDir(Uni->L[0], Dir);
  }
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AA4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_AA4 *Uni )
{
  twrTEXTURE *BufT[1000] = {0};
  twrBUFFER *BufB[1000] = {0};
  INT t = 10, b = 100 + 1;
  twrVERTEX_STD4 V[] = 
  {
    {{0, 0, 0}, {0, 0}, {0, 1, 0}, {0, 1, 0, 1}},
    {{1, 0, 0}, {0, 1}, {0, 1, 0}, {0, 0, 1, 1}},
    {{0, 0, 1}, {1, 0}, {0, 1, 0}, {1, 0, 0, 1}},
    {{1, 0, 1}, {1, 1}, {0, 1, 0}, {0, 0, 0, 1}},
  };
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  CHAR *SkyNames[] =
  {
    {"SkyOcean"},
    {"SkyNebula"},
    {"SkyFuture"},
    {"SkyLand"},
  };
  FLT q = Twr->Time;
  /*  
  if (Uni->L[0] != NULL)
    Twr->LightSetDir(Uni->L[0], Vec3Set(1, sin(Twr->Time), 0));
  */
  Twr->PrimDraw(Uni->Floor, MatrIdentity());
  Twr->ModelDraw(Uni->MyModel[998], MatrIdentity());
} /* End of 'TWR_UnitRender' function */

/* AA4 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleAA4( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_AA4))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleAA4' function */

/* END OF 'twr_unit_sample_AA4_0.c' FILE */
