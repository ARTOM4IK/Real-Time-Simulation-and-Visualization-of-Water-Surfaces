/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_vec4.h
 * PURPOSE     : Tough Water Rendering project.
 *               VEC4 functions declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_vec4_h_
#define __twr_mth_vec4_h_

#include "twr_def.h"

/* Set vector4 function.
 * ARGUMENTS:
 *   - numbers to be add:
 *       FLT X, FLT Y, FLT Z, FLT W;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Set( FLT X, FLT Y, FLT Z, FLT W )
{
  VEC4 v = {X, Y, Z, W};
 
  return v;
} /* End of 'Vec4Set' function */

/* Set vector4 filled with one number function.
 * ARGUMENTS:
 *   - coordinates value to be set:
 *       FLT A;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Set1( FLT A )
{
  VEC4 v = {A, A, A, A};
 
  return v;
} /* End of 'Vec4Set1' function */

/* Set vector4 filled with vector3 and scalar values function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       VEC3 V;
 *   - last component:
 *       FLT W;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4SetVec3( VEC3 V, FLT W )
{
  VEC4 v;

  v.X = V.X;
  v.Y = V.Y;
  v.Z = V.Z;
  v.W = W;
  return v;
} /* End of 'Vec4SetVec3' function */

/* Add two vectors4 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC4 V1, V2;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4AddVec4( VEC4 V1, VEC4 V2 )
{
  return Vec4Set(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z, V1.W + V2.W);
} /* End of 'Vec4AddVec4' function */

/* Subtract two vectors4 function.
 * ARGUMENTS:
 *   - vectors to be substracted:
 *       VEC4 V1, V2;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4SubVec4( VEC4 V1, VEC4 V2 )
{
  return Vec4Set(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z, V1.W - V2.W);
} /* End of 'Vec4SubVec4' function */

/* Multiply two vectors4 function.
 * ARGUMENTS:
 *   - vector to be multiplied:
 *       VEC4 V;
 *   - number to multiply by:
 *       FLT N;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4MulNum( VEC4 V, FLT N )
{
  return Vec4Set(V.X * N, V.Y * N, V.Z * N, V.W * N);
} /* End of 'Vec4MulNum' function */

/* Divide vector4 by number function.
 * ARGUMENTS:
 *   - vectors to be divided:
 *       VEC4 V;
 *   - number divide by:
 *       FLT N;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4DivNum( VEC4 V, FLT N )
{
  return Vec4Set(V.X / N, V.Y / N, V.Z / N, V.W / N);
} /* End of 'Vec4DivNum' function */

/* Set negative vector4 function.
 * ARGUMENTS:
 *   - vector to be translated to negative vector:
 *       VEC4 V;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Neg( VEC4 V )
{
  return Vec4Set(-V.X, -V.Y, -V.Z, -V.W);
} /* End of 'Vec4Neg' function */

/* Scalar product of 2 vectors4 function.
 * ARGUMENTS:
 *   - vectors to be scalar multiplied:
 *       VEC4 V1, VEC2 V2;
 * RETURNS:
 *   (FLT) result product.
 */
__inline FLT Vec4DotVec4( VEC4 V1, VEC4 V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z + V1.W * V2.W;
} /* End of 'Vec4DotVec4' function */

/* Len of vector4 function.
 * ARGUMENTS:
 *   - vector to get its length:
 *       VEC4 V;
 * RETURNS:
 *   (FLT) result length.
 */
__inline FLT Vec4Len( VEC4 V )
{
  FLT len = Vec4DotVec4(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'Vec4Len' function */

/* Square of len of vector4 function.
 * ARGUMENTS:
 *   - vector to get its length:
 *       VEC4 V;
 * RETURNS:
 *   (FLT) result square of length.
 */
__inline FLT Vec4Len2( VEC4 V )
{
  return Vec4DotVec4(V, V);
} /* End of 'Vec4Len2' function */

/* Normalize vector4 function.
 * ARGUMENTS:
 *   - vector to be normalized:
 *       VEC4 V;
 * RETURNS:
 *   (VEC4) result vector.
 */
__inline VEC4 Vec4Normalize( VEC4 V )
{
  FLT len = Vec4DotVec4(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return V;
  return Vec4DivNum(V, sqrt(len));
} /* End of 'Vec4Normalize' function */

/* Vector minimum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC4 V1, V2;
 * RETURNS:
 *   (VEC4) result vector with minimum components.
 */
__inline VEC4 Vec4MinVec4( VEC4 V1, VEC4 V2 )
{
  return Vec4Set(V1.X < V2.X ? V1.X : V2.X,
                 V1.Y < V2.Y ? V1.Y : V2.Y,
                 V1.Z < V2.Z ? V1.Z : V2.Z,
                 V1.W < V2.W ? V1.W : V2.W);
} /* End of 'Vec4MinVec4' function */

/* Vector maximum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC4 V1, V2;
 * RETURNS:
 *   (VEC4) result vector with maximum components.
 */
__inline VEC4 Vec4MaxVec4( VEC4 V1, VEC4 V2 )
{
  return Vec4Set(V1.X > V2.X ? V1.X : V2.X,
                 V1.Y > V2.Y ? V1.Y : V2.Y,
                 V1.Z > V2.Z ? V1.Z : V2.Z,
                 V1.W > V2.W ? V1.W : V2.W);
} /* End of 'Vec4MaxVec4' function */

/* Get distance between two points function.
 * ARGUMENTS:
 *   - input vectors:
 *       VEC4 V1, VEC4 V2;
 * RETURNS:
 *   (FLT) result distance.
 */
__inline FLT Vec4Distance( VEC4 V1, VEC4 V2 )
{
  return Vec4Len(Vec4SubVec4(V1, V2));
} /* End of 'Vec4Distance' function */

#endif /* __twr_mth_vec4_h_ */

/* END OF 'twr_mth_vec4.h' FILE */
