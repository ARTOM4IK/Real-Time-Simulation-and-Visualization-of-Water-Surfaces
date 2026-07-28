/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_base.c
 * PURPOSE     : Tough Water Rendering project.
 *               Render system inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 12.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

#include <wglew.h>
#include <gl/wglext.h>

#pragma comment(lib, "opengl32")

/* Resize projection function.
 * ARGUMENTS:
 *   - width and height of window:
 *       INT W, H;
 * RETURNS: None.
*/
static VOID TWR_RndResize( INT W, INT H )
{
  /* Check if sizes are wrong */
  if (W <= 0)
    W = 1;
  if (H <= 0)
    H = 1;
  glViewport(0, 0, W, H);

  /* Resize targets */
  Twr->RndTargetResize(Twr->TargetsPass[TWR_OPAQ_PASS], W, H);
  Twr->RndTargetResize(Twr->TargetsPass[TWR_TRANS_PASS], W, H);
  Twr->RndTargetResize(Twr->TargetsPass[TWR_OPAQ_FINAL_PASS], W, H);
  Twr->RndTargetResize(Twr->TargetsPass[TWR_TRANS_FINAL_PASS], W, H);
  Twr->RndTargetResize(Twr->TargetsPass[TWR_MERGE_PASS], W, H);
  Twr->RndTargetResize(Twr->TargetsPass[TWR_LAST_PASS], W, H);

  CamSetSize(&Twr->Cam, W, H);
} /* End of 'TWR_RndResize' function */

/* Copy frame from RndFrameDC into hDC function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
static VOID TWR_RndCopyFrame( VOID )
{
  SwapBuffers(Twr->hRndDC);
} /* End of 'TWR_RndCopyFrame' function */

/* Start draw function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
static VOID TWR_RndStart( VOID )
{
  INT modes[2];
  twrRND_UBO_CAMERA CamData;
  twrRND_UBO_SYNC SyncData;

  CamData.MatrVP = Twr->Cam.MatrVP;
  CamData.MatrV = Twr->Cam.MatrView;
  CamData.MatrP = Twr->Cam.MatrProj;

  CamData.CamLoc = Vec4SetVec3(Twr->Cam.Loc, 0);
  CamData.CamRight = Vec4SetVec3(Twr->Cam.Right, 0);
  CamData.CamUp = Vec4SetVec3(Twr->Cam.Up, 0);
  CamData.CamDir = Vec4SetVec3(Twr->Cam.Dir, 0);
  CamData.CamAt = Vec4SetVec3(Twr->Cam.At, 0);
  CamData.FrameWHProjSizeDist = Vec4Set(Twr->Cam.FrameW, Twr->Cam.FrameH, Twr->Cam.ProjSize, Twr->Cam.ProjDist);
  Twr->BufUpdate(Twr->RndShdBufCamera, 0, 0, &CamData);
  TWR_ProfilerCollect("Fill camera buffer");

  glGetIntegerv(GL_POLYGON_MODE, modes);
  SyncData.TimeDeltaTimeGlobalTimeDeltaTime =
     Vec4Set(Twr->Time, Twr->DeltaTime, Twr->GlobalTime, Twr->GlobalDeltaTime);
  SyncData.IsWireFrameIsPauseFPS = 
    Vec4Set(modes[0] == GL_LINE, Twr->IsPause, Twr->IsDebug, Twr->FPS);
  Twr->BufUpdate(Twr->RndShdBufSync, 0, 0, &SyncData);
  TWR_ProfilerCollect("Fill sync buffer");

  Twr->NumOfGlobalMatrixes = 0;
  Twr->TrianglesCount = 0;
  Twr->ShadersUpdate();
} /* End of 'TWR_RndStart' function */

/* End draw function function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
static VOID TWR_RndEnd( VOID )
{
  Twr->PostPassDraw();
  /*
  glFinish();
  glFlush();
  */
  SwapBuffers(Twr->hRndDC);
} /* End of 'TWR_RndEnd' function */

/* Matrix look-at viewer setup function.
 * ARGUMENTS:
 *   - viewer position, look-at point, approximate up direction:
 *       VEC3 Loc, At, Up;
 * RETURNS: None.
 */
static VOID TWR_RndCamSet( VEC3 Loc, VEC3 At, VEC3 Up )
{
  CamSet(&Twr->Cam, Loc, At, Up);
} /* End of 'TWR_RndCamSet' function */

/* Render system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
VOID TWR_SystemRenderInit( VOID )
{
  INT i, nums;
  PIXELFORMATDESCRIPTOR pfd = {0};
  PFNGLSWAPINTERVALEXTPROC wglSwapInterval = NULL;
  HGLRC hRC;
  INT PixelAttribs[] =
  {
    WGL_DRAW_TO_WINDOW_ARB, TRUE,
    WGL_SUPPORT_OPENGL_ARB, TRUE,
    WGL_DOUBLE_BUFFER_ARB, TRUE,
    WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    WGL_COLOR_BITS_ARB, 32,
    WGL_DEPTH_BITS_ARB, 32,
    0
  };
  INT ContextAttribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0
  };

  /* Prepare frame compatible device context */
  Twr->hRndDC = GetDC(Twr->hWnd);

  /* OpenGL init: pixel format setup */
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(Twr->hRndDC, &pfd);

  DescribePixelFormat(Twr->hRndDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(Twr->hRndDC, i, &pfd);

  /* OpenGL init: rendering context setup */
  Twr->hRndGLRC = wglCreateContext(Twr->hRndDC);
  wglMakeCurrent(Twr->hRndDC, Twr->hRndGLRC);

  if (glewInit() != GLEW_OK)
    exit(0);
  
  wglChoosePixelFormatARB(Twr->hRndDC, PixelAttribs, NULL, 1, &i, &nums);
  hRC = wglCreateContextAttribsARB(Twr->hRndDC, NULL, ContextAttribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(Twr->hRndGLRC);

  Twr->hRndGLRC = hRC;
  wglMakeCurrent(Twr->hRndDC, Twr->hRndGLRC);

  /* Render parameters setup */
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ZERO);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_PRIMITIVE_RESTART);
  glPrimitiveRestartIndex(-1);
  if (wglSwapIntervalEXT != NULL)
    wglSwapIntervalEXT(0);

#ifndef NDEBUG
  /* Debug output */
  Twr->Info((CHAR*)glGetString(GL_VERSION));
  OutputDebugString(glGetString(GL_VENDOR));
  OutputDebugString(glGetString(GL_RENDERER));
#endif /* NDEBUG */
 
#ifndef NDEBUG
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(TWR_glDebugOutput, NULL);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
#endif /* NDEBUG */

  /* Setup global data */
  Twr->CurrentFrameNo = 0;
  Twr->StoreFrameNo = 0;
  Twr->IsDebug = TRUE;
  Twr->FirstDraw = NULL;

  CamSetProj(&Twr->Cam, 0.1, 0.1, 1847);
  CamSetSize(&Twr->Cam, Twr->W, Twr->H);
  CamSet(&Twr->Cam, Vec3Set(8, 8, 8), Vec3Set(0, 0, 0), Vec3Set(0, 1, 0));

  Twr->RndResize = TWR_RndResize;
  Twr->RndCopyFrame = TWR_RndCopyFrame;
  Twr->RndStart = TWR_RndStart;
  Twr->RndEnd = TWR_RndEnd;
  Twr->RndCamSet = TWR_RndCamSet;

  TWR_SystemResourcesInit();
  TWR_SystemComputeInit();
  TWR_SystemTargetsInit();
  TWR_SystemPipelineInit();
  TWR_SystemMrkInit();
  TWR_SystemLightsInit();

  /* Create work buffers */
  Twr->RndShdBufCamera = Twr->BufCreate("Camera", TWR_BUF_BIND_CAMERA, sizeof(twrRND_UBO_CAMERA), NULL);
  Twr->RndShdBufSync = Twr->BufCreate("Sync", TWR_BUF_BIND_SYNC, sizeof(twrRND_UBO_SYNC), NULL);

  Twr->BufApply(Twr->RndShdBufCamera);
  Twr->BufApply(Twr->RndShdBufSync);

  Twr->Sys("Render system is initialized\n");
  Twr->SetColor(0x88FF88, 0);
  Twr->Log("Render system info:\n");
  Twr->SetColor(0x88FFFF, 0);
  Twr->Log(
    "  OpenGL version: %s\n"
    "  Rendered      : %s\n"
    "  Vendor        : %s\n",
    glGetString(GL_VERSION), glGetString(GL_RENDERER), glGetString(GL_VENDOR));
} /* End of 'TWR_SystemRenderInit' function */

/* Render system close function.
 * ARGUMENTS: None.
 * RETURNS: None.
*/
VOID TWR_SystemRenderClose( VOID )
{
  TWR_SystemLightsClose();
  TWR_SystemMrkClose();
  TWR_SystemPipelineClose();
  TWR_SystemTargetsClose();
  TWR_SystemComputeClose();
  TWR_SystemResourcesClose();
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(Twr->hRndGLRC);
  ReleaseDC(Twr->hWnd, Twr->hRndDC);
  Twr->Sys("Render system is deinitialized");
} /* End of 'TWR_SystemRenderClose' function */

/* END OF 'twr_rnd_base.c' FILE */
