/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_textures.h
 * PURPOSE     : Tough Water Rendering project.
 *               Textures system declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Nechaev Vladimir (VN4).
 * LAST UPDATE : 22.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_textures_h_
#define __twr_res_textures_h_

#include "twr_def.h"

/* Textures types */
typedef enum tagtwrTEXTURE_TYPE
{
  TWR_TEXTURE_2D = 0,           /* 2D texture */
  TWR_TEXTURE_3D = 1,           /* 3D texture */
  TWR_TEXTURE_CUBE_MAP = 2,     /* Texture for sky cubes */
} twrTEXTURE_TYPE;

/* Texture handle type */
typedef struct tagtwrTEXTURE
{
  CHAR Name[TWR_STR_MAX];   /* Texture name */
  INT W, H;                 /* Texture size in pixels */
  UINT TexId;               /* OpenGL texture Id */ 
  twrTEXTURE_TYPE Type;     /* Texture type */
  INT RefCounter;           /* Usage Counter */
  INT GLType;               /* OpenGL inner format type */
  BOOL IsMipmap;            /* Mipmap creation flag */
  BOOL IsTransparent;       /* Texture transparency flag */
} twrTEXTURE;

/* Textures system render type */
typedef struct tagtwrSYSTEM_TEXTURES
{
  /* Create 2D texture function.
   * ARGUMENTS:
   *   - texture name:
   *       CHAR *TextureName;
   *   - texture size:
   *       INT W, H;
   *   - mipmap creation flag:
   *       BOOL IsMipmap;
   *   - OpenGL texture type:
   *       INT GLType;
   *   - texture pixel color data (may be NULL):
   *       VOID *Bits;
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexCreate)( CHAR *TextureName, INT W, INT H, BOOL IsMipmap, INT GLType, VOID *Bits );

  /* Resize 2D texture function.
   * ARGUMENTS:
   *   - texture pointer:
   *       twrTEXTURE *Tex;
   *   - new texture size:
   *       INT W, H;
   * RETURNS: None.
   */
  VOID (*TexResize)( twrTEXTURE *Tex, INT W, INT H );

  /* Free texture function.
   * ARGUMENTS:
   *   - texture pointer:
   *       twrTEXTURE *Tex;
   * RETURNS: None.
   */
  VOID (*TexFree)( twrTEXTURE *Tex );

  /* Find texture in stock function.
   * ARGUMENTS:
   *   - Texture name:
   *       CHAR *TextureName;
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TextureFind)( CHAR *TextureName );

  /* Texture's usage counter increase function.
   * ARGUMENTS:
   *   - texture pointer:
   *       twrTEXTURE *Tex;
   * RETURNS: None.
   */
  VOID (*TexReferenceInc)( twrTEXTURE *Tex );

  /* Create 2D texture 1x1 RGBA32F from vector4 function.
   * ARGUMENTS:
   *   - texture name:
   *       CHAR *TextureName;
   *   - pixel color:
   *       VEC4 Value;
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexCreateFromVec4)( CHAR *TextureName, VEC4 Value );

  /* Create 2D texture 1x1 RGB32F from vector4 function.
   * ARGUMENTS:
   *   - texture name:
   *       CHAR *TextureName;
   *   - pixel color:
   *       VEC3 Value;
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexCreateFromVec3)( CHAR *TextureName, VEC3 Value );

  /* Get default texture function.
   * ARGUMENTS: None.
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexGetDefault)( VOID );

  /* Load 2D texture function.
   * ARGUMENTS: 
   *   - Texture name:
   *       CHAR *TextureName.
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexLoad)( CHAR *TextureName );

  /* Save 2D texture function.
   * ARGUMENTS: 
   *   - Texture name:
   *       CHAR *TextureName.
   * RETURNS: None.
   */
  VOID (*TexSave)( CHAR *TextureName );

  /* Create 3D Cube map texture function.
   * ARGUMENTS:
   *   - texture name:
   *       CHAR *TextureName;
   * RETURNS:
   *    twrTEXTURE *Tex.
   */
  twrTEXTURE * (*TexCreateCubeMap)( CHAR *TextureName );

} twrSYSTEM_TEXTURES;

/* Textures system initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemTexturesInit( VOID );

/* Textures system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemTexturesClose( VOID );

#endif /* __twr_res_textures_h_ */

/* END OF 'twr_res_textures.h' FILE */
