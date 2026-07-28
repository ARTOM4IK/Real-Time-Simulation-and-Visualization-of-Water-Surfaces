/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_models.h
 * PURPOSE     : Tough Water Rendering project.
 *               Models declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_models_h_
#define __twr_res_models_h_

#include "twr_def.h"

/* Primitive storage structure (with matrix) */
typedef struct tagtwrMODEL_PRIM
{
  twrPRIM *Prim; /* Primitive pointer */
  MATR Trans;    /* Transform matrix */
} twrMODEL_PRIM;

/* Model structure */
typedef struct tagtwrMODEL
{
  INT 
    NumOfPrims,         /* Number of primitives in model */
    MaxNumOfPrims;      /* Max allocated memory for primitives */
  twrMODEL_PRIM *Prims; /* Primitive storage array */
  VEC3 MinBB, MaxBB;    /* Model bound box */
  MATR Trans;           /* Model transformation matrix */
} twrMODEL;

/* Models system */
typedef struct tagtwrSYSTEM_MODELS
{
  /* Create model function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (twrMODEL *) pointer to model, NULL if model is not created.
   */
  twrMODEL * (*ModelCreate)( VOID );

  /* Create model by number of primitives function.
   * ARGUMENTS:
   *   - number of primitives in model:
   *       INT NumOfPrims;
   * RETURNS:
   *   (twrMODEL *) pointer to model, NULL if model is not created.
   */
  twrMODEL * (*ModelCreateByNumOfPrims)( INT NumOfPrims );

  /* Model free function.
   * ARGUMENTS:
   *   - model pointer:
   *       twrMODEL * Mdl;
   * RETURNS: None.
   */
  VOID (*ModelFree)( twrMODEL *Mdl );

  /* Primitive add to model function.
   * ARGUMENTS:
   *   - model pointer:
   *       twrMODEL *Mdl;
   *   - primitive pointer:
   *       twrPRIM *Pr;
   *   - transform matrix:
   *       MATR Trans;
   * RETURNS:
   *   (BOOL) TRUE if success, FALSE otherwise.
   */
  BOOL (*ModelPrimAdd)( twrMODEL *Mdl, twrPRIM *Pr, MATR Trans );

  /* Load g3dm model function.
   * ARGUMENTS:
   *   - material pattern:
   *       twrMATERIAL_PATTERN *MtlPat;
   *   - name of model file:
   *       CHAR *FileName;
   * RETURNS:
   *   (twrMODEL *) pointer to model, NULL if model is not loaded.
   */
  twrMODEL * (*ModelLoadG3DM)( twrMATERIAL_PATTERN *MtlPat, CHAR *FileName );

  /* Load g3dm model transformation matrix set function.
   * ARGUMENTS:
   *   - transform matrix:
   *       MATR Transform;
   * RETURNS: None.
   */
  VOID (*ModelLoadG3DMSetTransform)( MATR Transform );

  /* Model set primitive transformation function.
   * ARGUMENTS:
   *   - model pointer:
   *       twrMODEL *Mdl;
   *   - prim index:
   *       INT PrimNo;
   *   - transform matrix:
   *       MATR Trans;
   * RETURNS: None.
   */
  VOID (*ModelPrimSetTrans)( twrMODEL *Mdl, INT PrimNo, MATR Trans );

  /* Create model material pattern function.
   * ARGUMENTS: None.
   * RETURNS:
   *   (twrMATERIAL_PATTERN *) pointer to material pattern.
   */
  twrMATERIAL_PATTERN * (*ModelMtlPatCreate)( VOID );

  /* Model draw function.
   * ARGUMENTS:
   *   - model pointer:
   *       twrMODEL *Mdl;
   *   - world matrix:
   *       MATR World;
   * RETURNS: None.
   */
  VOID (*ModelDraw)( twrMODEL *Mdl, MATR World );

  /* Model eval bound box function.
   * ARGUMENTS:
   *   - model pointer:
   *       twrMODEL *Mdl;
   * RETURNS: None.
   */
  VOID (*ModelEvalBB)( twrMODEL *Mdl );
} twrSYSTEM_MODELS;

/* Set models deletion strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_ModelDeleteStrategy( twrMODEL *Mdl );

/* Models system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemModelsInit( VOID );

/* Models system close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemModelsClose( VOID );

#endif /* __twr_res_models_h_ */

/* END OF 'twr_res_models.h' FILE */
