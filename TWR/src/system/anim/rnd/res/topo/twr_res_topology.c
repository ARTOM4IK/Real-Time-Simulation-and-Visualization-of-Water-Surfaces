/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_topology.c
 * PURPOSE     : Tough Water Rendering project.
 *               Topology implementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3),
 *               Arsentev Artemy (AA4),
 *               Nechaev Vladimir (VN4),
 *               Kupchenkov Ivan (IK4),
 *               Belyh Andrey (AB7),
 *               Novozhilova Maryana (MN7),
 *               Porotikov Valery (VP7),
 *               Tretyakov Andrey (AT7),
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 21.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Triangle's tangent space evaluation function.
 * ARGUMENTS:
 *   - pointer to triangle's vertices:
 *       twrVERTEX_STD6 *V0, *V1, *V2;
 * RETURNS: None.
 */
static VOID TWR_EvalTriangleTS( twrVERTEX_STD6 *V0, twrVERTEX_STD6 *V1, twrVERTEX_STD6 *V2 )
{
  twrVERTEX_STD6 Q1, Q2;
  VEC3 Tng = Vec3Set1(0), Btn = Vec3Set1(0);
  FLT s1, s2, t1, t2, d;

  Q1.P = Vec3SubVec3(V1->P, V0->P);
  Q2.P = Vec3SubVec3(V2->P, V0->P);

  Q1.T = Vec2SubVec2(V1->T, V0->T);
  Q2.T = Vec2SubVec2(V2->T, V0->T);

  s1 = Q1.T.X;
  t1 = Q1.T.Y;
  s2 = Q2.T.X;
  t2 = Q2.T.Y;

  d = (s1 * t2 - s2 * t1);
  Tng = Vec3Set((t2 * Q1.P.X - t1 * Q2.P.X) / d,
                (t2 * Q1.P.Y - t1 * Q2.P.Y) / d,
                (t2 * Q1.P.Z - t1 * Q2.P.Z) / d);

  Btn = Vec3Set((-s2 * Q1.P.X - s1 * Q2.P.X) / d,
                (-s2 * Q1.P.Y - s1 * Q2.P.Y) / d,
                (-s2 * Q1.P.Z - s1 * Q2.P.Z) / d);

  /* Orthogonalization Gram–Schmidt */
  V0->N = Vec3Normalize(V0->N);
  Tng = Vec3Normalize(Vec3SubVec3(Tng, Vec3MulNum(V0->N, Vec3DotVec3(V0->N, Tng))));
  Btn = Vec3Normalize(Vec3SubVec3(Vec3SubVec3(Btn, Vec3MulNum(V0->N, Vec3DotVec3(V0->N, Btn))),
                                  Vec3SubVec3(Tng, Vec3MulNum(V0->N, Vec3DotVec3(V0->N, Tng)))));
  V0->Tng = Vec3AddVec3(V0->Tng, Tng);
  V0->Btn = Vec3AddVec3(V0->Btn, Btn);
  V1->Tng = Vec3AddVec3(V0->Tng, Tng);
  V1->Btn = Vec3AddVec3(V0->Btn, Btn);
  V2->Tng = Vec3AddVec3(V0->Tng, Tng);
  V2->Btn = Vec3AddVec3(V0->Btn, Btn);
} /* End of 'TWR_EvalTriangleTS' function */

/* Initialize topology system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyInit( VOID )
{
  Twr->EvalTriangleTS = TWR_EvalTriangleTS;
  Twr->Sys("Topology system is initialized");
} /* End of 'TWR_SystemTopologyInit' function */

/* Delete topology system function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTopologyClose( VOID )
{
  Twr->Sys("Topology system is deinitialized");
} /* End of 'TWR_SystemTopologyClose' function */

/* END OF 'twr_res_topology.c' FILE */
