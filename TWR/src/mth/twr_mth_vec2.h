/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_vec2.h
 * PURPOSE     : Tough Water Rendering project.
 *               VEC2 functions declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 10.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_vec2_h_
#define __twr_mth_vec2_h_

#include "twr_def.h"

/* Set vector2 function.
 * ARGUMENTS:
 *   - numbers to be add:
 *       FLT X, FLT Y;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2Set( FLT X, FLT Y )
{
  VEC2 v = {X, Y};
 
  return v;
} /* End of 'Vec2Set' function */

/* Set vector2 filled with one number function.
 * ARGUMENTS:
 *   - numbers to be add:
 *       FLT A;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2Set1( FLT A )
{
  VEC2 v = {A, A};
 
  return v;
} /* End of 'Vec2Set1' function */

/* Add two vectors2 function.
 * ARGUMENTS:
 *   - vectors to be add:
 *       VEC2 V1, V2;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2AddVec2( VEC2 V1, VEC2 V2 )
{
  return Vec2Set(V1.X + V2.X, V1.Y + V2.Y);
} /* End of 'Vec2AddVec2' function */

/* Subtract two vectors2 function.
 * ARGUMENTS:
 *   - vectors to be substracted:
 *       VEC2 V1, V2;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2SubVec2( VEC2 V1, VEC2 V2 )
{
  return Vec2Set(V1.X - V2.X, V1.Y - V2.Y);
} /* End of 'Vec2SubVec2' function */

/* Multiply vector2 and number function.
 * ARGUMENTS:
 *   - vector to be multiplied:
 *       VEC2 V;
 *   - number to multiply by:
 *       FLT N;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2MulNum( VEC2 V, FLT N )
{
  return Vec2Set(V.X * N, V.Y * N);
} /* End of 'Vec2MulNum' function */

/* Divide vector2 by number function.
 * ARGUMENTS:
 *   - vector to be divided:
 *       VEC2 V;
 *   - number divide by:
 *       FLT N;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2DivNum( VEC2 V, FLT N )
{
  return Vec2Set(V.X / N, V.Y / N);
} /* End of 'Vec2DivNum' function */

/* Set negative vector2 function.
 * ARGUMENTS:
 *   - vector to be translated to negative vector:
 *       VEC2 V;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2Neg( VEC2 V )
{
  return Vec2Set(-V.X, -V.Y);
} /* End of 'Vec2Neg' function */

/* Scalar product of 2 vectors2 function.
 * ARGUMENTS:
 *   - vectors to be scalar multiplied:
 *       VEC2 V1, VEC2 V2;
 * RETURNS:
 *   (FLT) result product.
 */
__inline FLT Vec2DotVec2( VEC2 V1, VEC2 V2 )
{
  return V1.X * V2.X + V1.Y * V2.Y;
} /* End of 'Vec2DotVec2' function */

/* Len of vector2 function.
 * ARGUMENTS:
 *   - vector to get its length:
 *       VEC2 V;
 * RETURNS:
 *   (FLT) result length.
 */
__inline FLT Vec2Len( VEC2 V )
{
  FLT len = Vec2DotVec2(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return len;
  return sqrt(len);
} /* End of 'Vec2Len' function */

/* Square of len of vector2 function.
 * ARGUMENTS:
 *   - vector to get its length:
 *       VEC2 V;
 * RETURNS:
 *   (FLT) result square of length.
 */
__inline FLT Vec2Len2( VEC2 V )
{
  return Vec2DotVec2(V, V);
} /* End of 'Vec2Len2' function */

/* Normalize vector2 function.
 * ARGUMENTS:
 *   - vector to be normalized:
 *       VEC2 V;
 * RETURNS:
 *   (VEC2) result vector.
 */
__inline VEC2 Vec2Normalize( VEC2 V )
{                           
  FLT len = Vec2DotVec2(V, V); /* Vector length */
 
  if (len == 1 || len == 0)
    return V;
  return Vec2DivNum(V, sqrt(len));
} /* End of 'Vec2Normalize' function */

/* Vector minimum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC2 V1, V2;
 * RETURNS:
 *   (VEC2) result vector with minimum components.
 */
__inline VEC2 Vec2MinVec2( VEC2 V1, VEC2 V2 )
{
  return Vec2Set(V1.X < V2.X ? V1.X : V2.X,
                 V1.Y < V2.Y ? V1.Y : V2.Y);
} /* End of 'Vec2MinVec2' function */

/* Vector maximum function.
 * ARGUMENTS:
 *   - source vectors:
 *       VEC2 V1, V2;
 * RETURNS:
 *   (VEC2) result vector with maximum components.
 */
__inline VEC2 Vec2MaxVec2( VEC2 V1, VEC2 V2 )
{
  return Vec2Set(V1.X > V2.X ? V1.X : V2.X,
                 V1.Y > V2.Y ? V1.Y : V2.Y);
} /* End of 'Vec2MaxVec2' function */

/* Get distance between two points function.
 * ARGUMENTS:
 *   - input vectors:
 *       VEC2 V1, VEC2 V2;
 * RETURNS:
 *   (FLT) result distance.
 */
__inline FLT Vec2Distance( VEC2 V1, VEC2 V2 )
{
  return Vec2Len(Vec2SubVec2(V1, V2));
} /* End of 'Vec2Distance' function */

#endif /* __twr_mth_vec2_h_ */

/* END OF 'twr_mth_vec2.h' FILE */
