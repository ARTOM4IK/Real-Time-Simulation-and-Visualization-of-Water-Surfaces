/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_AT7_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by AT7 implementation module.
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
#define TWR_LAND_SIZE 1024
#define TWR_TREES_COUNT 10000

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_AT7
{
  TWR_UNIT_BASE_FIELDS;

  twrMATERIAL_PATTERN *MyMtlPat;      /* Test material pattern */
  twrMATERIAL *MyMtl;                 /* Test material */
  twrPRIM *MyPr;                      /* Test primitive */
  twrMODEL *MyModel, *MyModelB;       /* Test models */
  twrMATERIAL_PATTERN *LandMtlPat,    /* Land material pattern */
                      *ForestMtlPat;  /* Forest material pattern */
  twrMATERIAL *LandMtl,               /* Land material */
              *ForestMtl;             /* Forest material */
  twrPRIM *MyLand;                    /* Land primitive */
  twrMODEL *Forest[4];                /* Forest trees models */
  MATR TreeMatrs[TWR_TREES_COUNT];    /* Trees transform matrices */
  CHAR TreeIDs[TWR_TREES_COUNT];      /* Trees library IDs */
  twrLIGHT *Dir;                      /* Directional light */
} twrUNIT_SAMPLE_AT7;

/* Create land function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AT7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitLandCreate( twrUNIT_SAMPLE_AT7 *Uni )
{
  INT i, j, k, *Ind;
  FLT *LandMap;
  twrVERTEX_STD4 *V;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  CHAR *ForestPaths[] =
  {
    "bin/models/forest_lib/Norway_Maple.g3dm",
    "bin/models/forest_lib/Pine1.g3dm",
    "bin/models/forest_lib/Tree.g3dm",
    "bin/models/forest_lib/LodgePine.g3dm"
  };

  /* Allocate memory for land data */
  if ((Ind = malloc(sizeof(INT) * (TWR_LAND_SIZE * 2 + 1) * (TWR_LAND_SIZE - 1) - 1)) == NULL)
    return;
  if ((LandMap = malloc(sizeof(FLT) * TWR_LAND_SIZE * TWR_LAND_SIZE)) == NULL)
    return;
  if ((V = malloc(sizeof(twrVERTEX_STD4) * TWR_LAND_SIZE * TWR_LAND_SIZE)) == NULL)
    return;
  
  /* Create indices for land grid */
  for (i = 0, k = 0; i < TWR_LAND_SIZE - 1; i++)
  {
    for (j = 0; j < TWR_LAND_SIZE; j++)
    {
      Ind[k++] = (i + 1) * TWR_LAND_SIZE + j;
      Ind[k++] = i * TWR_LAND_SIZE + j;
    }
    if (i != TWR_LAND_SIZE - 2)
      Ind[k++] = -1;
  }

  /* Set land params */
  for (i = 0; i < TWR_LAND_SIZE; i++)
    for (j = 0; j < TWR_LAND_SIZE; j++)
    {
      FLT h,
          h_arr[4] = {15, 15.5, 20, 35};
      VEC3 c[4] = {{0.5, 0.5, 0.9},
                {0.5, 0.5, 0.15},
                {0.15, 0.30, 0.15},
                {0.8, 0.8, 0.82}};

      V[i * TWR_LAND_SIZE + j].P = Vec3Set(TWR_GRID_SIZE * ((FLT)j / TWR_LAND_SIZE),
                                           0,
                                           TWR_GRID_SIZE * ((FLT)i / TWR_LAND_SIZE));
      h = 10.0 * NoiseTurb2D(V[i * TWR_LAND_SIZE + j].P.X / 30.0, V[i * TWR_LAND_SIZE + j].P.Z / 31.7, 10);
      V[i * TWR_LAND_SIZE + j].P.Y = h;
      V[i * TWR_LAND_SIZE + j].N = Vec3Set(0, 1, 0);
      V[i * TWR_LAND_SIZE + j].T.X = j / (TWR_LAND_SIZE - 1.0);
      V[i * TWR_LAND_SIZE + j].T.Y = i / (TWR_LAND_SIZE - 1.0);

      /* Set color by height */
      if (h < h_arr[0])
          V[i * TWR_LAND_SIZE + j].C = Vec4SetVec3(Vec3MulNum(c[1], 0.7), 1);
      else if (h < h_arr[1])
        V[i * TWR_LAND_SIZE + j].C = Vec4SetVec3(Vec3AddVec3(Vec3MulNum(Vec3MulNum(c[1], 0.7), (h_arr[1] - h) / (h_arr[1] - h_arr[0])), 
                                                             Vec3MulNum(c[1], 1 - (h_arr[1] - h) / (h_arr[1] - h_arr[0]))), 1);
      else if (h < h_arr[2])
        V[i * TWR_LAND_SIZE + j].C = Vec4SetVec3(Vec3AddVec3(Vec3MulNum(c[1], (h_arr[2] - h) / (h_arr[2] - h_arr[1])),
                                                             Vec3MulNum(c[2], 1 - (h_arr[2] - h) / (h_arr[2] - h_arr[1]))), 1);
      else if (h < h_arr[3])
        V[i * TWR_LAND_SIZE + j].C = Vec4SetVec3(Vec3AddVec3(Vec3MulNum(c[2], (h_arr[3] - h) / (h_arr[3] - h_arr[2])),
                                                             Vec3MulNum(c[3], 1 - (h_arr[3] - h) / (h_arr[3] - h_arr[2]))), 1);
      else
        V[i * TWR_LAND_SIZE + j].C = Vec4SetVec3(c[3], 1);

      LandMap[(i * TWR_LAND_SIZE + j)] = h;
    }

  /* Set normals */
  for (i = 0; i < TWR_LAND_SIZE - 1; i++)
    for (j = 0; j < TWR_LAND_SIZE; j++)
    {
      twrVERTEX_STD4
        *P00 = V + i * TWR_LAND_SIZE + j,
        *P01 = V + i * TWR_LAND_SIZE + j + 1,
        *P10 = V + (i + 1) * TWR_LAND_SIZE + j,
        *P11 = V + (i + 1) * TWR_LAND_SIZE + j + 1;
      VEC3 N;

      N = Vec3Normalize(Vec3CrossVec3(Vec3SubVec3(P00->P, P10->P),
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
  for (i = 0; i < TWR_LAND_SIZE * TWR_LAND_SIZE; i++)
    V[i].N = Vec3Neg(Vec3Normalize(V[i].N));

  /* Create land material pattern, material and primitive */
  Uni->LandMtlPat = Twr->MtlPatCreate("Land material pattern", "samples/at7/land", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->LandMtl = Twr->MtlCreate("Land material", Uni->LandMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->LandMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->LandMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.5), 1)));
  Twr->MtlBindTex(Uni->LandMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.4), 230)));
  Twr->MtlBindTex(Uni->LandMtl, 3, Twr->TexCreate("Land height texture", TWR_LAND_SIZE, TWR_LAND_SIZE, FALSE, GL_R32F, LandMap));
  Uni->MyLand =  Twr->PrimCreate(Uni->LandMtl, TWR_RND_PRIM_TRISTRIP, V, TWR_LAND_SIZE * TWR_LAND_SIZE, Ind, (TWR_LAND_SIZE * 2 + 1) * (TWR_LAND_SIZE - 1) - 1);

  /* Create forest material pattern */
  Uni->ForestMtlPat = Twr->MtlPatCreate("Forest material pattern", "samples/at7/forest", 6, 0, sizeof(V[0]), attrs, 4);
  Uni->ForestMtl = Twr->MtlCreate("Forest material", Uni->ForestMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->ForestMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->ForestMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->ForestMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 100)));
  Twr->MtlBindTex(Uni->ForestMtl, 4, Twr->TextureFind("Land height texture"));

  for (i = 0; i < sizeof(ForestPaths) / sizeof(ForestPaths[0]); i++)
  {
    Twr->ModelLoadG3DMSetTransform(MatrScale(Vec3Set1(0.1)));
    Uni->Forest[i] = Twr->ModelLoadG3DM(Uni->ForestMtlPat, ForestPaths[i]);
  }

  for (i = 0; i < TWR_TREES_COUNT; i++)
    Uni->TreeIDs[i] = rand() % (sizeof(ForestPaths) / sizeof(ForestPaths[0]));

  for (i = 0; i < TWR_TREES_COUNT; i++)
  {
    INT Pnts[4];
    FLT dx, dz;
    VEC3 Pos = Vec3Set((FLT)TWR_GRID_SIZE * rand() / RAND_MAX, 0, (FLT)TWR_GRID_SIZE * rand() / RAND_MAX);

    Pnts[0] = (INT)(Pos.X / TWR_GRID_SIZE * TWR_LAND_SIZE) + (INT)(Pos.Z / TWR_GRID_SIZE * TWR_LAND_SIZE) * TWR_LAND_SIZE;
    Pnts[1] = (INT)(Pos.X / TWR_GRID_SIZE * TWR_LAND_SIZE) + (INT)(Pos.Z / TWR_GRID_SIZE * TWR_LAND_SIZE + 1) * TWR_LAND_SIZE;
    Pnts[2] = (INT)(Pos.X / TWR_GRID_SIZE * TWR_LAND_SIZE + 1) + (INT)(Pos.Z / TWR_GRID_SIZE * TWR_LAND_SIZE + 1) * TWR_LAND_SIZE;
    Pnts[3] = (INT)(Pos.X / TWR_GRID_SIZE * TWR_LAND_SIZE + 1) + (INT)(Pos.Z / TWR_GRID_SIZE * TWR_LAND_SIZE) * TWR_LAND_SIZE;

    dx = Pos.X - (INT)(Pos.X / TWR_GRID_SIZE * TWR_LAND_SIZE) * (FLT)TWR_GRID_SIZE / TWR_LAND_SIZE;
    dz = Pos.Z - (INT)(Pos.Z / TWR_GRID_SIZE * TWR_LAND_SIZE) * (FLT)TWR_GRID_SIZE / TWR_LAND_SIZE;

    Pos.Y = (LandMap[Pnts[0]] * (1 - dx) + LandMap[Pnts[3]] * dx) * (1 - dz) + 
            (LandMap[Pnts[1]] * (1 - dx) + LandMap[Pnts[2]] * dx) * dz;
    if (Pos.Y < 16)
      Pos = Vec3Set1(0);
    Uni->TreeMatrs[i] = MatrTranslate(Pos);
  }
  free(LandMap);
  free(Ind);
  free(V);
} /* End of 'TWR_UnitLandCreate' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AT7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_AT7 *Uni )
{
  twrVERTEX_STD4 V[] = 
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 0}, {0, 0}, {0, 0, 1}, {0, 0, 1, 1}},
    {{1, 0, 0}, {0, 0}, {0, 0, 1}, {0, 1, 0, 1}},
    {{1, 1, 0}, {0, 0}, {0, 0, 1}, {1, 0, 0, 1}},
  },
  LandV[] = 
  {
    {{-100, -1, -100}, {0, 0}, {0, 1, 0}, {0.3, 0.8, 0.47, 1}},
    {{-100, -1, 100}, {0, 0}, {0, 1, 0}, {0.47, 0.8, 0.3, 1}},
    {{100, -1, -100}, {0, 0}, {0, 1, 0}, {0.28, 0.8, 0.42, 1}},
    {{100, -1, 100}, {0, 0}, {0, 1, 0}, {0.12, 0.8, 0.5, 1}},
  };
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  Uni->MyMtlPat = Twr->MtlPatCreate("Phong_STD4", "default", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MyMtl = Twr->MtlCreate("MyMtl", Uni->MyMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->MyMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MyMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->MyMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Uni->MyPr = Twr->PrimCreate(Uni->MyMtl, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
  
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModel = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/GAZ66.g3dm");
  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModelB = Twr->ModelLoadG3DM(Uni->MyMtlPat, "bin/models/x6.g3dm");

  TWR_UnitLandCreate(Uni);
  Twr->LightEnableShadow(Uni->Dir = Twr->LightAddDir(Vec3Set(1, 1, 0), Vec3Set1(0.8)));
  Twr->LightEnableShadow(Twr->LightAddSpot(Vec3Set(150, 60, 150), Vec3Set1(1), Vec3Set1(1), Vec3Set(1, 0.000, 0), Vec2Set(300, 500), Vec2Set(30, 47)));
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AT7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_AT7 *Uni )
{
  INT i;
  Twr->PrimFree(Uni->MyPr);
  Twr->ModelFree(Uni->MyModel);
  Twr->ModelFree(Uni->MyModelB);
  Twr->PrimFree(Uni->MyLand);

  for (i = 0; i < 4; i++)
    Twr->ModelFree(Uni->Forest[i]);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AT7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_AT7 *Uni )
{
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_AT7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_AT7 *Uni )
{
  INT i;
  Twr->LightSetDir(Uni->Dir, Vec3Set(sin(Twr->Time), 1, cos(Twr->Time)));
  /*
  twrMATERIAL_PATTERN *MyMtlPat;
  twrMATERIAL *MyMtl;
  twrMODEL *Mdl;
  twrPRIM *Pr;
  twrTEXTURE *Tex;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  twrVERTEX_STD4 V[] = 
  {
    {{0, 0, 8}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    {{0, 1, 8}, {0, 0}, {0, 0, 1}, {0, 0, 1, 1}},
    {{1, 0, 8}, {0, 0}, {0, 0, 1}, {0, 1, 0, 1}},
    {{1, 1, 8}, {0, 0}, {0, 0, 1}, {1, 0, 0, 1}},
  };

  for (i = 0; i < 5; i++)
  {
    MyMtlPat = Twr->MtlPatCreate("Phong_STD4", "default", 4, sizeof(twrVERTEX_STD4), attrs, 4);

    MyMtl = Twr->MtlCreate("MyMtl", MyMtlPat, TWR_MTL_OPAQUE);
    Twr->MtlBindTex(MyMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
    Twr->MtlBindTex(MyMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
    Twr->MtlBindTex(MyMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));

    Pr = Twr->PrimCreate(MyMtl, TWR_RND_PRIM_TRISTRIP, V, 4, NULL, 0);
    Mdl = Twr->ModelLoadG3DM(MyMtlPat, "bin/models/Spaceship.g3dm");
    Twr->PrimDraw(Pr, MatrIdentity());
    Twr->PrimFree(Pr);

    Twr->ModelDraw(Mdl, MatrMulMatr(MatrScale(Vec3Set1(0.001)), MatrTranslate(Vec3Set(i * 5, i * 5, i * 5))));
    Twr->ModelFree(Mdl);

    Tex = Twr->TexCreateFromVec3("Test", Vec3Set(0, 1, 2));
    Twr->TexFree(Tex);
  }
  */
  Twr->PrimDraw(Uni->MyLand, MatrIdentity());
  for (i = 0; i < TWR_TREES_COUNT; i++)
    Twr->ModelDraw(Uni->Forest[Uni->TreeIDs[i]], Uni->TreeMatrs[i]);
} /* End of 'TWR_UnitRender' function */

/* AT7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleAT7( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_AT7))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleAT7' function */

/* END OF 'twr_unit_sample_AT7_0.c' FILE */
