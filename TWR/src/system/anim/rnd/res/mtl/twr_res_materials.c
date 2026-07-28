/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_materials.c
 * PURPOSE     : Tough Water Rendering project.
 *               Materials implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 19.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Stock of materials */
static STOCK TWR_StkMaterials;

/* Pointer to default material */
static twrMATERIAL *DefMtl;

/* Create new material function.
 * ARGUMENTS:
 *   - new material name:
 *       CHAR *MtlName;
 *   - pointer to material pattern:
 *       twrMATERIAL_PATTERN *MtlPat;
 * RETURNS:
 *   (twrMATERIAL *) pointer to new material.
 */
static twrMATERIAL * TWR_MtlCreate( CHAR *MtlName, twrMATERIAL_PATTERN *MtlPat,
                                    twrTRANSPARENCY_FLAG IsTrans )
{
  INT i;
  twrMATERIAL *Mtl;
  twrTEXTURE *TexDef = Twr->TexGetDefault();

  Mtl = StockAlloc(&TWR_StkMaterials);
  memset(Mtl, 0, sizeof(twrMATERIAL));
  strncpy(Mtl->Name, MtlName, TWR_STR_MAX - 1);
  if (MtlPat != NULL)
    Mtl->MtlPat = MtlPat;
  else
    Mtl->MtlPat = Twr->MtlPatGetDefault();
  Mtl->IsTrans = IsTrans;
  Mtl->RefCounter = 0;
  Mtl->StoreFrameNo = 0;
  Mtl->NumOfBuffers = 0;
  Mtl->NumOfTextures = MtlPat->NumOfTextures;
  Mtl->Trans = 0;
  Twr->MtlPatReferenceInc(Mtl->MtlPat);
  for (i = 0; i < Mtl->NumOfTextures; i++)
  {
    Mtl->Texs[i] = TexDef;
    Twr->TexReferenceInc(Mtl->Texs[i]);
  }

  return Mtl;
} /* End of 'TWR_MtlCreate' function */

/* Delete material by its pointer function.
 * ARGUMENTS:
 *   - pointer to material:
 *       twrMATERIAL *Mtl; 
 * RETURNS: None.
 */
static VOID TWR_MtlFree( twrMATERIAL *Mtl )
{
  if (Mtl == NULL)
    return;
  Mtl->RefCounter--;
  if (Mtl->RefCounter <= 0)
  {
    Twr->MtlPatFree(Mtl->MtlPat);
    StockDelete(Mtl);
  }
} /* End of 'TWR_MtlFree' function */

/* Create default material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (twrMATERIAL *) pointer to default material.
 */
static twrMATERIAL * TWR_MtlCreateDefault( VOID )
{
  INT i;
  twrMATERIAL *Mtl;
  twrTEXTURE *TexDef = Twr->TexGetDefault();

  Mtl = StockAlloc(&TWR_StkMaterials);
  memset(Mtl, 0, sizeof(twrMATERIAL));
  strncpy(Mtl->Name, "Default material", TWR_STR_MAX - 1);
  Mtl->MtlPat = Twr->MtlPatGetDefault();
  Mtl->IsTrans = TWR_MTL_OPAQUE;
  Mtl->RefCounter = 0;
  Mtl->StoreFrameNo = 0;
  Mtl->NumOfBuffers = 0;
  Mtl->NumOfTextures = Mtl->MtlPat->NumOfTextures;
  Mtl->Trans = 1;
  Twr->MtlPatReferenceInc(Mtl->MtlPat);
  for (i = 0; i < Mtl->NumOfTextures; i++)
  {
    Mtl->Texs[i] = TexDef;
    Twr->TexReferenceInc(Mtl->Texs[i]);
  }
  Twr->MtlReferenceInc(Mtl);
  return Mtl;
} /* End of 'TWR_MtlCreateDefault' function */

/* Get default material function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (twrMATERIAL *) pointer to default material.
 */
static twrMATERIAL * TWR_MtlGetDefault( VOID )
{
  return DefMtl;
} /* End of 'TWR_MtlGetDefault' function */

/* Apply material (textures and buffers) to shader function.
 * ARGUMENTS:
 *   - material to be applied:
 *       twrMATERIAL *Mtl;
 * RETURNS: None.
 */
static VOID TWR_MtlApply( twrMATERIAL *Mtl )
{
  INT i;

  /* Apply textures to shader */
  for (i = 0; i < Mtl->NumOfTextures; i++)
    if (Mtl->Texs[i] != NULL)
    {
      glActiveTexture(GL_TEXTURE0 + i);
      if (Mtl->Texs[i]->Type == TWR_TEXTURE_2D)
        glBindTexture(GL_TEXTURE_2D, Mtl->Texs[i]->TexId);
      else if (Mtl->Texs[i]->Type == TWR_TEXTURE_3D)
        glBindTexture(GL_TEXTURE_3D, Mtl->Texs[i]->TexId);
      else if (Mtl->Texs[i]->Type == TWR_TEXTURE_CUBE_MAP)
        glBindTexture(GL_TEXTURE_CUBE_MAP, Mtl->Texs[i]->TexId);
      else
        return;
    }

  /* Apply buffers to shader */
  for (i = 0; i < Mtl->NumOfBuffers; i++)
    if (Mtl->Bufs[i] != NULL)
      Twr->BufApply(Mtl->Bufs[i]);

  /* Apply transparency coefficient through uniform */
  if (Mtl->IsTrans == TWR_MTL_TRANSPARENT)
    glUniform1f(1, Mtl->Trans);
} /* End of 'TWR_MtlApply' function */

/* Bind texture to the material function.
 * ARGUMENTS:
 *   - material to bind texture to:
 *       twrMATERIAL *Mtl;
 *   - texture index in textures array:
 *       INT TexNo;
 *   - texture to be binded:
 *       twrTEXTURE *Texture;
 * RETURNS: None.
 */
static VOID TWR_MtlBindTex( twrMATERIAL *Mtl, INT TexNo, twrTEXTURE *Texture )
{
  if (TexNo >= 0 && TexNo < Mtl->NumOfTextures)
  {
    Twr->TexFree(Mtl->Texs[TexNo]);
    Mtl->Texs[TexNo] = Texture;
    Twr->TexReferenceInc(Texture);
  }
} /* End of 'TWR_MtlBindTex' function */

/* Bind buffer to the material function.
 * ARGUMENTS:
 *   - material to bind buffer to:
 *       twrMATERIAL *Mtl;
 *   - buffer to be binded:
 *       twrBUFFER *Buf;
 * RETURNS: None.
 */
static VOID TWR_MtlBindBuf( twrMATERIAL *Mtl, twrBUFFER *Buf )
{
  if (Buf != NULL)
  {
    if (Mtl->NumOfBuffers >= TWR_MAX_BUFFERS)
      return;
    Mtl->Bufs[Mtl->NumOfBuffers] = Buf;
    Mtl->NumOfBuffers++;
    Twr->BufReferenceInc(Buf);
  }
} /* End of 'MtlBindBuf' function */

/* Increase counter of usage of material function.
 * ARGUMENTS:
 *   - material to increase reference counter of:
 *       twrMATERIAL *Mtl;
 * RETURNS: None.
 */
static VOID TWR_MtlReferenceInc( twrMATERIAL *Mtl )
{
  if (Mtl == NULL)
    return;
  Mtl->RefCounter++;
} /* End of 'TWR_MtlReferenceInc' function */

/* Set materials deletion strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_DeleteStrategyMaterial( twrMATERIAL *Mtl )
{
  INT i;

  /* Check if material is empty */
  if (Mtl == NULL)
    return;
  /* Free textures */
  Twr->Log("Delete material: '");
  Twr->SetColor(0x00FFFF, 0);
  Twr->Log(Mtl->Name);
  Twr->SetColor(0xFFFFFF, 0);
  Twr->Log("' [%p Ref: %d]\n", Mtl, Mtl->RefCounter);
  for (i = 0; i < Mtl->NumOfTextures; i++)
  {
    Twr->Log("     Delete texture %d: ", i);
    Twr->SetColor(0x00FF00, 0);
    Twr->Log(Mtl->Texs[i]->Name);
    Twr->SetColor(0xFFFFFF, 0);
    Twr->Log("[Ref: %d]\n", Mtl->Texs[i]->RefCounter);
    Twr->TexFree(Mtl->Texs[i]);
  }

  /* Free buffers */
  for (i = 0; i < Mtl->NumOfBuffers; i++)
    Twr->BufFree(Mtl->Bufs[i]);
} /* End of 'TWR_DeleteStrategyMaterial' function */

/* Initialize materials stock function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialsInit( VOID )
{
  Twr->MtlCreate = TWR_MtlCreate;
  Twr->MtlFree = TWR_MtlFree;
  Twr->MtlGetDefault = TWR_MtlGetDefault;
  Twr->MtlApply = TWR_MtlApply;
  Twr->MtlBindTex = TWR_MtlBindTex;
  Twr->MtlBindBuf = TWR_MtlBindBuf;
  Twr->MtlReferenceInc = TWR_MtlReferenceInc;

  /* Create stock of materials */
  StockCreate(&TWR_StkMaterials, "Materials", sizeof(twrMATERIAL),
    300, TWR_DeleteStrategyMaterial);
  /* Register stock of materials to strip */
  TWR_StockRegisterToStrip(&TWR_StkMaterials);

  /* Create default material */
  DefMtl = TWR_MtlCreateDefault();

  Twr->Sys("Materials system is initialized");
} /* End of 'TWR_SystemMaterialsInit' function */

/* Delete materials stock function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialsClose( VOID )
{
  StockFree(&TWR_StkMaterials);
  Twr->Sys("Materials system is deinitialized");
} /* End of 'TWR_SystemMaterialsClose' function */

/* END OF 'twr_res_materials.c' FILE */
