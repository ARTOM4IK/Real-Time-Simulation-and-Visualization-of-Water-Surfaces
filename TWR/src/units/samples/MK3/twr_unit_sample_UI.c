/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_UI.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by UI implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Maximum number of models in UI */
#define MAX_NUM_OF_MODELS 10

/* Maximum number of images for models of UI */
#define MAX_NUM_OF_IMAGES 10

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_UI
{
  TWR_UNIT_BASE_FIELDS;
  twrMATERIAL_PATTERN *MtlPat;          /* Material pattern for UI */
  twrIMAGE *Img[MAX_NUM_OF_IMAGES];      /* Array of images for UI */
  twrPRIM *Prim;                        /* Primitive to convert images in texture for UI */
  twrTEXTURE *Tex;                      /* Texure of UI objects */
  twrMODEL *Mdl[MAX_NUM_OF_MODELS];     /* Array of models for UI */
  twrMATERIAL *Mtl;                     /* Materual for UI models */
  BOOL IsActive;                        /* Flag of Activities of window */
  INT NumOfModels;                      /* Number of using models of array of models */
} twrUNIT_SAMPLE_UI;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_UI *Uni;
 *   - number of texture in array:
 *       INT NumTex;
 * RETURNS: None.
 */
static VOID TWR_UnitUICreateSlider( FLT* Var, CHAR* Name, FLT NormalNum, FLT MinNum, FLT MaxNum, FLT NumStep)
{
} /* End of 'TWR_UnitUICreateSlider' function */

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_UI *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_UI *Uni )
{
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /* Initialised system */
  Uni->NumOfModels = 0;

  /* Create a material pattern, texture, load images */
  Uni->MtlPat = Twr->MtlPatCreate("UI", "UI", 3, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->ImgLoad(Uni->Img[0], "bin/images/slider1.png");
  Uni->Mdl[0] = Twr->ModelCreate();
  Uni->Tex = Twr->TexCreate("UI", Uni->Img[0]->W, Uni->Img[0]->H, 0, GL_RGBA8, Uni->Img[0]->Pixels);
  Uni->Mtl = Twr->MtlCreate("UI", Uni->MtlPat, 0);
  Twr->MtlBindTex(Uni->Mtl, 0, Uni->Tex);
  Uni->Prim = Twr->PrimCreate(Uni->Mtl, TWR_RND_PRIM_TRISTRIP, NULL, 4, NULL, 0);
  Twr->ModelPrimAdd(Uni->Mdl[0], Uni->Prim, MatrIdentity());
  Uni->NumOfModels++;

  /* Corect parametrs drawing models */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, Uni->Tex->TexId);
  glBindTexture(GL_TEXTURE_2D, Uni->Tex->TexId);
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_UI *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_UI *Uni )
{
  INT i;

  Twr->MtlPatFree(Uni->MtlPat);
  for (i = 0; i < Uni->NumOfModels; i++)
    Twr->ModelFree(Uni->Mdl[i]);
  for (i = 0; i < MAX_NUM_OF_IMAGES; i++)
    Twr->ImgFree(Uni->Img[i]);
  Twr->TexFree(Uni->Tex);
  Twr->PrimFree(Uni->Prim);
} /* End of 'TWR_UnitClose' function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_UI *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_UI *Uni )
{
  if (Twr->Mdx >= 0 && Twr->Mdx < 10)
    Uni->IsActive = !Uni->IsActive;
  glUniform1f(1, Uni->IsActive);
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_UI *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_UI *Uni )
{
  Twr->ModelDraw(Uni->Mdl[0], MatrIdentity());
  Twr->ModelDraw(Uni->Mdl[1], MatrIdentity());
} /* End of 'TWR_UnitRender' function */

/* UI unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleUI( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_UI))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleUI' function */

/* END OF 'twr_unit_sample_UI.c' FILE */
