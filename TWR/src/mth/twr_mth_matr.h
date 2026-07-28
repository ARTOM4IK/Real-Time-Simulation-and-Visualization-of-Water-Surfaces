/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_matr.h
 * PURPOSE     : Tough Water Rendering project.
 *               Matrix functions declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 25.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_matr_h_
#define __twr_mth_matr_h_

#include "twr_mth_def.h"

/* Set matrix by 16 elements function.
 * ARGUMENTS:
 *   - matrix elements:
 *       FLT A00, A01, A02, A03, A10, A11, A12, A13, A20, A21, A22, A23, A30, A31, A32, A33;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrSet( FLT A00, FLT A01, FLT A02, FLT A03,
                       FLT A10, FLT A11, FLT A12, FLT A13,
                       FLT A20, FLT A21, FLT A22, FLT A23,
                       FLT A30, FLT A31, FLT A32, FLT A33 )
{
  MATR m =
  {
    {
      {A00, A01, A02, A03},
      {A10, A11, A12, A13},
      {A20, A21, A22, A23},
      {A30, A31, A32, A33},
    }
  };

  return m;
} /* End of 'MatrSet' function */

/* Get identity matrix function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (MATR) identity matrix.
 */
__inline MATR MatrIdentity( VOID )
{
  MATR I =
  {
    {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}
    }
  };

  return I;
} /* End of 'MatrIdentity' function */

/* Get translate matrix by vector function.
 * ARGUMENTS:
 *   - translate vector:
 *       VEC T;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranslate( VEC3 T )
{
  return MatrSet(1, 0, 0, 0,
                 0, 1, 0, 0,
                 0, 0, 1, 0,
                 T.X, T.Y, T.Z, 1);
} /* End of 'MatrTranslate' function */

/* Get scale matrix by vector function.
 * ARGUMENTS:
 *   - scale vector:
 *       VEC S;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrScale( VEC3 S )
{
  return MatrSet(S.X, 0, 0, 0,
                 0, S.Y, 0, 0,
                 0, 0, S.Z, 0,
                 0, 0, 0, 1);
} /* End of 'MatrScale' function */

/* Get rotate matrix on axis X by angle function.
 * ARGUMENTS:
 *   - angle in degrees:
 *       FLT AngleInDegrees;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateX( FLT AngleInDegrees )
{
  FLT s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

  return MatrSet(1, 0, 0, 0,
                 0, c, s, 0,
                 0, -s, c, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateX' function */

/* Get rotate matrix on axis Y by angle function.
 * ARGUMENTS:
 *   - angle in degrees:
 *       FLT AngleInDegrees;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateY( FLT AngleInDegrees )
{
  FLT s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

  return MatrSet(c, 0, -s, 0,
                 0, 1, 0, 0,
                 s, 0, c, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateY' function */

/* Get rotate matrix on axis Z by angle function.
 * ARGUMENTS:
 *   - angle in degrees:
 *       FLT AngleInDegrees;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotateZ( FLT AngleInDegrees )
{
  FLT s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees));

  return MatrSet(c, s, 0, 0,
                 -s, c, 0, 0,
                 0, 0, 1, 0,
                 0, 0, 0, 1);
} /* End of 'MatrRotateZ' function */

/* Get rotate matrix (vector axis) by angle function.
 * ARGUMENTS:
 *   - angle in degrees:
 *       FLT AngleInDegrees;
 *   - axis vector:
 *       VEC V;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrRotate( FLT AngleInDegrees, VEC3 R )
{
  FLT
    s = sin(D2R(AngleInDegrees)), c = cos(D2R(AngleInDegrees)),
    x = R.X, y = R.Y, z = R.Z;

  return MatrSet(x * x * (1 - c) + c, x * y * (1 - c) + z * s, x * z * (1 - c) - y * s, 0,
                 x * y * (1 - c) - z * s, y * y * (1 - c) + c, y * z * (1 - c) + x * s, 0,
                 x * z * (1 - c) + y * s, y * z * (1 - c) - x * s, z * z * (1 - c) + c, 0,
                 0, 0, 0, 1);
}
/* End of 'MatrRotate' function */

/* Multiply 2 matrixes function.
 * ARGUMENTS:
 *   - matrixes to multiply:
 *       MATR M1, M2;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrMulMatr( MATR M1, MATR M2 )
{
  return MatrSet(M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0] + M1.A[0][3] * M2.A[3][0],
                 M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1] + M1.A[0][3] * M2.A[3][1],
                 M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2] + M1.A[0][3] * M2.A[3][2],
                 M1.A[0][0] * M2.A[0][3] + M1.A[0][1] * M2.A[1][3] + M1.A[0][2] * M2.A[2][3] + M1.A[0][3] * M2.A[3][3],
                 M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0] + M1.A[1][3] * M2.A[3][0],
                 M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1] + M1.A[1][3] * M2.A[3][1],
                 M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2] + M1.A[1][3] * M2.A[3][2],
                 M1.A[1][0] * M2.A[0][3] + M1.A[1][1] * M2.A[1][3] + M1.A[1][2] * M2.A[2][3] + M1.A[1][3] * M2.A[3][3],
                 M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0] + M1.A[2][3] * M2.A[3][0],
                 M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1] + M1.A[2][3] * M2.A[3][1],
                 M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2] + M1.A[2][3] * M2.A[3][2],
                 M1.A[2][0] * M2.A[0][3] + M1.A[2][1] * M2.A[1][3] + M1.A[2][2] * M2.A[2][3] + M1.A[2][3] * M2.A[3][3],
                 M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M1.A[3][3] * M2.A[3][0],
                 M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M1.A[3][3] * M2.A[3][1],
                 M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M1.A[3][3] * M2.A[3][2],
                 M1.A[3][0] * M2.A[0][3] + M1.A[3][1] * M2.A[1][3] + M1.A[3][2] * M2.A[2][3] + M1.A[3][3] * M2.A[3][3]);
} /* End of 'MatrMulMatr' function */

/* Multiply 3 matrixes macrodefinition */
#define MatrMulMatr3(M1, M2, M3) MatrMulMatr(MatrMulMatr(M1, M2), M3)

/* Multiply 4 matrixes macrodefinition */
#define MatrMulMatr4(M1, M2, M3, M4) MatrMulMatr(MatrMulMatr3(M1, M2, M3), M4)

/* Transpose matrix function.
 * ARGUMENTS:
 *   - matrix to transpose:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrTranspose( MATR M )
{
  return MatrSet(M.A[0][0], M.A[1][0], M.A[2][0], M.A[3][0],
                 M.A[0][1], M.A[1][1], M.A[2][1], M.A[3][1],
                 M.A[0][2], M.A[1][2], M.A[2][2], M.A[3][2],
                 M.A[0][3], M.A[1][3], M.A[2][3], M.A[3][3]);
} /* End of 'MatrTranspose' function */

/* Get 3x3 matrix determinant function.
 * ARGUMENTS:
 *   - elements of matrix to get determinant of:
 *       FLT A11, A12, A13, A21, A22, A23, A31, A32, A33;
 * RETURNS:
 *   (FLT) matrix determinant.
 */
__inline FLT MatrDeterm3x3( FLT A11, FLT A12, FLT A13,
                            FLT A21, FLT A22, FLT A23,
                            FLT A31, FLT A32, FLT A33 )
{
  return A11 * A22 * A33 + A12 * A23 * A31 + A13 * A21 * A32 -
         A11 * A23 * A32 - A12 * A21 * A33 - A13 * A22 * A31;
} /* End of 'MatrDeterm3x3' function */

/* Get matrix determinant function.
 * ARGUMENTS:
 *   - matrix to get determinant of:
 *       MATR M;
 * RETURNS:
 *   (FLT) matrix determinant.
 */
__inline FLT MatrDeterm( MATR M )
{
  return
    M.A[0][0] * MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                              M.A[2][1], M.A[2][2], M.A[2][3],
                              M.A[3][1], M.A[3][2], M.A[3][3]) +
    -M.A[0][1] * MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                               M.A[2][0], M.A[2][2], M.A[2][3],
                               M.A[3][0], M.A[3][2], M.A[3][3]) +
    M.A[0][2] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                              M.A[2][0], M.A[2][1], M.A[2][3],
                              M.A[3][0], M.A[3][1], M.A[3][3]) +
    -M.A[0][3] * MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                               M.A[2][0], M.A[2][1], M.A[2][2],
                               M.A[3][0], M.A[3][1], M.A[3][2]);
} /* End of 'MatrDeterm' function */

/* Inverse matrix function.
 * ARGUMENTS:
 *   - matrix to inverse:
 *       MATR M;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrInverse( MATR M )
{
  FLT det = MatrDeterm(M);
  MATR r;
 
  if (det == 0)
    return MatrIdentity();
  /* Build adjoint matrix */
  r.A[0][0] =
    MatrDeterm3x3(M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][0] =
   -MatrDeterm3x3(M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][0] =
    MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][0] =
   -MatrDeterm3x3(M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
  r.A[0][1] =
   -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[2][1], M.A[2][2], M.A[2][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[2][0], M.A[2][2], M.A[2][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][1] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[2][0], M.A[2][1], M.A[2][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][1] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[2][0], M.A[2][1], M.A[2][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
  r.A[0][2] =
    MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[3][1], M.A[3][2], M.A[3][3]) / det;
  r.A[1][2] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[3][0], M.A[3][2], M.A[3][3]) / det;
  r.A[2][2] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[3][0], M.A[3][1], M.A[3][3]) / det;
  r.A[3][2] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[3][0], M.A[3][1], M.A[3][2]) / det;
  r.A[0][3] =
   -MatrDeterm3x3(M.A[0][1], M.A[0][2], M.A[0][3],
                  M.A[1][1], M.A[1][2], M.A[1][3],
                  M.A[2][1], M.A[2][2], M.A[2][3]) / det;
  r.A[1][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][2], M.A[0][3],
                  M.A[1][0], M.A[1][2], M.A[1][3],
                  M.A[2][0], M.A[2][2], M.A[2][3]) / det;
  r.A[2][3] =
   -MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][3],
                  M.A[1][0], M.A[1][1], M.A[1][3],
                  M.A[2][0], M.A[2][1], M.A[2][3]) / det;
  r.A[3][3] =
    MatrDeterm3x3(M.A[0][0], M.A[0][1], M.A[0][2],
                  M.A[1][0], M.A[1][1], M.A[1][2],
                  M.A[2][0], M.A[2][1], M.A[2][2]) / det;
  return r;
} /* End of 'MatrInverse' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC Loc, At, Up1;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrView( VEC3 Loc, VEC3 At, VEC3 Up1 )
{
  VEC3
    Dir = Vec3Normalize(Vec3SubVec3(At, Loc)),
    Right = Vec3Normalize(Vec3CrossVec3(Dir, Up1)),
    Up = Vec3Normalize(Vec3CrossVec3(Right, Dir));

  return MatrSet(Right.X, Up.X, -Dir.X, 0,
                 Right.Y, Up.Y, -Dir.Y, 0,
                 Right.Z, Up.Z, -Dir.Z, 0,
                 -Vec3DotVec3(Loc, Right), -Vec3DotVec3(Loc, Up), Vec3DotVec3(Loc, Dir), 1);
} /* End of 'MatrView' function */

/* Matrix ortho projection function.
 * ARGUMENTS:
 *   - view parameters:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrOrtho( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  return MatrSet(2 / (Right - Left), 0, 0, 0,
                 0, 2 / (Top - Bottom), 0, 0,
                 0, 0, -2 / (Far - Near), 0,
                 -(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), 1);
} /* End of 'MatrOrtho' function */

/* Matrix frustum projection function.
 * ARGUMENTS:
 *   - view parameters:
 *       FLT Left, Right, Bottom, Top, Near, Far;
 * RETURNS:
 *   (MATR) result matrix.
 */
__inline MATR MatrFrustum( FLT Left, FLT Right, FLT Bottom, FLT Top, FLT Near, FLT Far )
{
  return MatrSet(2 * Near / (Right - Left), 0, 0, 0,
                 0, 2 * Near / (Top - Bottom), 0, 0,
                 (Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),  -(Far + Near) / (Far - Near), -1,
                 0, 0, -2 * Near * Far / (Far - Near), 0);
} /* End of 'MatrFrustum' function */

#endif /* __twr_mth_matr_h_ */

/* END OF 'twr_mth_matr.h' FILE */
