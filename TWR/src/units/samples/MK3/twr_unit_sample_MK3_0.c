/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_MK3_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by MK3 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* max number of model */
#define MAX_MODEL_MK3 30

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_MK3
{
  TWR_UNIT_BASE_FIELDS;
  twrMODEL *Models[MAX_MODEL_MK3];
  VEC3 Deltas[MAX_MODEL_MK3];
  twrMATERIAL_PATTERN *MtlPat;
  INT NumOfModels;
  twrLIGHT *Lig;
} twrUNIT_SAMPLE_MK3;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MK3 *Uni;
 * RETURNS: None.
 */
/*
static VOID TWR_DrawImageInit( twrUNIT_SAMPLE_MK3 *Uni )
{
  INT x, y;
  twrIMAGE Img;
  
  if (Twr->ImgLoad(&Img, "bin/images/test/spellcraft.jpg"))
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
} /* End of 'TWR_UnitInit' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MK3 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_MK3 *Uni )
{
  INT i;
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };
  CHAR *FileNames[] = 
  {
    "bin/models/MK3_sample/trees/flower.g3dm", 
    "bin/models/MK3_sample/trees/sunflower.g3dm", "bin/models/MK3_sample/trees/hickory.g3dm", 
    "bin/models/MK3_sample/trees/rose.g3dm", "bin/models/MK3_sample/trees/orange.g3dm",
  };

  Uni->MtlPat = Twr->MtlPatCreate("Tree", "samples/mk3/trees", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  
  for (i = 0; i < sizeof(FileNames) / sizeof(FileNames[0]); i++)
  {
    Uni->Models[i] = Twr->ModelLoadG3DM(Uni->MtlPat, FileNames[i]), Uni->NumOfModels++;
    if (i == 0)
      Uni->Models[i]->Trans = MatrScale(Vec3Set1(0.2));
    else if (i == 1)
      Uni->Models[i]->Trans = MatrIdentity();
    else
      Uni->Models[i]->Trans = MatrIdentity();
  }

  Uni->Lig = Twr->LightAddDir(Vec3Set(15, 15, 15), Vec3Set(1, 1, 1));
  Twr->LightEnableShadow(Uni->Lig);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MK3 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_MK3 *Uni )
{
  INT i;

  for (i = 0; i < Uni->NumOfModels; i++)
    Twr->ModelFree(Uni->Models[i]);
  Twr->MtlPatFree(Uni->MtlPat);
  Uni->NumOfModels = 0;
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MK3 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_MK3 *Uni )
{
  if (Twr->Keys['1'] && Twr->Keys['S'])
    Uni->Models[1]->Trans = MatrRotateX(30 * Twr->Time);
  else if (Twr->Keys['1'] && Twr->Keys['D'])
    Uni->Models[1]->Trans = MatrRotateY(30 * Twr->Time);
  else if (Twr->Keys['1'] && Twr->Keys['F'])
    Uni->Models[1]->Trans = MatrRotateZ(30 * Twr->Time);
  else if (Twr->Keys['2'] && Twr->Keys['S'])
    Uni->Models[2]->Trans = MatrRotateX(30 * Twr->Time);
  else if (Twr->Keys['2'] && Twr->Keys['D'])
    Uni->Models[2]->Trans = MatrRotateY(30 * Twr->Time);
  else if (Twr->Keys['2'] && Twr->Keys['F'])
    Uni->Models[2]->Trans = MatrRotateZ(30 * Twr->Time);
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_MK3 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_MK3 *Uni )
{
  INT i;

  for (i = 0; i < Uni->NumOfModels; i++)
  {
    if (i == 0)
      Twr->ModelDraw(Uni->Models[i], MatrMulMatr4(MatrRotateY(3 * Twr->Time), Uni->Models[i]->Trans, 
        MatrTranslate(Vec3Set(i * 10, -3.5, 0)), MatrTranslate(Uni->Deltas[i])));
    else if (i == 1)
      Twr->ModelDraw(Uni->Models[i], MatrMulMatr4(MatrRotateY(9 * Twr->Time), Uni->Models[i]->Trans, 
        MatrTranslate(Vec3Set(i * 10, -3.5, 0)), MatrTranslate(Uni->Deltas[i])));
    else if (i == Uni->NumOfModels - 1)
      Twr->ModelDraw(Uni->Models[i], MatrMulMatr3(MatrRotateY(7 * Twr->Time), Uni->Models[i]->Trans, 
        MatrTranslate(Vec3Set(i * 10, -3.5 - 1.7, 0))));
    else
      Twr->ModelDraw(Uni->Models[i], MatrMulMatr3(MatrRotateY(5 * Twr->Time), Uni->Models[i]->Trans, 
        MatrTranslate(Vec3Set(i * 10, -3.5 - i * 1.3, 0))));
  }
} /* End of 'TWR_UnitRender' function */

/* MK3 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleMK3( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_MK3))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleMK3' function */

/* END OF 'twr_unit_sample_MK3_0.c' FILE */
