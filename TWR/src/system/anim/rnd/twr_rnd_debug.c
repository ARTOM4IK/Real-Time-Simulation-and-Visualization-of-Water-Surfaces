/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_debug.c
 * PURPOSE     : Tough Water Rendering project.
 *               Render debug inplementation module.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 28.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Debug output function.
 * ARGUMENTS:
 *   - source APi or device:
 *       UINT Source;
 *   - error type:
 *       UINT Type;
 *   - error message id:
 *       UINT Id, 
 *   - message severity:
 *       UINT severity, 
 *   - message text length:
 *       INT Length, 
 *   - message text:
 *       CHAR *Message, 
 *   - user addon parameters pointer:
 *       VOID *UserParam;
 * RETURNS: None.
 */
VOID APIENTRY TWR_glDebugOutput( UINT Source, UINT Type, UINT Id, UINT Severity,
                                 INT Length, const CHAR *Message,
                                 const VOID *UserParam )
{
  INT len = 0;
  static CHAR Buf[10000];

  /* Ignore non-significant error/warning codes */
  if (Id == 131169 || Id == 131185 || Id == 131218 || Id == 131204)
    return;

  len += wsprintf(Buf + len, "Debug message (%i) %s\n", Id, Message);

  switch (Source)
  {
  case GL_DEBUG_SOURCE_API:
    len += wsprintf(Buf + len, "Source: API\n");
    break;
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    len += wsprintf(Buf + len, "Source: Window System\n");
    break;
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    len += wsprintf(Buf + len, "Source: Shader Compiler\n");
    break;
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    len += wsprintf(Buf + len, "Source: Third Party\n");
    break;
  case GL_DEBUG_SOURCE_APPLICATION:
    len += wsprintf(Buf + len, "Source: Application\n");
    break;
  case GL_DEBUG_SOURCE_OTHER:
    len += wsprintf(Buf + len, "Source: Other\n");
    break;
  }

  len += wsprintf(Buf + len, "\n");

  switch (Type)
  {
  case GL_DEBUG_TYPE_ERROR:
    len += wsprintf(Buf + len, "Type: Error");
    break;
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    len += wsprintf(Buf + len, "Type: Deprecated Behaviour");
    break;
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    len += wsprintf(Buf + len, "Type: Undefined Behaviour");
    break; 
  case GL_DEBUG_TYPE_PORTABILITY:
    len += wsprintf(Buf + len, "Type: Portability");
    break;
  case GL_DEBUG_TYPE_PERFORMANCE:
    len += wsprintf(Buf + len, "Type: Performance");
    break;
  case GL_DEBUG_TYPE_MARKER:
    len += wsprintf(Buf + len, "Type: Marker");
    break;
  case GL_DEBUG_TYPE_PUSH_GROUP:
    len += wsprintf(Buf + len, "Type: Push Group");
    break;
  case GL_DEBUG_TYPE_POP_GROUP:
    len += wsprintf(Buf + len, "Type: Pop Group");
    break;
  case GL_DEBUG_TYPE_OTHER:
    len += wsprintf(Buf + len, "Type: Other");
    break;
  }
  len += wsprintf(Buf + len, "\n");

  switch (Severity)
  {
  case GL_DEBUG_SEVERITY_HIGH:
    len += wsprintf(Buf + len, "Severity: high");
    break;
  case GL_DEBUG_SEVERITY_MEDIUM:
    len += wsprintf(Buf + len, "Severity: medium");
    break;
  case GL_DEBUG_SEVERITY_LOW:
    len += wsprintf(Buf + len, "Severity: low");
    break;
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    len += wsprintf(Buf + len, "Severity: notification");
    break;
  }

  len += wsprintf(Buf + len, "\n\n");

  Twr->Error("\x1b[38;2;255;200;200m%s", Buf);
} /* End of 'TWR_glDebugOutput' function */

/* END OF 'twr_rnd_debug.c' FILE */
