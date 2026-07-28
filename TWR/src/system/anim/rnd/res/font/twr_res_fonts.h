/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_fonts.h
 * PURPOSE     : Tough Water Rendering project.
 *               Font resource declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Vitaly A. Galinsky (VG4).
 * LAST UPDATE : 21.04.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_fonts_h_
#define __twr_res_fonts_h_

#include "twr_def.h"

/* Font representation type */
typedef struct tagtwrFONT
{
  CHAR Name[TWR_STR_MAX];     /* Font name */
  twrPRIM *Glyphs[256];       /* Font characters primitives */
  twrPRIM *GlyphsScreen[256]; /* Font characters screen precision primitives */
  INT LineH, BaseH;           /* Font line height and base line height in pixels */
  FLT AdvanceX[256];          /* Every letter shift right value (0 if no letter present) */

  twrMATERIAL
    *MtlDirect,               /* Material patter for font 3D drawing */
    *MtlScreen;               /* Material patter for font screen space drawing */
} twrFONT;

/* Fonts system representation type */
typedef struct tagtwrSYSTEM_FONTS
{
  /* Load font function.
   * ARGUMENTS:
   *   - font file name:
   *       CHAR *FileName;
   *   - font letters draw color:
   *       VEC4 TextColor;
   * RETURNS:
   *   (twrFONT *) pointer to load font resourse or NULL if error is occured.
   */
  twrFONT * (*FntCreate)( CHAR *FileName, VEC4 TextColor );

  /* Font free function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
   * RETURNS: None.
   */
  VOID (*FntFree)( twrFONT *Fnt );

  /* Draw string in screen space function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
   *   - text to draw:
   *       CHAR *Text;
   *   - draw position:
   *       VEC3 Pos;
   *   - font size (in world space units):
   *       FLT Size;
   * RETURNS: None.
   */
  VOID (*FntDraw)( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size );

  /* Draw string in 3D space function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
   *   - text to draw:
   *       CHAR *Text;
   *   - draw position:
   *       VEC3 Pos;
   *   - font size (in world space units):
   *       FLT Size;
   * RETURNS: None.
   */
  VOID (*FntDraw3D)( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size );

  /* Draw string in billboard mode function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
   *   - text to draw:
   *       CHAR *Text;
   *   - draw position:
   *       VEC3 Pos;
   *   - font size (0 - for size 1.0, otherwise - size in world space):
   *       FLT Size;
   * RETURNS: None.
   */
  VOID (*FntDrawBB)( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size );

  /* Draw string in billboard screen size mode function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
   *   - text to draw:
   *       CHAR *Text;
   *   - draw position:
   *       VEC3 Pos;
   *   - font size in pixels (0 - for original size, otherwise - size in pixels):
   *       FLT Size;
   * RETURNS: None.
   */
  VOID (*FntDrawBBS)( twrFONT *Fnt, CHAR *Text, VEC3 Pos, FLT Size );

  /* Increase font usage counter function.
   * ARGUMENTS:
   *   - font pointer:
   *       twrFONT *Fnt;
  * RETURNS: None.
  */
  VOID (*FntReferenceInc)( twrFONT *Fnt );
} twrSYSTEM_FONTS;

/* Fonts system initialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemFontsInit( VOID );

/* Fonts system deinitialize function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID TWR_SystemFontsClose( VOID );

#endif /* __twr_res_fonts_h_ */

/* END OF 'twr_res_fonts.h' FILE */
