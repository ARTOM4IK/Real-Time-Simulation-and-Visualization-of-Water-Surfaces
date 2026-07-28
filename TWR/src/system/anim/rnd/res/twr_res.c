/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res.c
 * PURPOSE     : Tough Water Rendering project.
 *               ...
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
 * LAST UPDATE : 18.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "twr.h"

/* Resources system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemResourcesInit( VOID )
{
  TWR_SystemBuffersInit();
  TWR_SystemTexturesInit();
  TWR_SystemShadersInit();
  TWR_SystemTopologyInit();
  TWR_SystemTopologyGridInit();
  TWR_SystemMaterialPatternsInit();
  TWR_SystemMaterialsInit();
  TWR_SystemPrimsInit();
  TWR_SystemModelsInit();
  TWR_SystemFontsInit();
} /* End of 'TWR_SystemResourcesInit' function */

/* Resources system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemResourcesClose( VOID )
{
  TWR_SystemFontsClose();
  TWR_SystemModelsClose();
  TWR_SystemPrimsClose();
  TWR_SystemMaterialsClose();
  TWR_SystemMaterialPatternsClose();
  TWR_SystemTopologyGridClose();
  TWR_SystemTopologyClose();
  TWR_SystemShadersClose();
  TWR_SystemTexturesClose();
  TWR_SystemBuffersClose();
} /* End of 'TWR_SystemResourcesClose' function */

/* END OF 'twr_res.c' FILE */
