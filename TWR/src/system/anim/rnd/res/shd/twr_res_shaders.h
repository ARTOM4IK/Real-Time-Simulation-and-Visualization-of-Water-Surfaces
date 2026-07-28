/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_shaders.h
 * PURPOSE     : Tough Water Rendering project.
 *               Shaders declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_shaders_h_
#define __twr_res_shaders_h_

#include "twr_def.h"

/* Shader program store type */
typedef struct tagtwrSHADER
{
  CHAR Name[TWR_STR_MAX]; /* Shader filename prefix */
  INT ShdId[5];           /* Shaders program identificators */
} twrSHADER;

/* Shaders system handle type */
typedef struct tagtwrSYSTEM_SHADERS
{
  /* Add shader to stock function.
   * ARGUMENTS:
   *   - Shader name:
   *       CHAR *ShaderName;
   * RETURNS:
   *   (twrSHADER *) pointer to created shader.
   */
  twrSHADER * (*ShdCreate)( CHAR *ShaderName );

  /* Put shader to stock queue for delete function.
   * ARGUMENTS:
   *   - Shader name:
   *       CHAR *ShaderName;
   * RETURNS: None.
   */
  VOID (*ShdFree)( twrSHADER *Shd );

  /* Find shader in stock function.
   * ARGUMENTS:
   *   - shader name:
   *       CHAR *ShaderName;
   * RETURNS:
   *   (twrSHADER *) pointer to found shader.
   */
  twrSHADER * (*ShdFind)( CHAR *ShaderName );

  /* Apply shader function.
   * ARGUMENTS:
   *   - shader pointer:
   *       twrSHADER *Shd;
   *   - shader apply pass flag:
   *       twrTRANSPARENCY_FLAG PassFlag;
   * RETURNS:
   *   (BOOL) TRUE if shader apply successfuly, FALSE otherwise.
   */
  BOOL (*ShdApply)( twrSHADER *Shd, twrTRANSPARENCY_FLAG PassFlag );

  /* Update all shaders function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*ShadersUpdate)( VOID );
} twrSYSTEM_SHADERS;

/* Shaders system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemShadersInit( VOID );

/* Shaders system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemShadersClose( VOID );

#endif /* __twr_res_shaders_h_ */

/* END OF 'twr_res_shaders.h' FILE */
