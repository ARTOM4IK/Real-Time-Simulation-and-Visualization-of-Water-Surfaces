/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_pipeline.c
 * PURPOSE     : Tough Water Rendering project.
 *               Pipeline system inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 16.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"
/*
#include "water/still/twr_still_water.h"
*/

/* Fullscreen primitives for merge and last targets */
static twrPRIM *TWR_TrgMergeFullscreenPrim,
               *TWR_TrgLastFullscreenPrim;

static twrMATERIAL_PATTERN *SkyMtlPat;   /* Skybox material pattern */
static twrMATERIAL *SkyMtl;              /* Skybox material */
static twrPRIM *SkyPr;                   /* Skybox primitive */

/* Toggle global depth state function.
 * ARGUMENTS:
 *   - new depth state:
 *       twrDEPTH_FLAG DepthFlag;
 * RETURNS: None.
 */
static VOID TWR_RndToggleDepth( twrDEPTH_FLAG DepthFlag )
{
  /* Check depth reading flag and set appropriate GL_ state */
  /*  */
  if (DepthFlag & TWR_READ_BIT && !(Twr->DepthState & TWR_READ_BIT))
  {
    glEnable(GL_DEPTH_TEST);
    Twr->DepthState |= TWR_READ_BIT;
  }
  else if ((!(DepthFlag & TWR_READ_BIT)) & (Twr->DepthState & TWR_READ_BIT))
  {
    glDisable(GL_DEPTH_TEST);
    Twr->DepthState &= TWR_WRITE_BIT;
  }
  /* Check depth writing flag and set appropriate GL_ state */
  if ((DepthFlag & TWR_WRITE_BIT) && (!(Twr->DepthState & TWR_WRITE_BIT)))
  {
    glDepthMask(TRUE);
    Twr->DepthState |= TWR_WRITE_BIT;
  }
  else if ((!(DepthFlag & TWR_WRITE_BIT)) && (Twr->DepthState & TWR_WRITE_BIT))
  {
    glDepthMask(FALSE);
    Twr->DepthState &= TWR_READ_BIT;
  }
} /* End of 'TWR_RndToggleDepth' function */

/* Toggle global blend mode function.
 * ARGUMENTS:
 *   - new blend mode:
 *       twrBLEND_MODE BlendMode;
 * RETURNS: None.
 */
static VOID TWR_RndToggleBlend( twrBLEND_MODE BlendMode )
{
  /* Check blending mode and set appropriate GL_ state */
  if (BlendMode == TWR_BLEND_ALPHA && Twr->BlendState != TWR_BLEND_ALPHA)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  else if (BlendMode == TWR_BLEND_ADD && Twr->BlendState != TWR_BLEND_ADD)
    glBlendFunc(GL_ONE, GL_ONE);
  else if (BlendMode == TWR_BLEND_REPLACE && Twr->BlendState != TWR_BLEND_REPLACE)
    glBlendFunc(GL_ONE, GL_ZERO);
  Twr->BlendState = BlendMode;
} /* End of 'TWR_RndToggleBlend' function */

/* Set skybox function.
 * ARGUMENTS:
 *   - name of skybox:
 *       CHAR *Name;
 * RETURNS: None.
 */
VOID TWR_SetSkybox( CHAR *Name )
{
  if (SkyPr != NULL)
  {
    Twr->MtlBindTex(SkyMtl, 0, Twr->TexCreateCubeMap(Name));
    return;
  }
  SkyMtlPat = Twr->MtlPatCreate("Skybox material pattern", "skybox", 1, 0, 0, NULL, 0);
  SkyMtlPat->DepthFlag = TWR_DEPTH_NOT_READ_NOT_WRTIE;
  SkyMtl = Twr->MtlCreate("Skybox material", SkyMtlPat, TWR_MTL_OPAQUE);
  Twr->MtlBindTex(SkyMtl, 0, Twr->TexCreateCubeMap(Name));
  SkyPr = Twr->PrimCreate(SkyMtl, TWR_RND_PRIM_POINTS, NULL, 1, NULL, 0);
} /* End of 'TWR_SetSkybox' function */

/* Update camera buffer function.
 * ARGUMENTS:
 *   - input camera:
 *       CAMERA Cam;
 * RETURNS: None.
 */
static VOID TWR_CamBufUpdate( CAMERA Cam )
{
  twrRND_UBO_CAMERA CamData;

  CamData.MatrVP = Twr->Cam.MatrVP;
  CamData.MatrV = Twr->Cam.MatrView;
  CamData.MatrP = Twr->Cam.MatrProj;

  CamData.CamLoc = Vec4SetVec3(Cam.Loc, 0);
  CamData.CamRight = Vec4SetVec3(Cam.Right, 0);
  CamData.CamUp = Vec4SetVec3(Cam.Up, 0);
  CamData.CamDir = Vec4SetVec3(Cam.Dir, 0);
  CamData.CamAt = Vec4SetVec3(Cam.At, 0);
  CamData.FrameWHProjSizeDist = Vec4Set(Cam.FrameW, Cam.FrameH, Cam.ProjSize, Cam.ProjDist);
  Twr->BufUpdate(Twr->RndShdBufCamera, 0, 0, &CamData);
} /* End of 'TWR_CamBufUpdate' function */

/* Close skybox function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_CloseSkybox( VOID )
{
  if (SkyPr == NULL)
    return;
  Twr->PrimFree(SkyPr);
} /* End of 'TWR_CloseSkybox' function */

/* Draw current skybox function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_DrawSkybox( VOID )
{
  if (Twr->ShdApply(SkyMtlPat->Shd, TWR_MTL_OPAQUE))
  {
    Twr->ToggleDepth(TWR_DEPTH_NOT_READ_NOT_WRTIE);
    Twr->MtlApply(SkyMtl);
    Twr->PrimDrawInstant(SkyPr);
    /* Instantly draw sky primitive to framebuffer */
    Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
  }
} /* End of 'TWR_DrawSkybox' function */

/* Merge opaq and trans targets function.
 * ARGUMENTS:
 *   - pointer to target in which merge result will be drawn:
 *       twrTARGET *Trg;
 *   - flag if final primitives will be drawn:
 *       BOOL IsFinalDraw;
 * RETURNS: None.
 */
static VOID TWR_RndDoMerge( twrTARGET *Trg, BOOL IsFinalDraw )
{
  /* Select merge target or choose default */
  if (Trg == NULL)
    Twr->RndTargetApplyClear(Twr->TargetsPass[TWR_MERGE_PASS]);
  else
    Twr->RndTargetApplyClear(Trg);
  if (Twr->ShdApply(TWR_TrgMergeFullscreenPrim->Mtl->MtlPat->Shd, TWR_MTL_FINAL))
  {
    Twr->MtlApply(TWR_TrgMergeFullscreenPrim->Mtl);
    /*
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    */
    Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
    Twr->ToggleBlend(TWR_BLEND_REPLACE);

    Twr->PrimDrawInstant(TWR_TrgMergeFullscreenPrim);
  }
  /* Draw final primitives if it's necessary */
  if (IsFinalDraw)
    Twr->PostPrepassDraw(Twr->TargetsPass[TWR_MERGE_PASS], Twr->Cam, TWR_MTL_FINAL, FALSE);
} /* End of 'TWR_RndDoMerge' function */

/* Draw final buffer to screen function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_RndDoLast( VOID )
{
  Twr->RndTargetApplyClear(NULL); /* &Twr->TargetsPass[TWR_MERGE_PASS]); */
  Twr->ToggleBlend(TWR_BLEND_REPLACE);
  if (Twr->ShdApply(TWR_TrgLastFullscreenPrim->Mtl->MtlPat->Shd, TWR_MTL_FINAL))
  {
    Twr->MtlApply(TWR_TrgLastFullscreenPrim->Mtl);
    Twr->PrimDrawInstant(TWR_TrgLastFullscreenPrim);
  }
} /* End of 'TWR_RndDoLast' function */

/* Draw every primitive into one target function.
 * ARGUMENTS:
 *   - pointer to aim target:
 *       twrTARGET *Trg;
 *   - pointer to new camera:
 *       CAMERA *Cam;
 *   - pass flag:
 *       twrTRANSPARENCY_FLAG Pass;
 *   - flag if we need to clear target:
 *       BOOL IsTargetClear;
 * RETURNS: None.
 */
static VOID TWR_RndPostPrepassDraw( twrTARGET *Trg, CAMERA Cam, twrTRANSPARENCY_FLAG Pass,
                                    BOOL IsTargetClear )
{
  twrMATERIAL_PATTERN *MtlPat;
  twrMATERIAL *Mtl;
  twrPRIM *Pr;
  CAMERA OldCam = Twr->Cam;

  Twr->CamBufUpdate(Cam);
  /* Apply and clear target */
  if (IsTargetClear)
    Twr->RndTargetApplyClear(Trg);
  else
    Twr->RndTargetApply(Trg);

  if (Pass == TWR_MTL_SHADOW)
    Twr->GlobalMatrBufsCompute();

  /* Drawing skybox */
  if (Pass == TWR_OPAQ_PASS)
    TWR_DrawSkybox();

  /* Draw all markers */
  if (Pass != TWR_MTL_SHADOW && Pass != TWR_MTL_FINAL)
    Twr->MrkDrawAll(Pass);

  /* Draw every primitive from every material from every material pattern */
  MtlPat = Twr->FirstDraw;
  while (MtlPat != NULL)
  {
    if (Twr->ShdApply(MtlPat->Shd, Pass))
    {
      Twr->MtlPatApply(MtlPat);
      Mtl = MtlPat->FirstDraw;
      while (Mtl != NULL)
      {
        Twr->MtlApply(Mtl);
        Pr = Mtl->FirstDraw;
        while (Pr != NULL)
        {
          if (Pr->Mtl->IsTrans == Pass || Pass == TWR_MTL_SHADOW)
            Twr->PrimDrawInstant(Pr);
          Pr = Pr->NextDraw;
        }
        Mtl = Mtl->NextDraw;
      }
    }
    MtlPat = MtlPat->NextDraw;
  }
  Twr->CamBufUpdate(Twr->Cam = OldCam);
} /* End of 'TWR_RndPostPrepassDraw' function */

/* Draw every shadowing primitive function.
 * ARGUMENTS:
 *   - pointer to shadow target:
 *       twrTARGET *ShadowTarget;
 *   - new camera:
 *       CAMERA Cam;
 *   - pass flag:
 *       twrTRANSPARENCY_FLAG Pass;
 * RETURNS: None.
 */
static VOID TWR_RndPostPassDrawShadows( twrTARGET *ShadowTarget, CAMERA Cam, twrTRANSPARENCY_FLAG Pass )
{
  Twr->ToggleBlend(TWR_BLEND_REPLACE);
  Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(4, 4);
  Twr->PostPrepassDraw(ShadowTarget, Cam, Pass, TRUE);
  glDisable(GL_POLYGON_OFFSET_FILL);
} /* End of 'TWR_RndPostPassDrawShadows' function */

/* Draw scene into target function.
 * ARGUMENTS:
 *   - pointer to target:
 *       twrTARGET *Target;
 *   - new camera:
 *       CAMERA Cam;
 * RETURNS: None.
 */
static VOID TWR_RndTrgPassDraw( twrTARGET *Target, CAMERA Cam )
{
  CAMERA OldCam = Twr->Cam;
  static CHAR *PassNames[] =
  {
    "G-Buf opaque fill",
    "Final opaque fill",
    "G-Buf trans fill",
    "Final trans fill",
  };
  static twrTRANSPARENCY_FLAG PassFlags[] =
  {
    TWR_MTL_OPAQUE,
    TWR_MTL_FINAL,
    TWR_MTL_TRANSPARENT,
  };

  Twr->Cam = Cam;
  Twr->CamBufUpdate(Cam);
  Twr->GlobalMatrBufsCompute();
  TWR_ProfilerCollect("Compute matrix");

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  /* Opaque G-Bufs pass */
  Twr->ToggleBlend(TWR_BLEND_REPLACE);
  Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
  Twr->PostPrepassDraw(Twr->TargetsPass[TWR_OPAQ_PASS], Cam, TWR_MTL_OPAQUE, TRUE);
  TWR_ProfilerCollect(PassNames[0]);

  Twr->LightRedrawShadows(PassFlags[0]);
  /* Set active OPAQ_FINAL target */
  Twr->RndTargetApplyClear(Twr->TargetsPass[TWR_OPAQ_FINAL_PASS]);
  TWR_ProfilerCollect(PassNames[1]);

  /* Lighting */
  Twr->LightRedraw(PassFlags[0]);
  TWR_ProfilerCollect("Lighting opaq");

  /* Transparent G-Bufs pass */
  Twr->PostPrepassDraw(Twr->TargetsPass[TWR_TRANS_PASS], Cam, TWR_MTL_TRANSPARENT, TRUE);
  TWR_ProfilerCollect(PassNames[2]);

  Twr->LightRedrawShadows(PassFlags[2]);
  /* Set active TRANS_FINAL target */
  Twr->RndTargetApplyClear(Twr->TargetsPass[TWR_TRANS_FINAL_PASS]);
  TWR_ProfilerCollect(PassNames[1]);

  /* Lighting */
  Twr->LightRedraw(PassFlags[2]);
  TWR_ProfilerCollect("Lighting trans");

  /* Merge */
  TWR_RndDoMerge(Target, FALSE);
  TWR_ProfilerCollect("Merge stage");

  /* Reset old camera */
  Twr->CamBufUpdate(Twr->Cam = OldCam);
} /* End of 'TWR_RndTrgPassDraw' function */

/* Draw every primitive function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_RndPostPassDraw( VOID )
{
  /*
  twrMATERIAL_PATTERN *MtlPat;
  twrMATERIAL *Mtl;
  INT PassNo;
  twrPRIM *Pr;
  */
  INT modes[2];
  static CHAR *PassNames[] =
  {
    "G-Buf opaque fill",
    "Final opaque fill",
    "G-Buf trans fill",
    "Final trans fill",
  };
  static twrTRANSPARENCY_FLAG PassFlags[] =
  {
    TWR_MTL_OPAQUE,
    TWR_MTL_FINAL,
    TWR_MTL_TRANSPARENT,
    TWR_MTL_FINAL,
  };

  Twr->GlobalMatrixesArrayFill();
  Twr->MrkMatrixesToGlobalMatrixesArray();
  TWR_ProfilerCollect("Matrix array fill");
  Twr->GlobalMatrBufsUpdate();
  TWR_ProfilerCollect("Matrix buffer update");
  Twr->GlobalMatrBufsCompute();
  TWR_ProfilerCollect("Compute matrix");
  
  glGetIntegerv(GL_POLYGON_MODE, modes);

  /*
  Twr->StillWaterTargetTest();

  Twr->GlobalMatrixesArrayFill();
  Twr->MrkMatrixesToGlobalMatrixesArray();
  TWR_ProfilerCollect("Matrix array fill");
  Twr->GlobalMatrBufsUpdate();
  TWR_ProfilerCollect("Matrix buffer update");
  Twr->GlobalMatrBufsCompute();
  TWR_ProfilerCollect("Compute matrix");
  */
#if 1
  /*** Draw scene to targets ***/

  /* Opaque G-Bufs pass */
  glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
  Twr->ToggleBlend(TWR_BLEND_REPLACE);
  Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
  Twr->PostPrepassDraw(Twr->TargetsPass[TWR_OPAQ_PASS], Twr->Cam, TWR_MTL_OPAQUE, TRUE);
  TWR_ProfilerCollect(PassNames[0]);

  Twr->LightRedrawShadows(PassFlags[0]);
  /* Set active OPAQ_FINAL target */
  Twr->RndTargetApplyClear(Twr->TargetsPass[TWR_OPAQ_FINAL_PASS]);
  TWR_ProfilerCollect(PassNames[1]);

  /* Lighting */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  Twr->LightRedraw(PassFlags[0]);
  TWR_ProfilerCollect("Lighting opaq");

  /* Transparent G-Bufs pass */
  glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
  Twr->PostPrepassDraw(Twr->TargetsPass[TWR_TRANS_PASS], Twr->Cam, TWR_MTL_TRANSPARENT, TRUE);
  TWR_ProfilerCollect(PassNames[2]);

  Twr->LightRedrawShadows(PassFlags[2]);
  /* Set active TRANS_FINAL target */
  Twr->RndTargetApplyClear(Twr->TargetsPass[TWR_TRANS_FINAL_PASS]);
  TWR_ProfilerCollect(PassNames[1]);

  /* Lighting */
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  Twr->LightRedraw(PassFlags[2]);
  TWR_ProfilerCollect("Lighting trans");

  /* Merge */
  TWR_RndDoMerge(NULL, TRUE);
  TWR_ProfilerCollect("Merge stage");

  /* Last */
  TWR_RndDoLast();
  TWR_ProfilerCollect("Last(Final) stage");
  glPolygonMode(GL_FRONT_AND_BACK, modes[0]);

#else
  /* Opaque / opaque final / trans / trans final passes */
  for (PassNo = 0; PassNo < 4; PassNo += 2)
  {
    Twr->RndTargetApplyClear(Twr->TargetsPass[PassNo]);

    TWR_ProfilerCollect("Target clear");

    Twr->ToggleBlend(TWR_BLEND_REPLACE);
    Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);

    /* Drawing skybox */
    if (PassNo == TWR_OPAQ_PASS)
    {
      TWR_DrawSkybox();
      TWR_ProfilerCollect("Draw SkyBox");
      Twr->ToggleDepth(TWR_DEPTH_READ_AND_WRITE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, modes[0]);

    /* Draw all markers */
    Twr->MrkDrawAll(PassNo / 2);

    TWR_ProfilerCollect("Target clear");
    MtlPat = Twr->FirstDraw;
    while (MtlPat != NULL)
    {
      if (Twr->ShdApply(MtlPat->Shd, PassFlags[PassNo]))
      {
        Twr->MtlPatApply(MtlPat);
        Mtl = MtlPat->FirstDraw;
        while (Mtl != NULL)
        {
          Twr->MtlApply(Mtl);
          Pr = Mtl->FirstDraw;
          while (Pr != NULL)
          {
            if (Pr->Mtl->IsTrans == TWR_MTL_OPAQUE && PassNo == TWR_OPAQ_PASS ||
                Pr->Mtl->IsTrans == TWR_MTL_TRANSPARENT && PassNo == TWR_TRANS_PASS)
              Twr->PrimDrawInstant(Pr);
            Pr = Pr->NextDraw;
          }
          Mtl = Mtl->NextDraw;
        }
      }
      MtlPat = MtlPat->NextDraw;
    }
    TWR_ProfilerCollect(PassNames[PassNo]);

    Twr->LightRedrawShadows(PassFlags[PassNo]);

    /* Set active OPAQ_FINAL or TRANS_FINAL targets */
    Twr->RndTargetApplyClear(Twr->TargetsPass[PassNo + 1]);

    TWR_ProfilerCollect(PassNames[PassNo + 1]);

    /* Lighting */
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    Twr->LightRedraw(PassFlags[PassNo]);

    TWR_ProfilerCollect("Lighting");
  }
  /* Merge */
  TWR_RndDoMerge(NULL, TRUE);
  TWR_ProfilerCollect("Merge stage");

  /* Last */
  TWR_RndDoLast();
  TWR_ProfilerCollect("Last(Final) stage");
  glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
#endif
} /* End of 'TWR_RndPostPassDraw' function */

/* Pipeline system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPipelineInit( VOID )
{
  twrMATERIAL_PATTERN *MergeMtlPat, *LastMtlPat;
  twrMATERIAL *MergeMtl, *LastMtl;
  twrVERTEX_STD4 V[] =
  {
    {{0, 0, 0}, {0, 0}, {0, 0, 0}, {1, 1, 1, 1}},
  };
  twrVERTEX_ATTRS attrs[] =
  {
    {"InPosition", 3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, P)},
    {"InTexCoord", 2, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, T)},
    {"InNormal",   3, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, N)},
    {"InColor",    4, TRUE, TWR_GetFieldOffset(twrVERTEX_STD4, C)},
  };

  /* Create merge target fullscreen primitive */
  MergeMtlPat = Twr->MtlPatCreate("Merge pattern", "targets/merge", 4, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  MergeMtl = Twr->MtlCreate("Merge material", MergeMtlPat, TWR_MTL_FINAL);
  Twr->MtlBindTex(MergeMtl, 0, Twr->TargetsPass[TWR_OPAQ_FINAL_PASS]->Attachments[0]);
  Twr->MtlBindTex(MergeMtl, 1, Twr->TargetsPass[TWR_OPAQ_PASS]->Depth);
  Twr->MtlBindTex(MergeMtl, 2, Twr->TargetsPass[TWR_TRANS_FINAL_PASS]->Attachments[0]);
  Twr->MtlBindTex(MergeMtl, 3, Twr->TargetsPass[TWR_TRANS_PASS]->Depth);

  TWR_TrgMergeFullscreenPrim = Twr->PrimCreate(MergeMtl, TWR_RND_PRIM_POINTS, V, 1 ,NULL, 0);

  /* Create last target fullscreen primitive */
  LastMtlPat = Twr->MtlPatCreate("Last pattern", "targets/final", 14, 0, sizeof(twrVERTEX_STD4), attrs, 4);
  LastMtl = Twr->MtlCreate("Last material", LastMtlPat, TWR_MTL_FINAL);

  /* Bind to material attachments of merge, opaq and trans targets */
  Twr->MtlBindTex(LastMtl, 0, Twr->TargetsPass[TWR_MERGE_PASS]->Attachments[0]);
  Twr->MtlBindTex(LastMtl, 1, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[0]);
  Twr->MtlBindTex(LastMtl, 2, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[1]);
  Twr->MtlBindTex(LastMtl, 3, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[2]);
  Twr->MtlBindTex(LastMtl, 4, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[3]);
  Twr->MtlBindTex(LastMtl, 5, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[4]);
  Twr->MtlBindTex(LastMtl, 6, Twr->TargetsPass[TWR_OPAQ_PASS]->Attachments[5]);
  Twr->MtlBindTex(LastMtl, 7, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[0]);
  Twr->MtlBindTex(LastMtl, 8, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[1]);
  Twr->MtlBindTex(LastMtl, 9, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[2]);
  Twr->MtlBindTex(LastMtl, 10, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[3]);
  Twr->MtlBindTex(LastMtl, 11, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[4]);
  Twr->MtlBindTex(LastMtl, 12, Twr->TargetsPass[TWR_TRANS_PASS]->Attachments[5]);
  Twr->MtlBindTex(LastMtl, 13, Twr->TargetsPass[TWR_MERGE_PASS]->Depth);

  TWR_TrgLastFullscreenPrim = Twr->PrimCreate(LastMtl, TWR_RND_PRIM_POINTS, V, 1 ,NULL, 0);

  /* Set default blend and depth states */
  Twr->DepthState = TWR_DEPTH_READ_AND_WRITE;
  Twr->BlendState = TWR_BLEND_REPLACE;

  Twr->PostPassDraw = TWR_RndPostPassDraw;
  Twr->TrgPassDraw = TWR_RndTrgPassDraw;
  Twr->PostPassDrawShadows = TWR_RndPostPassDrawShadows;
  Twr->PostPrepassDraw = TWR_RndPostPrepassDraw;
  Twr->ToggleDepth = TWR_RndToggleDepth;
  Twr->ToggleBlend = TWR_RndToggleBlend;
  Twr->SetSkybox = TWR_SetSkybox;
  Twr->CamBufUpdate = TWR_CamBufUpdate;
  TWR_SetSkybox("SkyLight");

  Twr->Sys("Rendering pipeline system is initialized");
} /* End of 'TWR_SystemPipelineInit' function */

/* Pipeline system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPipelineClose( VOID )
{
  TWR_CloseSkybox();
  Twr->PrimFree(TWR_TrgMergeFullscreenPrim);
  Twr->PrimFree(TWR_TrgLastFullscreenPrim);
  Twr->Sys("Pipeline is deinitialized");
} /* End of 'TWR_SystemPipelineClose' function */

/* END OF 'twr_rnd_pipeline.c' FILE */
