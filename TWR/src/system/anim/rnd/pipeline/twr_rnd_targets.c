/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_targets.c
 * PURPOSE     : Tough Water Rendering project.
 *               Targets system inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 25.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <stdio.h>

/* Stock of targets */
static STOCK TWR_StkTargets;

/* Target apply function.
 * ARGUMENTS:
 *   - pointer to target:
 *       twrTARGET *Trg;
 * RETURNS: None.
 */
VOID TWR_RndTargetApply( twrTARGET *Trg )
{
  if (Trg == NULL)
  {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, Twr->W, Twr->H);
  }
  else
  {
    glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBOId);
    if (Trg->IsDepthShare)
      glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Trg->Depth->TexId, 0);
    /* Apply depth texture, test */
    glViewport(0, 0, Trg->W, Trg->H);
  }
} /* End of 'TWR_RndTargetApply' function */

/* Clear target function.
 * ARGUMENTS:
 *   - pointer to target:
 *       twrTARGET *Trg;
 * RETURNS: None.
 */
VOID TWR_RndTargetClear( twrTARGET *Trg )
{
  INT i;
  VEC4 clear_color = {0};
  FLT clear_depth = 1;

  if (Trg == NULL)
  {
    glClearBufferfv(GL_COLOR, 0, &clear_color.X);
    glClearBufferfv(GL_DEPTH, 0, &clear_depth);
  }
  else
  {
    for (i = 0; i < Trg->NumOfAttachments; i++)
      /*if (Trg->Attachments[i]->GLType != */
      glClearBufferfv(GL_COLOR, i, &clear_color.X);
    if (Trg->Depth != NULL && !Trg->IsDepthShare)
      glClearBufferfv(GL_DEPTH, 0, &clear_depth);
  }
} /* End of 'TWR_RndTargetClear' function */


/* Apply and clear target function.
 * ARGUMENTS:
 *   - pointer to target:
 *       twrTARGET *Trg;
 * RETURNS: None.
 */
VOID TWR_RndTargetApplyClear( twrTARGET *Trg )
{
  TWR_RndTargetApply(Trg);
  TWR_RndTargetClear(Trg);
} /* End of 'TWR_RndTargetApplyClear' function */

/* Create target function.
 * ARGUMENTS:
 *   - name of target:
 *       CHAR *TrgName;
 *   - number of color attachments:
 *       INT NumOfAttachments;
 *   - flag is depth used in others targets (so we don't clear it):
 *       BOOL IsDepth;
 *   - target size:
 *       INT W, H;
 * RETURNS: 
 *   (twrTARGET *) pointer to created target or NULL.
 */
twrTARGET * TWR_RndTargetCreate( CHAR *TrgName, INT NumOfAttachments, BOOL IsDepth, INT W, INT H )
{
  INT i, status;
  UINT DrawBuffer[8];
  twrTARGET *Trg;
  static CHAR *AttachmentNames[] =
  {
    "Ka attachment",
    "Kd trans attachment",
    "Ks attachment",
    "Normal attachment",
    "Position attachment",
    "Out color attachment",
    "Tangent attachment",
    "Bitangent attachment",
  }, Buf[1000];

  if ((Trg = StockAlloc(&TWR_StkTargets)) == NULL)
    return NULL;
  memset(Trg, 0, sizeof(twrTARGET));
  if (NumOfAttachments > 8)
    NumOfAttachments = 8;

  memcpy(Trg->Name, TrgName, TWR_STR_MAX);
  Trg->W = W;
  Trg->H = H;
  Trg->NumOfAttachments = NumOfAttachments;
  Trg->IsDepthShare = FALSE;
  glGenFramebuffers(1, &Trg->FBOId);
  glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBOId);

  /* Create attachments */
  for (i = 0; i < NumOfAttachments; i++)
  {
    sprintf(Buf, "%s target: %s", TrgName, AttachmentNames[i]);
    Trg->Attachments[i] = Twr->TexCreate(Buf, W, H, FALSE, GL_RGBA32F, NULL);
    Twr->TexReferenceInc(Trg->Attachments[i]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Trg->Attachments[i]->TexId, 0);
    DrawBuffer[i] = GL_COLOR_ATTACHMENT0 + i;
  }
  glDrawBuffers(NumOfAttachments, DrawBuffer);

  if (IsDepth != FALSE)
  {
    sprintf(Buf, "%s target: Depth attachment", TrgName);
    Trg->Depth = Twr->TexCreate(Buf, W, H, FALSE, GL_DEPTH_COMPONENT24, NULL);
    Twr->TexReferenceInc(Trg->Depth);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Trg->Depth->TexId, 0);
  }
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
  {
    Twr->Error("Frame buffer completeness error");
    return NULL;
  }
  return Trg;
} /* End of 'TWR_RndTargetCreate' function */

/* Free target function.
 * ARGUMENTS:
 *   - pointer to target:
 *       twrTARGET *Trg;
 * RETURNS: None.
*/
VOID TWR_RndTargetFree( twrTARGET *Trg )
{
  if (Trg == NULL)
    return;
  StockDelete(Trg);
} /* End of 'TWR_RndTargetFree' function */

/* Set target delete strategy function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
static VOID TWR_DeleteStrategyTarget( twrTARGET *Trg )
{
  INT i;

  if (Trg == NULL)
    return;
  glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBOId);
  for (i = 0; i < Trg->NumOfAttachments; i++)
    Twr->TexFree(Trg->Attachments[i]);
  if (Trg->Depth != NULL)
    Twr->TexFree(Trg->Depth);
  glDeleteFramebuffers(1, &Trg->FBOId);
  memset(Trg, 0, sizeof(twrTARGET));
  /*free(Trg);*/
} /* End of 'TWR_DeleteStrategyTarget' function */

/* Target resize function.
 * ARGUMENTS:
 *   - target pointer:
 *       twrTARGET *Trg;
 *   - new target size:
 *       INT W, H;
 * RETURNS: None.
 */
VOID TWR_RndTargetResize( twrTARGET *Trg, INT W, INT H )
{
  INT i, status;

  if (Trg == NULL)
    return;

  Trg->W = W;
  Trg->H = H;
  glBindFramebuffer(GL_FRAMEBUFFER, Trg->FBOId);
  /* Resize attachments */
  for (i = 0; i < Trg->NumOfAttachments; i++)
  {
    Twr->TexResize(Trg->Attachments[i], W, H);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, Trg->Attachments[i]->TexId, 0);
  }
  if (Trg->Depth != NULL && !Trg->IsDepthShare)
  {
    Twr->TexResize(Trg->Depth, W, H);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Trg->Depth->TexId, 0);
  }
  status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    Twr->Error("Frame buffer resize completeness error");
} /* End of 'TWR_RndTargetResize' function */

/* Share depth from one target to another function.
  * ARGUMENTS:
  *   - pointer to targets:
  *       twrTARGET *SrcTrg, *DstTrg;
  * RETURNS: None.
  */
static VOID TWR_RndTargetShareDepth( twrTARGET *SrcTrg, twrTARGET *DstTrg )
{
  if (SrcTrg == NULL || SrcTrg->Depth == NULL)
  {
    Twr->Error("Tried to share corrupted depth\n");
    return;
  }
  else if (DstTrg == NULL)
  {
    Twr->Error("Tried to share depth with not existing target");
    return;
  }
  DstTrg->IsDepthShare = TRUE;
  DstTrg->Depth = SrcTrg->Depth;
  glBindFramebuffer(GL_FRAMEBUFFER, DstTrg->FBOId);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DstTrg->Depth->TexId, 0);
  /*
  Twr->TexReferenceInc(SrcTrg->Depth);
  */
} /* End of 'TWR_RndTargetShareDepth' function */

/* Share depth from one target to another function.
  * ARGUMENTS:
  *   - pointer to new depth texture:
  *       twrTEXTURE *NewDepth;
  *   - pointer to target:
  *       twrTARGET *DstTrg;
  * RETURNS: None.
  */
static VOID TWR_RndTargetBindDepth( twrTEXTURE *NewDepth, twrTARGET *DstTrg )
{
  if (NewDepth == NULL)
  {
    Twr->Error("Tried to bind corrupted depth\n");
    return;
  }
  else if (DstTrg == NULL)
  {
    Twr->Error("Tried to bind depth to not existing target");
    return;
  }
  DstTrg->IsDepthShare = TRUE;
  DstTrg->Depth = NewDepth;
  glBindFramebuffer(GL_FRAMEBUFFER, DstTrg->FBOId);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DstTrg->Depth->TexId, 0);
  /*
  Twr->TexReferenceInc(NewDepth);
  */
} /* End of 'TWR_RndTargetBindDepth' function */

/* Create depth attachment function.
  * ARGUMENTS:
  *   - width and height of attachment:
  *       INT W, H;
  * RETURNS:
  *   twrTEXTURE *Tex 
  */
static twrTEXTURE * TWR_RndTargetCreateDepth( INT W, INT H )
{
  CHAR Buf[1000];
  twrTEXTURE *Depth;

  if (W < 0)
    W = 0;
  if (H < 0)
    H = 0;

  sprintf(Buf, "Created depth attachment");
  Depth = Twr->TexCreate(Buf, W, H, FALSE, GL_DEPTH_COMPONENT24, NULL);
  Twr->TexReferenceInc(Depth);
  return Depth;
  /*
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Trg->Depth->TexId, 0);
  */
} /* End of 'TWR_RndTargetCreateDepth' function */

/* Targets system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTargetsInit( VOID )
{
  Twr->RndTargetApply = TWR_RndTargetApply;
  Twr->RndTargetClear = TWR_RndTargetClear;
  Twr->RndTargetApplyClear = TWR_RndTargetApplyClear;
  Twr->RndTargetCreate = TWR_RndTargetCreate;
  Twr->RndTargetFree = TWR_RndTargetFree;
  Twr->RndTargetResize = TWR_RndTargetResize;
  Twr->RndTargetShareDepth = TWR_RndTargetShareDepth;
  Twr->RndTargetBindDepth = TWR_RndTargetBindDepth;
  Twr->RndTargetCreateDepth = TWR_RndTargetCreateDepth;

  /* Create stock for primitives */
  StockCreate(&TWR_StkTargets, "Targets", sizeof(twrTARGET),
              30, TWR_DeleteStrategyTarget);
  TWR_StockRegisterToStrip(&TWR_StkTargets);

  /* Targets creation */
  Twr->TargetsPass[TWR_OPAQ_PASS] = Twr->RndTargetCreate("Opaq G-Bufs pass", 6, TRUE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_TRANS_PASS] = Twr->RndTargetCreate("Trans G-Bufs pass", 6, TRUE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_OPAQ_FINAL_PASS] = Twr->RndTargetCreate("Opaq final pass", 1, TRUE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_TRANS_FINAL_PASS] = Twr->RndTargetCreate("Trans final pass", 1, TRUE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_MERGE_PASS] = Twr->RndTargetCreate("Merge pass", 1, TRUE, Twr->W, Twr->H);
  /*
  Twr->TargetsPass[TWR_MERGE_PASS]->Attachments[1] = Twr->RndTargetCreateDepth(Twr->W, Twr->H);
  Twr->TargetsPass[TWR_MERGE_PASS]->NumOfAttachments++;
  */
  Twr->TargetsPass[TWR_LAST_PASS] = Twr->RndTargetCreate("Last pass", 1, FALSE, Twr->W, Twr->H);
  Twr->RndTargetBindDepth(Twr->TargetsPass[TWR_MERGE_PASS]->Depth, Twr->TargetsPass[TWR_LAST_PASS]);
  /*
  Twr->TargetsPass[TWR_MERGE_PASS] = Twr->RndTargetCreate("Merge pass", 1, TRUE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_LAST_PASS] = Twr->RndTargetCreate("Last pass", 1, FALSE, Twr->W, Twr->H);
  Twr->TargetsPass[TWR_LAST_PASS]->IsDepthShare = TRUE;
  Twr->TargetsPass[TWR_LAST_PASS]->Depth = Twr->TargetsPass[TWR_MERGE_PASS]->Depth;
  */

  Twr->Sys("Target system is initialized");
} /* End of 'TWR_SystemTargetsInit' function */

/* Targets system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTargetsClose( VOID )
{
  StockFree(&TWR_StkTargets);
  Twr->Sys("Target system is deinitialized");
} /* End of 'TWR_SystemTargetsClose' function */

/* END OF 'twr_rnd_targets.c' FILE */
