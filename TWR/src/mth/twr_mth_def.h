/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_def.h
 * PURPOSE     : Tough Water Rendering project.
 *               Math representation types declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_def_h_
#define __twr_mth_def_h_

#include <math.h>

#include <commondf.h>

/* Base float point types */
typedef DOUBLE DBL;
typedef FLOAT FLT;

/* Angle transform constants */
#define PI 3.14159265358979323846
#define D2R(A) ((A) * (PI / 180.0))
#define R2D(A) ((A) * (180.0 / PI))

/* Vector representation types */
typedef struct tagVEC2
{
  FLT X, Y; /* Vector coordinates */
} VEC2;

typedef struct tagVEC3
{
  FLT X, Y, Z; /* Vector coordinates */
} VEC3;

typedef struct tagVEC4
{
  FLT X, Y, Z, W; /* Vector coordinates */
} VEC4;

/* Matrix 4x4 representation type */
typedef struct tagMATR
{
  FLT A[4][4]; /* Matrix array */
} MATR;

/* Camera structure */
typedef struct tagCAMERA
{
  VEC3
    Loc,        /* Camera location */
    At,         /* Camera look-at point */
    Dir,        /* Camera direction */
    Right,      /* Camera right direction */
    Up;         /* Camera up direction */
  MATR
    MatrView,   /* View matrix */
    MatrProj,   /* Projection matrix */
    MatrVP;     /* Stored (View * Proj) matrix */
  INT
    FrameW, FrameH; /* Frame size (in pixels) */
  FLT
    Wp, Hp,      /* Project plane size */
    ProjSize,    /* Project plane fit square */
    ProjDist,    /* Distance to project plane from viewer (near) */
    ProjFarClip; /* Distance to project for clip plane (far) */
} CAMERA;

#endif /* __twr_mth_def_h_ */

/* END OF 'twr_mth_def.h' FILE */
