/***************************************************************
 * Copyright (C) 2025
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : twr_res_img.h
 * PURPOSE     : Tough Water Rendering project.
 *               Work with images declaration file.
 * PROGRAMMER  : CGSG'2024.
 *               Kochugueva Maria (MK3).
 * LAST UPDATE : 26.03.2025
 * NOTE        : Module prefix 'twr'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __twr_res_img_h_
#define __twr_res_img_h_

#include "twr_def.h"

/* Struct of image */
typedef struct tagtwrIMAGE
{
  INT W, H;              /* Weight and height of image */
  DWORD *Pixels;           /* Pointer to array of pixels(4 bytes) */
} twrIMAGE;

/* Struct of system image */
typedef struct tagtwrSYSTEM_IMAGE
{
  /* The rendering image loading at file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName; 
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgLoad)( twrIMAGE *Img, CHAR *FileName );

  /* The rendering image free program function.
   *  ARGUMENTS:
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: None.
   */
  VOID (*ImgFree)( twrIMAGE *Img );

  /* The rendering image create program function.
   * ARGUMENTS:
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   *   - height and weight of picture:
   *       INT W, H;
   * RETURNS:
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgCreate)( twrIMAGE *Img, INT W, INT H );
  /* The rendering image in format BMP saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveBMP)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format G32 saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveG32)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format G24 saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveG24)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format DDS saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveDDS)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format TGA saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveTGA)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format JPG saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSaveJPG)( twrIMAGE *Img, CHAR *FileName );
  /* The rendering image in format PNG saving to file program function.
   * ARGUMENTS:
   *   - name of file:
   *       CHAR *FileName;
   *   - pointer on structure of image:
   *       twrIMAGE *Img;
   * RETURNS: 
   *   (BOOL) TRUE if successful, FALSE otherwise.
   */
  BOOL (*ImgSavePNG)( twrIMAGE *Img, CHAR *FileName );
} twrSYSTEM_IMAGE;

/* The rendering image loading at file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_ImgLoad( twrIMAGE *Img, CHAR *FileName );

/* The rendering image free program function.
 *  ARGUMENTS:
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: None.
 */
VOID TWR_ImgFree( twrIMAGE *Img );

/* The rendering image create program function.
 * ARGUMENTS:
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 *   - height and weight of picture:
 *       INT W, H;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_ImgCreate( twrIMAGE *Img, INT W, INT H );

/***
 * Functions about formats 
 ***/

/* The rendering image loading at file format G24 program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadG24( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format G24 saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveG24( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format G24 and G32 saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveG32( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format G32 program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadG32( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format BMP saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveBMP( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format BMP program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadBMP( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format TGA saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveTGA( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format TGA program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadTGA( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format JPG saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveJPG( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format JPG program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadJPG( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format DDS saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSaveDDS( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format DDS program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadDDS( twrIMAGE *Img, CHAR *FileName );

/* The rendering image in format PNG saving to file program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName;
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS: 
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndSavePNG( twrIMAGE *Img, CHAR *FileName );

/* The rendering image loading at file format PNG program function.
 * ARGUMENTS:
 *   - name of file:
 *       CHAR *FileName; 
 *   - pointer on structure of image:
 *       twrIMAGE *Img;
 * RETURNS:
 *   (BOOL) TRUE if successful, FALSE otherwise.
 */
BOOL TWR_RndLoadPNG( twrIMAGE *Img, CHAR *FileName );

/***
 *  Functions work with image system
 ***/

/* Image system initialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemImageInit( VOID );

/* Img system deinitialization function.
 * ARGUMENTS: None
 * RETURNS: None.
 */
VOID TWR_SystemImageClose( VOID );

#endif /* __twr_res_img_h_ */

/* END OF 'twr_res_img.h' FILE */
