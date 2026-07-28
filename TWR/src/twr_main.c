  /***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_main.c
 * PURPOSE     : Tough Water Rendering project.
 *               Main startup entry point module.
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
 * LAST UPDATE : 23.04.2025
 * NOTE        : Module prefix 'twr '.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Main startup program function.
 * ARGUMENTS:
 *   - application instance handle:
 *       HINSTANCE hInstance;
 *   - previouse application instance handle
 *     (not used, alway NULL):
 *       HINSTANCE hPrevInstance;
 *   - command line string:
 *       CHAR *CmdLine;
 *   - command line window show parameter (see SW_***):
 *       INT CmdShow;
 * RETURNS:
 *   (INT) system error level value.
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT CmdShow )
{
  SetDbgMemHooks();
  TWR_Init();
  SetForegroundWindow(Twr->hWnd);
  
  Twr->FileScenesLoad("bin/scenes/vn4local.twr");
  Twr->SceneSetByName("TestScene");
  Twr->Run();

  Twr->Log("\n\n");
  Twr->Warning("System goto shutdown...");
  TWR_Close();
  return 30;
} /* End of 'WinMain' function */

/* END OF 'twr_main.c' FILE */
