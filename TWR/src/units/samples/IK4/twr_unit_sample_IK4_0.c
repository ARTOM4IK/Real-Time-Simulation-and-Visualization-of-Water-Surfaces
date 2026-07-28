/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_IK4_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by IK4 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#define HELIC_GRID_SIZE 30

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_IK4
{
  TWR_UNIT_BASE_FIELDS;
  twrMATERIAL_PATTERN *MyMtlPat;              /* Test material pattern */
  twrMATERIAL *MyMtl;                         /* Test material */
  VEC3 Poz[HELIC_GRID_SIZE][HELIC_GRID_SIZE]; /* Array of pozitions */
  twrPRIM *MyPr;                              /* Test primitive */
  twrMODEL *MyModel1, *MyModel2, *MyModel3;   /* Test models */
  twrMODEL *MyModel4;                         /* Test model */
  twrMODEL *Model;                            /* Water model */
  twrLIGHT *L1, *L2;                          /* Test light */
} twrUNIT_SAMPLE_IK4;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_IK4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_IK4 *Uni )
{
  INT i, j;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  
  for (j = 0; j < HELIC_GRID_SIZE; j++)
    for (i = 0; i < HELIC_GRID_SIZE; i++)
      Uni->Poz[i][j] = Vec3Set(-i * 47.0 + (rand() % 30) + HELIC_GRID_SIZE * 25.0, 0, -j * 47.0 + (rand() % 30) + HELIC_GRID_SIZE * 25.0);

  Uni->MyMtlPat = Twr->MtlPatCreate("Phong_STD4", "samples/ik4", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MyMtl = Twr->MtlCreate("MyMtl", Uni->MyMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->MyMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MyMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->MyMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));

  Uni->MyModel4 = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/boat.g3dm");
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModel1 = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/Spaceship.g3dm");
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModel2 = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/helic.g3dm");
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModel3 = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/Toyota_Land_Cruiser.g3dm");
  
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->Model = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/ocean.g3dm");
  
  Uni->L1 = Twr->LightAddDir(Vec3Set(47, 47, 0), Vec3Set(0.47, 0.47, 0.4718));
  Twr->LightEnableShadow(Uni->L1);
  Uni->L2 = Twr->LightAddPoint(Vec3Set(0, 10, 0), Vec3Set(0.47, 0.08, 0.08), Vec3Set(1, 0, 0), Vec2Set(47, 102));
  Twr->LightEnableShadow(Uni->L2);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_IK4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_IK4 *Uni )
{
  Twr->ModelFree(Uni->MyModel1);
  Twr->ModelFree(Uni->MyModel2);
  Twr->PrimFree(Uni->MyPr);
  Twr->LightDisable(Uni->L1);
  Twr->LightDisable(Uni->L2);
  Twr->MtlFree(Uni->MyMtl);
  Twr->MtlPatFree(Uni->MyMtlPat);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_IK4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_IK4 *Uni )
{ 
  INT i, j;

  for (j = 0; j < HELIC_GRID_SIZE; j++)
    for (i = 0; i < HELIC_GRID_SIZE; i++)
      Uni->Poz[i][j] = Vec3AddVec3(Uni->Poz[i][j], Vec3Set(0, j / 18.0 + 3.0 - i / 3.0 + (18.0 - j) / 10.2 * i * j / 1.8 * sin(Twr->Time * 2 * 1.8 + 14.0 - j - i / 1.8 + i), 0));

  Uni->L1->Dir = Vec3Set(4.7 + 5.0 * cos(Twr->Time / 3.0), 4.7 + 5.0 * sin(Twr->Time / 3.0), 0);
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_IK4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_IK4 *Uni )
{
  INT i, j;

  Twr->ModelDraw(Uni->MyModel4, MatrMulMatr(MatrScale(Vec3Set1(0.03 + 0.008)), MatrTranslate(Vec3Set(-18 + 8 * sin(Twr->Time), -25 + 3.0 + 8 * cos(Twr->Time) + 8 * sin(Twr->Time), 102))));
  Twr->ModelDraw(Uni->MyModel4, MatrMulMatr(MatrScale(Vec3Set1(0.03 + 0.008)), MatrTranslate(Vec3Set(102 * 1.8 + 18 * sin(Twr->Time), -40 - 3.0 + 8 * cos(Twr->Time) + 8 * sin(Twr->Time), -102))));
  
  Twr->ModelDraw(Uni->Model, MatrMulMatr3(MatrRotateZ(sin(Twr->Time) * 1.8 * 1.8 + cos(Twr->Time * 0.8) * 1.8), MatrRotateX(sin(Twr->Time * 1.8) * cos(Twr->Time) * 1.8), MatrMulMatr(MatrScale(Vec3Set1(18.0 - 4.7 - 3.0)), MatrTranslate(Vec3Set(0, 47 + 8 * cos(Twr->Time) + 8 * sin(Twr->Time), 0)))));
  
  for (j = -8; j < 8; j++)
    for (i = -8; i < 8; i++)
      Twr->ModelDraw(Uni->MyModel1, MatrMulMatr3(MatrScale(Vec3Set1(0.0003)), MatrRotateY(1.8 * 30 * Twr->Time - i * 3.0 + j * 3.0 - 30 * sin(Twr->Time / 1.8)), MatrTranslate(Vec3Set(i * 4.7 + 18 * sin(Twr->Time * 3.0), 47.0, j * 4.7 - 8.47 * cos(Twr->Time * 1.8)))));
  
  for (i = -4; i < 4; i++)
    for (j = -4; j < 4; j++)
      Twr->ModelDraw(Uni->MyModel3, MatrMulMatr3(MatrScale(Vec3Set1(3.0)), MatrRotateX(1.8 * 30 * Twr->Time - i * 3.0 * 3.0 + j * 30 * sin(Twr->Time)), MatrTranslate(Vec3Set(-8 + i * 3.0 * 4.7, i * sin(Twr->Time) * j - j * i + 18, 3 + j * 3.0 * 4.7))));
  
  for (j = 0; j < HELIC_GRID_SIZE; j++)
    for (i = 0; i < HELIC_GRID_SIZE; i++)
    {
      Twr->ModelDraw(Uni->MyModel2, MatrMulMatr3(MatrScale(Vec3Set1(1.08)), MatrMulMatr3(MatrRotateY(8 * Twr->Time * (0.30 + i - j)), MatrRotateX(8 * sin(Twr->Time * 0.30 * (14.0 - j)) / i), MatrRotateZ(8 * sin(Twr->Time * 0.30 * i))), MatrTranslate(Uni->Poz[i][j])));
      Uni->Poz[i][j] = Vec3SubVec3(Uni->Poz[i][j], Vec3Set(0, j / 18.0 + 3.0 - i / 3.0 + (18.0 - j) / 10.2 * i * j / 1.8 * sin(Twr->Time * 2 * 1.8 + 14.0 - j - i / 1.8 + i), 0));
    }

} /* End of 'TWR_UnitRender' function */

/* IK4 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleIK4( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_IK4))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleIK4' function */

/* END OF 'twr_unit_sample_IK4_0.c' FILE */
