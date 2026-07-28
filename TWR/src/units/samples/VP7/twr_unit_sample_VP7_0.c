/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_sample_VP7_0.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit sample by VP7 implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Porotikov Valery (VP7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/* Sample unit description type */
typedef struct tagtwrUNIT_SAMPLE_VP7
{
  TWR_UNIT_BASE_FIELDS;
} twrUNIT_SAMPLE_VP7;

/* Store vertices of model */
typedef struct tagtwrMARKER_TEST
{
  INT NumOfVertexes;     /* Number of vertexes */
  VEC3 *V;               /* Array vertix */
} twrMARKER_TEST;


static twrMARKER_TEST TWR_MrkTest; /* Struct with coordinate and number points cow */
 
/* Get point from g3dm function.
 * ARGUMENTS:
 *   - File name:
 *       CHAR *FileName;
 * RETURNS:
 *   (twrMARKER_TEST *) points object array.
 */
static twrMARKER_TEST TWR_VertexFromG3dm( CHAR *FileName )
{
  FILE *F;                      /* Point to file */
  BYTE *ptr, *pnt;              /* Points to plase of file */
  INT
    flen,                       /* Len of file */
    NumOfVertex = 0,            /* Numbers of vertex */
    p, a, i = 0;                /* Counters */
  DWORD NumOfPrims;             /* Number of primitives in model */
  VEC3 *V;                      /* Array of vertices */
  twrMARKER_TEST points_object; /* Struct for return vertices */

  /* Check having of file */
  if ((F = fopen(FileName, "rb")) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, file not found");
    points_object.NumOfVertexes = 0;
    return points_object;
  }

  /* Obtain file length */
  fseek(F, 0, SEEK_END);
  flen = ftell(F);
  /* Allocate memory for whole file */
  if ((ptr = malloc(flen)) == NULL)
  {
    Twr->SetColor(0xFF8888, 0);
    Twr->Log(" - error, not enough memory for load file bulk");
    fclose(F);
    points_object.NumOfVertexes = 0;
    return points_object;
  }
  /* Load whole file to memory */
  rewind(F);
  fread(ptr, 1, flen, F);
  fclose(F);

  /* Swap */
  ptr += 4;
  NumOfPrims = *(DWORD *)ptr;
  ptr += 12;

  pnt = ptr;

  /* Finding the number of vertices */
  for (p = 0 ; p < NumOfPrims; p++)
  {
    DWORD NumOfVert;
    DWORD NumOfFacetIndexes;

    NumOfVert = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 8 + sizeof(twrVERTEX_STD4) * NumOfVert + sizeof(INT) * NumOfFacetIndexes;

    NumOfVertex += NumOfVert;
  }

  /* Return to vertices */
  ptr = pnt;
  /* Allocate memory for array vertices */
  if ((V = malloc(sizeof(VEC3) * NumOfVertex)) == NULL)
  {
    points_object.NumOfVertexes = 0;
    return points_object;
  }
  for (p = 0 ; p < NumOfPrims; p++)
  {
    twrVERTEX_STD4 *V4;
    DWORD NumOfVert;
    DWORD NumOfFacetIndexes;

    NumOfVert = *(DWORD *)ptr;
    ptr += 4;
    NumOfFacetIndexes = *(DWORD *)ptr;
    ptr += 8;
    /* Vertices in STD4 */
    V4 = (twrVERTEX_STD4 *)ptr;

    for (a = 0; a < NumOfVert; a++)
      V[i + a] = V4[a].P;
    i += a;
  }

  points_object.V = V;
  points_object.NumOfVertexes = NumOfVertex;

  return points_object;
} /* End of 'TWR_UnitInit' function */


/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VP7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_SAMPLE_VP7 *Uni )
{
  TWR_MrkTest = TWR_VertexFromG3dm("bin/models/cow.g3dm");
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VP7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_SAMPLE_VP7 *Uni )
{
} /* End of 'TWR_UnitClose'0 function */
 
/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VP7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_SAMPLE_VP7 *Uni )
{
} /* End of 'TWR_UnitResponse' function */
 
/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_SAMPLE_VP7 *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_SAMPLE_VP7 *Uni )
{
  /*
  INT i, k, j, s = Twr->Time + 5;
  for (i = -s; i < s; i++)
    for (k = -s; k < s; k++)
      for (j = -s; j < s; j++)
        Twr->MrkDrawSphere(Vec3Set1(0), Vec3Set1(0.2), Vec4Set(1, 0, 1, 1), MatrTranslate(Vec3Set(i, k, j)));
  */
  /*
  for (i = 0; i < TWR_MrkTest.NumOfVertexes; i++)
    Twr->MrkDrawSphere(TWR_MrkTest.V[i], Vec3Set1(0.1), Vec4Set(1, 0, 1, 1), MatrIdentity());
  */

  INT i, k, j, r, s = Twr->Time + 5;

  srand(30);
  for (i = -s; i < s; i++)
    for (k = -s; k < s; k++)
      for (j = -s; j < s; j++)
      {
        r = rand() & 3;
        switch (r)
        {
        case 0:
          Twr->MrkDrawSphere(Vec3Set(i, k, j), Vec3Set1(0.3), Vec4Set(1, 0, 1, 1), MatrIdentity());
          break;
        case 1:
          Twr->MrkDrawBox(Vec3Set(i + 0.25, k + 0.25, j + 0.25), Vec3Set(i + 0.75, k + 0.75, j + 0.75), Vec4Set(1, 0, 1, 1), MatrIdentity());
          break;
        case 2:
          Twr->MrkDrawCylinder(Vec3Set(i + 0.25, k + 0.25, j + 0.25), 0.8, Vec3Set(i + 0.75, k + 0.75, j + 0.75), 0.8, Vec4Set(1, 0, 1, 1), MatrIdentity());
          break;
        case 3:
          Twr->MrkDrawTorus(Vec3Set(i, k, j), Vec2Set(0.2, 0.1), Vec4Set(1, 0, 1, 1), MatrIdentity());
          break;
        }
      }
  /*
  FLT i, s = 100;

  for (i = 0; i < s; i++)
    Twr->MrkDrawSphere(Vec3Set1(0), Vec3Set1(0.3 + i * 5), Vec4SetVec3(Vec3Set1(i / s), 0.5), MatrIdentity());
  */
} /* End of 'TWR_UnitRender' function */

/* VP7 unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateSampleVP7( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_SAMPLE_VP7))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateSampleVP7' function */

/* END OF 'twr_unit_sample_VP7_0.c' FILE */
