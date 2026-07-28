/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_test_AB7_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit test by AB7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 07.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Test unit description type */
typedef struct tagtwrUNIT_TEST_AB7
{
  TWR_UNIT_BASE_FIELDS;
  STOCK StkTest;                 /* Test stock */
  twrMATERIAL_PATTERN *MyMtlPat; /* Test material pattern */
  twrMATERIAL *MyMtl;            /* Test material */
  twrPRIM *MyPr;                 /* Test primitive */
} twrUNIT_TEST_AB7;

/* Test structure */
typedef struct tagtwrTEST
{
  INT No;     /* Test number */
  DWORD *Arr; /* Test malloc */
} twrTEST;

/* Test delete strategy function.
 * ARGUMENTS:
 *   - test object pointer:
 *       twrTEST *T;
 * RETURNS: None.
 */
static VOID TWR_DeleteStrategyTest( twrTEST *T )
{
  if (T == NULL)
    return;
  free(T->Arr);
  memset(T, 0, sizeof(twrTEST));
} /* End of 'TWR_DeleteStrategyTest' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_TEST_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_TEST_AB7 *Uni )
{
  twrVERTEX_STD4 V[] = 
  {
    {{0, 0, 8}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 8}, {0, 0}, {0, 0, 1}, {0, 0, 1, 1}},
    {{1, 0, 8}, {0, 0}, {0, 0, 1}, {0, 1, 0, 1}},
    {{1, 1, 8}, {0, 0}, {0, 0, 1}, {1, 0, 0, 1}},
  };
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  StockCreate(&Uni->StkTest, "Test AB7 stock", sizeof(twrTEST), 5, TWR_DeleteStrategyTest);
  Uni->MyMtlPat = Twr->MtlPatCreate("Phong_STD4", "default", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MyMtl = Twr->MtlCreate("MyMtl", Uni->MyMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->MyMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MyMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->MyMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Uni->MyPr = Twr->PrimCreate(Uni->MyMtl, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);

} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_TEST_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_TEST_AB7 *Uni )
{
  StockFree(&Uni->StkTest);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_TEST_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_TEST_AB7 *Uni )
{
  INT i;

  for (i = 0; i < 100; i++)
  {
    twrTEST *T = StockAlloc(&Uni->StkTest);

    T->No = i;
    T->Arr = malloc(sizeof(DWORD) * i);
  }
  StockDeleteAll(&Uni->StkTest);
  StockStrip(&Uni->StkTest);
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_TEST_AB7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_TEST_AB7 *Uni )
{
  twrVERTEX_STD4 V[] = 
  {
    {{0, 0, 8}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 8}, {0, 0}, {0, 0, 1}, {0, 0, 1, 1}},
    {{1, 0, 8}, {0, 0}, {0, 0, 1}, {0, 1, 0, 1}},
    {{1, 1, 8}, {0, 0}, {0, 0, 1}, {1, 0, 0, 1}},
  };

  twrPRIM *Pr = Twr->PrimCreate(Uni->MyMtl, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
  twrTEXTURE *Tex = Twr->TexCreateFromVec3("Test", Vec3Set(0, 1, 2));
  twrMODEL *Mdl = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/Spaceship.g3dm");

  Twr->PrimDraw(Pr, MatrIdentity());
  Twr->PrimFree(Pr);
  Twr->TexFree(Tex);
  Twr->ModelDraw(Mdl, MatrScale(Vec3Set1(0.001)));
  Twr->ModelFree(Mdl);
} /* End of 'TWR_UnitRender' function */

/* AB7 unit test creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateTestAB7( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_TEST_AB7))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateTestAB7' function */

/* END OF 'twr_unit_test_AB7_0.c' FILE */
