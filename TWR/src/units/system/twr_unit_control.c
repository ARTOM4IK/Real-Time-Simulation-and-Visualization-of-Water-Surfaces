/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_unit_control.c
 * PURPOSE     : Tough Water Rendering project.
 *               Unit control implementation file.
 * PROGRAMMER  : CGSG'2024.
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Control unit description type */
typedef struct tagtwrUNIT_CTRL
{
  TWR_UNIT_BASE_FIELDS;          /* Base fields: Init, Close, Response, Render */
  twrFONT *LogoFnt;              /* Logo font */
  INT Pov;
} twrUNIT_CTRL;

/* Unit initialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_CTRL *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitInit( twrUNIT_CTRL *Uni )
{
  Uni->Pov = 1;
  CamSet(&Twr->Cam, Twr->Cam.Loc, Twr->Cam.At, Vec3Set(0, 1, 0));
  Uni->LogoFnt = Twr->FntCreate("CGSG-Logo", Vec4Set(1, 1, 1, 0.8));
} /* End of 'TWR_UnitInit' function */

/* Unit deinitialization function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_CTRL *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitClose( twrUNIT_CTRL *Uni )
{
} /* End of 'TWR_UnitClose' function */

/* Unit inter frame events handle function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_CTRL *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitResponse( twrUNIT_CTRL *Uni )
{
  INT modes[2];
  DBL Dist = Vec3Len(Vec3SubVec3(Twr->Cam.At, Twr->Cam.Loc));

  /*** Common usage control ***/
  /* Set next scene */
  if (Twr->Keys[VK_SHIFT] && Twr->KeysClick['C'])
    Twr->IsNextSceneToSet = TRUE;
  /* Pause */
  if (Twr->Keys[VK_SHIFT] && Twr->KeysClick['P'])
    Twr->IsPause = !Twr->IsPause;
  /* Debug toggle mode */
  if (Twr->Keys[VK_SHIFT] && Twr->KeysClick['D'])
    Twr->IsDebug = !Twr->IsDebug;
  /* Full screen */
  if (Twr->KeysClick[VK_F11])
    Twr->FlipFullScreen();
  /* Set camera to the initial position */
  if (Twr->Keys[VK_CONTROL] && Twr->KeysClick['F'])
    CamSet(&Twr->Cam, Vec3Set(8, 8, 8), Vec3Set(0, 0, 0), Vec3Set(0, 1, 0));

  /* Wireframe / solid switch */
  if (Twr->Keys[VK_SHIFT] && Twr->KeysClick['W'])
  {
    glGetIntegerv(GL_POLYGON_MODE, modes);
    if (modes[0] == GL_LINE)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  /* Camera handle */
  if (Twr->Keys[VK_LCONTROL])
  {
    /* Rotation handle */
    DBL
      cosT = (Twr->Cam.Loc.Y - Twr->Cam.At.Y) / Dist,
      sinT = sqrt(1 - cosT * cosT),
      plen = Dist * sinT,
      cosP = (Twr->Cam.Loc.Z - Twr->Cam.At.Z) / plen,
      sinP = (Twr->Cam.Loc.X - Twr->Cam.At.X) / plen,
      Azimuth = R2D(atan2(sinP, cosP)),
      Elevator = R2D(atan2(sinT, cosT)),
      coef = 8,
      speed = 0;

    if (Twr->KeysClick['V'])
    {
      Uni->Pov = !Uni->Pov;
      if (Uni->Pov == 1)
        Dist = 20, speed = 0;
    }
    if (Uni->Pov == 0)
    {
      Dist = 0.1;
      speed = 5;
      if (Twr->Keys['W'] || Twr->Keys['A'] || Twr->Keys['S'] || Twr->Keys['D'] || Twr->Keys['Q'] || Twr->Keys['E'])
      {
        VEC3 Dir = {0};

        if (Twr->Keys['S'])
          Dir = Vec3Neg(Twr->Cam.Dir);
        if (Twr->Keys['W'])
          Dir = Twr->Cam.Dir;
        if (Twr->Keys['A'])
          Dir = Vec3Normalize(Vec3Neg(Twr->Cam.Right));
        if (Twr->Keys['D'])
          Dir = Vec3Normalize(Twr->Cam.Right);
        if (Twr->Keys['Q'])
          Dir = Vec3Neg(Twr->Cam.Up);
        if (Twr->Keys['E'])
          Dir = Twr->Cam.Up;

        if ((Twr->Keys['A'] && Twr->Keys['D']) || (Twr->Keys['S'] && Twr->Keys['W']))
          Dir = Vec3Set1(0);

        if (Twr->Keys['W'] && Twr->Keys['D'])
          Dir = Vec3Normalize(Vec3AddVec3(Twr->Cam.Dir, Twr->Cam.Right));
        if (Twr->Keys['W'] && Twr->Keys['A'])
          Dir = Vec3Normalize(Vec3AddVec3(Twr->Cam.Dir, Vec3Neg(Twr->Cam.Right)));
        if (Twr->Keys['S'] && Twr->Keys['A'])
          Dir = Vec3Normalize(Vec3AddVec3(Vec3Neg(Twr->Cam.Right), Vec3Neg(Twr->Cam.Right)));
        if (Twr->Keys['S'] && Twr->Keys['D'])
          Dir = Vec3Normalize(Vec3AddVec3(Vec3Neg(Twr->Cam.Dir), Twr->Cam.Right));

        Twr->Cam.Loc = Vec3AddVec3(Twr->Cam.Loc, Vec3MulNum(Dir, Twr->DeltaTime * 60 + Twr->Keys[VK_SHIFT])), Twr->Cam.At = Vec3AddVec3(Twr->Cam.At, Vec3MulNum(Dir, Twr->DeltaTime * 30 + Twr->Keys[VK_SHIFT]));
      }
    }

    if (Twr->Keys[VK_SHIFT])
      coef = 102;

    Azimuth +=
       log10(1.1 + Dist * Dist + speed) * (-1 * Twr->Keys[VK_LBUTTON] * 0.05 * Twr->Mdx * log10(10 + Dist * Dist) +
       Twr->GlobalDeltaTime * coef * (Twr->Keys[VK_LEFT] - Twr->Keys[VK_RIGHT]) * (speed + 1));

    Elevator +=
       log10(1.1 + Dist * Dist + speed) * (-1 * Twr->Keys[VK_LBUTTON] * 0.05 * Twr->Mdy +
       Twr->GlobalDeltaTime * coef * (Twr->Keys[VK_UP] - Twr->Keys[VK_DOWN]) * (speed + 1));

    if (Elevator < 0.08)
      Elevator = 0.08;
    else if (Elevator > 178.90)
      Elevator = 178.90;

    Dist +=
      log10(1.1 + Dist * Dist) * (1 + Twr->Keys[VK_SHIFT] * 8) *
      (0.005 * Twr->Mdz + Twr->GlobalDeltaTime * 18 * (Twr->Keys[VK_NEXT] - Twr->Keys[VK_PRIOR]));
    if (Dist < 0.05)
      Dist = 0.05;

    /* Handle camera position */
    if (Twr->Keys[VK_RBUTTON])
    {
      DBL Wp, Hp, sx, sy;
      VEC3 dv;

      Wp = Twr->Cam.ProjSize;
      Hp = Twr->Cam.ProjSize;

      if (Twr->W > Twr->H)
        Wp *= (DBL)Twr->W / Twr->H;
      else
        Hp *= (DBL)Twr->H / Twr->W;

      sx = -Twr->Mdx * Wp / Twr->W * Dist / Twr->Cam.ProjDist;
      sy = Twr->Mdy * Hp / Twr->H * Dist / Twr->Cam.ProjDist;

      dv = Vec3AddVec3(Vec3MulNum(Twr->Cam.Right, sx),
                       Vec3MulNum(Twr->Cam.Up, sy));

      Twr->Cam.At = Vec3AddVec3(Twr->Cam.At, dv);
      Twr->Cam.Loc = Vec3AddVec3(Twr->Cam.Loc, dv);
      /*Twr->Cam.At = Vec3AddVec3(Twr->Cam.At, Vec3MulNum(Vec3SubVec3(dv, Twr->Cam.At), Twr->GlobalDeltaTime));*/
      /*Twr->Cam.Loc = Vec3AddVec3(Twr->Cam.Loc, Vec3MulNum(dv, Twr->GlobalDeltaTime));*/
    }

    /* Setup result camera */
    CamSet(&Twr->Cam, PointTransform(Vec3Set(0, Dist, 0),
                                     MatrMulMatr3(MatrRotateX(Elevator),
                                                  MatrRotateY(Azimuth),
                                                  MatrTranslate(Twr->Cam.At))),
                                     Twr->Cam.At,
                                     Vec3Set(0, 1, 0));
  }
} /* End of 'TWR_UnitResponse' function */

/* Unit render function.
 * ARGUMENTS:
 *   - self-pointer to unit object:
 *       twrUNIT_CTRL *Uni;
 * RETURNS: None.
 */
static VOID TWR_UnitRender( twrUNIT_CTRL *Uni )
{
  if (Uni->LogoFnt != NULL)
  {
    /* Test output: top left and right logo drawing:
    Twr->FntDraw(Uni->LogoFnt, "G", Vec3Set(0, 0, 0), 0);
    Twr->FntDraw(Uni->LogoFnt, "G", Vec3Set(Twr->W - Uni->LogoFnt->AdvanceX['G'] * Uni->LogoFnt->LineH, 0, 0), 0);
    */
    /* Bottom-right logo drawing */
    Twr->FntDraw(Uni->LogoFnt, "G", Vec3Set(Twr->W - Uni->LogoFnt->AdvanceX['G'] * Uni->LogoFnt->LineH, -Twr->H + 1 * Uni->LogoFnt->LineH, -1), 0);
  }
} /* End of 'TWR_UnitRender' function */

/* Control unit sample creation function.
 * ARGUMENTS:
 *   - creation parameters (optional, may be NULL):
 *       twrPARAMS *Params;
 * RETURNS:
 *   (twrUNIT *) pointer to unit;
 */
twrUNIT * TWR_UnitCreateControl( twrPARAMS *Params )
{
  twrUNIT *Uni;

  /* Memory allocation */
  if ((Uni = Twr->UnitCreate(sizeof(twrUNIT_CTRL))) == NULL)
    return NULL;
  /* Setup unit methods */
  Uni->Init = (VOID *)TWR_UnitInit;
  Uni->Close = (VOID *)TWR_UnitClose;
  Uni->Response = (VOID *)TWR_UnitResponse;
  Uni->Render = (VOID *)TWR_UnitRender;

  return Uni;
} /* End of 'TWR_UnitCreateControl' function */

/* END OF 'twr_unit_control.c' FILE */
