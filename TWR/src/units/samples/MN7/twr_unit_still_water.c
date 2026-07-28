/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_still_water.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit still water by MN7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#define LAND_SIZE 300

/* Sample unit description type */
typedef struct tagtwrUNIT_STILL_WATER
{
  TWR_UNIT_BASE_FIELDS;

  twrMATERIAL_PATTERN
    *PlaneMtlPat,                      /* Plane (water) material pattern */
    *SkyMtlPat,                        /* Sky material pattern (for skybox)*/
    *LandMtlPat;                       /* Land material pattern */
  twrMATERIAL
    *PlaneMtl,                         /* Plane (water) material */
    *SkyMtl,                           /* Sky material (for skybox)*/
    *LandMtl;                          /* Land material */
  twrGRID *Grid, *GridPlane;           /* Grids (for land and water) */
  twrPRIM
    *MyPr1,                            /* Test primitive */
    *PrimGrid,                         /* Grid primitive */
    *PrimPlane,                        /* Plane primitive */
    *PrimRefractionTarget;             /* Refraction texture primitive */
  twrMODEL *MyModel;                   /* Test model */
  twrTEXTURE *NMTexture, *DuDvTexture; /* Normal map and du-dv textures (used in water) */
} twrUNIT_STILL_WATER;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_STILL_WATER *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_STILL_WATER *Uni )
{
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  INT W = 1024, H = 1024, x, y, cnt = 0;
  FLT *LandBits;
  twrTEXTURE *HeightTexture;
  FLT min_h = 30;

  /* Create water material and material pattern */
  Uni->PlaneMtlPat = Twr->MtlPatCreate("Plane material pattern", "samples/mn7/water", 5, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(Uni->PlaneMtlPat);
  Uni->PlaneMtl = Twr->MtlCreate("Plane material", Uni->PlaneMtlPat, TWR_MTL_TRANSPARENT);
  Twr->MtlReferenceInc(Uni->PlaneMtl);

  /* Load normal mapping and dudv textures */
  Uni->NMTexture = Twr->TexLoad("bin/textures/WaterTextures/water_NM.bmp");
  Uni->DuDvTexture = Twr->TexLoad("bin/textures/WaterTextures/water_dudv.bmp");

  /* Bind textures to water material */
  Twr->MtlBindTex(Uni->PlaneMtl, 0, Twr->TexCreateFromVec4("Ka water texture", Vec4Set(0.5, 0.5, 0.5, 1)));
  Twr->MtlBindTex(Uni->PlaneMtl, 1, Twr->TexCreateFromVec4("Kd water texture", Vec4Set(0, 0, 1, 0.5)));
  Twr->MtlBindTex(Uni->PlaneMtl, 2, Twr->TexCreateFromVec4("Ks water texture", Vec4Set(1, 1, 1, 1)));
  Twr->MtlBindTex(Uni->PlaneMtl, 3, Uni->NMTexture);
  Twr->MtlBindTex(Uni->PlaneMtl, 4, Uni->DuDvTexture);

  /* Create land material and material pattern */
  Uni->LandMtlPat = Twr->MtlPatCreate("Land material pattern", "samples/mn7/land", 6, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(Uni->LandMtlPat);
  strncpy(Uni->LandMtlPat->Shd->Name, "samples/mn7/land", TWR_STR_MAX - 1);
  Uni->LandMtl = Twr->MtlCreate("Land material", Uni->LandMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlReferenceInc(Uni->LandMtl);

  /* Refraction target primitive */
  /* Uni->PrimRefractionTarget = Twr->PrimCreate(Uni->PlaneMtl, TWR_RND_PRIM_TRIMESH, V, 4, Ind, 6); */

  /* Create grid for land */
  Uni->Grid = Twr->GridCreate(W, H);
  Uni->GridPlane = Twr->GridCreate(W, H);

  LandBits = malloc(sizeof(FLT) * W * H);

  for (y = 0; y < H; y++)
    for (x = 0; x < W; x++)
    {
      FLT mid_dist_x = 0, mid_dist_z = 0;

      Uni->Grid->V[y * W + x].P = Vec3Set(LAND_SIZE * x / (W - 1.0),
                                          1,
                                          LAND_SIZE * y / (H - 1.0));
      Uni->GridPlane->V[y * W + x].P = Vec3Set(LAND_SIZE * x / (W - 1.0),
                                          1,
                                          LAND_SIZE * y / (H - 1.0));

      Uni->Grid->V[y * W + x].P.Y = 1.7 * NoiseTurb2D((Uni->Grid->V[y * W + x].P.X + Uni->Grid->V[y * W + x].P.Z) / 10.0,
                                                Uni->Grid->V[y * W + x].P.Z / 10.0, 8);

      mid_dist_x = x < W / 2 ? W / 2 - x : x - W / 2;
      mid_dist_z = y < H / 2 ? H / 2 - y : y - H / 2;
      Uni->Grid->V[y * W + x].P.Y += (sqrt(pow(mid_dist_x, 2) + pow(mid_dist_z, 2)) +
                                     Noise2D(mid_dist_x * 10, mid_dist_z * 10)) / 10.0;
      Uni->Grid->V[y * W + x].C = Vec4DivNum(Vec4Set(17, 217, 67, 255), 255);

      LandBits[y * W + x] = Uni->Grid->V[y * W + x].P.Y;

      Uni->GridPlane->V[y * W + x].P.Y = min_h;
      Uni->GridPlane->V[y * W + x].N = Vec3Set(0, 1, 0);
      /* Uni->GridPlane->V[y * W + x].C = Vec4DivNum(Vec4Set(65, 217, 20, 150), 255); */
    }

  /* Create height texture from grid heights */
  HeightTexture = Twr->TexCreate("MN7 height texture", W, H, FALSE, GL_R32F, LandBits);
  Twr->MtlBindTex(Uni->LandMtl, 5, HeightTexture);
  free(LandBits);

  /* Calculate normals for land and create primitive from grid */
  Twr->GridAutoNormals(Uni->Grid);
  Uni->PrimGrid = Twr->PrimFromGrid(Uni->LandMtl, Uni->Grid);

  /* Calculate normals for water and create primitive from grid */
  Twr->GridAutoNormals(Uni->GridPlane);
  Uni->PrimPlane = Twr->PrimFromGrid(Uni->PlaneMtl, Uni->GridPlane);

  Twr->RefractionTarget = Twr->RndTargetCreate("Refraction target", 1, TRUE, Twr->W, Twr->H);

  /* Fill target for refraction texture */
  /*
  glEnable(GL_CLIP_DISTANCE0);
  Twr->PostPrepassDraw(Twr->TargetsPass[TWR_MERGE_PASS], TempCam, TWR_MTL_TRANSPARENT, TRUE);
  Twr->RefractionTarget = Twr->TargetsPass[TWR_MERGE_PASS];
  glDisable(GL_CLIP_DISTANCE0);
  */

  /* Add lights */
  Twr->LightAddDir(Vec3Set(1, 1, 0), Vec3Set(0.5, 0.5, 0.5));
  /* Twr->LightEnableShadow(Twr->LightAddDir(Vec3Set(1, 1, 0), Vec3Set(0.5, 0.5, 0.5))); */
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_STILL_WATER *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_STILL_WATER *Uni )
{
  Twr->PrimFree(Uni->PrimGrid);
  Twr->GridFree(Uni->Grid);
  Twr->PrimFree(Uni->PrimPlane);
  Twr->GridFree(Uni->GridPlane);
  Twr->MtlPatFree(Uni->PlaneMtlPat);
  Twr->MtlPatFree(Uni->LandMtlPat);
  Twr->MtlFree(Uni->PlaneMtl);
  Twr->MtlFree(Uni->LandMtl);
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_STILL_WATER *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_STILL_WATER *Uni )
{
} /* End of 'TWR_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_STILL_WATER *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_STILL_WATER *Uni )
{
  MATR m;
  /* CAMERA TempCam = Twr->Cam; */

  m = MatrIdentity();
  Twr->PrimDraw(Uni->PrimGrid, m);
  Twr->PrimDraw(Uni->PrimPlane, m);
} /* End of 'TWR_UnitRender' function */

/* MN7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateStillWater( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_STILL_WATER))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateTestMN7' function */

/* END OF 'twr_unit_still_water.c' FILE */
