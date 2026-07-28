/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_compute.h
 * PURPOSE     : Tough Water Rendering project.
 *               Scenes declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kupchenkov Ivan (IK4).
 * LAST UPDATE : 25.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_scenes_h_
#define __twr_scenes_h_

#include "twr_def.h"

/* Special type for scenes */
typedef struct tagtwrSCENE
{
  /* CHAR SceneFileName[TWR_STR_MAX]; */
  CHAR SceneName[TWR_STR_MAX];
  INT *UnitsNum;
  UINT NumOfUnits,
    MaxNumOfUnits;
  VEC3 CamLoc, CamAt;
} twrSCENE;

/*** Scenes system representation type ***/
typedef struct tagtwrSYSTEM_SCENES
{
  twrSCENE *Scenes;      /* Flex array of loaded scenes */ 
  UINT NumOfScenes,      /* Number of scenes */
    MaxNumOfScenes;      /* Current max size scene array */
  UINT UsingSceneNum;    /* Number of using scene in scenes array */
  BOOL IsNextSceneToSet; /* Flag for setting next scene */
  
  /* Get scene number in scenes array function
   * ARGUMENTS:
   *   - name of scene:
   *       CHAR *NameOfScene;
   * RETURNS:
   *   (INT) number of scene in scenes array.
   */
  INT (*GetSceneNumByName)( CHAR *NameOfScene );
  
  /* Get scene name by its number in scenes array function
   * ARGUMENTS:
   *   - number of scene in scenes array:
   *       CHAR *NameOfScene;
   * RETURNS:
   *   (CHAR *) name of scene.
   */
  CHAR * (*GetSceneNameByNum)( INT SceneNum );
  
  /* Scene setting function
   * ARGUMENTS: 
   *   - name of file, from which scene will be loaded:
   *       CHAR *FileName; 
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE if it is no scene.
   */
  BOOL (*SceneSetByName)( CHAR *NameOfScene );

  /* Scene setting by its name function
   * ARGUMENTS: 
   *   - name of scene:
   *       CHAR *NameOfScene; 
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE if it is no scene.
   */
  BOOL (*SceneSetByNum)( INT SceneNum );
  
  /* Scene loading function
   * ARGUMENTS: 
   *   - name of file, from which scene will be loaded:
   *       CHAR *FileName; 
   *   - name of loading scene:
   *       CHAR *SceneName;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE if it is no file or scene.
   */
  BOOL (*SceneLoad)( CHAR *FileName, CHAR *SceneName );

   /* Loading all scenes from file to scene array function
   * ARGUMENTS: 
   *   - name of file, from which scene will be loaded:
   *       CHAR *FileName; 
   * RETURNS: None.
   */
  VOID (*FileScenesLoad)( CHAR *FileName );

} twrSYSTEM_SCENES;

/* Scenes system initialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemScenesInit( VOID );

/* Scenes system deinitialization function
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemScenesClose( VOID );

/* END OF 'twr_scenes.h' FILE */

#endif /* __twr_scenes_h_ */
