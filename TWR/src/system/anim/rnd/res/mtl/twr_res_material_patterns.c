/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_material_patterns.c
 * PURPOSE     : Tough Water Rendering project.
 *               Material patterns implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <string.h>

/* Stock of material patterns */
static STOCK TWR_StkMaterialPatterns;

/* Pointer to default material pattern */
static twrMATERIAL_PATTERN *DefMtlPat;

/* Create new material pattern function.
 * ARGUMENTS:
 *   - new material pattern name:
 *       CHAR *MtlPatName;
 *   - shader name:
 *       CHAR *ShaderName;
 *   - number of textures:
 *       INT NumOfTextures;
 *   - number of buffers:
 *       INT NumOfBuffers;
 *   - stride (size of one vertex):
 *       INT Stride;
 *   - vertices array:
 *       twrVERTEX_ATTRS *Attrs;
 *   - amount of attributes:
 *       INT NumOfAttrs;
 * RETURNS:
 *   (twrMATERIAL_PATTERN *) pointer to new material pattern.
 */
static twrMATERIAL_PATTERN *TWR_MtlPatCreate( CHAR *MtlPatName, CHAR *ShaderName, INT NumOfTextures,
         INT NumOfBuffers, INT Stride, twrVERTEX_ATTRS *Attrs, INT NumOfAttrs )
{
  twrMATERIAL_PATTERN *MtlPat;

  MtlPat = StockAlloc(&TWR_StkMaterialPatterns);
  strncpy(MtlPat->Name, MtlPatName, TWR_STR_MAX - 1);
  MtlPat->Shd = Twr->ShdCreate(ShaderName);
  MtlPat->NumOfTextures = NumOfTextures;
  MtlPat->RefCounter = 0;
  MtlPat->NumOfAttributes = NumOfAttrs;
  MtlPat->NumOfBuffers = NumOfBuffers;
  MtlPat->VertexStride = Stride;
  MtlPat->DepthFlag = TWR_DEPTH_READ_AND_WRITE;
  MtlPat->BlendMode = TWR_BLEND_REPLACE;

  memcpy(MtlPat->Attrs, Attrs, sizeof(twrVERTEX_ATTRS) * NumOfAttrs);

  return MtlPat;
} /* End of 'TWR_MtlPatCreate' function */

/* Delete material pattern by its pointer function.
 * ARGUMENTS:
 *   - pointer to material pattern:
 *       twrMATERIAL_PATTERN *MtlPat; 
 * RETURNS: None.
 */
static VOID TWR_MtlPatFree( twrMATERIAL_PATTERN *MtlPat )
{
  if (MtlPat == NULL)
    return;
  MtlPat->RefCounter--;
  if (MtlPat->RefCounter <= 0)
    StockDelete(MtlPat);
} /* End of 'TWR_MtlPatFree' function */

/* Create default material pattern function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_MtlPatCreateDefault( VOID )
{
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  DefMtlPat = Twr->MtlPatCreate("Default material pattern (Phong STD4)", "default", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  Twr->MtlPatReferenceInc(DefMtlPat);
} /* End of 'TWR_MtlPatCreateDefault' function */

/* Get default material pattern function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (twrMATERIAL_PATTERN *) pointer to default material pattern.
 */
static twrMATERIAL_PATTERN *TWR_MtlPatGetDefault( VOID )
{
  Twr->MtlPatReferenceInc(DefMtlPat);
  return DefMtlPat;
} /* End of 'TWR_MtlPatGetDefault' function */

/* Apply material pattern (buffers) to shader, set depth and blending function.
 * ARGUMENTS:
 *   - material pattern to be applied:
 *       twrMATERIAL_PATTERN *MtlPat;
 * RETURNS: None.
 */
static VOID TWR_MtlPatApply( twrMATERIAL_PATTERN *MtlPat )
{
  INT i;

  Twr->ToggleDepth(MtlPat->DepthFlag);
  Twr->ToggleBlend(MtlPat->BlendMode);

  /* Apply buffers to shader */
  for (i = 0; i < MtlPat->NumOfBuffers; i++)
    if (MtlPat->Bufs[i] != NULL)
      Twr->BufApply(MtlPat->Bufs[i]);
} /* End of 'TWR_MtlPatApply' function */

/* Bind buffer to the material pattern function.
 * ARGUMENTS:
 *   - material pattern to bind buffer to:
 *       twrMATERIAL_PATTERN *MtlPat;
 *   - buffer to be binded:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_MtlPatBindBuf( twrMATERIAL_PATTERN *MtlPat, twrBUFFER *Buf )
{
  if (Buf != NULL)
  {
    if (MtlPat->NumOfBuffers >= TWR_MAX_BUFFERS)
      return;
    MtlPat->Bufs[MtlPat->NumOfBuffers] = Buf;
    MtlPat->NumOfBuffers++;
  }
} /* End of 'TWR_MtlPatBindBuf' function */

/* Set depth flag to the material pattern function.
 * ARGUMENTS:
 *   - material pattern to set depth flag to:
 *       twrMATERIAL_PATTERN *MtlPat;
 *   - depth flag:
 *       twrDEPTH_FLAG DepthFlag;
 * RETURNS: None.
 */
static VOID TWR_MtlPatSetDepthFlag( twrMATERIAL_PATTERN *MtlPat, twrDEPTH_FLAG DepthFlag )
{
  MtlPat->DepthFlag = DepthFlag;
} /* End of 'TWR_MtlPatSetDepthFlag' function */

/* Set blending mode to the material pattern function.
 * ARGUMENTS:
 *   - material pattern to set blending mode to:
 *       twrMATERIAL_PATTERN *MtlPat;
 *   - blending mode:
 *       twrBLEND_MODE BlendMode;
 * RETURNS: None.
 */
static VOID TWR_MtlPatSetBlendMode( twrMATERIAL_PATTERN *MtlPat, twrBLEND_MODE BlendMode )
{
  MtlPat->BlendMode = BlendMode;
} /* End of 'TWR_MtlPatSetBlendMode' function */

/* Find material by name in material pattern stock function.
 * ARGUMENTS:
 *   - material pattern name
 *       CHAR *MtlPatName;
 *   - material pattern file name:
 *       CHAR *FindName;
 * RETURNS:
 *   (BOOL) TRUE if material pattern is found, FALSE otherwise.
 */
static BOOL TWR_MtlPatFind( twrMATERIAL_PATTERN *MtlPat, CHAR *FindName )
{
  return (strncmp(MtlPat->Name, FindName, TWR_STR_MAX)) == 0;
} /* End of 'TWR_MtlPatFind' function */

/* Increase counter of usage of material pattern function.
 * ARGUMENTS:
 *   - material pattern to increase reference counter of:
 *       twrMATERIAL_PATTERN *MtlPat;
 * RETURNS: None.
 */
static VOID TWR_MtlPatReferenceInc( twrMATERIAL_PATTERN *MtlPat )
{
  if (MtlPat == NULL)
    return;
  MtlPat->RefCounter++;
} /* End of 'TWR_MtlPatReferenceInc' function */

/* Set material patterns deletion strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_DeleteStrategyMtlPattern( twrMATERIAL_PATTERN *MtlPat )
{
  INT i;

  MtlPat->FirstDraw = NULL;
  MtlPat->NextDraw = NULL;
  MtlPat->Shd = NULL;
  /* Free buffers */
  for (i = 0; i < MtlPat->NumOfBuffers; i++)
    Twr->BufFree(MtlPat->Bufs[i]);
} /* End of 'TWR_DeleteStrategyMtlPattern' function */

/* Initialize material patterns system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialPatternsInit( VOID )
{
  Twr->MtlPatCreate = TWR_MtlPatCreate;
  Twr->MtlPatFree = TWR_MtlPatFree;
  Twr->MtlPatCreateDefault = TWR_MtlPatCreateDefault;
  Twr->MtlPatGetDefault = TWR_MtlPatGetDefault;
  Twr->MtlPatApply = TWR_MtlPatApply;
  Twr->MtlPatBindBuf = TWR_MtlPatBindBuf;
  Twr->MtlPatSetDepthFlag = TWR_MtlPatSetDepthFlag;
  Twr->MtlPatSetBlendMode = TWR_MtlPatSetBlendMode;
  Twr->MtlPatFind = TWR_MtlPatFind;
  Twr->MtlPatReferenceInc = TWR_MtlPatReferenceInc;

  /* Create stock of material patterns */
  StockCreate(&TWR_StkMaterialPatterns, "Material patterns", sizeof(twrMATERIAL_PATTERN),
    300, TWR_DeleteStrategyMtlPattern);
  /* Register stock of materials patterns to strip */
  TWR_StockRegisterToStrip(&TWR_StkMaterialPatterns);

  /* Create default material pattern */
  Twr->MtlPatCreateDefault();

  Twr->Sys("Material patterns system is initialized");
} /* End of 'TWR_SystemMaterialPatternsInit' function */

/* Delete material patterns system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialPatternsClose( VOID )
{
  StockFree(&TWR_StkMaterialPatterns);
  Twr->Sys("Material patterns system is deinitialized");
} /* End of 'TWR_SystemMaterialPatternsClose' function */

/* END OF 'twr_res_material_patterns.c' FILE */
