/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_lights.c
 * PURPOSE     : Tough Water Rendering project.
 *               Lights module.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 14.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
#include "twr_rnd_lights.h"

/* Shadow map size */ 
#define TWR_SHADOW_MAP_SIZE (4096 * 1)

/* Stock of lights */
static STOCK TWR_StkLights;

/* Try to store MtlPats, Mtls
 * to get rid of searching them in stock
 * (we know exact number of them) */

/* Light material patterns */
static twrMATERIAL_PATTERN
  *LightMtlPatAmbient,  /* Ambient light material pattern */
  *LightMtlPatPoint,    /* Point light material pattern */
  *LightMtlPatDir,      /* Directional light material pattern */
  *LightMtlPatSpot;     /* Spot light material pattern */

/* Light materials */
static twrMATERIAL
  *LightMtlAmbient[2],  /* Ambient light material */
  *LightMtlPoint[2],    /* Point light material */
  *LightMtlDir[2],      /* Directional light material */
  *LightMtlSpot[2];     /* Spot light material */

/* Light materials */
static twrPRIM
  *LightPrimAmbient[2], /* Ambient light primitive */
  *LightPrimPoint[2],   /* Point light primitive */
  *LightPrimDir[2],     /* Directional light primitive */
  *LightPrimSpot[2];    /* Spot light primitive */

/* Update light source data on GPU function.
 * ARGUMENTS:
 *   - light source pointer:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightUpdate( twrLIGHT *Lig )
{
  twrRND_BUF_LIGHT LigData;

  /* Change data */
  LigData.PosCutOffAngle1 = 
    Vec4SetVec3(Lig->Pos, Lig->CutOffAngleCos.X = cos(D2R(Lig->CutOffAngle.X)));
  LigData.DirCutOffAngle2 =
    Vec4SetVec3(Lig->Dir, Lig->CutOffAngleCos.Y = cos(D2R(Lig->CutOffAngle.Y)));
  LigData.ColorCutOffDist1 =
    Vec4SetVec3(Lig->Color, Lig->CutOffDist.X);
  LigData.AttCutOffDist2 =
    Vec4SetVec3(Lig->Att, Lig->CutOffDist.Y);
  LigData.IsShadowLightTypeCutAngle12 =
    Vec4Set(Lig->IsShadowCast, Lig->Type,
            Lig->CutOffAngle.X, Lig->CutOffAngle.Y);
  LigData.ShadowMatr = Lig->ShadowMatr;

  /* Update buffer */
  Twr->BufUpdate(Lig->Buffer, 0, sizeof(twrRND_BUF_LIGHT), &LigData);
} /* End of 'TWR_LightUpdate' function */

/* Add light source function.
 * ARGUMENTS:
 *   - light source type:
 *       twrLIGHT_TYPE Type;
 * RETURNS:
 *   (twrLIGHT *) pointer to add light source or NULL if no space for a
 *   new light source.
 */
static twrLIGHT * TWR_LightAdd( twrLIGHT_TYPE Type )
{
  twrLIGHT *L;
  static CHAR Buf[102];
  static CHAR *Names[] =
  {
    "Ambient",
    "Point",
    "Directional",
    "Spot",
  };
  static twrPRIM **LightPrims[] =
  {
    LightPrimAmbient,
    LightPrimPoint,
    LightPrimDir,
    LightPrimSpot,
  };

  if (Type < TWR_LIGHT_AMBIENT || Type > TWR_LIGHT_SPOT)
    return NULL;

  if ((L = StockAlloc(&TWR_StkLights)) == NULL)
    return NULL;

  /* Fill with data */
  L->Type = Type;
  L->Pos = Vec3Set(0, 1, 0);
  L->Dir = Vec3Set(0, 1, 0);
  L->Color = Vec3Set1(1);
  L->Att = Vec3Set(1, 0, 0);
  L->CutOffDist = Vec2Set(8, 18);
  L->CutOffAngle = Vec2Set(30, 47);
  L->IsShadowCast = FALSE;
  L->IsEnabled = TRUE;

  L->Prim[0] = LightPrims[Type][0];
  L->Prim[1] = LightPrims[Type][1];

  /* Create, update and apply light buffer */
  wsprintf(Buf, "Light source: %s", Names[Type]);
  L->Buffer = Twr->BufCreate(Buf, TWR_BUF_BIND_LIGHTS, sizeof(twrRND_BUF_LIGHT), NULL);
  Twr->BufReferenceInc(L->Buffer);
  Twr->BufApply(L->Buffer);
  Twr->LightUpdate(L);

  return L;
} /* End of 'TWR_LightAdd' function */

/* Add point light source function.
 * ARGUMENTS:
 *   - point light source position:
 *       VEC3 Pos;
 *   - point light source color:
 *       VEC3 Color;
 *   - point light source attenuation:
 *       VEC3 Att;
 *   - point light source cut-off distance:
 *       VEC3 CutOffDist;
 * RETURNS:
 *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
 */
static twrLIGHT * TWR_LightAddPoint( VEC3 Pos, VEC3 Color, VEC3 Att, VEC2 CutOffDist )
{
  twrLIGHT *L;
 
  if ((L = TWR_LightAdd(TWR_LIGHT_POINT)) == NULL)
    return NULL;
  L->Pos = Pos;
  L->Color = Color;
  L->Att = Att;
  L->CutOffDist = CutOffDist;
  TWR_LightUpdate(L);

  return L;
} /* End of 'TWR_LightAddPoint' function */

/* Add directional light source function.
 * ARGUMENTS:
 *   - directional light source direction:
 *       VEC3 Dir;
 *   - directional light source color:
 *       VEC3 Color;
 * RETURNS:
 *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
 */
static twrLIGHT * TWR_LightAddDir( VEC3 Dir, VEC3 Color )
{
  twrLIGHT *L;

  if ((L = TWR_LightAdd(TWR_LIGHT_DIR)) == NULL)
    return NULL;
  L->Dir = Dir;
  L->Color = Color;
  TWR_LightUpdate(L);

  return L;
} /* End of 'TWR_LightAddDir' function */

/* Add spot light source function.
 * ARGUMENTS:
 *   - spot light source position:
 *       VEC3 Pos;
 *   - spot light source look-at point:
 *       VEC3 At;
 *   - spot light source color:
 *       VEC3 Color;
 *   - spot light source attenuation:
 *       VEC3 Att;
 *   - spot light source cut-off angle:
 *       VEC2 CutOffAngle;
 *   - spot light source cut-off distance:
 *       VEC2 CutOffDist;
 * RETURNS:
 *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
 */
static twrLIGHT * TWR_LightAddSpot( VEC3 Pos, VEC3 At, VEC3 Color, VEC3 Att, VEC2 CutOffDist, VEC2 CutOffAngle )
{
  twrLIGHT *L;
  
  if ((L = TWR_LightAdd(TWR_LIGHT_SPOT)) == NULL)
    return NULL;
  L->Pos = Pos;
  L->Color = Color;
  L->Dir = Vec3Normalize(Vec3SubVec3(At, Pos));
  L->CutOffAngle = CutOffAngle;
  L->CutOffAngleCos = Vec2Set(cos(D2R(CutOffAngle.X)), cos(D2R(CutOffAngle.Y)));
  L->Att = Att;
  L->CutOffDist = CutOffDist;
  TWR_LightUpdate(L);

  return L;
} /* End of 'TWR_LightAddSpot' function */

/* Enable shadow cast in light source function.
 * ARGUMENTS:
 *   - light source pointer:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightEnableShadow( twrLIGHT *Lig )
{
  /* Check if light or FBO already exists */
  if (Lig == NULL || Lig->Type == TWR_LIGHT_AMBIENT || Lig->Type == TWR_LIGHT_POINT || Lig->ShadowTarget != NULL)
    return;

  /* Create shadow FBO */
  Lig->ShadowTarget = Twr->RndTargetCreate("Light shadow target", 0, TRUE, TWR_SHADOW_MAP_SIZE, TWR_SHADOW_MAP_SIZE);

  Lig->IsShadowCast = TRUE;
  Lig->ShadowSize = TWR_SHADOW_MAP_SIZE;
} /* End of 'TWR_LightEnableShadow' function */

/* Enable light source function.
 * ARGUMENTS:
 *   - light source pointer:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightEnable( twrLIGHT *Lig )
{
  Lig->IsEnabled = TRUE;
} /* End of 'TWR_LightEnable' function */

/* Disable light source function.
 * ARGUMENTS:
 *   - light source pointer:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightDisable( twrLIGHT *Lig )
{
  Lig->IsEnabled = FALSE;
} /* End of 'TWR_LightDisable' function */

/* Redraw all light sources shadow function.
 * ARGUMENTS:
 *   - render pass flag:
 *       twrTRANSPARENCY_FLAG PassFlag;
 * RETURNS: None.
 */
static VOID TWR_LightRedrawShadows( twrTRANSPARENCY_FLAG PassFlag )
{
  twrLIGHT *L;

  L = StockFirst(&TWR_StkLights);

  /* Shadow cast pass */
  while (L != NULL)
  {
    if (L->IsShadowCast && L->IsEnabled)
      if (L->Type == TWR_LIGHT_DIR)
      {
#if 0
        CAMERA OldCam = Twr->Cam;

        /* Set camera from light source */
        Twr->Cam.MatrProj = MatrOrtho(-size, size, -size, size,
          -(size + 47), far_dist);
        CamSet(&Twr->Cam, Twr->Cam.At, Vec3SubVec3(Twr->Cam.At, L->Dir),
               Vec3Set(0, 1, 0));
        L->ShadowMatr = Twr->Cam.MatrVP;
        Twr->LightUpdate(L);

        /*
        Twr->CamBufUpdate(Twr->Cam);
        Twr->GlobalMatrBufsCompute();
        */
        /* Set shadow map frame buffer */
        /*
        Twr->RndTargetApplyClear(L->ShadowTarget);
        glBindFramebuffer(GL_FRAMEBUFFER, L->ShadowTarget[PassFlag]->FBOId);
        glBindFramebuffer(GL_FRAMEBUFFER, TWR_SHADOW_PASS);
        */

        /* Redraw all scene */
        Twr->PostPassDrawShadows(L->ShadowTarget, Twr->Cam, TWR_MTL_SHADOW);

        /* Restore camera data */
        Twr->Cam = OldCam;
        Twr->CamBufUpdate(Twr->Cam);
#endif /* 0 */
        INT i;
        MATR InvVP = MatrInverse(Twr->Cam.MatrVP);
        CAMERA OldCam = Twr->Cam;
        FLT Z = 1;
        VEC3
          Corners[8], 
          NDC_Corners[8] =
          {
            {-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1},
            {-1, -1, Z}, {1, -1, Z}, {1, 1, Z}, {-1, 1, Z}
          };
        FLT ShadowDistance = 5, ShadowTolerance = 0, fr_left, fr_right, fr_bottom, fr_top, fr_near, fr_far;
        MATR LightView;
        VEC3 MinBB = {0}, MaxBB = {0};
        CAMERA cc = OldCam;

        CamSetProj(&cc, cc.ProjSize, cc.ProjDist, 1.1 * Vec3Distance(cc.Loc, cc.At));
        InvVP = MatrInverse(cc.MatrVP);

        CamSet(&Twr->Cam, Vec3AddVec3(Twr->Cam.At, L->Dir), Twr->Cam.At, Vec3Set(0, 1, 0));
        LightView = Twr->Cam.MatrView;

        /* Obtain camera frustum corners points in world CS */
        for (i = 0; i < 8; i++)
          Corners[i] = Vec3MulMatr(NDC_Corners[i], InvVP);

        /* Obtain min-max bound box in camera view CS */
        for (i = 0; i < 8; i++)
        {
          VEC3 p = PointTransform(Corners[i], LightView);

          if (i == 0)
            MinBB = MaxBB = p;
          else
          {
            MinBB = Vec3MinVec3(MinBB, p);
            MaxBB = Vec3MaxVec3(MaxBB, p);
          }
        }
        fr_left = MinBB.X;
        fr_right = MaxBB.X;
        fr_bottom = MinBB.Y;
        fr_top = MaxBB.Y;
        fr_near = MinBB.Z - ShadowDistance;
        fr_far = MaxBB.Z + ShadowDistance;

        /* Set camera from light source */
        Twr->Cam.MatrProj =
          MatrOrtho(fr_left - ShadowTolerance, fr_right + ShadowTolerance,
                    fr_bottom - ShadowTolerance, fr_top + ShadowTolerance,
                    -fr_far - ShadowTolerance - 50, -fr_near + ShadowTolerance);
                    //fr_near - ShadowTolerance, fr_far + ShadowTolerance);
        Twr->Cam.MatrVP = MatrMulMatr(Twr->Cam.MatrView, Twr->Cam.MatrProj);

        L->ShadowMatr = Twr->Cam.MatrVP;
        Twr->LightUpdate(L);

        /* Redraw all scene */
        Twr->PostPassDrawShadows(L->ShadowTarget, Twr->Cam, TWR_MTL_SHADOW);

        /* Restore camera data */
        Twr->Cam = OldCam;
        Twr->CamBufUpdate(Twr->Cam);
      }
      else if (L->Type == TWR_LIGHT_SPOT)
      {
        CAMERA OldCam = Twr->Cam;
        FLT
          clear_depth = 1,
          proj_dist = 0.1, far_clip = L->CutOffDist.Y * 1.2,
          size = tan(D2R(L->CutOffAngle.Y)) * proj_dist;

        /* Set camera from light source */
        Twr->Cam.MatrProj = MatrFrustum(-size, size, -size, size, proj_dist, far_clip);
        CamSet(&Twr->Cam, L->Pos, Vec3AddVec3(L->Pos, L->Dir), Vec3Set(0, 1, 0));
        L->ShadowMatr = Twr->Cam.MatrVP;
        Twr->LightUpdate(L);

        /* redraw all scene */
        Twr->PostPassDrawShadows(L->ShadowTarget, Twr->Cam, TWR_MTL_SHADOW);

        /* Restore camera data */
        Twr->Cam = OldCam;
        Twr->CamBufUpdate(Twr->Cam);
      }
      L = StockNext(L);
  }
  Twr->GlobalMatrBufsCompute();
} /* End of 'TWR_LightRedrawShadows' function */

/* Redraw all light sources function.
 * ARGUMENTS:
 *   - render pass flag:
 *       twrTRANSPARENCY_FLAG PassFlag;
 * RETURNS: None.
 */
static VOID TWR_LightRedraw( twrTRANSPARENCY_FLAG PassFlag )
{
  twrLIGHT *L;

  /* Get first light from stock of lights */
  L = StockFirst(&TWR_StkLights);

  /* Output light sources */
  Twr->ToggleBlend(TWR_BLEND_ADD);
  Twr->ToggleDepth(TWR_DEPTH_NOT_READ_NOT_WRTIE);
  while (L != NULL)
  { 
    if (L->IsEnabled && Twr->ShdApply(L->Prim[PassFlag]->Mtl->MtlPat->Shd, TWR_MTL_FINAL))
    {
      Twr->BufApply(L->Buffer);
      Twr->MtlApply(L->Prim[PassFlag]->Mtl);
      if (L->IsShadowCast)
      {
        glActiveTexture(GL_TEXTURE0 + 6);
        glBindTexture(GL_TEXTURE_2D, L->ShadowTarget->Depth->TexId);
      }
      Twr->PrimDrawInstant(L->Prim[PassFlag]);
    }
    L = StockNext(L);
  }

  Twr->ToggleBlend(TWR_BLEND_ALPHA);
  Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
} /* End of 'TWR_LightRedraw' function */

/* Set light position function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new position:
 *       VEC3 Pos;
 * RETURNS: None.
 */
static VOID TWR_LightSetPos( twrLIGHT *Lig, VEC3 Pos )
{
  if (Lig == NULL)
    return;

  Lig->Pos = Pos;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetPos' function */

/* Set light direction function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new direction:
 *       VEC3 Dir;
 * RETURNS: None.
 */
static VOID TWR_LightSetDir( twrLIGHT *Lig, VEC3 Dir )
{
  if (Lig == NULL)
    return;

  Lig->Dir = Dir;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetDir' function */

/* Set light color function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new color:
 *       VEC3 Color;
 * RETURNS: None.
 */
static VOID TWR_LightSetColor( twrLIGHT *Lig, VEC3 Color )
{
  if (Lig == NULL)
    return;

  Lig->Color = Color;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetColor' function */

/* Set light attenuation function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new attenuation:
 *       VEC3 Att;
 * RETURNS: None.
 */
static VOID TWR_LightSetAtt( twrLIGHT *Lig, VEC3 Att )
{
  if (Lig == NULL)
    return;

  Lig->Att = Att;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetAtt' function */

/* Set light cut-off angle function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new cut-off angle:
 *       VEC2 CutOffAngle;
 * RETURNS: None.
 */
static VOID TWR_LightSetAngle( twrLIGHT *Lig, VEC2 CutOffAngle )
{
  if (Lig == NULL)
    return;

  Lig->CutOffAngle = CutOffAngle;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetAngle' function */

/* Set light cut-off angle cosine function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new cut-off angle cosine:
 *       VEC2 Att;
 * RETURNS: None.
 */
static VOID TWR_LightSetCos( twrLIGHT *Lig, VEC2 CutOffAngleCos )
{
  if (Lig == NULL)
    return;

  Lig->CutOffAngleCos = CutOffAngleCos;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetCos' function */

/* Set light cut-off distance function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 *   - new cut-off distance:
 *       VEC2 CutOffDist;
 * RETURNS: None.
 */
static VOID TWR_LightSetDist( twrLIGHT *Lig, VEC2 CutOffDist )
{
  if (Lig == NULL)
    return;

  Lig->CutOffDist = CutOffDist;
  Twr->LightUpdate(Lig);
} /* End of 'TWR_LightSetDist' function */

/* Delete light from stock function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightFree( twrLIGHT *Lig )
{
  if (Lig == NULL)
    return;

  if (Lig->IsShadowCast)
    Twr->RndTargetFree(Lig->ShadowTarget);
  Twr->BufFree(Lig->Buffer);
  Twr->PrimFree(Lig->Prim[0]);
  Twr->PrimFree(Lig->Prim[1]);

  StockDelete(Lig);
} /* End of 'TWR_LightFree' function */

/* Delete lights strategy function.
 * ARGUMENTS:
 *   - light source:
 *       twrLIGHT *Lig;
 * RETURNS: None.
 */
static VOID TWR_LightsDeleteStrategy( twrLIGHT *Lig )
{
  Twr->BufFree(Lig->Buffer);
  if (Lig->IsShadowCast)
    Twr->RndTargetFree(Lig->ShadowTarget);
} /* End of 'TWR_LightsDeleteStrategy' function */

/* Light system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLightsInit( VOID )
{
  INT i, j;

  Twr->LightAddDir = TWR_LightAddDir;
  Twr->LightAddSpot = TWR_LightAddSpot;
  Twr->LightAddPoint = TWR_LightAddPoint;
  Twr->LightEnableShadow = TWR_LightEnableShadow;
  Twr->LightRedraw = TWR_LightRedraw;
  Twr->LightRedrawShadows = TWR_LightRedrawShadows;
  Twr->LightUpdate = TWR_LightUpdate;
  Twr->LightEnable = TWR_LightEnable;
  Twr->LightDisable = TWR_LightDisable;
  Twr->LightSetPos = TWR_LightSetPos;
  Twr->LightSetDir = TWR_LightSetDir;
  Twr->LightSetColor = TWR_LightSetColor;
  Twr->LightSetAtt = TWR_LightSetAtt;
  Twr->LightSetAngle = TWR_LightSetAngle;
  Twr->LightSetCos = TWR_LightSetCos;
  Twr->LightSetDist = TWR_LightSetDist;
  Twr->LightFree = TWR_LightFree;

  /* Create material patterns */
  LightMtlPatAmbient = Twr->MtlPatCreate("LightMtlPatAmbient", "lights/ambient", 6, 0, 0, NULL, 0);
  Twr->MtlPatReferenceInc(LightMtlPatAmbient);
  LightMtlPatPoint = Twr->MtlPatCreate("LightMtlPatPoint", "lights/point", 6, 0, 0, NULL, 0);
  Twr->MtlPatReferenceInc(LightMtlPatPoint);
  LightMtlPatSpot = Twr->MtlPatCreate("LightMtlPatSpot", "lights/spot", 7, 0, 0, NULL, 0);
  Twr->MtlPatReferenceInc(LightMtlPatSpot);
  LightMtlPatDir = Twr->MtlPatCreate("LightMtlPatDir", "lights/dir", 7, 0, 0, NULL, 0);
  Twr->MtlPatReferenceInc(LightMtlPatDir);

  for (i = 0; i < 2; i++)
  {
    /* Create light materials */
    LightMtlAmbient[i] = Twr->MtlCreate("AmbientLight", LightMtlPatAmbient, i);
    Twr->MtlReferenceInc(LightMtlAmbient[i]);
    LightMtlPoint[i] = Twr->MtlCreate("PointLight", LightMtlPatPoint, i);
    Twr->MtlReferenceInc(LightMtlPoint[i]);
    LightMtlSpot[i] = Twr->MtlCreate("SpotLight", LightMtlPatSpot, i);
    Twr->MtlReferenceInc(LightMtlSpot[i]);
    LightMtlDir[i] = Twr->MtlCreate("DirectionalLight", LightMtlPatDir, i);
    Twr->MtlReferenceInc(LightMtlDir[i]);

    /* Bind textures to materials */
    for (j = 0; j < 6; j++)
    {
      Twr->MtlBindTex(LightMtlAmbient[i], j, Twr->TargetsPass[i * 2]->Attachments[j]);
      Twr->MtlBindTex(LightMtlPoint[i], j, Twr->TargetsPass[i * 2]->Attachments[j]);
      Twr->MtlBindTex(LightMtlSpot[i], j, Twr->TargetsPass[i * 2]->Attachments[j]);
      Twr->MtlBindTex(LightMtlDir[i], j, Twr->TargetsPass[i * 2]->Attachments[j]);
    }
    /* Create full screen light primitive */
    LightPrimAmbient[i] = Twr->PrimCreate(LightMtlAmbient[i], TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
    LightPrimPoint[i] = Twr->PrimCreate(LightMtlPoint[i], TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
    LightPrimDir[i] = Twr->PrimCreate(LightMtlDir[i], TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
    LightPrimSpot[i] = Twr->PrimCreate(LightMtlSpot[i], TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
  }

  /* Create stock of lights */
  StockCreate(&TWR_StkLights, "Lights", sizeof(twrLIGHT), 300, TWR_LightsDeleteStrategy);

  /* Register stock of lights to strip */
  TWR_StockRegisterToStrip(&TWR_StkLights);

  Twr->Sys("Lights system is initialized");
 
  /* Default ambient light */
  if (TWR_LightAdd(TWR_LIGHT_AMBIENT) != NULL)
    Twr->Info("Ambient light added");
} /* End of 'TWR_SystemLightsInit' function */

/* Light system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemLightsClose( VOID )
{
  INT i;

  for (i = 0; i < 2; i++)
  {
    Twr->PrimFree(LightPrimAmbient[i]);
    Twr->PrimFree(LightPrimPoint[i]);
    Twr->PrimFree(LightPrimDir[i]);
    Twr->PrimFree(LightPrimSpot[i]);
  }

  /* Free stock */
  StockFree(&TWR_StkLights);
  Twr->Sys("Lights system is deinitialized");
} /* End of 'TWR_SystemLightsClose' function */

/* END OF 'twr_rnd_lights.c' FILE */
