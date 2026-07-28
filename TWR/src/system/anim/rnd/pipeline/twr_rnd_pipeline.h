/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_pipeline.h
 * PURPOSE     : Tough Water Rendering project.
 *               Pipeline system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_pipeline_h_
#define __twr_rnd_pipeline_h_

/* Pipeline system structure */
typedef struct tagtwrSYSTEM_PIPELINE
{
  /*** Redner state variables ***/
  twrDEPTH_FLAG DepthState; /* Current state of reading / writing depth */
  twrBLEND_MODE BlendState; /* Current state of blend function */

  /* Draw every primitive function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*PostPassDraw)( VOID );

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
  VOID (*PostPassDrawShadows)( twrTARGET *ShadowTarget, CAMERA Cam, twrTRANSPARENCY_FLAG Pass );

  /* Draw every primitive into one target function.
   * ARGUMENTS:
   *   - pointer to aim target:
   *       twrTARGET *Trg;
   *   - new camera:
   *       CAMERA Cam;
   *   - pass flag:
   *       twrTRANSPARENCY_FLAG Pass;
   *   - flag if we need to clear target:
   *       BOOL IsTargetClear;
   * RETURNS: None.
   */
  VOID (*PostPrepassDraw)( twrTARGET *Trg, CAMERA Cam, twrTRANSPARENCY_FLAG Pass,
                           BOOL IsTargetClear );

  /* Draw scene into target function.
   * ARGUMENTS:
   *   - pointer to target:
   *       twrTARGET *Target;
   *   - new camera:
   *       CAMERA Cam;
   * RETURNS: None.
   */
  VOID (*TrgPassDraw)( twrTARGET *Target, CAMERA Cam );

  /* Set skybox function.
   * ARGUMENTS:
   *   - name of skybox:
   *       CHAR *Name;
   * RETURNS: None.
   */
  VOID (*SetSkybox)( CHAR *Name );

  /* Update camera buffer function.
   * ARGUMENTS:
   *   - input camera:
   *       CAMERA Cam;
   * RETURNS: None.
   */
  VOID (*CamBufUpdate)( CAMERA Cam );

  /* Toggle global depth state function.
   * ARGUMENTS:
   *   - new depth state:
   *       twrDEPTH_FLAG DepthFlag;
   * RETURNS: None.
   */
  VOID (*ToggleDepth)( twrDEPTH_FLAG DepthFlag );

  /* Toggle global blend mode function.
   * ARGUMENTS:
   *   - new blend mode:
   *       twrBLEND_MODE BlendMode;
   * RETURNS: None.
   */
  VOID (*ToggleBlend)( twrBLEND_MODE BlendMode );

} twrSYSTEM_PIPELINE;

/* Pipeline system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPipelineInit( VOID );

/* Pipeline system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemPipelineClose( VOID );

#endif /* __twr_rnd_pipeline_h_ */

/* END OF 'twr_rnd_pipeline.h' FILE */
