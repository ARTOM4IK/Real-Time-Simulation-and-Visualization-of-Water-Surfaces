/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_MN7_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by MN7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_MN7
{
  TWR_UNIT_BASE_FIELDS;

  twrMATERIAL_PATTERN *MyMtlPat, *SkyMtlPat, *LandMtlPat;    /* Test material patterns */
  twrMATERIAL *MyMtl, *SkyMtl, *LandMtl;                     /* Test materials */
  twrGRID *Grid, *GridPlane;                                 /* Test grid */
  twrPRIM *MyPr1, *PrimGrid, *PrimPlane;                     /* Test primitives */
  twrMODEL *MyModel;                                         /* Test model */
} twrUNIT_SAMPLE_MN7;

/* Image drawing function.
 * ARGUMENTS:
 *   - image name:
 *       CHAR *ImgName;
 * RETURNS: None.
 */
static VOID TWR_DrawImageCheckMK3( CHAR *ImgName )
{
  INT x, y;
  twrIMAGE Img;

  if (Twr->ImgLoad(&Img, ImgName))
  {
    Twr->Log("\n");
    for (y = 0; y < Img.H; y++)
    {
      for (x = 0; x < Img.W; x++)
      {
        INT
          c = Img.Pixels[y * Img.W + x],
          r = (BYTE)(c >> 16),
          g = (BYTE)(c >> 8),
          b = (BYTE)(c >> 0);
        Twr->Log("\x1b[38;2;%d;%d;%dm30", r, g, b);
      }
      Twr->Log("\n");
    }
  TWR_ImgFree(&Img);
  }
} /* End of 'TWR_DrawImageCheckMK3' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MN7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_MN7 *Uni )
{
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /*
  Uni->SkyMtlPat = Twr->MtlPatCreate("sky_box", "sky_box", 4, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->SkyMtl = Twr->MtlCreate("skyboxmaterial", Uni->SkyMtlPat, TWR_MTL_OPAQUE);

  Uni->MyMtlPat = Twr->MtlPatCreate("Phong_STD4", "default", 3, sizeof(twrVERTEX_STD4), attrs, 4);
  Uni->MyMtl = Twr->MtlCreate("MyMtl", Uni->MyMtlPat, TWR_MTL_OPAQUE);

  Twr->MtlBindTex(Uni->MyMtl, 0, Twr->TexCreateFromVec3("Ka", Vec3Set1(0.1)));
  Twr->MtlBindTex(Uni->MyMtl, 1, Twr->TexCreateFromVec4("KdTrans", Vec4SetVec3(Vec3Set1(0.8), 1)));
  Twr->MtlBindTex(Uni->MyMtl, 2, Twr->TexCreateFromVec4("KsPh", Vec4SetVec3(Vec3Set1(0.5), 30)));
  Twr->MtlBindTex(Uni->SkyMtl, 3, Twr->TexCreateCubeMap("SkyNebula"));

  Twr->ModelLoadG3DMSetTransform(MatrRotateX(-90));
  Uni->MyModel = Twr->ModelLoadG3DM(NULL, "bin/models/x6.g3dm");
  */

  /* if (TRUE)
  {
    twrVERTEX_STD4 V1[] =
    {
      {{Uni->MyModel->MinBB.X, Uni->MyModel->MinBB.Y, Uni->MyModel->MinBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MinBB.X, Uni->MyModel->MaxBB.Y, Uni->MyModel->MinBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MinBB.X, Uni->MyModel->MaxBB.Y, Uni->MyModel->MaxBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MinBB.X, Uni->MyModel->MinBB.Y, Uni->MyModel->MaxBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},

      {{Uni->MyModel->MaxBB.X, Uni->MyModel->MinBB.Y, Uni->MyModel->MinBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MaxBB.X, Uni->MyModel->MaxBB.Y, Uni->MyModel->MinBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MaxBB.X, Uni->MyModel->MaxBB.Y, Uni->MyModel->MaxBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
      {{Uni->MyModel->MaxBB.X, Uni->MyModel->MinBB.Y, Uni->MyModel->MaxBB.Z}, {0, 0}, {0, 0, 1}, {1, 1, 1, 1}},
    };
    INT Ind[] =
    {
      0, 1, 2,
      0, 2, 3,
      3, 2, 6,
      3, 6, 7,
      1, 2, 5,
      5, 2, 6,
      0, 3, 7,
      0, 7, 4,
      0, 1, 5,
      0, 5, 4,
      4, 5, 6,
      4, 6, 7
    };

    glLineWidth(3);
    Uni->MyPr1 = Twr->PrimCreate(Uni->MyMtl, TWR_RND_PRIM_LINES, V1, 8, Ind, 36);
  } */

  /*
  Uni->MyMtlPat = Twr->MtlPatGetDefault();
  Twr->MtlPatReferenceInc(Uni->MyMtlPat);
  strncpy(Uni->MyMtlPat->Shd->Name, "normal_mapping", TWR_STR_MAX - 1);
  Uni->MyMtl = Twr->MtlCreate("MyMtl", Uni->MyMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlPatReferenceInc(Uni->MyMtlPat);

  Uni->MyModel = Twr->ModelLoadG3DM(NULL, "bin/models/gallery.g3dm");
  */
  /* Uni->MyModel = Twr->ModelLoadG3DM(NULL, "bin/models/interior.g3dm"); */

  Twr->LightEnableShadow(Twr->LightAddDir(Vec3Set(1, 1, 0), Vec3Set(0.5, 0.5, 0.5)));

  /* TWR_DrawImageCheckMK3("bin/images/test/1.jpg"); */
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MN7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_MN7 *Uni )
{
  /*
  Twr->ModelFree(Uni->MyModel);
  Twr->MtlFree(Uni->MyMtl);
  Twr->MtlPatFree(Uni->MyMtlPat);
  */
  /* Twr->ModelFree(Uni->MyModel); */
  /* Twr->PrimFree(Uni->MyPr1); */
  /* Twr->MtlFree(Uni->MyMtl);
  Twr->MtlPatFree(Uni->MyMtlPat); */
  /*
  Twr->GridFree(Uni->Grid);
  Twr->PrimFree(Uni->PrimGrid);
  */
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MN7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_MN7 *Uni )
{
} /* End of 'TWR_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MN7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_MN7 *Uni )
{
  /* MATR m; */

  /*
  m = MatrMulMatr3(MatrScale(Vec3Set1(1.1)),
                   MatrRotateY(30 * Twr->Time), MatrTranslate(Vec3Set(-3.5, 30, 0)));
  Twr->ModelDraw(Uni->MyModel, MatrIdentity());
  */
} /* End of 'TWR_UnitRender' function */

/* MN7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleMN7( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_MN7))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleMN7' function */

/* END OF 'twr_unit_sample_MN7_0.c' FILE */
