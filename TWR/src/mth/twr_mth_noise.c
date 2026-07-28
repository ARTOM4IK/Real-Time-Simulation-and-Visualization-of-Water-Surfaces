/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_noise.c
 * PURPOSE     : Tough Water Rendering project.
 *               Math noise module implementation file.
 * PROGRAMMER  : CGSG'2024.
 *               Belyh Andrey (AB7).
 * LAST UPDATE : 07.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Noise table parameters */
#define TAB_BITS 8
#define TAB_SIZE (1 << TAB_BITS)
#define TAB_MASK (TAB_SIZE - 1)

/* Noise numbers array */
static FLT TabNoise[TAB_SIZE];

/* Permutation array */
static INT Perm[TAB_SIZE];

/* Random indexing macro definitions */
#define INDEX1(X) (Perm[(X) & TAB_MASK])
#define INDEX2(X, Y) (Perm[(X + INDEX1(Y)) & TAB_MASK])
#define INDEX3(X, Y, Z) (Perm[(X + INDEX2(Y, Z)) & TAB_MASK])
#define INDEX4(X, Y, Z, W) (Perm[(X + INDEX3(Y, Z, W)) & TAB_MASK])

/*** Noise module initialization ***/

/* Noise table initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID NoiseInit( VOID )
{
  INT i;

  /* Fill noise table */
  for (i = 0; i < TAB_SIZE; i++)
    TabNoise[i] = (FLT)rand() / RAND_MAX;
  /* Fill permutation array */
  for (i = 0; i < TAB_SIZE; i++)
    Perm[i] = i;
  /* Shuffle permutation array */
  for (i = 0; i < TAB_SIZE * TAB_SIZE; i++)
  {
    INT
      a = rand() & TAB_MASK,
      b = rand() & TAB_MASK,
      tmp = Perm[a];

    Perm[a] = Perm[b];
    Perm[b] = tmp;
  }
} /* End of 'NoiseInit' function */

/*** Float noise ***/

/* Noise 1-dimension obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       FLT X;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise1D( FLT X )
{
  INT ix = floor(X), ix1;
  FLT fx = X - ix;

  fx = (3 - 2 * fx) * fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;

  return TabNoise[ix] * (1 - fx) +
         TabNoise[ix1] * fx;
} /* End of 'Noise1D' function */

/* Noise 2-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise2D( FLT X, FLT Y )
{
  INT ix = floor(X), ix1,
      iy = floor(Y), iy1;
  FLT fx = X - ix,
      fy = Y - iy;

  fx = (3 - 2 * fx) * fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;

  fy = (3 - 2 * fy) * fy * fy;
  iy &= TAB_MASK;
  iy1 = (iy + 1) & TAB_MASK;

  return TabNoise[INDEX2(iy, ix)] * (1 - fx) * (1 - fy) +
         TabNoise[INDEX2(iy, ix1)] * fx * (1 - fy) +
         TabNoise[INDEX2(iy1, ix)] * (1 - fx) * fy +
         TabNoise[INDEX2(iy1, ix1)] * fx * fy;
} /* End of 'Noise2D' function */

/* Noise 3-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise3D( FLT X, FLT Y, FLT Z )
{
  INT ix = floor(X), ix1,
      iy = floor(Y), iy1,
      iz = floor(Z), iz1;
  FLT fx = X - ix,
      fy = Y - iy,
      fz = Z - iz;

  fx = (3 - 2 * fx) * fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;

  fy = (3 - 2 * fy) * fy * fy;
  iy &= TAB_MASK;
  iy1 = (iy + 1) & TAB_MASK;

  fz = (3 - 2 * fz) * fz * fz;
  iz &= TAB_MASK;
  iz1 = (iz + 1) & TAB_MASK;

  return TabNoise[INDEX3(ix, iy, iz)] * (1 - fx) * (1 - fy) * (1 - fz) +
         TabNoise[INDEX3(ix1, iy, iz)] * fx * (1 - fy) * (1 - fz) +
         TabNoise[INDEX3(ix, iy1, iz)] * (1 - fx) * fy * (1 - fz) +
         TabNoise[INDEX3(ix1, iy1, iz)] * fx * fy * (1 - fz) +
         TabNoise[INDEX3(ix, iy, iz1)] * (1 - fx) * (1 - fy) * fz +
         TabNoise[INDEX3(ix, iy1, iz1)] * (1 - fx) * fy * fz +
         TabNoise[INDEX3(ix1, iy, iz1)] * fx * (1 - fy) * fz +
         TabNoise[INDEX3(iz1, iy1, ix1)] * fx * fy * fz;
} /* End of 'Noise3D' function */

/* Noise 4-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z, W;
 * RETURNS:
 *   (FLT) noise value.
 */
FLT Noise4D( FLT X, FLT Y, FLT Z, FLT W )
{
  INT ix = floor(X), ix1,
      iy = floor(Y), iy1,
      iz = floor(Z), iz1,
      iw = floor(W), iw1;
  FLT fx = X - ix,
      fy = Y - iy,
      fz = Z - iz,
      fw = W - iw;

  fx = (3 - 2 * fx) * fx * fx;
  ix &= TAB_MASK;
  ix1 = (ix + 1) & TAB_MASK;

  fy = (3 - 2 * fy) * fy * fy;
  iy &= TAB_MASK;
  iy1 = (iy + 1) & TAB_MASK;

  fz = (3 - 2 * fz) * fz * fz;
  iz &= TAB_MASK;
  iz1 = (iz + 1) & TAB_MASK;

  fw = (3 - 2 * fw) * fw * fw;
  iw &= TAB_MASK;
  iw1 = (iw + 1) & TAB_MASK;

  return TabNoise[INDEX4(ix, iy, iz, iw)] * (1 - fx) * (1 - fy) * (1 - fz) * (1 - fw) +
         TabNoise[INDEX4(ix1, iy, iz, iw)] * fx * (1 - fy) * (1 - fz) * (1 - fw) +
         TabNoise[INDEX4(ix, iy1, iz, iw)] * (1 - fx) * fy * (1 - fz) * (1 - fw) +
         TabNoise[INDEX4(ix, iy, iz1, iw)] * (1 - fx) * (1 - fy) * fz * (1 - fw) +
         TabNoise[INDEX4(ix1, iy1, iz, iw)] * fx * fy * (1 - fz) * (1 - fw) +
         TabNoise[INDEX4(ix1, iy, iz1, iw)] * fx * (1 - fy) * fz * (1 - fw) +
         TabNoise[INDEX4(ix, iy1, iz1, iw)] * (1 - fx) * fy * fz * (1 - fw) +
         TabNoise[INDEX4(ix1, iy1, iz1, iw)] * fx * fy * fz * (1 - fw) +
         TabNoise[INDEX4(ix, iy, iz, iw1)] * (1 - fx) * (1 - fy) * (1 - fz) * fw +
         TabNoise[INDEX4(ix1, iy, iz, iw1)] * fx * (1 - fy) * (1 - fz) * fw +
         TabNoise[INDEX4(ix, iy1, iz, iw1)] * (1 - fx) * fy * (1 - fz) * fw +
         TabNoise[INDEX4(ix, iy, iz1, iw1)] * (1 - fx) * (1 - fy) * fz * fw +
         TabNoise[INDEX4(ix1, iy1, iz, iw1)] * fx * fy * (1 - fz) * fw +
         TabNoise[INDEX4(ix1, iy, iz1, iw1)] * fx * (1 - fy) * fz * fw +
         TabNoise[INDEX4(ix, iy1, iz1, iw1)] * (1 - fx) * fy * fz * fw +
         TabNoise[INDEX4(ix1, iy1, iz1, iw1)] * fx * fy * fz * fw;
} /* End of 'Noise4D' function */

/* Turbulent noise 1-dimension obtain function.
 * ARGUMENTS:
 *   - noise argument:
 *       FLT X;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb1D( FLT X, INT Octaves )
{
  INT i, frac = 1;
  FLT val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise1D(X) / frac;
    X = (X + 0.8) * 2;
    frac *= 2;
  }

  return val * (2.0 - 1.0 / (1 << Octaves));
} /* End of 'NoiseTurb1D' function */

/* Turbulent noise 2-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb2D( FLT X, FLT Y, INT Octaves )
{
  INT i, frac = 1;
  FLT val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise2D(X, Y) / frac;
    X = (X + 0.30) * 2;
    Y = (Y + 0.47) * 2;
    frac *= 2;
  }

  return val * (2.0 - 1.0 / (1 << Octaves));
} /* End of 'NoiseTurb2D' function */

/* Turbulent noise 3-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb3D( FLT X, FLT Y, FLT Z, INT Octaves )
{
  INT i, frac = 1;
  FLT val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise3D(X, Y, Z) / frac;
    X = (X + 0.8) * 2;
    Y = (Y + 0.30) * 2;
    Z = (Z + 0.47) * 2;
    frac *= 2;
  }

  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'NoiseTurb3D' function */

/* Turbulent noise 4-dimension obtain function.
 * ARGUMENTS:
 *   - noise arguments:
 *       FLT X, Y, Z, W;
 *   - number of octaves:
 *       INT Octaves;
 * RETURNS:
 *   (FLT) turbulence value.
 */
FLT NoiseTurb4D( FLT X, FLT Y, FLT Z, FLT W, INT Octaves )
{
  INT i, frac = 1;
  FLT val = 0;

  for (i = 0; i < Octaves; i++)
  {
    val += Noise4D(X, Y, Z, W) / frac;
    X = (X + 0.8) * 2;
    Y = (Y + 0.30) * 2;
    Z = (Z + 0.47) * 2;
    W = (W + 0.102) * 2;
    frac *= 2;
  }

  return val * (1 << (Octaves - 1)) / ((1 << Octaves) - 1);
} /* End of 'NoiseTurb4D' function */

/* END OF 'twr_mth_noise.c' FILE */
