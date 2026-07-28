/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_prim.h
 * PURPOSE     : Tough Water Rendering project.
 *               Primitives declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_prim_h_
#define __twr_res_prim_h_

#include "twr_def.h"

/* Primitive type */
typedef enum tagtwrPRIM_TYPE
{
  TWR_RND_PRIM_POINTS,          /* Array of points –                     GL_POINTS */
  TWR_RND_PRIM_LINES,           /* Line segments (by 2 points) –         GL_LINES */
  TWR_RND_PRIM_TRIMESH,         /* Triangle mesh - array of triangles –  GL_TRIANGLES */
  TWR_RND_PRIM_TRISTRIP,        /* Triangle strip - strip of triangles - GL_TRIANGLE_STRIP */
  TWR_RND_PRIM_PATCH,           /* Patches - patchpoints -               GL_PATCHES */
} twrPRIM_TYPE;

/* Primitive representation type */
typedef struct tagtwrPRIM twrPRIM;
struct tagtwrPRIM
{
  twrPRIM_TYPE Type;     /* Primitive type */
  INT
    VA,                  /* Vertex array Id */
    VBuf,                /* Vertex buffer Id */
    IBuf;                /* Index buffer Id (if 0 - use only vertex buffer) */
  twrMATERIAL *Mtl;      /* Pointer to primitive's material */
  INT NumOfElements;     /* Number of indices/vertices */
  INT NumOfPatchPoints;  /* Number of patch points */
  INT NumOfInstances;    /* Counter for instancing, 0 - not use */
  INT NumOfTriangles;    /* Number of triangles in primitive */
  VEC3 MinBB, MaxBB;     /* Bound box edge points */
  twrPRIM *NextDraw;     /* Next primitive in queue to be drawn */
  MATR *WorldMatrixes;   /* Buffer that contains matrices of every instance of this primitive */
  INT WorldMatrixesSize; /* Current matrix array size */
  INT FirstMatrixIndex;  /* Index of first world matrix of this primitive in global matrix array */
  UINT64 StoreFrameNo;   /* Number of frame when primitive was created */
};

/* Primitive handle system */
typedef struct tagtwrSYSTEM_PRIMS
{
  /* Pointers to primitive functions */

  /* Create primitive function.
   * ARGUMENTS:
   *   - pointer to primitive material:
   *       twrMATERIAL *Mtl;
   *   - primitive type:
   *       twrPRIM_TYPE Type;
   *   - vertex attributes array:
   *       VOID *V;
   *   - vertex attributes array size:
   *       INT NoofV;
   *   - primitive vertex index array:
   *       INT *Ind;
   *   - primitive vertex index array size:
   *       INT NoofI;
   * RETURNS:
   *   (twrPRIM) pointer to created primitive.
   */
  twrPRIM * (*PrimCreate)( twrMATERIAL *Mtl, twrPRIM_TYPE Type,
                           VOID *V, INT NoofV, INT *Ind, INT NoofI );

  /* Free primitive function.
   * ARGUMENTS:
   *   - Pointer to primitive:
   *       twrPRIM *Pr;
   * RETURNS: None.
   */
  VOID (*PrimFree)( twrPRIM *Pr );
  
  /* Set primitive delete strategy function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*DeleteStrategyPrim)( twrPRIM *Pr );

  /* Draw primtive function.
   * ARGUMENTS:
   *   - Pointer to primitive:
   *       twrPRIM *Pr;
   *   - world transform matrix:
   *       MATR WorldMatr;
   * RETURNS: None.
   */
  VOID (*PrimDraw)( twrPRIM *Pr, MATR WorldMatr );

  /* Draw primtive at final pass function.
   * ARGUMENTS:
   *   - Pointer to primitive:
   *       twrPRIM *Pr;
   *   - world transform matrix:
   *       MATR WorldMatr;
   * RETURNS: None.
   */
  VOID (*PrimDrawFinal)( twrPRIM *Pr, MATR WorldMatr );

  /* Instant draw primtive function.
   * ARGUMENTS:
   *   - pointer to primitive:
   *       twrPRIM *Pr;
   * RETURNS: None.
   */
  VOID (*PrimDrawInstant)( twrPRIM *Pr );

  /* Primitive bound box obtaining function.
   * ARGUMENTS:
   *   - pointer to result min-max vectors:
   *       VEC *MinBB, *MaxBB;
   *   - pointer to vertex attributes array:
   *       at7VERTEX *V;
   *   - vertex attributes array size:
   *       NoofV;
   *   - material pattern with vertex attribs:
   *       twrMATERIAL_PATTERN *MtlPat;
   * RETURNS: None.
   */
  VOID (*PrimEvalBB)( VEC3 *MinBB, VEC3 *MaxBB, VOID *V, INT NoofV, twrMATERIAL_PATTERN *MtlPat );
  /* VOID (*PrimBindMaterial)( twrPRIM *Pr, twrMATERIAL *mtl ); */
} twrSYSTEM_PRIMS;

/* Primitives system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPrimsInit( VOID );

/* Primitive system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPrimsClose( VOID );

#endif /* __twr_res_prim_h_ */

/* END OF 'twr_res_prim.h' FILE */
