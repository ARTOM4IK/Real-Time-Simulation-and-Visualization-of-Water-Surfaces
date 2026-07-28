/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_markers.c
 * PURPOSE     : Tough Water Rendering project.
 *             : Markers handle module.
 * PROGRAMMER  : CGSG'2024.
 *               Porotikov Valery (VP7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#define TWR_FORWARD_ALLOCATION_MATRS 10000000

/***
 * Markers variables
 ***/

static twrMARKER 
         *TWR_Markers[2][4];                           /* Markers array */
static INT                                 
         TWR_MrkMaxSize[2][4],                         /* Max size array markers now */
         TWR_MrkSize[2][4],                            /* Array number markers */
         TWR_MrkBufferSize[2][4];                      /* Size buffers */
         TWR_MrkMatrStartNum[2][4];                    /* Number of starting marker matrixes in global matrixes array*/
static twrBUFFER *TWR_MrkBuffers[2][4];                /* Array point to markers buffer */
static twrSHADER *TWR_MrkShaders[4];                   /* Array point to markers shaders */
static twrMATERIAL *TWR_MrkMaterial[2][4];             /* Array point to markers material */
static twrPRIM *TWR_MrkPrims[2][4];                    /* Array point to markers primitives */
static twrMATERIAL_PATTERN *TWR_MrkMaterialPattern[4]; /* Array point to markers material pattern */

/***
 * Markers function
 ***/

/* Draw marker function.
 * ARGUMENTS:
 *   - type markers:
 *       INT MarkerType;
 *   - cylinder start point:
 *         Marker data:
 *           sphere:   P0.xyz - center position, P1.xyz - radiuses
 *           cylinder: P0.xyz - first point center, P0.w - radius,
 *                     P1.xyz - second point center, P1.w - radius
 *           torus:    P0.xyz - center position,
 *                     P1.xy - outher and inner radiuses
 *           box:      P0.xyz - first corner (min) position,
 *                     P1.xyz - second corner (max) position.
 *       VEC4 P0, P1;
 *   - cylinder color:
 *       VEC4 Color;
 *   - stransform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_MrkAddToDraw( INT MarkerType, VEC4 P0, VEC4 P1, VEC4 Color, MATR Transform )
{
  twrMARKER NewElement;
  INT ot = Color.W != 1, ArrSize;

  /* Initialization new element */
  NewElement.Color = Color;
  NewElement.P0 = P0;
  NewElement.P1 = P1;
  NewElement.Transform = Transform;

  if ((ArrSize = TWR_MrkSize[ot][MarkerType]) >= TWR_MrkMaxSize[ot][MarkerType])
  {
    /* Allocate a block with new size */
    twrMARKER *block =
      malloc(sizeof(twrMARKER) * (TWR_MrkSize[ot][MarkerType] +
        TWR_MARKER_FORWARD_ALLOCATION));

    /* Check for memory allocation error */
    if (block == NULL)
      return;
    if (TWR_Markers[ot][MarkerType] != NULL)
      /* Copy markers matrix array in block */
      memcpy(block, TWR_Markers[ot][MarkerType],
        sizeof(twrMARKER) * TWR_MrkSize[ot][MarkerType]);
    /* free of matrix array markers */
    free(TWR_Markers[ot][MarkerType]);
    /* block new matrix array markers */
    TWR_Markers[ot][MarkerType] = block;
    TWR_MrkMaxSize[ot][MarkerType] += TWR_MARKER_FORWARD_ALLOCATION;
    Twr->BufResize(TWR_MrkBuffers[ot][MarkerType], TWR_MrkMaxSize[ot][MarkerType] * sizeof(twrMARKER));
  }
  /* Add new element */
  TWR_Markers[ot][MarkerType][ArrSize] = NewElement;
  TWR_MrkSize[ot][MarkerType]++;
} /* End of 'TWR_MrkAddToDraw' function */

/* Markers buffer resize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_MrkBufferResize( VOID )
{
  INT i, j; /* Counters */
  
  for(i = 0; i < 2; i++)
    for(j = 0; j < 4; j++)
      if (TWR_MrkMaxSize[i][j] > TWR_MrkBufferSize[i][j])
        /* Markers buffer data resize */
        Twr->BufResize(TWR_MrkBuffers[i][j], TWR_MrkMaxSize[i][j]);
}  /* End of 'TWR_MrkResizeArray function. */

/* Draw sphere function.
 * ARGUMENTS:
 *   - sphere center point:
 *       VEC3 C;
 *   - sphere (ellipsoid) radiuses:
 *       VEC3 R;
 *   - sphere color:
 *       VEC3 Color;
 *   - transform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_MrkDrawSphere( VEC3 C, VEC3 R, VEC4 Color, MATR Transform )
{
  TWR_MrkAddToDraw(TWR_MARKER_STORE_SPHERE, Vec4SetVec3(C, 0), Vec4SetVec3(R, 0), Color, Transform);
} /* End of 'TWR_MrkDrawSphere' function */

/* Draw cylinder function.
 * ARGUMENTS:
 *   - cylinder start point:
 *       VEC3 P0;
 *   - cylinder start point radius:
 *       FLT R0;
 *   - cylinder end point:
 *       VEC3 P1;
 *   - cylinder end point radius:
 *       FLT R1;
 *   - cylinder color:
 *       VEC3 Color;
 *   - stransform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_MrkDrawCylinder( VEC3 P0, FLT R0, VEC3 P1, FLT R1, VEC4 Color,
                                 MATR Transform )
{
  TWR_MrkAddToDraw(TWR_MARKER_STORE_CYLINDER,
    Vec4SetVec3(P0, R0), Vec4SetVec3(P1, R1), Color, Transform);
} /* End of 'TWR_MrkDrawCylinder' function */

/* Draw torus function.
 * ARGUMENTS:
 *   - torus center point:
 *       VEC3 C;
 *   - torus (external and internal) radiuses:
 *       VEC2 R;
 *   - torus color:
 *       VEC4 Color;
 *   - transform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_MrkDrawTorus( VEC3 C, VEC2 R, VEC4 Color, MATR Transform )
{
  TWR_MrkAddToDraw(TWR_MARKER_STORE_TORUS,
    Vec4SetVec3(C, 0), Vec4Set(R.X, R.Y, 0, 0), Color, Transform);
} /* End of 'TWR_MrkDrawTorus' function */

/* Draw trans box function.
 * ARGUMENTS:
 *   - box points:
 *       VEC3 P0, P1.
 *   - cylinder color:
 *       VEC4 Color;
 *   - transform matrix:
 *       MATR Transform;
 * RETURNS: None.
 */
static VOID TWR_MrkDrawBox( VEC3 P0, VEC3 P1, VEC4 Color,
                            MATR Transform )
{                                                   
  TWR_MrkAddToDraw(TWR_MARKER_STORE_BOX,
    Vec4SetVec3(P0, 0), Vec4SetVec3(P1, 0), Color, Transform);
} /* End of 'TWR_MrkDrawBox' function */

/* Markers buffer update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_MrkBufUpdate( INT OpaqueTrans )
{
  INT i;

  /* Markers buffer data update */
  for (i = 0; i < 4; i++)
    Twr->BufUpdate(TWR_MrkBuffers[OpaqueTrans][i], 0, sizeof(twrMARKER) * TWR_MrkSize[OpaqueTrans][i], TWR_Markers[OpaqueTrans][i]);
} /* End of 'TWR_MrkBufUpdate' function */

/* Draw markers function.
 * ARGUMENTS:
 *   - transparency flag:
 *       INT OpaqueTrans;
 * RETURNS: None.
 */
static VOID TWR_MrkDrawAll( INT OpaqueTrans )
{
  INT i; 

  /* Buffer marker array update */
  Twr->MrkBufUpdate(OpaqueTrans);

  TWR_ProfilerAddText("Markers: sphere    %u", TWR_MrkSize[OpaqueTrans][TWR_MARKER_STORE_SPHERE]);
  TWR_ProfilerAddText("Markers: cylinders %u", TWR_MrkSize[OpaqueTrans][TWR_MARKER_STORE_CYLINDER]);
  TWR_ProfilerAddText("Markers: box       %u", TWR_MrkSize[OpaqueTrans][TWR_MARKER_STORE_BOX]);
  TWR_ProfilerAddText("Markers: torus     %u", TWR_MrkSize[OpaqueTrans][TWR_MARKER_STORE_TORUS]);

  Twr->ToggleBlend(TWR_BLEND_ALPHA);
  for (i = 0; i < 4; i++)
  {
    if (TWR_MrkSize[OpaqueTrans][i] != 0 && Twr->ShdApply(TWR_MrkMaterial[OpaqueTrans][i]->MtlPat->Shd, OpaqueTrans))
    {
      glUniform1i(2, TWR_MrkMatrStartNum[OpaqueTrans][i]);
      Twr->MtlApply(TWR_MrkMaterial[OpaqueTrans][i]);
      TWR_MrkPrims[OpaqueTrans][i]->NumOfInstances = TWR_MrkSize[OpaqueTrans][i];
      Twr->PrimDrawInstant(TWR_MrkPrims[OpaqueTrans][i]);
      TWR_MrkSize[OpaqueTrans][i] = 0;
    }
  }
} /* End of 'TWR_MrkDrawAll' function */

/* Add marker matrixes to global matrixes array function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_MrkMatrixesToGlobalMatrixesArray( VOID )
{
  INT i, ot, num;

  for (ot = 0; ot < 2; ot++)
    for (num = 0; num < 4; num++)
    {
      TWR_MrkMatrStartNum[ot][num] = Twr->NumOfGlobalMatrixes;
      for (i = 0; i < TWR_MrkSize[ot][num]; i++)
        {
          if (Twr->NumOfGlobalMatrixes >= Twr->MaxNumOfGlobalMatrixes)
          {
            MATR *block;
            
            block = malloc(sizeof(MATR) * (Twr->MaxNumOfGlobalMatrixes + TWR_FORWARD_ALLOCATION_MATRS));
            if (block == NULL)
              return;
            if (Twr->GlobalMatrixes != NULL)
              memcpy(block, Twr->GlobalMatrixes, sizeof(MATR) * Twr->NumOfGlobalMatrixes); 
            free(Twr->GlobalMatrixes);
            Twr->GlobalMatrixes = block;
            Twr->MaxNumOfGlobalMatrixes += TWR_FORWARD_ALLOCATION_MATRS;
          }
          Twr->GlobalMatrixes[Twr->NumOfGlobalMatrixes] = TWR_Markers[ot][num][i].Transform;
          Twr->NumOfGlobalMatrixes++;
        }
    }
} /* End of 'TWR_MrkMatrixesToGlobalMatrixesArray' function */

/* Marker system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMrkInit( VOID )
{
  INT i, j;              /* Counters */
  CHAR 
    *shader_name[4],     /* Array name shaders */
    *mtl_name[2][4],     /* Array name materials */
    *mtl_pattern_name[4]; /* Array name material patterns */

  /* Initializatopn name shaders */
  shader_name[TWR_MARKER_STORE_SPHERE] = "marker/sphere";
  shader_name[TWR_MARKER_STORE_CYLINDER] = "marker/cylinder";
  shader_name[TWR_MARKER_STORE_TORUS] = "marker/torus";
  shader_name[TWR_MARKER_STORE_BOX] = "marker/box";

  /* Initialization name matirials */
  mtl_name[TWR_MARKER_OPAQUE][TWR_MARKER_STORE_SPHERE] = "Marker opaque sphere";
  mtl_name[TWR_MARKER_TRANSPARENT][TWR_MARKER_STORE_SPHERE] = "Marker transparant sphere";
  mtl_name[TWR_MARKER_OPAQUE][TWR_MARKER_STORE_CYLINDER] = "Marker opaque cylinder";
  mtl_name[TWR_MARKER_TRANSPARENT][TWR_MARKER_STORE_CYLINDER] = "Marker transperent cylinder";
  mtl_name[TWR_MARKER_OPAQUE][TWR_MARKER_STORE_TORUS] = "Marker opaque torus";
  mtl_name[TWR_MARKER_TRANSPARENT][TWR_MARKER_STORE_TORUS] = "Marker transperent torus";
  mtl_name[TWR_MARKER_OPAQUE][TWR_MARKER_STORE_BOX] = "Marker opaque box";
  mtl_name[TWR_MARKER_TRANSPARENT][TWR_MARKER_STORE_BOX] = "Marker transperent box";

  /* Initialization name materials pattern */
  mtl_pattern_name[TWR_MARKER_STORE_SPHERE] = "Marker sphere";
  mtl_pattern_name[TWR_MARKER_STORE_CYLINDER] = "Marker cylinder";
  mtl_pattern_name[TWR_MARKER_STORE_TORUS] = "Marker torus";
  mtl_pattern_name[TWR_MARKER_STORE_BOX] = "Marker box";

  /* Markers function initialization */
  Twr->MrkDrawSphere = TWR_MrkDrawSphere;
  Twr->MrkDrawCylinder = TWR_MrkDrawCylinder;
  Twr->MrkDrawBox = TWR_MrkDrawBox;
  Twr->MrkDrawTorus = TWR_MrkDrawTorus;
  Twr->MrkBufUpdate = TWR_MrkBufUpdate;
  Twr->MrkDrawAll = TWR_MrkDrawAll;
  Twr->MrkBufferResize = TWR_MrkBufferResize;
  Twr->MrkMatrixesToGlobalMatrixesArray = TWR_MrkMatrixesToGlobalMatrixesArray;

  /* Initialization markers size array and size markers array matrix */
  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
    {
      TWR_MrkSize[i][j] = 0;
      TWR_MrkMaxSize[i][j] = 0;
    }

  for (i = 0; i < 4; i++)
  {    
    /* Create markers shader */
    TWR_MrkShaders[i] = Twr->ShdCreate(shader_name[i]);

    /* Create material pattern */
    TWR_MrkMaterialPattern[i] =
      Twr->MtlPatCreate(mtl_pattern_name[i], shader_name[i], 0, 0, sizeof(twrMARKER), NULL, 0);
  }

  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
    {
      /* Create markers buffer */
      TWR_MrkBuffers[i][j] = Twr->BufCreate(mtl_name[i][j], TWR_BUF_BIND_MARKER, 0, NULL);
      Twr->BufReferenceInc(TWR_MrkBuffers[i][j]);

      /* Create shaders material */
      TWR_MrkMaterial[i][j] =
        Twr->MtlCreate(mtl_name[i][j], TWR_MrkMaterialPattern[j], i);
    
      /* Bind buffer markers to the material markers */
      Twr->MtlBindBuf(TWR_MrkMaterial[i][j], TWR_MrkBuffers[i][j]);

      /* Create primitives markers */
      if (j != TWR_MARKER_STORE_BOX)
      {
        TWR_MrkPrims[i][j] =
          Twr->PrimCreate(TWR_MrkMaterial[i][j],
                          TWR_RND_PRIM_PATCH, NULL, 1, NULL, 0);

        TWR_MrkPrims[i][j]->NumOfPatchPoints = 1;
      }
      else
        TWR_MrkPrims[i][j] = 
          Twr->PrimCreate(TWR_MrkMaterial[i][j],
                          TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
    }
  Twr->Sys("Marker system is initialized");
} /* End of 'TWR_SystemMrkInit' function */

/* Marker system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMrkClose( VOID )
{
  INT i, j;

  /* Free markers buffer */
  for (i = 0; i < 2; i++)
    for (j = 0; j < 4; j++)
    {
      /* Free primitives markers */
      Twr->PrimFree(TWR_MrkPrims[i][j]);

      /* Free material patern markers */
      Twr->MtlFree(TWR_MrkMaterial[i][j]);

      /* Free bufers markers */
      Twr->BufFree(TWR_MrkBuffers[i][j]);

      /* Free array markers */
      free(TWR_Markers[i][j]);
    }

  /* Free markers shader array */
  for (i = 0; i < 4; i++)
  {
    /* Free shaders markers */
    Twr->ShdFree(TWR_MrkShaders[i]);

    /* Free materials patern markers */
    Twr->MtlPatFree(TWR_MrkMaterialPattern[i]);
  }

  /* Send console a message about successful deinitialization */
  Twr->Sys("Marker system is deinitialized");
} /* End of 'TWR_RndMarkerClose' function */

/* END OF 'twr_rnd_markers.c' FILE */
