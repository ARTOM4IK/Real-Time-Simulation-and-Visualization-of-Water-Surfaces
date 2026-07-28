/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_mth_cam.h
 * PURPOSE     : Tough Water Rendering project.
 *               Camera functions declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 16.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_mth_cam_h_
#define __twr_mth_cam_h_

/* Setting camera function.
 * ARGUMENTS:
 *   - camera data pointer:
 *       CAMERA *Cam;
 *   - vectors specyfying direction of camera:
 *       VEC3 Loc, At, Up;
 * RETURNS: None.
 */
__inline VOID CamSet( CAMERA *Cam, VEC3 Loc, VEC3 At, VEC3 Up )
{
  Cam->MatrView = MatrView(Loc, At, Up);
  Cam->Right = Vec3Set(Cam->MatrView.A[0][0], Cam->MatrView.A[1][0], Cam->MatrView.A[2][0]);
  Cam->Up = Vec3Set(Cam->MatrView.A[0][1], Cam->MatrView.A[1][1], Cam->MatrView.A[2][1]);
  Cam->Dir = Vec3Set(-Cam->MatrView.A[0][2], -Cam->MatrView.A[1][2], -Cam->MatrView.A[2][2]);
  Cam->Loc = Loc;
  Cam->At = At;

  /* Recalculate projection */
  Cam->MatrVP = MatrMulMatr(Cam->MatrView, Cam->MatrProj);
} /* End of 'CamSet' function */

/* Setting camera frame size function.
 * ARGUMENTS:
 *   - camera data pointer:
 *       CAMERA *Cam;
 *   - camera projection data (size, project distance, far clip plane distance):
 *       FLT ProjSize, ProjDist, ProjFarClip;
 * RETURNS: None.
 */
__inline VOID CamSetProj( CAMERA *Cam, FLT ProjSize, FLT ProjDist, FLT ProjFarClip )
{
  Cam->ProjDist = ProjDist;
  Cam->ProjFarClip = ProjFarClip;

  Cam->Wp = Cam->Hp = Cam->ProjSize = ProjSize;
  /* Correct aspect ratio */
  if (Cam->FrameW >= Cam->FrameH)
    Cam->Wp *= (DBL)Cam->FrameW / Cam->FrameH;
  else
    Cam->Hp *= (DBL)Cam->FrameH / Cam->FrameW;

  Cam->MatrProj =
    MatrFrustum(-Cam->Wp / 2, Cam->Wp / 2, -Cam->Hp / 2, Cam->Hp / 2,
      Cam->ProjDist, Cam->ProjFarClip);
  Cam->MatrVP = MatrMulMatr(Cam->MatrView, Cam->MatrProj);
} /* End of 'CamSetProj' function */

/* Setting projection data function.
 * ARGUMENTS:
 *   - camera data pointer:
 *       CAMERA *Cam;
 *   - camera frame size (in pixels):
 *       INT FrameW, FrameH;
 * RETURNS: None.
 */
__inline VOID CamSetSize( CAMERA *Cam, INT FrameW, INT FrameH )
{
  /* Setup projection */  
  Cam->FrameW = FrameW;
  Cam->FrameH = FrameH;

  /* Recalculate projection */
  CamSetProj(Cam, Cam->ProjSize, Cam->ProjDist, Cam->ProjFarClip);
} /* End of 'CamSetSize' function */

#endif /* __twr_mth_cam_h_ */

/* END OF 'twr_mth_cam.h' FILE */
