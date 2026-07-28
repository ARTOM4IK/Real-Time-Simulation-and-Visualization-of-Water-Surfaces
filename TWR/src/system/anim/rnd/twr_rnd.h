/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd.h
 * PURPOSE     : Tough Water Rendering project.
 *               Render system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7),
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_h_
#define __twr_rnd_h_

#define GLEW_STATIC
#include <glew.h>

#include "res\twr_res.h"
#include "pipeline\twr_rnd_targets.h"
#include "pipeline\twr_rnd_pipeline.h"
#include "light\twr_rnd_lights.h"
#include "mrk\twr_rnd_markers.h"
#include "compute\twr_rnd_compute.h"

/*  */
typedef BOOL (WINAPI * PFNGLSWAPINTERVALEXTPROC)(INT Interval);

/* Render system structure */
typedef struct tagtwrSYSTEM_RENDER
{
  /* Resources system */
  union
  {
    twrSYSTEM_RENDER_RESOURCES;
    twrSYSTEM_RENDER_RESOURCES Resources;
  };

  /* Lights system */
  union
  {
    twrSYSTEM_RENDER_LIGHTS;
    twrSYSTEM_RENDER_LIGHTS Lights;
  };

  /* Markers system */
  union
  {
    twrSYSTEM_RENDER_MARKERS;
    twrSYSTEM_RENDER_MARKERS Markers;
  };

  /* Targets system */
  union
  {
    twrSYSTEM_TARGETS;
    twrSYSTEM_TARGETS Targets;
  };

  /* Rendering pipeline system */
  union
  {
    twrSYSTEM_PIPELINE;
    twrSYSTEM_PIPELINE pipeline;
  };

  /* Compute system */
  union
  {
    twrSYSTEM_RENDER_COMPUTE;
    twrSYSTEM_RENDER_COMPUTE Compute;
  };

  /* Render data */
  twrMATERIAL_PATTERN *FirstDraw; /* First material pattern to draw */
  UINT NumOfGlobalMatrixes,       /* Number of global primitive world matrixes */
       MaxNumOfGlobalMatrixes;    /* Current max size of global matrixes array */
  MATR *GlobalMatrixes;           /* Global matrixes array */
  BOOL IsDebug;             /* Is debug mode flag */
  UINT64 TrianglesCount;    /* Total number of triangles at current frame */
  UINT64 CurrentFrameNo;    /* Current frame number */
  UINT StoreFrameNo;        /* Frame number of first mtlpat store */
  HDC hRndDC;               /* Render device context handle */
  HGLRC hRndGLRC;           /* Rendering context handle */

  CAMERA Cam;               /* Current render camera */

  /* Buffers references */
  twrBUFFER
    *RndShdBufCamera, /* Camera buffer */
    *RndShdBufSync;   /* Sync data buffer */

  /* Pointers to render functions */

  /* Resize projection function.
   * ARGUMENTS:
   *   - width and height of window:
   *       INT W, H;
   * RETURNS: None.
   */
  VOID (*RndResize)( INT W, INT H );

  /* Copy frame from RndFrameDC into hDC function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndCopyFrame)( VOID );

  /* Start draw function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndStart)( VOID );

  /* End draw function function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*RndEnd)( VOID );

  /* Matrix look-at viewer setup function.
   * ARGUMENTS:
   *   - viewer position, look-at point, approximate up direction:
   *       VEC3 Loc, At, Up;
   * RETURNS: None.
   */
  VOID (*RndCamSet)( VEC3 Loc, VEC3 At, VEC3 Up );

} twrSYSTEM_RENDER;

/*** UBO common data types and variables ***/

/* Camera UBO data UBO representation trype (update once per camera/frame change) */
typedef struct tagtwrRND_UBO_CAMERA
{
  MATR MatrVP;              /* Premultiplied 'view' and 'projection' matrixes */
  MATR MatrV;               /* Matrix 'view' */
  MATR MatrP;               /* Matrix 'projection' */

  VEC4 CamLoc;              /* Camera location */
  VEC4 CamRight;            /* Camera right direction */
  VEC4 CamUp;               /* Camera up direction */
  VEC4 CamDir;              /* Camera forward direction */
  VEC4 CamAt;               /* Camera at position */
  VEC4 FrameWHProjSizeDist; /* Camera frame size, projection size and distance */
} twrRND_UBO_CAMERA;

/* Synchronization data UBO representation trype (update every frame) */
typedef struct tagtwrRND_UBO_SYNC
{
  VEC4 TimeDeltaTimeGlobalTimeDeltaTime; /* Time parameters */
  VEC4 IsWireFrameIsPauseFPS;            /* Wireframe and pause flags and FPS value */
} twrRND_UBO_SYNC;

/* Render system close function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
VOID TWR_SystemRenderClose( VOID );

/* Render system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
VOID TWR_SystemRenderInit( VOID );

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id, 
 *   - message severity:
 *       UINT severity, 
 *   - message text length:
 *       INT Length, 
 *   - message text:
 *       CHAR *Message, 
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY TWR_glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                 INT Length, const CHAR *Message,
                                 const VOID *UserParam );

#endif /* __twr_rnd_h_ */

/* END OF 'twr_rnd.h' FILE */
