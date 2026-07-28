/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_rnd_lights.h
 * PURPOSE     : Tough Water Rendering project.
 *               Lights declaration module.
 * PROGRAMMER  : CGSG'2024.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 07.05.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_rnd_lights_h_
#define __twr_rnd_lights_h_

#include "twr_def.h"

/* Light sources types */
typedef enum tagtwrLIGHT_TYPE
{
  TWR_LIGHT_AMBIENT = 0, /* Ambient light source type */
  TWR_LIGHT_POINT = 1,   /* Point light source type */
  TWR_LIGHT_DIR = 2,     /* Directional light source type */
  TWR_LIGHT_SPOT = 3,    /* Spot light source type */
} twrLIGHT_TYPE;

/* Light source representation type */
typedef struct tagtwrLIGHT
{
  twrLIGHT_TYPE Type;         /* Light source type */
  VEC3
    Pos,                      /* Point and spot light source position */
    Dir,                      /* Directional and spot light source direction */
    Color,                    /* Light source color */
    Att;                      /* Attenuation by distance coefficients: CC CL CQ */
  VEC2 CutOffAngle;           /* Start and end cut-off angles in degrees for spot
                               * light sources */
  VEC2 CutOffAngleCos;        /* Start and end cut-off angles cosines for spot
                               * light sources */
  VEC2 CutOffDist;            /* Start/end distance for point and spot light sources */

  BOOL
    IsShadowCast,             /* Shadow cast flag */
    IsEnabled;                /* Is enabled light flag */
  MATR ShadowMatr;            /* Shadow cast matrix */
  twrTARGET *ShadowTarget;    /* Shadow target */
  INT ShadowSize;             /* Shadow depth texture size */
  twrBUFFER *Buffer;          /* Light buffer */
  twrPRIM *Prim[2];           /* Light primitive to draw */
} twrLIGHT; 

/* Light source draw parameters data buffer representation type
 * (update at light source creation or modify) */
typedef struct tagtwrRND_BUF_LIGHT
{
  VEC4 PosCutOffAngle1;             /* Point and spot light source position and cut-off
                                     * start angle cosine for spot light sources */
  VEC4 DirCutOffAngle2;             /* Directional and spot light source direction and
                                     * cut-off end angle cosine for spot light sources */
  VEC4 ColorCutOffDist1;            /* Light source color and start distance for point
                                     * and spot light sources */
  VEC4 AttCutOffDist2;              /* Attenuation by distance coefficients: CC CL CQ
                                     * and end distance for point and spot light sources */
  VEC4 IsShadowLightTypeCutAngle12; /* Shadow flag, light type, cut of angles
                                     * in degrees */
  MATR ShadowMatr;                  /* Shadow map matrix */
} twrRND_BUF_LIGHT;

/* buffers system handle type */
typedef struct tagtwrSYSTEM_RENDER_LIGHTS
{
  /* Update light source data on GPU function.
   * ARGUMENTS:
   *   - light source pointer:
   *       twrLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightUpdate)( twrLIGHT *Lig );

  /* Enable shadow cast in light source function.
   * ARGUMENTS:
   *   - light source pointer:
   *       twrLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightEnableShadow)( twrLIGHT *Lig );

  /* Redraw all light sources shadow function.
   * ARGUMENTS:
   *   - render pass flag:
   *       twrTRANSPARENCY_FLAG PassFlag;
   * RETURNS: None.
   */
  VOID (*LightRedrawShadows)( twrTRANSPARENCY_FLAG PassFlag );

  /* Redraw all light sources function.
   * ARGUMENTS:
   *   - render pass flag:
   *       twrTRANSPARENCY_FLAG PassFlag;
   * RETURNS: None.
   */
  VOID (*LightRedraw)( twrTRANSPARENCY_FLAG PassFlag );

  /* Add point light source function.
   * ARGUMENTS:
   *   - point light source position:
   *       VEC3 Pos;
   *   - point light source color:
   *       VEC3 Color;
   *   - point light source attenuation:
   *       VEC3 Att;
   *   - point light source cut-off distance:
   *       VEC3 CutOffDist;
   * RETURNS:
   *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
  */
  twrLIGHT * (*LightAddPoint)( VEC3 Pos, VEC3 Color, VEC3 Att, VEC2 CutOffDist );

  /* Add directional light source function.
   * ARGUMENTS:
   *   - directional light source direction:
   *       VEC3 Dir;
   *   - directional light source color:
   *       VEC3 Color;
   * RETURNS:
   *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
   */
  twrLIGHT * (*LightAddDir)( VEC3 Dir, VEC3 Color );

  /* Add spot light source function.
   * ARGUMENTS:
   *   - spot light source position:
   *       VEC3 Pos;
   *   - spot light source look-at point:
   *       VEC3 At;
   *   - spot light source color:
   *       VEC3 Color;
   *   - spot light source attenuation:
   *       VEC3 Att;
   *   - spot light source cut-off angle:
   *       VEC2 CutOffAngle;
   *   - spot light source cut-off distance:
   *       VEC2 CutOffDist;
   * RETURNS:
   *   (twrLIGHT *) pointer to add light source or NULL if no space for a new ligth source.
  */
  twrLIGHT * (*LightAddSpot)( VEC3 Pos, VEC3 At, VEC3 Color, VEC3 Att, VEC2 CutOffDist, VEC2 CutOffAngle );

  /* Enable light source function.
   * ARGUMENTS:
   *   - light source pointer:
   *       twrLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightEnable)( twrLIGHT *Lig );

  /* Enable light source function.
   * ARGUMENTS:
   *   - light source pointer:
   *       twrLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightDisable)( twrLIGHT *Lig );

  /* Set light position function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new position:
   *       VEC3 Pos;
   * RETURNS: None.
   */
  VOID (*LightSetPos)( twrLIGHT *Lig, VEC3 Pos );

  /* Set light direction function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new direction:
   *       VEC3 Dir;
   * RETURNS: None.
   */
  VOID (*LightSetDir)( twrLIGHT *Lig, VEC3 Dir );

  /* Set light color function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new color:
   *       VEC3 Color;
   * RETURNS: None.
   */
  VOID (*LightSetColor)( twrLIGHT *Lig, VEC3 Color );

  /* Set light attenuation function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new attenuation:
   *       VEC3 Att;
   * RETURNS: None.
   */
  VOID (*LightSetAtt)( twrLIGHT *Lig, VEC3 Att );

  /* Set light cut-off angle function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new cut-off angle:
   *       VEC2 CutOffAngle;
   * RETURNS: None.
   */
  VOID (*LightSetAngle)( twrLIGHT *Lig, VEC2 CutOffAngle );

  /* Set light cut-off angle cosine function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new cut-off angle cosine:
   *       VEC2 Att;
   * RETURNS: None.
   */
  VOID (*LightSetCos)( twrLIGHT *Lig, VEC2 CutOffAngleCos );

  /* Set light cut-off distance function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   *   - new cut-off distance:
   *       VEC2 CutOffDist;
   * RETURNS: None.
   */
  VOID (*LightSetDist)( twrLIGHT *Lig, VEC2 CutOffDist );

  /* Delete light from stock function.
   * ARGUMENTS:
   *   - light source:
   *       twrLIGHT *Lig;
   * RETURNS: None.
   */
  VOID (*LightFree)( twrLIGHT *Lig );

} twrSYSTEM_RENDER_LIGHTS;

  /* Light system initialization function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID TWR_SystemLightsInit( VOID );

  /* Light system deinitialization function.
   * ARGUMENTS: None.
   * RETURNS: None.
   */
  VOID TWR_SystemLightsClose( VOID );

#endif /* __twr_rnd_lights_h_ */

/* END OF 'twr_rnd_lights.h' FILE */
