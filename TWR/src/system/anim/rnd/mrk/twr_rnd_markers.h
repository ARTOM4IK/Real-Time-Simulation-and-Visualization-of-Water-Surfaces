/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_markers.h
 * PURPOSE     : Tough Water Rendering project.
 *               Marker handle declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Porotikov Valery (VP7),
 * LAST UPDATE : 25.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_markers_h_
#define __twr_rnd_markers_h_

#include "twr_def.h"

/*** 
 * Markers structs
 ***/

typedef struct tagtwrMARKER
{
  VEC4 Color;     /* Marker color. */
  VEC4 P0, P1;    /* Marker data:
                   *   sphere: P0.xyz - center position, P1.xyz - radiuses
                   *   cylinder: P0.xyz - first point center, P0.w - radius,
                   *             P1.xyz - second point center, P1.w - radius
                   *   torus:    P0.xyz - center position,
                   *             P1.xy - outher and inner radiuses
                   *   box:      P0.xyz - first corner (min) position,
                   *             P1.xyz - second corner (max) position.
                   */
  MATR Transform; /* Matrix transform. */
} twrMARKER;

/* buffers system handle type */
typedef struct tagtwrSYSTEM_RENDER_MARKERS
{
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
  VOID (*MrkDrawSphere)( VEC3 C, VEC3 R, VEC4 Color, MATR Transform );

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
  VOID (*MrkDrawCylinder)( VEC3 P0, FLT R0, VEC3 P1, FLT R1, VEC4 Color,
                                  MATR Transform );
  
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
  VOID (*MrkDrawTorus)( VEC3 C, VEC2 R, VEC4 Color, MATR Transform );
  
  /* Draw Trans rectangular paralelipid function.
   * ARGUMENTS:
   *   - rectangular paralelipid points:
   *       VEC3 P0, P1.
   *   - cylinder color:
   *       VEC4 Color;
   *   - stransform matrix:
   *       MATR Transform;
   * RETURNS: None.
   */
  VOID (*MrkDrawBox)( VEC3 P0, VEC3 P1, VEC4 Color,
                             MATR Transform );
  
  /* Markers buffer resize function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
   VOID (*MrkBufferResize)( VOID );

  /* Markers buffer update function.
   * ARGUMENTS:
   *   - transparency flag:
   *       INT OpaqueTrans;
   * RETURNS: None.
   */
  VOID (*MrkBufUpdate)( INT OpaqueTrans );

  /* Draw markers function.
   * ARGUMENTS:
   *   - transparency flag:
   *       INT OpaqueTrans;
   * RETURNS: None.
   */
  VOID (*MrkDrawAll)( INT OpaqueTrans );

  /* Add marker matrixes to global matrixes array function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*MrkMatrixesToGlobalMatrixesArray)( VOID );


} twrSYSTEM_RENDER_MARKERS;


/***
 * Markers derfine
 ***/

/* Marker is a opaque */
#define TWR_MARKER_OPAQUE         0
/* Marker is a transparent */
#define TWR_MARKER_TRANSPARENT    1
/* Number sphere */
#define TWR_MARKER_STORE_SPHERE   0
/* Number cylinder */
#define TWR_MARKER_STORE_CYLINDER 1
/* Number torus */
#define TWR_MARKER_STORE_TORUS    2
/* Number box */
#define TWR_MARKER_STORE_BOX      3
/* Number of markers when adding */
#define TWR_MARKER_FORWARD_ALLOCATION 100000

/***
 * Markers handle function
 ***/

/* Marker system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMrkInit( VOID );

/* Marker system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemMrkClose( VOID );

#endif /* __twr_rnd_markers_h_ */

/* END OF 'twr_rnd_markers.h' FILE */
  
