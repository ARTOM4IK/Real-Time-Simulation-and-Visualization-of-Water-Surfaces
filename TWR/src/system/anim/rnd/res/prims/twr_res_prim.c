/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_prim.c
 * PURPOSE     : Tough Water Rendering project.
 *               Primitive handle system implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 19.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Stock of primitives */
static STOCK TWR_StkPrims;

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
static twrPRIM * TWR_PrimCreate( twrMATERIAL *Mtl, twrPRIM_TYPE Type,
                                 VOID *V, INT NoofV, INT *Ind, INT NoofI )
{
  INT i;
  twrPRIM *Pr;

  if ((Pr = StockAlloc(&TWR_StkPrims)) == NULL)
    return NULL;

  Twr->Log("\nCreate prim: v:%d, i:%d", NoofV, NoofI); 

  memset(Pr, 0, sizeof(twrPRIM));
  Pr->Type = Type;

  glGenVertexArrays(1, &Pr->VA);
  /* Vertex data */
  if (V != NULL && NoofV != 0)
  {
    glGenBuffers(1, &Pr->VBuf);
    glBindVertexArray(Pr->VA);

    /* Setd vertex data to buffer  */
    glBindBuffer(GL_ARRAY_BUFFER, Pr->VBuf);
    glBufferData(GL_ARRAY_BUFFER, Mtl->MtlPat->VertexStride * NoofV, V, GL_STATIC_DRAW);

    for (i = 0; i < Mtl->MtlPat->NumOfAttributes; i++)
    {
      glVertexAttribPointer(i,
        Mtl->MtlPat->Attrs[i].NumOfComponents,
        Mtl->MtlPat->Attrs[i].IsFloat ? GL_FLOAT : GL_INT,
        FALSE,
        Mtl->MtlPat->VertexStride,
        (VOID *)Mtl->MtlPat->Attrs[i].Offset);
      glEnableVertexAttribArray(i);
    }
    glBindVertexArray(0);
    Twr->PrimEvalBB(&Pr->MinBB, &Pr->MaxBB, V, NoofV, Mtl->MtlPat);
  }
  /* Index data */
  if (Ind != NULL && NoofI != 0)
  {
    Pr->NumOfElements = NoofI;
    glGenBuffers(1, &Pr->IBuf);
    /* Enable buffer */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
    /* Put data into memeory (NumOfI - indicies count, Ind - index array) */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INT) * NoofI, Ind, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    Pr->NumOfTriangles = Type == TWR_RND_PRIM_TRIMESH ? NoofI / 3 : Type == TWR_RND_PRIM_TRISTRIP ? NoofI / 2 - 1 : 0;
  }
  else
  {
    Pr->NumOfElements = NoofV;
    Pr->NumOfTriangles = Type == TWR_RND_PRIM_TRIMESH ? NoofV / 3 : 0;
  }

  Pr->NextDraw = NULL;
  Pr->NumOfInstances = 0;
  Pr->StoreFrameNo = 0;
  Pr->Mtl = Mtl;
  Twr->MtlReferenceInc(Mtl);
  /* Twr->Log("\nLoaded primitive - %d vertices, %d indices, %d triangles\n", NoofV, Pr->NumOfElements, Pr->NumOfTriangles); */
  return Pr;
} /* End of 'TWR_PrimCreate' function */


/* Memory allocation constant for flex array of matrix */
#define TWR_FORWARD_ALLOCATION_MATRS 300

/* Free primitive function.
 * ARGUMENTS:
 *   - Pointer to primitive:
 *       twrPRIM *Pr;
 * RETURNS: None.
 */
static VOID TWR_PrimFree( twrPRIM *Pr )
{
  if (Pr == NULL)
    return;
  Twr->MtlFree(Pr->Mtl);
  StockDelete(Pr);
} /* End of 'TWR_PrimFree' function */

/* Set primitive delete strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_DeleteStrategyPrim( twrPRIM *Pr )
{
  if (Pr == NULL)
    return;

  Twr->Log("Delete prim:");
  Twr->SetColor(0xFFFF00, 0);
  Twr->Log(" %p VA: %d (NOE: %d)", Pr, Pr->VA, Pr->NumOfElements);
  Twr->SetColor(0xFFFFFF, 0);
  Twr->Log("\n");
  
  glBindVertexArray(Pr->VA);
  /* Disconnect buffer */
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &Pr->VBuf);
  glDeleteBuffers(1, &Pr->IBuf);
  /* Deactivete vertex array */
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &Pr->VA);
  free(Pr->WorldMatrixes);
  memset(Pr, 0, sizeof(twrPRIM));
  Pr->VA = -102;
} /* End of 'TWR_DeleteStrategyPrim' function */

/* Draw primtive function.
 * ARGUMENTS:
 *   - Pointer to primitive:
 *       twrPRIM *Pr;
 *   - world transform matrix:
 *       MATR WorldMatr;
 * RETURNS: None.
 */
static VOID TWR_PrimDraw( twrPRIM *Pr, MATR WorldMatr )
{
  twrMATERIAL_PATTERN *MtlPat;
  twrMATERIAL *Mtl;

  /* Check for a new frame */
  if (Pr->StoreFrameNo != Twr->CurrentFrameNo)
  {
    Pr->NumOfInstances = 0;
    Pr->StoreFrameNo = Twr->CurrentFrameNo;
  }
  /* Store instance matrix */
  if (Pr->NumOfInstances >= Pr->WorldMatrixesSize)
  {
    /* Allocate a block with new size */
    MATR *block = malloc(sizeof(MATR) * (Pr->NumOfInstances + TWR_FORWARD_ALLOCATION_MATRS));

    if (block == NULL)
      return; /* No memory! */
    if (Pr->WorldMatrixes != NULL)
      memcpy(block, Pr->WorldMatrixes, sizeof(MATR) * Pr->NumOfInstances);
    free(Pr->WorldMatrixes);
    Pr->WorldMatrixes = block;
    block = NULL;
    Pr->WorldMatrixesSize = Pr->NumOfInstances + TWR_FORWARD_ALLOCATION_MATRS;
  }
  Pr->WorldMatrixes[Pr->NumOfInstances++] = WorldMatr;

  /* Add primitive to material primitive draw list */
  if (Pr->NumOfInstances == 1)
  {
    Mtl = Pr->Mtl;
    if (Mtl->StoreFrameNo != Twr->CurrentFrameNo)
    {
      Pr->NextDraw = NULL;
      Mtl->StoreFrameNo = Twr->CurrentFrameNo;

      /* Add material to material patern material draw list */
      MtlPat = Mtl->MtlPat;
      if (MtlPat->StoreFrameNo != Twr->CurrentFrameNo)
      {
        Mtl->NextDraw = NULL;
        MtlPat->StoreFrameNo = Twr->CurrentFrameNo;

        /* Add material patern to global material pattern draw list */
        if (Twr->StoreFrameNo != Twr->CurrentFrameNo)
        {
          MtlPat->NextDraw = NULL;
          Twr->StoreFrameNo = Twr->CurrentFrameNo;
        }
        else
          MtlPat->NextDraw = Twr->FirstDraw;
        Twr->FirstDraw = MtlPat;
      }
      else
        Mtl->NextDraw = MtlPat->FirstDraw;
      MtlPat->FirstDraw = Mtl;
    }
    else
      Pr->NextDraw = Mtl->FirstDraw;
    Mtl->FirstDraw = Pr;
  }
} /* End of 'TWR_PrimDraw' function */

/* Instant draw primtive function.
 * ARGUMENTS:
 *   - Pointer to primitive:
 *       twrPRIM *Pr;
 * RETURNS: None.
 */
static VOID TWR_PrimDrawInstant( twrPRIM *Pr )
{
  INT
    gl_prim_type =
      Pr->Type == TWR_RND_PRIM_TRIMESH ? GL_TRIANGLES :
      Pr->Type == TWR_RND_PRIM_LINES ? GL_LINE_STRIP :
      Pr->Type == TWR_RND_PRIM_TRISTRIP ? GL_TRIANGLE_STRIP:
      Pr->Type == TWR_RND_PRIM_PATCH ? GL_PATCHES:
      GL_POINTS;

  /* Pass render uniforms */
  glUniform1i(0, Pr->FirstMatrixIndex);

  if (gl_prim_type == GL_PATCHES)
    glPatchParameteri(GL_PATCH_VERTICES, Pr->NumOfPatchPoints);

  glBindVertexArray(Pr->VA);
  if (Pr->NumOfInstances < 2)
  {
    if (Pr->IBuf == 0)
      glDrawArrays(gl_prim_type, 0, Pr->NumOfElements);
    else
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElements(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  }
  else
  {
    if (Pr->IBuf != 0)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Pr->IBuf);
      glDrawElementsInstanced(gl_prim_type, Pr->NumOfElements, GL_UNSIGNED_INT, NULL,
        Pr->NumOfInstances);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
      glDrawArraysInstanced(gl_prim_type, 0, Pr->NumOfElements, Pr->NumOfInstances);
  }
  glBindVertexArray(0);
  Twr->TrianglesCount += Pr->NumOfInstances * Pr->NumOfTriangles;
} /* End of 'TWR_PrimDrawInstant' function */

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
static VOID TWR_PrimEvalBB( VEC3 *MinBB, VEC3 *MaxBB, VOID *V, INT NoofV, twrMATERIAL_PATTERN *MtlPat )
{
  BYTE *ptr = (BYTE *)V;
  INT i, Stride = MtlPat->VertexStride;

  if (V == NULL || NoofV == 0)
    return;
  *MinBB = *MaxBB = *(VEC3 *)ptr;
  ptr += Stride;
  for (i = 1; i < NoofV; i++)
  {
    if (MinBB->X > *(FLT *)ptr)
      MinBB->X = *(FLT *)ptr;
    else if (MaxBB->X < *(FLT *)ptr)
      MaxBB->X = *(FLT *)ptr;

    ptr += sizeof(FLT);
    if (MinBB->Y > *(FLT *)ptr)
      MinBB->Y = *(FLT *)ptr;
    else if (MaxBB->Y < *(FLT *)ptr)
      MaxBB->Y = *(FLT *)ptr;

    ptr += sizeof(FLT);
    if (MinBB->Z > *(FLT *)ptr)
      MinBB->Z = *(FLT *)ptr;
    else if (MaxBB->Z < *(FLT *)ptr)
      MaxBB->Z = *(FLT *)ptr;
    ptr += Stride - sizeof(FLT) * 2;
  }
} /* End of 'TWR_RndPrimEvalBB' function */

/* Primitives system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPrimsInit( VOID )
{
  /* Set pointers to primitive functions */
  Twr->PrimDraw = TWR_PrimDraw;
  Twr->PrimDrawInstant = TWR_PrimDrawInstant;
  Twr->PrimCreate = TWR_PrimCreate;
  Twr->PrimFree = TWR_PrimFree;
  Twr->PrimEvalBB = TWR_PrimEvalBB;

  /* Create stock for primitives */
  StockCreate(&TWR_StkPrims, "Primitives", sizeof(twrPRIM),
              100, TWR_DeleteStrategyPrim);
  TWR_StockRegisterToStrip(&TWR_StkPrims);

  Twr->Sys("Primitives system is initialized");
} /* End of 'TWR_SystemPrimsInit' function */

/* Primitive system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPrimsClose( VOID )
{
  StockFree(&TWR_StkPrims);
  Twr->Sys("Primitives system is deinitialized");
} /* End of 'TWR_SystemPrimsClose' function */

/* END OF 'twr_res_prim.c' FILE */
