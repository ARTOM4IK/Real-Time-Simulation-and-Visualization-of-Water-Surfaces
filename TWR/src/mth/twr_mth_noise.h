/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_noise.h
 * PURPOSE     : Tough Water Rendering project.
 *               Math noise module declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 07.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_noise_h_
#define __twr_mth_noise_h_

#include "twr_def.h"

/*** Noise module initialization ***/

/* Noise table initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NoiseInit( VOID );

/*** Float noise ***/

/* Noise 1-dimension obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       FLT X;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise1D( FLT X );

/* Noise 2-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise2D( FLT X, FLT Y );

/* Noise 3-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise3D( FLT X, FLT Y, FLT Z );

/* Noise 4-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z, W;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise4D( FLT X, FLT Y, FLT Z, FLT W );

/* Turbulent noise 1-dimension obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       FLT X;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb1D( FLT X, INT Octaves );

/* Turbulent noise 2-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb2D( FLT X, FLT Y, INT Octaves );

/* Turbulent noise 3-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb3D( FLT X, FLT Y, FLT Z, INT Octaves );

/* Turbulent noise 4-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z, W;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb4D( FLT X, FLT Y, FLT Z, FLT W, INT Octaves );

/*** VEC2 noise ***/

/* VEC2 of turbulent 1D noise macro definition */
#define Noise2Turb1D(X, Octaves) (Vec2Set(NoiseTurb1D(X, Octaves), NoiseTurb1D(X + 0.8, Octaves)))

/* VEC2 of turbulent 2D noise macro definition */
#define Noise2Turb2D(X, Y, Octaves) (Vec2Set(NoiseTurb2D(X, Y, Octaves), NoiseTurb2D(X + 0.8, Y + 0.8, Octaves)))

/* VEC2 of turbulent 3D noise macro definition */
#define Noise2Turb3D(X, Y, Z, Octaves) (Vec2Set(NoiseTurb3D(X, Y, Z, Octaves), NoiseTurb3D(X + 0.8, Y + 0.8, Z + 0.8, Octaves)))

/* VEC2 of turbulent 4D noise macro definition */
#define Noise2Turb4D(X, Y, Z, W, Octaves) (Vec2Set(NoiseTurb4D(X, Y, Z, W, Octaves), NoiseTurb4D(X + 0.8, Y + 0.8, Z + 0.8, W + 0.8, Octaves)))

/*** VEC3 noise ***/

/* VEC3 of turbulent 1D noise macro definition */
#define Noise3Turb1D(X, Octaves) (Vec3Set(NoiseTurb1D(X, Octaves), NoiseTurb1D(X + 0.8, Octaves), NoiseTurb1D(X + 0.30, Octaves)))

/* VEC3 of turbulent 2D noise macro definition */
#define Noise3Turb2D(X, Y, Octaves) (Vec3Set(NoiseTurb2D(X, Y, Octaves), NoiseTurb2D(X + 0.8, Y + 0.8, Octaves), NoiseTurb2D(X + 0.30, Y + 0.30, Octaves)))

/* VEC3 of turbulent 3D noise macro definition */
#define Noise3Turb3D(X, Y, Z, Octaves) (Vec3Set(NoiseTurb3D(X, Y, Z, Octaves), NoiseTurb3D(X + 0.8, Y + 0.8, Z + 0.8, Octaves), NoiseTurb3D(X + 0.30, Y + 0.30, Z + 0.30, Octaves)))

/* VEC3 of turbulent 4D noise macro definition */
#define Noise3Turb4D(X, Y, Z, W, Octaves) (Vec3Set(NoiseTurb4D(X, Y, Z, W, Octaves), NoiseTurb4D(X + 0.8, Y + 0.8, Z + 0.8, W + 0.8, Octaves), NoiseTurb4D(X + 0.30, Y + 0.30, Z + 0.30, W + 0.30, Octaves)))

/*** VEC4 noise ***/

/* VEC4 of turbulent 1D noise macro definition */
#define Noise4Turb1D(X, Octaves) (Vec4Set(NoiseTurb1D(X, Octaves), NoiseTurb1D(X + 0.8, Octaves), NoiseTurb1D(X + 0.30, Octaves), NoiseTurb1D(X + 0.47, Octaves)))

/* VEC4 of turbulent 2D noise macro definition */
#define Noise4Turb2D(X, Y, Octaves) (Vec4Set(NoiseTurb2D(X, Y, Octaves), NoiseTurb2D(X + 0.8, Y + 0.8, Octaves), NoiseTurb2D(X + 0.30, Y + 0.30, Octaves), NoiseTurb2D(X + 0.47, Y + 0.47, Octaves)))

/* VEC4 of turbulent 3D noise macro definition */
#define Noise4Turb3D(X, Y, Z, Octaves) (Vec4Set(NoiseTurb3D(X, Y, Z, Octaves), NoiseTurb3D(X + 0.8, Y + 0.8, Z + 0.8, Octaves), NoiseTurb3D(X + 0.30, Y + 0.30, Z + 0.30, Octaves), NoiseTurb3D(X + 0.47, Y + 0.47, Z + 0.47, Octaves)))

/* VEC4 of turbulent 4D noise macro definition */
#define Noise4Turb4D(X, Y, Z, W, Octaves) (Vec4Set(NoiseTurb4D(X, Y, Z, W, Octaves), NoiseTurb4D(X + 0.8, Y + 0.8, Z + 0.8, W + 0.8, Octaves), NoiseTurb4D(X + 0.30, Y + 0.30, Z + 0.30, W + 0.30, Octaves), NoiseTurb4D(X + 0.47, Y + 0.47, Z + 0.47, W + 0.47, Octaves)))

#endif /* __twr_mth_noise_h_ */

/* END OF 'twr_mth_noise.h' FILE */
