/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_vec3.h
 * PURPOSE     : Tough Water Rendering project.
 *               VEC3 functions declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_vec3_h_
#define __twr_mth_vec3_h_

#include "twr_def.h"

/* Set vector3 function.
 * ARGUMENTS:
 *   - numbers to be add:
 *       FLT X, FLT Y, FLT Z;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3Set( FLT X, FLT Y, FLT Z )
{
  VEC3 v = {X, Y, Z};
 
  return v;
} /* End of 'Vec3Set' function */

/* Set vector3 filled with one number function.
 * ARGUMENTS:
 *   - number to be add:
 *       FLT A;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3Set1( FLT A )
{
  VEC3 v = {A, A, A};
 
  return v;
} /* End of 'Vec3Set1' function */

/* Add two vectors3 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC3 V1, V2;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3AddVec3( VEC3 V1, VEC3 V2 )
{
  return Vec3Set(V1.X + V2.X, V1.Y + V2.Y, V1.Z + V2.Z);
} /* End of 'Vec3AddVec3' function */

/* Subtract two vectors3 function.
 * ARGUMENTS:
 *   - vectors to be substracted:
 *       VEC3 V1, V2;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3SubVec3( VEC3 V1, VEC3 V2 )
{
  return Vec3Set(V1.X - V2.X, V1.Y - V2.Y, V1.Z - V2.Z);
} /* End of 'Vec3SubVec3' function */

/* Multiply vector3 and number function.
 * ARGUMENTS:
 *   - vector to be multiplied:
 *       VEC3 V;
 *   - number to multiply by:
 *       FLT N;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3MulNum( VEC3 V, FLT N )
{
  return Vec3Set(V.X * N, V.Y * N, V.Z * N);
} /* End of 'Vec3MulNum' function */

/* Divide vector3 by number function.
 * ARGUMENTS:
 *   - vector to be divided:
 *       VEC3 V;
 *   - number to divide by:
 *       FLT N;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3DivNum( VEC3 V, FLT N )
{
  return Vec3Set(V.X / N, V.Y / N, V.Z / N);
} /* End of 'Vec3DivNum' function */

/* Set negative vector3 function.
 * ARGUMENTS:
 *   - vector to be translated to negative vector:
 *       VEC3 V;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3Neg( VEC3 V )
{
  return Vec3Set(-V.X, -V.Y, -V.Z);
} /* End of 'Vec3Neg' function */

/* Scalar product of 2 vectors3 function.
 * ARGUMENTS:
 *   - vectors to be scalar multiplied:
 *       VEC3 V1, VEC3 V2;
 * RETURNS:
 *   (FLT) result product.
 */
__inline FLT Vec3DotVec3( VEC3 V1, VEC3 V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y + V1.Z * V2.Z;
} /* End of 'Vec3DotVec3' function */

/* Vector product of 2 vectors3 function.
 * ARGUMENTS:
 *   - vectors to be vector multiplied:
 *       VEC3 V1, VEC3 V2;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3CrossVec3( VEC3 V1, VEC3 V2 )
{
  return Vec3Set(V1.Y * V2.Z - V1.Z * V2.Y, V1.Z * V2.X - V1.X * V2.Z, V1.X * V2.Y - V1.Y * V2.X );
} /* End of 'Vec3CrossVec3' function */

/* Len of vector3 function.
 * ARGUMENTS:
 *   - vector to get its length:
 *       VEC3 V;
 * RETURNS:
 *   (FLT) result length.
 */
__inline FLT Vec3Len( VEC3 V )
{
  FLT len = Vec3DotVec3(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'Vec3Len' function */

/* Square of len of vector3 function.
 * ARGUMENTS:
 *   - vector to get square of its length:
 *       VEC3 V;
 * RETURNS:
 *   (FLT) result square of length.
 */
__inline FLT Vec3Len2( VEC3 V )
{
  return Vec3DotVec3(V, V);
} /* End of 'Vec3Len2' function */

/* Normalize vector3 function.
 * ARGUMENTS:
 *   - vector to be normalized:
 *       VEC3 V;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3Normalize( VEC3 V )
{
  FLT len = Vec3DotVec3(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return V;
  return Vec3DivNum(V, sqrt(len));
} /* End of 'Vec3Normalize' function */

/* Vector3 by matrix multiplication (with homogenious divide) function.
 * ARGUMENTS:
 *   - source vector:
 *       VEC3 V;
 *   - multiplied matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3MulMatr( VEC3 V, MATR M )
{
  FLT w = V.X * M.A[0][3] + V.Y * M.A[1][3] + V.Z * M.A[2][3] + M.A[3][3];

  return Vec3Set((V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0]) / w,
                (V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1]) / w,
                (V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]) / w);
} /* End of 'Vec3MulMatr' function */

/* Transform point by matrix function.
 * ARGUMENTS:
 *   - point to tranform:
 *       VEC3 V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC3) result point.
 */
__inline VEC3 PointTransform( VEC3 V, MATR M )
{
  return Vec3Set(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0] + M.A[3][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1] + M.A[3][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2] + M.A[3][2]);
} /* End of 'PointTransform' function */

/* Transform vector by matrix function.
 * ARGUMENTS:
 *   - vector to tranform:
 *       VEC3 V;
 *   - transformation matrix:
 *       MATR M;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 VectorTransform( VEC3 V, MATR M )
{
  return Vec3Set(V.X * M.A[0][0] + V.Y * M.A[1][0] + V.Z * M.A[2][0],
                V.X * M.A[0][1] + V.Y * M.A[1][1] + V.Z * M.A[2][1],
                V.X * M.A[0][2] + V.Y * M.A[1][2] + V.Z * M.A[2][2]);
} /* End of 'VectorTransform' function */

/* Vector minimum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC3 V1, V2;
 * RETURNS:
 *   (VEC3) result vector with minimum components.
 */
__inline VEC3 Vec3MinVec3( VEC3 V1, VEC3 V2 )
{
  return Vec3Set(V1.X < V2.X ? V1.X : V2.X,
                 V1.Y < V2.Y ? V1.Y : V2.Y,
                 V1.Z < V2.Z ? V1.Z : V2.Z);
} /* End of 'Vec3MinVec3' function */

/* Vector maximum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC3 V1, V2;
 * RETURNS:
 *   (VEC3) result vector with maximum components.
 */
__inline VEC3 Vec3MaxVec3( VEC3 V1, VEC3 V2 )
{
  return Vec3Set(V1.X > V2.X ? V1.X : V2.X,
                 V1.Y > V2.Y ? V1.Y : V2.Y,
                 V1.Z > V2.Z ? V1.Z : V2.Z);
} /* End of 'Vec3MaxVec3' function */

/* Set vector3 filled with XYZ coorinates of vector4 function.
 * ARGUMENTS:
 *   - vector coordinates:
 *       VEC4 V;
 * RETURNS:
 *   (VEC3) result vector.
 */
__inline VEC3 Vec3SetVec4( VEC4 V )
{
  VEC3 v;

  v.X = V.X;
  v.Y = V.Y;
  v.Z = V.Z;
  return v;
} /* End of 'Vec3SetVec4' function */

/* Get distance between two points function.
 * ARGUMENTS:
 *   - input vectors:
 *       VEC3 V1, VEC3 V2;
 * RETURNS:
 *   (FLT) result distance.
 */
__inline FLT Vec3Distance( VEC3 V1, VEC3 V2 )
{
  return Vec3Len(Vec3SubVec3(V1, V2));
} /* End of 'Vec3Distance' function */

#endif /* __twr_mth_vec3_h_ */

/* END OF 'twr_mth_vec3.h' FILE */
