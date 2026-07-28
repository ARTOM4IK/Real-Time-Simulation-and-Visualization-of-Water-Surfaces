/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_materials.h
 * PURPOSE     : Tough Water Rendering project.
 *               Materials declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_materials_h_
#define __twr_res_materials_h_

#include "twr_def.h"

/* Maximum number of textures in material */
#define TWR_MAX_TEXTURES 16
/* Maximum number of buffers in material */
#define TWR_MAX_BUFFERS 8

/* Transparency flag for materials */
typedef enum tagtwrTRANSPARENCY_FLAG
{
  TWR_MTL_OPAQUE = 0,      /* Material is opaque */
  TWR_MTL_TRANSPARENT = 1, /* Material is transparent */
  TWR_MTL_SHADOW = 2,      /* Material should be drawn in the shadow cast pass */
  TWR_MTL_FINAL = 3,       /* Material should be drawn in the final/last pass */
  TWR_MTL_COMPUTE = 4,     /* Material should be drawn in the compute pass */
} twrTRANSPARENCY_FLAG;

/* Material representation type */
typedef struct tagtwrMATERIAL
{
  CHAR Name[TWR_STR_MAX];               /* Material name */
  twrMATERIAL_PATTERN *MtlPat;          /* Pointer to material pattern */
  twrTRANSPARENCY_FLAG IsTrans;         /* Transparency flag */
  FLT Trans;                            /* Transparency coefficient (if there is texture as well) */
  INT RefCounter;                       /* Reference counter */
  UINT64 StoreFrameNo;                  /* Index of frame when material
                                         * was first stored */
  INT NumOfBuffers;                     /* Number of buffers */
  twrBUFFER *Bufs[TWR_MAX_BUFFERS];     /* Buffers array */
  INT NumOfTextures;                    /* Number of material's textures */
  twrTEXTURE *Texs[TWR_MAX_TEXTURES];   /* Array of textures (maximum 16) */
  twrPRIM *FirstDraw;                   /* Pointer to the first primitive
                                         * of this material to draw */
  twrMATERIAL *NextDraw;                /* Pointer to the next material
                                         * of this material pattern to draw */
} twrMATERIAL;

/* Materials system structure */
typedef struct tagtwrSYSTEM_MATERIALS
{
  /* Create new material function.
   * ARGUMENTS:
   *   - new material name:
   *       CHAR *MtlName;
   *   - pointer to material pattern:
   *       twrMATERIAL_PATTERN *MtlPat; 
   * RETURNS:
   *   (twrMATERIAL *) pointer to new material.
   */
  twrMATERIAL * (*MtlCreate)( CHAR *MtlName, twrMATERIAL_PATTERN *MtlPat,
                              twrTRANSPARENCY_FLAG IsTrans );

  /* Delete material by its pointer function.
   * ARGUMENTS:
   *   - pointer to material:
   *       twrMATERIAL *Mtl; 
   * RETURNS: None.
   */
  VOID (*MtlFree)( twrMATERIAL *Mtl );

  /* Get default material function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (twrMATERIAL *) pointer to default material.
   */
  twrMATERIAL * (*MtlGetDefault)( VOID );

  /* Apply material (textures and buffers) to shader function.
   * ARGUMENTS:
   *   - material to be applied:
   *       twrMATERIAL *Mtl;
   * RETURNS: None.
   */
  VOID (*MtlApply)( twrMATERIAL *Mtl );

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
  VOID (*MtlBindTex)( twrMATERIAL *Mtl, INT TexNo, twrTEXTURE *Texture );

  /* Bind buffer to the material function.
   * ARGUMENTS:
   *   - material to bind buffer to:
   *       twrMATERIAL *Mtl;
   *   - buffer to be binded:
   *       twrBUFFER *Buf;
   * RETURNS: None.
   */
  VOID (*MtlBindBuf)( twrMATERIAL *Mtl, twrBUFFER *Buf );

  /* Increase counter of usage of material function.
   * ARGUMENTS:
   *   - material to increase reference counter of:
   *       twrMATERIAL *Mtl;
   * RETURNS: None.
   */
  VOID (*MtlReferenceInc)( twrMATERIAL *Mtl );
} twrSYSTEM_MATERIALS;

/* initialize materials stock function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialsInit( VOID );

/* Delete materials stock function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMaterialsClose( VOID );

/* Set materials deletion strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_DeleteStrategyMaterial( twrMATERIAL *Mtl );

#endif /* __twr_res_materials_h_ */

/* END OF 'twr_res_materials.h' FILE */
