/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_VN4_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by VN4 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <time.h>

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_VN4
{
  TWR_UNIT_BASE_FIELDS;

  twrMATERIAL_PATTERN *MtlPatGround, *MtlPatWater, *MtlPatModel; /* Test material pattern */
  twrMATERIAL *MtlGround, *MtlWater;                             /* Test material */
  twrPRIM *PrGround, *PrWater, *PrDebug;                         /* Test primitive */
  twrMODEL *Model;                                               /* Test module */
  twrLIGHT *L[10];                                               /* Test light */
  twrTARGET * Trg[6];
  VEC3 Pos;
  FLT HeightW;
} twrUNIT_SAMPLE_VN4;

static VOID TWR_Debug( twrUNIT_SAMPLE_VN4 *Uni )
{
  twrMATERIAL *MtlD = NULL;
  twrMATERIAL_PATTERN *MtlPatD = NULL;
  twrVERTEX_STD4 V[] = 
  {
    {{-1, -1, -1}, {0, 0}, {0, 0, 1}, {0, 0, 0, 0.5}},
    {{1, -1, -1}, {0, 0}, {0, 0, 1}, {0, 0, 0, 0.5}},
    {{-1, 1, -1}, {0, 0}, {0, 0, 1}, {0, 0, 0, 0.5}},
    {{1, 1, -1}, {0, 0}, {0, 0, 1}, {0, 0, 0, 0.5}},

    {{-1, 1, -1}, {0, 0}, {0, 1, 0}, {0, 0, 0, 0.5}},
    {{1, 1, -1}, {0, 0}, {0, 1, 0}, {0, 0, 0, 0.5}},
    {{-1, 1, 1}, {0, 0}, {0, 1, 0}, {0, 0, 0, 0.5}},
    {{1, 1, 1}, {0, 0}, {0, 1, 0}, {0, 0, 0, 0.5}},

    {{-1, 1, 1}, {0, 0}, {0, 1, 1}, {0, 0, 0, 0.5}},
    {{1, 1, 1}, {0, 0}, {0, 1, 1}, {0, 0, 0, 0.5}},
    {{-1, -1, 1}, {0, 0}, {0, 1, 1}, {0, 0, 0, 0.5}},
    {{1, -1, 1}, {0, 0}, {0, 1, 1}, {0, 0, 0, 0.5}},

    {{-1, -1, -1}, {0, 0}, {1, 0, 0}, {0, 0, 0, 0.5}},
    {{-1, 1, -1}, {0, 0}, {1, 0, 0}, {0, 0, 0, 0.5}},
    {{-1, -1, 1}, {0, 0}, {1, 0, 0}, {0, 0, 0, 0.5}},
    {{-1, 1, 1}, {0, 0}, {1, 0, 0}, {0, 0, 0, 0.5}},

    {{1, -1, -1}, {0, 0}, {1, 0, 1}, {0, 0, 0, 0.5}},
    {{1, 1, -1}, {0, 0}, {1, 0, 1}, {0, 0, 0, 0.5}},
    {{1, -1, 1}, {0, 0}, {1, 0, 1}, {0, 0, 0, 0.5}},
    {{1, 1, 1}, {0, 0}, {1, 0, 1}, {0, 0, 0, 0.5}},

    {{-1, -1, -1}, {0, 0}, {1, 1, 0}, {0, 0, 0, 0.5}},
    {{1, -1, -1}, {0, 0}, {1, 1, 0}, {0, 0, 0, 0.5}},
    {{-1, -1, 1}, {0, 0}, {1, 1, 0}, {0, 0, 0, 0.5}},
    {{1, -1, 1}, {0, 0}, {1, 1, 0}, {0, 0, 0, 0.5}},
  };
  INT I[] = {0, 1, 2, 3, -1, 4, 5, 6, 7, -1, 8, 9, 10, 11, -1, 12, 13, 14, 15, -1, 16, 17, 18, 19, -1, 20, 21, 22, 23};
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  MtlPatD = Twr->MtlPatCreate("debug pattern", "samples/vn4/debugRR", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  MtlPatD->DepthFlag = TWR_DEPTH_READ_AND_WRITE;
  MtlD = Twr->MtlCreate("debug material", MtlPatD, TWR_MTL_TRANSPARENT);
  Twr->MtlBindTex(MtlD, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(MtlD, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.3), 1)));
  Twr->MtlBindTex(MtlD, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Twr->MtlBindBuf(MtlD, Twr->BufCreate("debug refraction buffer", 30, sizeof(INT) * 6, NULL));
  Uni->PrDebug = Twr->PrimCreate(MtlD, TWR_RND_PRIM_TRISTRIP, V, 24, I, sizeof(I) / sizeof(*I));
}

static VOID TWR_LightResponse( twrUNIT_SAMPLE_VN4 *Uni )
{
  if (Twr->Keys['I'] + Twr->Keys['J'] + Twr->Keys['K'] + Twr->Keys['L'] > 0)
  {
    VEC3 Dir = {0};

    Dir = Uni->L[0]->Dir;
    Dir = Vec3MulMatr(Dir, MatrRotate(30 * Twr->DeltaTime, Vec3Normalize(Vec3Set(Twr->Keys['I'] - Twr->Keys['K'], 0, Twr->Keys['J'] - Twr->Keys['L']))));
    Twr->LightSetDir(Uni->L[0], Dir);
  }
}

static VOID TWR_SetGround( twrUNIT_SAMPLE_VN4 *Uni )
{
  INT i;
  twrGRID *Ground = NULL, *Water = NULL;
  INT GrSize = 1000, RandomValue = rand() / (FLT)RAND_MAX * GrSize / 2, MinY = 10000, MaxY = -1, Box = 100, RadiusW = 250, RadiusE = 400, MinWaterY = 1000, MaxWaterY = -1;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  INT Cx, Cz;

  srand(Twr->Time);

  if (Uni->PrGround != NULL)
    Twr->PrimFree(Uni->PrGround);
  if (Uni->PrWater != NULL)
    Twr->PrimFree(Uni->PrWater);

  Uni->MtlPatGround = Twr->MtlPatCreate("ground pattern", "samples/vn4/ground", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MtlGround = Twr->MtlCreate("ground material", Uni->MtlPatGround, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(Uni->MtlGround, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MtlGround, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.3), 1)));
  Twr->MtlBindTex(Uni->MtlGround, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));

  Uni->MtlPatWater = Twr->MtlPatCreate("water pattern", "samples/vn4/water", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MtlWater = Twr->MtlCreate("water material", Uni->MtlPatWater, TWR_MTL_TRANSPARENT);
  Twr->MtlBindTex(Uni->MtlWater, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MtlWater, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.3), 0.5)));
  Twr->MtlBindTex(Uni->MtlWater, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Twr->MtlBindTex(Uni->MtlWater, 3, Twr->TexCreateCubeMap("Storforsen4"));

  Ground = Twr->GridCreate(GrSize, GrSize);
  Water = Twr->GridCreate(2 * RadiusW, 2 * RadiusW);

  for (i = 0; i < GrSize * GrSize; i++)
  {
    Ground->V[i].P.Y = RandomValue / 2.0 * NoiseTurb2D(Ground->V[i].P.X / RandomValue, Ground->V[i].P.Z / RandomValue, 6);
    
    if (Ground->V[i].P.Y < MinY)
      MinY = Ground->V[i].P.Y;
    if (Ground->V[i].P.Y > MaxY)
      MaxY = Ground->V[i].P.Y;

  }
  
  Cx = GrSize / 2 + Ground->V[0].P.X, Cz = GrSize / 2 + Ground->V[0].P.Z;

  for (i = 0; i < GrSize * GrSize; i++)
  {
    INT W = i % GrSize, H = i / GrSize;

    Ground->V[i].P.Y -= MinY;
    Ground->V[i].P.Y = Ground->V[i].P.Y * (Box / 4) / (FLT)(MaxY - MinY);
    Ground->V[i].P.X = Ground->V[i].P.X * Box / (FLT)(GrSize);
    Ground->V[i].P.Z = Ground->V[i].P.Z * Box / (FLT)(GrSize);

    if ((W - Cx) * (W - Cx) + (H - Cz) * (H - Cz) < RadiusE * RadiusE)
      Ground->V[i].P.Y *= Vec3Len(Vec3Set(W - Cx, 0, H - Cz)) / RadiusE;
    if ((W - Cx) * (W - Cx) + (H - Cz) * (H - Cz) < RadiusW * RadiusW)
    {
      FLT h = Vec3Len(Vec3Set(W - Cx, 0, H - Cz)) / RadiusW;

      Ground->V[i].P.Y *= h * h;

      if (Ground->V[i].P.Y < MinWaterY)
        MinWaterY = Ground->V[i].P.Y;
      if (Ground->V[i].P.Y > MaxWaterY)
        MaxWaterY = Ground->V[i].P.Y;
    }
  }

  Twr->GridAutoNormals(Ground);
  Uni->PrGround = Twr->PrimFromGrid(Uni->MtlGround, Ground);
  Twr->GridFree(Ground);

  for (i = 0; i < 4 * RadiusW * RadiusW; i++)
  {
    float r = NoiseTurb4D(rand() / (FLT)RAND_MAX, rand() / (FLT)RAND_MAX, rand() / (FLT)RAND_MAX, rand() / (FLT)RAND_MAX, 2);
    Uni->HeightW = (MaxWaterY - MinWaterY) * 0.3 + r / RadiusW * 20;

    Water->V[i].P = Vec3AddVec3(Water->V[i].P, Vec3Set(Cx - RadiusW, Uni->HeightW, Cz - RadiusW));

    Water->V[i].P.X = Water->V[i].P.X * Box / (FLT)(GrSize);
    Water->V[i].P.Z = Water->V[i].P.Z * Box / (FLT)(GrSize);
  }
  Twr->GridAutoNormals(Water);

  Uni->PrWater = Twr->PrimFromGrid(Uni->MtlWater, Water);
  Twr->GridFree(Water);

}

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VN4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_VN4 *Uni )
{
  INT Size = 1024, i;

  TWR_SetGround(Uni);
  TWR_Debug(Uni);
  // Twr->ModelLoadG3DMSetTransform(MatrScale(Vec3Set1(2)));
  // Uni->Model = Twr->ModelLoadG3DM(Uni->MtlPatModel, "bin/models/x6.g3dm");
  for (i = 0; i < 6; i++)
    Uni->Trg[i] = Twr->RndTargetCreate("Cubemap", 1, FALSE, Size, Size);

  Uni->L[0] = Twr->LightAddDir(Vec3Set(1, 1, 1), Vec3Set(1, 0.8, 0.5));
  Twr->LightEnableShadow(Uni->L[0]);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VN4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_VN4 *Uni )
{
  Twr->ModelFree(Uni->Model);

  Twr->PrimFree(Uni->PrGround);

  Twr->PrimFree(Uni->PrWater);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VN4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_VN4 *Uni )
{
  if (Twr->Cam.Dir.Y < 0)
    Uni->Pos = Vec3AddVec3(Twr->Cam.Loc, Vec3MulNum(Twr->Cam.Dir, (Twr->Cam.Loc.Y - Uni->HeightW) / Vec3DotVec3(Twr->Cam.Dir, Vec3Set(0, -1, 0))));

  TWR_LightResponse(Uni);

  if (Twr->KeysClick['G'])
    TWR_SetGround(Uni);
} /* End of 'TWR_UnitResponse' function */

VOID TWR_SidesCubeCheck( twrUNIT_SAMPLE_VN4 *Uni )
{
  INT i;
  FLT c = 0.707106;
  twrMATERIAL *MtlD = NULL;
  twrMATERIAL_PATTERN *MtlPatD = NULL;
  BOOL S[6] = {0, 0, 0, 0, 0, 1};
  VEC3 Dir = Twr->Cam.Dir;

  if (Dir.X < c && Dir.X > -c && Dir.Y < c && Dir.Y > -c)
    S[0] = TRUE;
  if (Dir.Y < -c)
    S[1] = TRUE;
  /*
  if (Dir.X < c && Dir.X > -c && Dir.Y < c && Dir.Y > -c)
    S[2] = TRUE;
  if ((Dir.X > c || Dir.X < -c) && Dir.Y < c && Dir.Y > -c)
    S[3] = TRUE;
  if (Dir.X < c && Dir.X > -c && Dir.Y < c && Dir.Y > -c)
    S[4] = TRUE;
  */
  Twr->BufUpdate(Uni->PrDebug->Mtl->Bufs[0], 0, 0, S);
}
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VN4 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_VN4 *Uni )
{
  CAMERA OldCam = {0};
  INT Size = 1024, farclip = 1000;
  MATR m = MatrMulMatr(MatrScale(Vec3Set1(0.1)), MatrTranslate(Vec3AddVec3(Vec3Set(Twr->Cam.Dir.X * 1, Twr->Cam.Dir.Y * 1, Twr->Cam.Dir.Z * 1), Twr->Cam.Loc)));

//   TWR_SidesCubeCheck(Uni);
//  Twr->PrimDraw(Uni->PrDebug, MatrMulMatr(MatrTranslate(Vec3AddVec3(Vec3Set(0, 0, -10), Vec3Set(0, 0, 0))), MatrInverse(Twr->Cam.MatrView)));                                  /* MatrRotate(180, Twr->Cam.Dir) */
//  Twr->PrimDraw(Uni->PrDebug, m);

  Twr->PrimDraw(Uni->PrGround, MatrIdentity());
  Twr->PrimDraw(Uni->PrWater, MatrIdentity());
  Twr->ModelDraw(Uni->Model, MatrIdentity());
//  Twr->MrkDrawBox(Vec3AddVec3(Uni->Pos, Vec3Set(1, 1, 1)), Vec3AddVec3(Uni->Pos, Vec3Set(-1, -1, -1)), Vec4Set(1, 0, 1, 1), MatrIdentity());
/*
  OldCam = Twr->Cam;

  Twr->Cam.Loc = Uni->Pos;
  Twr->Cam.MatrProj = MatrOrtho(-Size, Size, -Size, Size, -Size, farclip);
  Twr->Cam.FrameW = Twr->Cam.FrameH = 512;
  CamSetSize(&Twr->Cam, Twr->Cam.FrameW, Twr->Cam.FrameH);

  CamSet(&Twr->Cam, Uni->Pos, Vec3Set(1, 0, 0),
         Vec3Set(0, 1, 0));
  Twr->CamBufUpdate(Twr->Cam);
  Twr->GlobalMatrBufsCompute();
  Twr->TrgPassDraw(Uni->Trg[0], Twr->Cam);
  
  Twr->Cam = OldCam;
  Twr->CamBufUpdate(Twr->Cam);
  Twr->GlobalMatrBufsCompute();
*/
  /*
  CamSet(&C, Uni->Pos, Vec3Set(-1, 0, 0),
         Vec3Set(0, 1, 0));
  Twr->TrgPassDraw(Uni->Trg[1], C);

  CamSet(&C, Uni->Pos, Vec3Set(0, 0.999, 0),
         Vec3Set(0, 1, 0));
  Twr->TrgPassDraw(Uni->Trg[2], C);

  CamSet(&C, Uni->Pos, Vec3Set(0, -0.999, 0),
         Vec3Set(0, 1, 0));
  Twr->TrgPassDraw(Uni->Trg[3], C);

  CamSet(&C, Uni->Pos, Vec3Set(0, 0, 1),
         Vec3Set(0, 1, 0));
  Twr->TrgPassDraw(Uni->Trg[4], C);

  CamSet(&C, Uni->Pos, Vec3Set(0, 0, -1),
         Vec3Set(0, 1, 0));
  Twr->TrgPassDraw(Uni->Trg[5], C);
*/
} /* End of 'TWR_UnitRender' function */

/* VN4 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleVN4( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_VN4))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;
  
  return Uni;
} /* End of 'TWR_UnitCreateSampleVN4' function */

/* END OF 'twr_unit_sample_VN4_0.c' FILE */
