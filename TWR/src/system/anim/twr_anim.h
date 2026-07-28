/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_anim.h
 * PURPOSE     : Tough Water Rendering project.
 *               Animation system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Tretyakov Andrey (AT7).
 * LAST UPDATE : 15.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_anim_h_
#define __twr_anim_h_

#include "win/twr_win.h"
#include "rnd/twr_rnd.h"
#include "twr_timer.h"
#include "twr_input.h"
#include "utils/stock/twr_stock.h"
#include "scenes/twr_scenes.h"
#include "water/still/twr_still_water.h"

/* Animation unit representation type forward definition */
typedef struct tagtwrUNIT twrUNIT;

/* Base unit fields macro */
#define TWR_UNIT_BASE_FIELDS \
  VOID (*Init)( twrUNIT *Uni );     \
  VOID (*Close)( twrUNIT *Uni );    \
  VOID (*Response)( twrUNIT *Uni ); \
  VOID (*Render)( twrUNIT *Uni )

/* Animation unit representation type */
struct tagtwrUNIT
{
  TWR_UNIT_BASE_FIELDS;
};

/* Animation system representation type */
typedef struct tagtwrSYSTEM_ANIMATION
{
  /* Scene system */
  union
  {
    twrSYSTEM_SCENES;
    twrSYSTEM_SCENES ScenesSys;
  };
  
  /* Image system */
  union
  {
    twrSYSTEM_IMAGE;
    twrSYSTEM_IMAGE  Img;
  };
  /* Window system */
  union
  {
    twrSYSTEM_WINDOW;
    twrSYSTEM_WINDOW Win;
  };

  /* Timer system */
  union
  {
    twrSYSTEM_TIMER;
    twrSYSTEM_TIMER Timer;
  };

  /* Input system */
  union
  {
    twrSYSTEM_INPUT;
    twrSYSTEM_INPUT Input;
  };

  /* Still water system */
  union
  {
    twrSYSTEM_STILL_WATER;
    twrSYSTEM_STILL_WATER StillWater;
  };

  /* Render system */
  union
  {
    twrSYSTEM_RENDER;
    twrSYSTEM_RENDER Render;
  };

  /* Animation units stock */
  STOCK StkUnits;

  /* Pointers to functions: */

  /* Animation resize function.
   * ARGUMENTS:
   *   - window width and height:
   *       INT W, H;
   * RETURNS: None.
   */
  VOID (*AnimResize)( INT W, INT H );

  /* Animation one frame render function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID (*AnimRender)( VOID );

  /* Unit creation function.
   * ARGUMENTS:
   *   - unit representation type size in bytes:
   *       UINT Size;
   * RETURNS:
   *   (twrUNIT *) pointer to created unit.
   */
  twrUNIT * (*UnitCreate)( UINT Size );

  /* Add unit to unit's array function.
   * ARGUMENTS:
   *   - unit to be added:
   *       twrUNIT *Uni;
   * RETURNS: None.
   */
  VOID (*UnitAdd)( twrUNIT *Uni );

  /* Add unit to unit's array by dictionary name function.
   * ARGUMENTS:
   *   - unit identification name:
   *       CHAR *Name;
   * RETURNS: None.
   */
  VOID (*UnitAddByName)( CHAR *Name );
  /* Get unit number in dictionary function.
   * ARGUMENTS:
   *   - unit identification name:
   *       CHAR *Name;
   * RETURNS: 
       (INT) number of unit in units dictionary (-1 if no unit).
   */
  INT (*GetUnitNum)( CHAR *Name );

  /* Create unit by its number in dictionary function.
   * ARGUMENTS:
   *   - unit number in dictionary:
   *       INT Num;
   * RETURNS: None.
   */
  VOID (*UnitAddByNum)( INT Num );

} twrSYSTEM_ANIMATION;

/* Animation system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemAnimationInit( VOID );

/* Animation system deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemAnimationClose( VOID );

#endif /* __twr_anim_h_ */

/* END OF 'twr_anim.h' FILE */
