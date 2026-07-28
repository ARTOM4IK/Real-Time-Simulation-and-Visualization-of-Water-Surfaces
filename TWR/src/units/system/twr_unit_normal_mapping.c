/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_normal_mapping.c.c
 * PURPOSE     : Tough Water Rendering project.
 *               Normal mapping test unit implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Normal mapping unit description type */
typedef struct tagtwrUNIT_NORMAL_MAPPING
{
  TWR_UNIT_BASE_FIELDS;

  twrMATERIAL_PATTERN *MtlPat;       /* Test material patternû */
  twrMATERIAL *Mtl1;                 /* Test material */
  twrPRIM *Pr;                       /* Test primitive */
  twrMODEL *Model;                   /* Test model */
} twrUNIT_NORMAL_MAPPING;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_NORMAL_MAPPING *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_NORMAL_MAPPING *Uni )
{
  /**/
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord",   2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",     3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",      4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  twrVERTEX_STD4 V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 10, 0}, {0, 5}, {0, 0, 1}, {1, 1, 1, 1}},
    {{10, 0, 0}, {5, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{10, 10, 0}, {5, 5}, {0, 0, 1}, {1, 1, 1, 1}}
  };

  twrVERTEX_ATTRS attrs1[] =
  {
    {"InPosition",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, P)},
    {"InTexCoord",   2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, T)},
    {"InNormal",     3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, N)},
    {"InColor",      4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, C)},
    {"InTangent",    3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Tng)},
    {"InBitangent",  3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD6, Btn)},
  };
  /*twrVERTEX6 V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 10, 0}, {0, 1}, {0, 0, 1}, {1, 1, 1, 1}},
    {{10, 0, 0}, {1, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{10, 10, 0}, {1, 1}, {0, 0, 1}, {1, 1, 1, 1}}
  };*/
  INT Ind[] =
  {
    0, 1, 2,
    0, 2, 3
  };

  /*
  Uni->MtlPat = Twr->MtlPatCreate("Phong_STD6", "normal_mapping", 4, sizeof(twrVERTEX_STD6), attrs1, 6);
  Uni->Mtl1 = Twr->MtlCreate("Mtl1", Uni->MtlPat, TWR_MTL_OPAQUE);
  */

  /*Uni->MtlPat = Twr->MtlPatCreate("Phong_STD6", "normal_mapping", 4, sizeof(twrVERTEX_STD6), attrs1, 6);*/

  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  /* Twr->ModelLoadG3DMSetTransform(MatrScale(Vec3Set(0.1, 0.1, 0.1))); */
  Uni->Model = Twr->ModelLoadG3DM(NULL, "bin/models/x6.g3dm");

  Twr->LightEnableShadow(Twr->LightAddDir(Vec3Set(1, 1, 0), Vec3Set(1, 1, 1)));
  /* Uni->Pr = Twr->PrimCreate(Uni->Mtl1, TWR_RND_PRIM_TRIMESH, V, 4, NULL, 0); */
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_NORMAL_MAPPING *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_NORMAL_MAPPING *Uni )
{
  Twr->PrimFree(Uni->Pr);
  Twr->MtlFree(Uni->Mtl1);
  Twr->MtlPatFree(Uni->MtlPat);
  Twr->ModelFree(Uni->Model);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_NORMAL_MAPPING *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_NORMAL_MAPPING *Uni )
{
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_NORMAL_MAPPING *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_NORMAL_MAPPING *Uni )
{
  /*Twr->PrimDraw(Uni->Pr, MatrTranslate(Vec3Set(0, 0, 0)));*/
  Twr->ModelDraw(Uni->Model, MatrIdentity());
} /* End of 'TWR_UnitRender' function */

/* Normal mapping unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateNormalMapping( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_NORMAL_MAPPING))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleNormal mapping' function */

/* END OF 'twr_unit_normal_mapping.c.c' FILE */
