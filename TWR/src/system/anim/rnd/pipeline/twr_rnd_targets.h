/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_targets.h
 * PURPOSE     : Tough Water Rendering project.
 *               Targets system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 28.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_targets_h_
#define __twr_rnd_targets_h_

/* Render pass type (for opaque or transparent prims) */
typedef enum tagtwrTRG_PASS_TYPE
{
  TWR_OPAQ_PASS =        0, /* Index of opaq G-Buf pass */
  TWR_OPAQ_FINAL_PASS =  1, /* Index of opaq final pass */
  TWR_TRANS_PASS =       2, /* Index of trans G-Buf pass */
  TWR_TRANS_FINAL_PASS = 3, /* Index of trans final pass */
  TWR_SHADOW_PASS =      4, /* Index of shadow pass */
  TWR_MERGE_PASS =       5, /* Index of merge pass */
  TWR_LAST_PASS =        6, /* Index of last pass */
} twrTRG_PASS_TYPE;

/* Structure for targets */
typedef struct twrtagTARGET
{
  CHAR Name[TWR_STR_MAX];     /* Name of target */
  UINT FBOId;                 /* ID of target's FBO */
  INT W, H;                   /* Width and height of target's attachments */
  twrTEXTURE *Attachments[8]; /* Array of pointers to target's attachments */
  INT NumOfAttachments;       /* Actual number of target's attachments */
  twrTEXTURE *Depth;          /* Depth attachment */
  BOOL IsDepthShare;          /* Is depth shared with others targets */
} twrTARGET;

/* Target system structure */
typedef struct tagtwrSYSTEM_TARGETS
{
  /* Targets data */
  twrTARGET * TargetsPass[7];  /* Opaque/Trans x G-Buf/Lighting, shadow, merge & last targets */

  /* Pointers to target functions */

  /* Target apply function.
   * ARGUMENTS:
   *   - pointer to target:
   *       twrTARGET *Trg;
   * RETURNS: None.
   */
  VOID (*RndTargetApply)( twrTARGET *Trg );

  /* Clear target function.
   * ARGUMENTS:
   *   - pointer to target:
   *       twrTARGET *Trg;
   * RETURNS: None.
   */
  VOID (*RndTargetClear)( twrTARGET *Trg );

  /* Apply and clear target function.
   * ARGUMENTS:
   *   - pointer to target:
   *       twrTARGET *Trg;
   * RETURNS: None.
   */
  VOID (*RndTargetApplyClear)( twrTARGET *Trg );

  /* Share depth from one target to another function.
   * ARGUMENTS:
   *   - pointer to targets:
   *       twrTARGET *SrcTrg, *DstTrg;
   * RETURNS: None.
   */
  VOID (*RndTargetShareDepth)( twrTARGET *SrcTrg, twrTARGET *DstTrg );

  /* Share depth from one target to another function.
   * ARGUMENTS:
   *   - pointer to new depth texture:
   *       twrTEXTURE *NewDepth;
   *   - pointer to target:
   *       twrTARGET *DstTrg;
   * RETURNS: None.
   */
  VOID (*RndTargetBindDepth)( twrTEXTURE *NewDepth, twrTARGET *DstTrg );

  /* Create depth attachment function.
  * ARGUMENTS:
  *   - width and height of attachment:
  *       INT W, H;
  * RETURNS:
  *   twrTEXTURE *Tex 
  */
  twrTEXTURE * (*RndTargetCreateDepth)( INT W, INT H );

  /* Create target function.
   * ARGUMENTS:
   *   - name of target:
   *       CHAR *TrgName;
   *   - number of color attachments:
   *       INT NumOfAttachments;
   *   - flag is depth copied from others targets (so we don't clear it):
   *       BOOL IsDepth;
   *   - target size:
   *       INT W, H;
   * RETURNS: 
   *   (twrTARGET *) pointer to created target or NULL.
   */
  twrTARGET * (*RndTargetCreate)( CHAR *TrgName, INT NumOfAttachments, BOOL IsDepth, INT W, INT H );

  /* Free target function.
   * ARGUMENTS:
   *   - pointer to target:
   *       twrTARGET *Trg;
   * RETURNS: None.
   */
  VOID (*RndTargetFree)( twrTARGET *Trg );

  /* Target resize function.
   * ARGUMENTS:
   *   - target pointer:
   *       twrTARGET *Trg;
   *   - new target size:
   *       INT W, H;
   * RETURNS: None.
   */
  VOID (*RndTargetResize)( twrTARGET *Trg, INT W, INT H );
} twrSYSTEM_TARGETS;

/* Targets system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTargetsInit( VOID );

/* Targets system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTargetsClose( VOID );

#endif /* __twr_rnd_targets_h_ */

/* END OF 'twr_rnd_targets.h' FILE */
